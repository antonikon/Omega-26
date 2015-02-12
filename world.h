#pragma once

#include <QtCore>
#include <Box2D/Box2D.h>
#include <QPainter>

#include "element.h"

class TBullet;
class TTank;

class TWorld {
public:
    TWorld();
    void AddTank(const QPoint &pos, qreal angle, uint32 bodyIndex,
                 uint32 slotIndex, uint32 gunIndex);
    void AddBullet(TBullet *bullet);
    void DebugDraw(QPainter &painter);
    void Update();
    TTank *GetTank(uint index);
    b2World *GetWorld();
private:
    b2World *World;
    QVector <TBodyElementInfo*> BodyInfoList;
    QVector <TSlotElementInfo*> SlotInfoList;
    QVector <TGunElementInfo*> GunInfoList;
    QVector <TTank*> TankList;
    QVector <TBullet*> BulletList;
};
