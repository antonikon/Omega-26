#include "world.h"

#include "tank.h"

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
    slotInfo->AngularSpeed = 90;
    SlotInfoList.append(slotInfo);

    TGunElementInfo *gunInfo = new TGunElementInfo;
    gunInfo->GunType = TGT_Light;
    gunInfo->Damage = 1;
    gunInfo->Distance = 100;
    gunInfo->Penetrate = 5;
    gunInfo->Speed = 100;
    gunInfo->Reloading = 2 * (1000 / 17);
    GunInfoList.append(gunInfo);
}

void TWorld::AddTank(const QPoint &pos, qreal angle, uint32 bodyIndex, uint32 slotIndex, uint32 gunIndex)
{
    TTank *tank = new TTank(pos, angle, this, BodyInfoList[bodyIndex],
                            SlotInfoList[slotIndex], GunInfoList[gunIndex]);
    TankList.append(tank);
}

void TWorld::AddBullet(TBullet *bullet) {
    BulletList.append(bullet);
}

void TWorld::DebugDraw(QPainter &painter) {
    for (int i = 0; i < TankList.size(); ++i) {
        TankList[i]->DebugDraw(painter);
    }
    for (int i = 0; i < BulletList.size(); ++i) {
        BulletList[i]->DebugDraw(painter);
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

b2World *TWorld::GetWorld() {
    return World;
}
