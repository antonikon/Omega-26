#pragma once

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
