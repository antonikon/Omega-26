#pragma once

#include <QtCore>
#include <QPainter>
#include <Box2D/Box2D.h>

#include "element.h"

class TWorld;

struct TBullet {
public:
    TBullet(const QPoint &pos, qreal angle, TWorld *world,
            const TGunElementInfo *gunInfo);
    void DebugDraw(QPainter &painter);
public:
    b2Body *Body;
    const TGunElementInfo *GunInfo;
};

class TTank {
public:
    TTank(const QPoint &pos, qreal angle, TWorld *world,
          const TBodyElementInfo *bodyInfo, const TSlotElementInfo *slotInfo,
          const TGunElementInfo *gunInfo);
    ~TTank();
    void DebugDraw(QPainter &painter);
    static void DebugDrawFixture(QPainter &painter, b2Fixture *fixt);
    void MotorUp(qreal speed);
    void Turn(qreal speed);
    void TurnGun(qreal speed);
    void Fire();
    void Update();
private:
    TWorld *World;
    TBodyElement *Body;
    TSlotElement *Slot;
    TGunElement *Gun;
};
