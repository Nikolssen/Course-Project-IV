#pragma once
#include "Atom.h"
struct Vertex2D
{
    int x;
    int y;
    int prevx;
    int prevy;
    bool shortRear;
    bool shortFront;
    Atom atom;
};