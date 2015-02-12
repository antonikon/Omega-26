#pragma once

#include <QtCore>
#include <Box2D/Box2D.h>

#include "types.h"

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
    uint32 Reloading;
};

struct TElement {
public:
    TElement(const TElementInfo *info);
public:
    const TElementInfo *Info;
    uint32 Hp;
    b2Fixture* Fixture;
};

struct TBodyElement: public TElement {
    TBodyElement(const TBodyElementInfo *info);
    ~TBodyElement();
};

struct TSlotElement: public TElement {
public:
    TSlotElement(const TSlotElementInfo *info);
    ~TSlotElement();
public:
    b2RevoluteJoint *Motor;
};

struct TGunElement: public TElement {
public:
    TGunElement(const TGunElementInfo *info);
    ~TGunElement();
public:
    uint ReloadingProgress = 0;
};
