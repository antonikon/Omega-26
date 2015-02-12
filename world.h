#pragma once

#include <QtCore>
#include <Box2D/Box2D.h>
#include <QPainter>

enum ETankBodyType {
    TBT_Light,
    TBT_Medium,
    TBT_Heavy
};

enum ETankSlotType {
    TST_Light,
    TST_Medium,
    TST_Heavy
};

enum ETankGunType {
    TGT_Light,
    TGT_Medium,
    TGT_Heavy
};

enum EElementType {
    ET_Body,
    ET_Slot,
    ET_Gun
};

enum EBodyFilter {
    BF_TankBody = 0x0001,
    BF_TankSlot = 0x0002,
    BF_TankGun = 0x0004,
    BF_WorldObject = 0x0008,
    BF_Bullet = 0x0010
};

struct TElementInfo {
    EElementType Type;
    uint32 Armor;
    uint32 MaxHp;
};

struct TBodyElementInfo {
    ETankBodyType BodyType;
    qreal LinearSpeed;
    qreal AngularSpeed;
};

struct TSlotElementInfo {
    ETankSlotType SlotType;
    qreal AngularSpeed;
};

struct TGunElementInfo {
    ETankGunType GunType;
    uint32 Damage;
    uint32 Penetrate;
    qreal Speed;
    qreal Distance;
};

struct TElement {
    TElement(const TElementInfo *info);
    const TElementInfo *Info;
    uint32 Hp;
    b2Fixture* Fixture;
};

struct TBodyElement: public TElement {
    TBodyElement(const TBodyElementInfo *info);
    ~TBodyElement();
};

struct TSlotElement: public TElement {
    TSlotElement(const TSlotElementInfo *info);
    ~TSlotElement();
};

struct TGunElement: public TElement {
    TGunElement(const TGunElementInfo *info);
    ~TGunElement();
};

class TTank {
public:
    TTank(const QPoint &pos, qreal angle, b2World *world,
          const TBodyElementInfo *bodyInfo, const TSlotElementInfo *slotInfo,
          const TGunElementInfo *gunInfo);
    ~TTank();
    void DebugDraw(QPainter &painter);
    void MotorUp(qreal speed);
    void Turn(qreal speed);
    void Update();
private:
    void DebugDrawFixture(QPainter &painter, b2Fixture *fixt);
private:
    TBodyElement *Body;
    TSlotElement *Slot;
    TGunElement *Gun;
};

class TWorld {
public:
    TWorld();
    void AddTank(const QPoint &pos, qreal angle, uint32 bodyIndex,
                 uint32 slotIndex, uint32 gunIndex);
    void DebugDraw(QPainter &painter);
    void Update();
    TTank *GetTank(uint index);
private:
    b2World *World;
    QVector <TBodyElementInfo*> BodyInfoList;
    QVector <TSlotElementInfo*> SlotInfoList;
    QVector <TGunElementInfo*> GunInfoList;
    QVector <TTank*> TankList;
};
