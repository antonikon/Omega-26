#include "world.h"

TWorld::TWorld():
    World(new b2World(b2Vec2(0,0)))
{
    TBodyElementInfo *bodyInfo = new TBodyElementInfo;
    bodyInfo->BodyType = TBT_Light;
    bodyInfo->LinearSpeed = 200;
    bodyInfo->AngularSpeed = 90;
    BodyInfoList.append(bodyInfo);

    TSlotElementInfo *slotInfo = new TSlotElementInfo;
    slotInfo->SlotType = TST_Light;
    slotInfo->AngularSpeed = 1;
    SlotInfoList.append(slotInfo);

    TGunElementInfo *gunInfo = new TGunElementInfo;
    gunInfo->GunType = TGT_Light;
    gunInfo->Damage = 1;
    gunInfo->Distance = 100;
    gunInfo->Penetrate = 5;
    gunInfo->Speed = 5;
    GunInfoList.append(gunInfo);
}

void TWorld::AddTank(const QPoint &pos, qreal angle, uint32 bodyIndex, uint32 slotIndex, uint32 gunIndex)
{
    TTank *tank = new TTank(pos, angle, World, BodyInfoList[bodyIndex],
                            SlotInfoList[slotIndex], GunInfoList[gunIndex]);
    TankList.append(tank);
}

void TWorld::DebugDraw(QPainter &painter) {
    for (int i = 0; i < TankList.size(); ++i) {
        TankList[i]->DebugDraw(painter);
    }
}

void TWorld::Update() {
    World->Step(1 / 60.0, 10, 10);
    for (int i = 0; i < TankList.size(); ++i) {
        TankList[i]->Update();
    }
}

TTank *TWorld::GetTank(uint index) {
    return TankList[index];
}


TTank::TTank(const QPoint &pos, qreal angle, b2World *world,
             const TBodyElementInfo *bodyInfo, const TSlotElementInfo *slotInfo,
             const TGunElementInfo *gunInfo):
    Body(new TBodyElement(bodyInfo)),
    Slot(new TSlotElement(slotInfo)),
    Gun(new TGunElement(gunInfo))
{
    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    bodyDef.position.Set(pos.x() / 100.0, pos.y() / 100.0);
    bodyDef.angle = angle / 180 * M_PI;
    bodyDef.linearDamping = 10;
    bodyDef.angularDamping = 10;
    b2Body *body = world->CreateBody(&bodyDef);

    b2FixtureDef fixtDef;
    fixtDef.density = 1;
    fixtDef.friction = 0.5;
    fixtDef.restitution = 0.5;
    fixtDef.filter.categoryBits = BF_TankBody;
    fixtDef.filter.maskBits = BF_TankBody | BF_WorldObject | BF_Bullet;
    b2PolygonShape boxShape;
    boxShape.SetAsBox(52 / 200.0, 72 / 200.0);
    fixtDef.shape = &boxShape;
    Body->Fixture = body->CreateFixture(&fixtDef);
    Body->Fixture->SetUserData(Body);

    bodyDef.linearDamping = 0;
    bodyDef.angularDamping = 0;

    body = world->CreateBody(&bodyDef);
    fixtDef.filter.categoryBits = BF_TankSlot;
    fixtDef.filter.maskBits = BF_Bullet | BF_TankGun;
    b2CircleShape circleShape;
    circleShape.m_radius = 15 / 100.0;
    fixtDef.shape = &circleShape;
    Slot->Fixture = body->CreateFixture(&fixtDef);
    Slot->Fixture->SetUserData(Slot);

    boxShape.SetAsBox(5 / 200.0, 50 / 200.0, b2Vec2(0, -50 / 200.0), 0);
    fixtDef.shape = &boxShape;
    fixtDef.filter.categoryBits = BF_TankGun;
    fixtDef.filter.maskBits = BF_WorldObject | BF_Bullet | BF_TankSlot;
    Gun->Fixture = body->CreateFixture(&fixtDef);
    Gun->Fixture->SetUserData(Gun);

    b2WeldJointDef jointDef;
    jointDef.Initialize(Body->Fixture->GetBody(), Slot->Fixture->GetBody(),
                        Body->Fixture->GetBody()->GetWorldCenter());
    world->CreateJoint(&jointDef);
}

