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

    std::wstring Description() {
        std::wstring vertex = L"";
        if (atom.freeBonds > 0)
            {
                vertex = L"H";
                if (atom.freeBonds > 1)
                    vertex += std::to_wstring(atom.freeBonds);
            }
            switch (atom.element)
            {
            case Element::Chlorine:
                vertex += L"Cl";
                break;
            case Element::Bromine:
                vertex += L"Br";
                break;
            case Element::Fluorine:
                vertex += L" F";
                break;
            case Element::Iodine:
                vertex += L" I";
                break;
            case Element::Sulfur:
                vertex += L"S";
                break;
            case Element::Oxygen:
                if (atom.freeBonds != 1)
                    vertex += L"O";
                else
                    vertex = L"O" + vertex;
                break;
            case Element::Phosphorus:
                vertex = L"P" + vertex;
                break;
            case Element::Nitrogen:
                vertex = L"N" + vertex;
                break;
            case Element::Carbon:
                vertex = L"C" + vertex;
                break;
            }
            return vertex;
    }
};