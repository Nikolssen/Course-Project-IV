#pragma once
#include "Vertex3D.h"

struct ConverterNode
{
	ConverterNode* first = nullptr;
	ConverterNode* second = nullptr;
	ConverterNode* third = nullptr;
	ConverterNode* fourth = nullptr;
	ConverterNode* parent = nullptr;
	Vertex3D vertex;
	int index = 0;
	int lastBond = 0;
	int bond = 0;
};