TTank::~TTank() {
    delete Gun;
    delete Slot;
    delete Body;
}


void TTank::DebugDraw(QPainter &painter) {
    DebugDrawFixture(painter, Body->Fixture);
    DebugDrawFixture(painter, Slot->Fixture);
    DebugDrawFixture(painter, Gun->Fixture);
}

void TTank::MotorUp(qreal speed) {
    b2Body *body = Body->Fixture->GetBody();
    TBodyElementInfo *info = (TBodyElementInfo*)Body->Info;
    qreal x = sin(body->GetAngle()) * info->LinearSpeed / 100.0 * speed;
    qreal y = -cos(body->GetAngle()) * info->LinearSpeed / 100.0 * speed;
    Body->Fixture->GetBody()->SetLinearVelocity(b2Vec2(x, y));
}

void TTank::Turn(qreal speed) {
    b2Body *body = Body->Fixture->GetBody();
    b2Body *gun = Slot->Fixture->GetBody();
    TBodyElementInfo *info = (TBodyElementInfo*)Body->Info;
    body->SetAngularVelocity(info->AngularSpeed / 180.0 * M_PI * speed);
}

void TTank::Update() {
    b2Body *body = Body->Fixture->GetBody();
    qreal angle = body->GetAngle();
    b2Vec2 velocity = body->GetLinearVelocity();
    qreal velocityAngle = (qAtan2(velocity.y, velocity.x) + M_PI_2);
    qreal deltaAngle = angle - velocityAngle;
    qreal newVelocityLen = velocity.Length() * cos(deltaAngle);
    qreal x = sin(angle) * newVelocityLen;
    qreal y = -cos(angle) * newVelocityLen;
    body->SetLinearVelocity(b2Vec2(x, y));
    //qDebug() << body->GetAngle() * 180 / M_PI << velocityAngle;
}

void TTank::DebugDrawFixture(QPainter &painter, b2Fixture *fixt) {
    b2Body *body = fixt->GetBody();
    b2Shape *shape = fixt->GetShape();
    if (shape->GetType() == b2Shape::e_polygon) {
        b2PolygonShape *poly = (b2PolygonShape*)shape;
        for (int i = 1; i < poly->GetVertexCount(); ++i) {
            b2Vec2 pointA = body->GetWorldPoint(poly->GetVertex(i - 1));
            b2Vec2 pointB = body->GetWorldPoint(poly->GetVertex(i));
            painter.drawLine(pointA.x * 100, pointA.y * 100,
                             pointB.x * 100, pointB.y * 100);
        }
        {
            b2Vec2 pointA = body->GetWorldPoint(poly->GetVertex(0));
            b2Vec2 pointB = body->GetWorldPoint(poly->GetVertex(
                                                    poly->GetVertexCount() - 1));
            painter.drawLine(pointA.x * 100, pointA.y * 100,
                             pointB.x * 100, pointB.y * 100);
        }
    } else if (shape->GetType() == b2Shape::e_circle) {
        b2CircleShape *circle = (b2CircleShape*)shape;
        b2Vec2 center = body->GetWorldPoint(circle->GetVertex(0));
        painter.drawEllipse(QPoint(center.x * 100, center.y * 100),
                            int(circle->m_radius * 100),
                            int(circle->m_radius * 100));
    }
}

TGunElement::~TGunElement() {
    Fixture->GetBody()->DestroyFixture(Fixture);
}


TSlotElement::~TSlotElement() {
    b2Body *body = Fixture->GetBody();
    body->DestroyFixture(Fixture);
    body->GetWorld()->DestroyBody(body);
}


TBodyElement::TBodyElement(const TBodyElementInfo *info):
    TElement((TElementInfo*)info)
{
}

TSlotElement::TSlotElement(const TSlotElementInfo *info):
    TElement((TElementInfo*)info)
{
}

TGunElement::TGunElement(const TGunElementInfo *info):
    TElement((TElementInfo*)info)
{
}

TBodyElement::~TBodyElement() {
    b2Body *body = Fixture->GetBody();
    body->GetWorld()->DestroyBody(body);
}


TElement::TElement(const TElementInfo *info):
    Info(info)
{
    Hp = info->MaxHp;
}
