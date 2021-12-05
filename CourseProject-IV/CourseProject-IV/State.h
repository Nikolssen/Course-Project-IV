#pragma once
#include "Element.h"

struct State
{
public:
	int GetBond() { return this->bond; }
	void SetBond(int bond);

	Element GetElement() { return this->element; }
	void SetElement(Element element);
	static int GetMaxBonds(Element element);
private:
	int bond = 1;
	Element element = Element::Carbon;
};
