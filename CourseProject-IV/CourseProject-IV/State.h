#pragma once
#include "Element.h"

struct State
{
public:
	int getBond() { return this->bond; }
	void setBond(int bond);

	Element getElement() { return this->element; }
	void setElement(Element element);
	static int getMaxBonds(Element element);
private:
	int bond = 1;
	Element element = Element::Carbon;
};
