#include "State.h"

void State::SetBond(int bond) {
    if (GetMaxBonds(element) >= bond)
        this->bond = bond;
}

void State::SetElement(Element element) {
    this->element = element;
    if (bond > GetMaxBonds(element))
        bond = 1;
}

int State::GetMaxBonds(Element element) {
    switch (element)
    {
    case Element::Carbon:
        return 4;
    case Element::Nitrogen:
    case Element::Phosphorus:
        return 3;
    case Element::Oxygen:
    case Element::Sulfur:
        return 2;
    default:
        return 1;
    }
}