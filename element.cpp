#include "element.h"

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
