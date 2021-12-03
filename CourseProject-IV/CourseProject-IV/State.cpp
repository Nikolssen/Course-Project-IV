#include "State.h"

void State::setBond(int bond) {
    if (getMaxBonds(element) >= bond)
        this->bond = bond;
}

void State::setElement(Element element) {
    this->element = element;
    if (bond > getMaxBonds(element))
        bond = 1;
}

int State::getMaxBonds(Element element) {
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