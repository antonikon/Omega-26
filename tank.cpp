#include "tank.h"

#include "world.h"

TTank::TTank(const QPoint &pos, qreal angle, TWorld *world,
             const TBodyElementInfo *bodyInfo, const TSlotElementInfo *slotInfo,
             const TGunElementInfo *gunInfo):
    World(world),
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
    b2Body *body = world->GetWorld()->CreateBody(&bodyDef);

    b2FixtureDef fixtDef;
    fixtDef.density = 10;
    fixtDef.friction = 0.5;
    fixtDef.restitution = 0.25;
    fixtDef.filter.categoryBits = BF_TankBody;
    fixtDef.filter.maskBits = BF_TankBody | BF_WorldObject | BF_Bullet;
    b2PolygonShape boxShape;
    boxShape.SetAsBox(52 / 200.0, 72 / 200.0);
    fixtDef.shape = &boxShape;
    Body->Fixture = body->CreateFixture(&fixtDef);
    Body->Fixture->SetUserData(Body);

    bodyDef.linearDamping = 0;
    bodyDef.angularDamping = 0;

    body = world->GetWorld()->CreateBody(&bodyDef);
    fixtDef.filter.categoryBits = BF_TankSlot;
    fixtDef.filter.maskBits = BF_Bullet | BF_TankGun;
    b2CircleShape circleShape;
    circleShape.m_radius = 15 / 100.0;
    fixtDef.shape = &circleShape;
    Slot->Fixture = body->CreateFixture(&fixtDef);
    Slot->Fixture->SetUserData(Slot);

    boxShape.SetAsBox(5 / 200.0, 50 / 200.0, b2Vec2(0, -50 / 200.0), 0);
    fixtDef.density = 0.1;
    fixtDef.shape = &boxShape;
    fixtDef.filter.categoryBits = BF_TankGun;
    fixtDef.filter.maskBits = BF_WorldObject | BF_Bullet | BF_TankSlot | BF_TankGun;
    Gun->Fixture = body->CreateFixture(&fixtDef);
    Gun->Fixture->SetUserData(Gun);

    b2RevoluteJointDef jointDef;
    jointDef.Initialize(Body->Fixture->GetBody(), Slot->Fixture->GetBody(),
                        Body->Fixture->GetBody()->GetWorldCenter());
    jointDef.enableMotor = true;
    jointDef.maxMotorTorque = 10;
    Slot->Motor = (b2RevoluteJoint*)world->GetWorld()->CreateJoint(&jointDef);
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
    TBodyElementInfo *info = (TBodyElementInfo*)Body->Info;
    body->SetAngularVelocity(info->AngularSpeed / 180.0 * M_PI * speed);
}

void TTank::TurnGun(qreal speed) {
    TSlotElementInfo *info = (TSlotElementInfo*)Slot->Info;
    Slot->Motor->SetMotorSpeed(info->AngularSpeed / 180 * M_PI * speed);
}

void TTank::Fire() {
    if (Gun->ReloadingProgress == 0) {
        b2Body *body = Gun->Fixture->GetBody();
        TGunElementInfo *info = (TGunElementInfo*)Gun->Info;
        b2Vec2 position = body->GetWorldCenter();
        TBullet *newBullet = new TBullet(QPoint(position.x * 100,
                                            position.y * 100 + 50), 0,
                                     World, info);
        Gun->ReloadingProgress = info->Reloading;
    }
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
    if (Gun->ReloadingProgress > 0) {
        Gun->ReloadingProgress--;
    }
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

TBullet::TBullet(const QPoint &pos, qreal angle, TWorld *world,
                 const TGunElementInfo *gunInfo):
    GunInfo(gunInfo)
{
    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    bodyDef.bullet = true;
    bodyDef.position.Set(pos.x() / 100.0, pos.y() / 100.0);
    bodyDef.angle = angle;
    Body = world->GetWorld()->CreateBody(&bodyDef);

    b2FixtureDef fixtDef;
    fixtDef.density = 0.01;
    fixtDef.restitution = 0;
    fixtDef.friction = 1;
    fixtDef.filter.categoryBits = BF_Bullet;
    fixtDef.filter.maskBits = BF_TankBody | BF_TankSlot
            | BF_TankGun | BF_WorldObject;

    b2PolygonShape poly;
   /* poly.m_count = 5;
    poly.m_vertices[0].Set(0, 0);
    poly.m_vertices[1].Set(4 / 100.0, 0);
    poly.m_vertices[2].Set(4 / 100.0, 10 / 100.0);
    poly.m_vertices[3].Set(2 / 100.0, 15 / 100.0);
    poly.m_vertices[4].Set(0, 10 / 100.0);*/
    poly.SetAsBox(4 / 200.0, 10 / 200.0);
    fixtDef.shape = &poly;
    Body->CreateFixture(&fixtDef);

    Body->SetUserData(this);

    qreal x = sin(angle) * gunInfo->Speed;
    qreal y = -cos(angle) * gunInfo->Speed;
    Body->SetLinearVelocity(b2Vec2(x, y));

    world->AddBullet(this);
}

void TBullet::DebugDraw(QPainter &painter) {
    TTank::DebugDrawFixture(painter, Body->GetFixtureList());
}
