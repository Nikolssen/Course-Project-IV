#include "Converter.h"
#include "Converter.h"

Converter::Converter()
{
}

Converter::~Converter()
{
    vertices3D.clear();
}
std::vector<Vertex3D> Converter::ConvertToCalotte(std::vector<Vertex2D>& vertices2D)
{

    vertices3D.clear();

    if (IsSimple(vertices2D))
        return vertices3D;

    ConverterNode* root = new ConverterNode();
    root->vertex.element = vertices2D[0].atom.element;

    MakeTree(root, vertices2D);
    SortTree(root);
    InitCoordinates(root);
    ClearTree(root);
    CentrifyCoordinates();
    return vertices3D;
}
void Converter::MakeTree(ConverterNode* root, std::vector<Vertex2D>& vertices2D)
{
    for (int k = root->index + 1; k < vertices2D.size(); k++)
    {
        if ((vertices2D.at(root->index).x == vertices2D.at(k).prevx) && (vertices2D.at(root->index).y == vertices2D.at(k).prevy))
        {
            ConverterNode* daughter = new ConverterNode();
            daughter->parent = root;
            daughter->index = k;
            daughter->vertex.element = vertices2D.at(k).atom.element;
            daughter->bond = vertices2D.at(k).atom.bond;
            daughter->lastBond = 0;
            switch (root->lastBond)
            {
            case 0:
                root->first = daughter;
                break;
            case 1:
                root->second = daughter;
                break;
            case 2:
                root->third = daughter;
                break;
            default:
                root->fourth = daughter;
                break;
            }

            root->lastBond += 1;
            if (State::GetMaxBonds(daughter->vertex.element) - daughter->bond > 0)
                MakeTree(daughter, vertices2D);
        }
    }
    if (vertices2D.at(root->index).atom.freeBonds)
        for (int j = 0; j < vertices2D.at(root->index).atom.freeBonds; j++)
        {
            ConverterNode* daughter = new ConverterNode();
            daughter->parent = root;
            daughter->index = -1;
            daughter->vertex.element = Element::Hydrogen;
            daughter->bond = 1;
            daughter->lastBond = 0;
            switch (root->lastBond)
            {
            case 0:
                root->first = daughter;
                break;
            case 1:
                root->second = daughter;
                break;
            case 2:
                root->third = daughter;
                break;
            default:
                root->fourth = daughter;
                break;
            }

            root->lastBond++;
        }
}
void Converter::InitCoordinates(ConverterNode* root)
{
    root->vertex.point = { 0, 0, 0 };

    if (root->first->bond == 3)
    {
        root->first->vertex.point = { 0, 0.8, 0 };
        if (root->second != nullptr)
            root->second->vertex.point = { 0, -0.8, 0 };
    }
    else
        AdvancedSpin(root);

    if (root->first != nullptr)
        ImplementCoordinates(root->first);
    if (root->second != nullptr)
        ImplementCoordinates(root->second);
    if (root->third != nullptr)
        ImplementCoordinates(root->third);
    if (root->fourth != nullptr)
        ImplementCoordinates(root->fourth);

}
void Converter::ImplementCoordinates(ConverterNode* root)
{
    if (root->lastBond == 0)
        return;

    if ((root->first->bond == 3) || ((root->first->bond == 2) && (root->bond == 2)) || (root->bond == 3))
    {
        root->first->vertex.point.x = 2 * root->vertex.point.x - root->parent->vertex.point.x;
        root->first->vertex.point.y = 2 * root->vertex.point.y - root->parent->vertex.point.y;
        root->first->vertex.point.z = 2 * root->vertex.point.z - root->parent->vertex.point.z;
    }
    else
        AdvancedSpin(root);

    if (root->first != nullptr)
        ImplementCoordinates(root->first);
    if (root->second != nullptr)
        ImplementCoordinates(root->second);
    if (root->third != nullptr)
        ImplementCoordinates(root->third);

}

void Converter::SortTree(ConverterNode* root)
{
    switch (root->lastBond)
    {
    case 2:
        if (root->first->bond < root->second->bond)
        {
            ConverterNode* temp = root->second;
            root->second = root->first;
            root->first = temp;
        }
        break;
    case 3:
        if (root->second->bond > root->first->bond)
        {
            ConverterNode* temp = root->second;
            root->second = root->first;
            root->first = temp;
        }
        else if (root->third->bond > root->second->bond)
        {
            ConverterNode* temp = root->third;
            root->third = root->second;
            root->second = temp;
        }
        break;

    }
    if (root->first != nullptr)
        SortTree(root->first);
    if (root->second != nullptr)
        SortTree(root->second);
    if (root->third != nullptr)
        SortTree(root->third);
    if (root->fourth != nullptr)
        SortTree(root->fourth);
}
void Converter::ClearTree(ConverterNode* root)
{
    if (root->first != nullptr)
        ClearTree(root->first);
    if (root->second != nullptr)
        ClearTree(root->second);
    if (root->third != nullptr)
        ClearTree(root->third);
    if (root->fourth != nullptr)
        ClearTree(root->fourth);
    vertices3D.push_back(root->vertex);
    delete root;
}
Point Converter::Spin(Point& root, float phi, float theta)
{

    Point result{};
    float r = 0.8;
    result.z = r * cos(theta) * sin(phi);
    result.x = r * sin(theta) * sin(phi);
    result.y = r * cos(phi);
    return result;
}

void Converter::AdvancedSpin(ConverterNode* root)
{

    const float r = 0.8;
    if (root->parent == nullptr)
    {
        switch (root->lastBond)
        {
        case 4:
            root->fourth->vertex.point.x = root->vertex.point.x + r * sin(0) * sin(0);
            root->fourth->vertex.point.y = root->vertex.point.y + r * cos(0);
            root->fourth->vertex.point.z = root->vertex.point.z + r * cos(0) * sin(0);
        case 3:
            root->third->vertex.point.x = root->vertex.point.x + r * sin(0) * sin(2.0944);
            root->third->vertex.point.y = root->vertex.point.y + r * cos(2.0944);
            root->third->vertex.point.z = root->vertex.point.z + r * cos(0) * sin(2.0944);
        case 2:
            root->second->vertex.point.x = root->vertex.point.x + r * sin(4.18879) * sin(2.0944);
            root->second->vertex.point.y = root->vertex.point.y + r * cos(2.0944);
            root->second->vertex.point.z = root->vertex.point.z + r * cos(4.18879) * sin(2.0944);
        case 1:
            root->first->vertex.point.x = root->vertex.point.x + r * sin(2.0944) * sin(2.0944);
            root->first->vertex.point.y = root->vertex.point.y + r * cos(2.0944);
            root->first->vertex.point.z = root->vertex.point.z + r * cos(2.0944) * sin(2.0944);
            break;
        }
    }
    else
    {

        if ((round(root->vertex.point.x * 100) == round(root->parent->vertex.point.x * 100)) && (round(root->vertex.point.z * 100) == round(root->parent->vertex.point.z * 100)))
        {
            if (root->vertex.point.y > root->parent->vertex.point.y)
            {
                if (root->third != nullptr)
                {
                    root->third->vertex.point.x = root->vertex.point.x + r * sin(3.14159) * sin(1.0472);
                    root->third->vertex.point.y = root->vertex.point.y + r * cos(1.0472);
                    root->third->vertex.point.z = root->vertex.point.z + r * cos(3.14159) * sin(1.0472);
                }


                if (root->second != nullptr)
                {
                    root->second->vertex.point.x = root->vertex.point.x + r * sin(-1.0472) * sin(1.0472);
                    root->second->vertex.point.y = root->vertex.point.y + r * cos(1.0472);
                    root->second->vertex.point.z = root->vertex.point.z + r * cos(-1.0472) * sin(1.0472);
                }

                if (root->first != nullptr)
                {
                    root->first->vertex.point.x = root->vertex.point.x + r * sin(1.0472) * sin(1.0472);
                    root->first->vertex.point.y = root->vertex.point.y + r * cos(1.0472);
                    root->first->vertex.point.z = root->vertex.point.z + r * cos(1.0472) * sin(1.0472);
                }
                return;
            }
            else
            {
                if (root->third != nullptr)
                {
                    root->third->vertex.point.x = root->vertex.point.x + r * sin(0) * sin(2.0944);
                    root->third->vertex.point.y = root->vertex.point.y + r * cos(2.0944);
                    root->third->vertex.point.z = root->vertex.point.z + r * cos(0) * sin(2.0944);
                }
                if (root->second != nullptr)
                {
                    root->second->vertex.point.x = root->vertex.point.x + r * sin(4.18879) * sin(2.0944);
                    root->second->vertex.point.y = root->vertex.point.y + r * cos(2.0944);
                    root->second->vertex.point.z = root->vertex.point.z + r * cos(4.18879) * sin(2.0944);
                }

                if (root->first != nullptr)
                {
                    root->first->vertex.point.x = root->vertex.point.x + r * sin(2.0944) * sin(2.0944);
                    root->first->vertex.point.y = root->vertex.point.y + r * cos(2.0944);
                    root->first->vertex.point.z = root->vertex.point.z + r * cos(2.0944) * sin(2.0944);
                }
                return;
            }
        }
        else
        {
            if ((root->vertex.point.x < root->parent->vertex.point.x) && (root->vertex.point.y < root->parent->vertex.point.y) && (root->vertex.point.z < root->parent->vertex.point.z))
            {
                if (root->third != nullptr)
                {
                    root->third->vertex.point.x = root->vertex.point.x + r * sin(0) * sin(3.14159);
                    root->third->vertex.point.y = root->vertex.point.y + r * cos(3.14159);
                    root->third->vertex.point.z = root->vertex.point.z + r * cos(0) * sin(3.14159);
                }
                if (root->second != nullptr)
                {
                    root->second->vertex.point.x = root->vertex.point.x + r * sin(3.14159) * sin(1.0472);
                    root->second->vertex.point.y = root->vertex.point.y + r * cos(1.0472);
                    root->second->vertex.point.z = root->vertex.point.z + r * cos(3.14159) * sin(1.0472);
                }

                if (root->first != nullptr)
                {

                    root->first->vertex.point.x = root->vertex.point.x + r * sin(-1.0472) * sin(1.0472);
                    root->first->vertex.point.y = root->vertex.point.y + r * cos(1.0472);
                    root->first->vertex.point.z = root->vertex.point.z + r * cos(-1.0472) * sin(1.0472);
                }
                return;
            }

            if ((root->vertex.point.x > root->parent->vertex.point.x) && (root->vertex.point.y < root->parent->vertex.point.y) && (root->vertex.point.z < root->parent->vertex.point.z))
            {
                if (root->third != nullptr)
                {
                    root->third->vertex.point.x = root->vertex.point.x + r * sin(0) * sin(3.14159);
                    root->third->vertex.point.y = root->vertex.point.y + r * cos(3.14159);
                    root->third->vertex.point.z = root->vertex.point.z + r * cos(0) * sin(3.14159);
                }

                if (root->second != nullptr)
                {
                    root->second->vertex.point.x = root->vertex.point.x + r * sin(3.14159) * sin(1.0472);
                    root->second->vertex.point.y = root->vertex.point.y + r * cos(1.0472);
                    root->second->vertex.point.z = root->vertex.point.z + r * cos(3.14159) * sin(1.0472);
                }
                if (root->first != nullptr)
                {

                    root->first->vertex.point.x = root->vertex.point.x + r * sin(1.0472) * sin(1.0472);
                    root->first->vertex.point.y = root->vertex.point.y + r * cos(1.0472);
                    root->first->vertex.point.z = root->vertex.point.z + r * cos(1.0472) * sin(1.0472);
                }
                return;
            }

            if ((root->vertex.point.x < root->parent->vertex.point.x) && (root->vertex.point.y > root->parent->vertex.point.y) && (root->vertex.point.z > root->parent->vertex.point.z))
            {
                if (root->third != nullptr)
                {
                    root->third->vertex.point.x = root->vertex.point.x + r * sin(0) * sin(0);
                    root->third->vertex.point.y = root->vertex.point.y + r * cos(0);
                    root->third->vertex.point.z = root->vertex.point.z + r * cos(0) * sin(0);
                }

                if (root->second != nullptr)
                {
                    root->second->vertex.point.x = root->vertex.point.x + r * sin(0) * sin(2.0944);
                    root->second->vertex.point.y = root->vertex.point.y + r * cos(2.0944);
                    root->second->vertex.point.z = root->vertex.point.z + r * cos(0) * sin(2.0944);
                }
                if (root->first != nullptr)
                {

                    root->first->vertex.point.x = root->vertex.point.x + r * sin(4.18879) * sin(2.0944);
                    root->first->vertex.point.y = root->vertex.point.y + r * cos(2.0944);
                    root->first->vertex.point.z = root->vertex.point.z + r * cos(4.18879) * sin(2.0944);
                }
                return;

            }
            if ((root->vertex.point.x > root->parent->vertex.point.x) && (root->vertex.point.y > root->parent->vertex.point.y) && (root->vertex.point.z > root->parent->vertex.point.z))
            {
                if (root->third != nullptr)
                {
                    root->third->vertex.point.x = root->vertex.point.x + r * sin(0) * sin(0);
                    root->third->vertex.point.y = root->vertex.point.y + r * cos(0);
                    root->third->vertex.point.z = root->vertex.point.z + r * cos(0) * sin(0);
                }

                if (root->second != nullptr)
                {
                    root->second->vertex.point.x = root->vertex.point.x + r * sin(0) * sin(2.0944);
                    root->second->vertex.point.y = root->vertex.point.y + r * cos(2.0944);
                    root->second->vertex.point.z = root->vertex.point.z + r * cos(0) * sin(2.0944);
                }
                if (root->first != nullptr)
                {

                    root->first->vertex.point.x = root->vertex.point.x + r * sin(2.0944) * sin(2.0944);
                    root->first->vertex.point.y = root->vertex.point.y + r * cos(2.0944);
                    root->first->vertex.point.z = root->vertex.point.z + r * cos(2.0944) * sin(2.0944);
                }
                return;
            }

            if ((round(root->vertex.point.x * 100) == round(root->parent->vertex.point.x * 100)) && (root->vertex.point.y < root->parent->vertex.point.y) && (root->vertex.point.z > root->parent->vertex.point.z))
            {
                if (root->third != nullptr)
                {
                    root->third->vertex.point.x = root->vertex.point.x + r * sin(0) * sin(3.14159);
                    root->third->vertex.point.y = root->vertex.point.y + r * cos(3.14159);
                    root->third->vertex.point.z = root->vertex.point.z + r * cos(0) * sin(3.14159);
                }

                if (root->second != nullptr)
                {
                    root->second->vertex.point.x = root->vertex.point.x + r * sin(1.0472) * sin(1.0472);
                    root->second->vertex.point.y = root->vertex.point.y + r * cos(1.0472);
                    root->second->vertex.point.z = root->vertex.point.z + r * cos(1.0472) * sin(1.0472);
                }
                if (root->first != nullptr)
                {
                    root->first->vertex.point.x = root->vertex.point.x + r * sin(-1.0472) * sin(1.0472);
                    root->first->vertex.point.y = root->vertex.point.y + r * cos(1.0472);
                    root->first->vertex.point.z = root->vertex.point.z + r * cos(-1.0472) * sin(1.0472);
                }
                return;

            }

            if ((round(root->vertex.point.x * 100) == round(root->parent->vertex.point.x * 100)) && (root->vertex.point.y > root->parent->vertex.point.y) && (root->vertex.point.z < root->parent->vertex.point.z))
            {
                if (root->third != nullptr)
                {
                    root->third->vertex.point.x = root->vertex.point.x + r * sin(0) * sin(0);
                    root->third->vertex.point.y = root->vertex.point.y + r * cos(0);
                    root->third->vertex.point.z = root->vertex.point.z + r * cos(0) * sin(0);
                }
                if (root->second != nullptr)
                {
                    root->second->vertex.point.x = root->vertex.point.x + r * sin(4.18879) * sin(2.0944);
                    root->second->vertex.point.y = root->vertex.point.y + r * cos(2.0944);
                    root->second->vertex.point.z = root->vertex.point.z + r * cos(4.18879) * sin(2.0944);
                }

                if (root->first != nullptr)
                {
                    root->first->vertex.point.x = root->vertex.point.x + r * sin(2.0944) * sin(2.0944);
                    root->first->vertex.point.y = root->vertex.point.y + r * cos(2.0944);
                    root->first->vertex.point.z = root->vertex.point.z + r * cos(2.0944) * sin(2.0944);
                }
                return;

            }

            if ((round(root->vertex.point.x * 100) == round(root->parent->vertex.point.x * 100)) && (root->vertex.point.y < root->parent->vertex.point.y) && (root->vertex.point.z < root->parent->vertex.point.z))
            {
                if (root->third != nullptr)
                {
                    root->third->vertex.point.x = root->vertex.point.x + r * sin(0) * sin(3.14159);
                    root->third->vertex.point.y = root->vertex.point.y + r * cos(3.14159);
                    root->third->vertex.point.z = root->vertex.point.z + r * cos(0) * sin(3.14159);
                }

                if (root->second != nullptr)
                {
                    root->second->vertex.point.x = root->vertex.point.x + r * sin(2.0944) * sin(1.0472);
                    root->second->vertex.point.y = root->vertex.point.y + r * cos(1.0472);
                    root->second->vertex.point.z = root->vertex.point.z + r * cos(2.0944) * sin(1.0472);
                }

                if (root->first != nullptr)
                {
                    root->first->vertex.point.x = root->vertex.point.x + r * sin(4.18879) * sin(1.0472);
                    root->first->vertex.point.y = root->vertex.point.y + r * cos(1.0472);
                    root->first->vertex.point.z = root->vertex.point.z + r * cos(4.18879) * sin(1.0472);
                }
                return;
            }

            if ((round(root->vertex.point.x * 100) == round(root->parent->vertex.point.x * 100)) && (root->vertex.point.y > root->parent->vertex.point.y) && (root->vertex.point.z > root->parent->vertex.point.z))
            {
                if (root->third != nullptr)
                {
                    root->third->vertex.point.x = root->vertex.point.x + r * sin(0) * sin(0);
                    root->third->vertex.point.y = root->vertex.point.y + r * cos(0);
                    root->third->vertex.point.z = root->vertex.point.z + r * cos(0) * sin(0);
                }

                if (root->second != nullptr)
                {
                    root->second->vertex.point.x = root->vertex.point.x + r * sin(1.0472) * sin(1.0472);
                    root->second->vertex.point.y = root->vertex.point.y + r * cos(1.0472);
                    root->second->vertex.point.z = root->vertex.point.z + r * cos(1.0472) * sin(1.0472);
                }

                if (root->first != nullptr)
                {
                    root->first->vertex.point.x = root->vertex.point.x + r * sin(-1.0472) * sin(1.0472);
                    root->first->vertex.point.y = root->vertex.point.y + r * cos(1.0472);
                    root->first->vertex.point.z = root->vertex.point.z + r * cos(-1.0472) * sin(1.0472);
                }
                return;
            }




            if ((root->vertex.point.x < root->parent->vertex.point.x) && (root->vertex.point.y > root->parent->vertex.point.y) && (root->vertex.point.z < root->parent->vertex.point.z))
            {
                if (root->third != nullptr)
                {
                    root->third->vertex.point.x = root->vertex.point.x + r * sin(0) * sin(0);
                    root->third->vertex.point.y = root->vertex.point.y + r * cos(0);
                    root->third->vertex.point.z = root->vertex.point.z + r * cos(0) * sin(0);
                }

                if (root->second != nullptr)
                {
                    root->second->vertex.point.x = root->vertex.point.x + r * sin(3.14159) * sin(2.0944);
                    root->second->vertex.point.y = root->vertex.point.y + r * cos(2.0944);
                    root->second->vertex.point.z = root->vertex.point.z + r * cos(3.14159) * sin(2.0944);
                }

                if (root->first != nullptr)
                {
                    root->first->vertex.point.x = root->vertex.point.x + r * sin(-1.0472) * sin(2.0944);
                    root->first->vertex.point.y = root->vertex.point.y + r * cos(2.0944);
                    root->first->vertex.point.z = root->vertex.point.z + r * cos(-1.0472) * sin(2.0944);
                }

                return;
            }
            if ((root->vertex.point.x > root->parent->vertex.point.x) && (root->vertex.point.y > root->parent->vertex.point.y) && (root->vertex.point.z < root->parent->vertex.point.z))
            {
                if (root->third != nullptr)
                {
                    root->third->vertex.point.x = root->vertex.point.x + r * sin(0) * sin(0);
                    root->third->vertex.point.y = root->vertex.point.y + r * cos(0);
                    root->third->vertex.point.z = root->vertex.point.z + r * cos(0) * sin(0);
                }

                if (root->second != nullptr)
                {
                    root->second->vertex.point.x = root->vertex.point.x + r * sin(3.14159) * sin(2.0944);
                    root->second->vertex.point.y = root->vertex.point.y + r * cos(2.0944);
                    root->second->vertex.point.z = root->vertex.point.z + r * cos(3.14159) * sin(2.0944);
                }

                if (root->first != nullptr)
                {
                    root->first->vertex.point.x = root->vertex.point.x + r * sin(1.0472) * sin(2.0944);
                    root->first->vertex.point.y = root->vertex.point.y + r * cos(2.0944);
                    root->first->vertex.point.z = root->vertex.point.z + r * cos(1.0472) * sin(2.0944);
                }

                return;
            }

            if ((root->vertex.point.x < root->parent->vertex.point.x) && (root->vertex.point.y < root->parent->vertex.point.y) && (root->vertex.point.z > root->parent->vertex.point.z))
            {
                if (root->third != nullptr)
                {
                    root->third->vertex.point.x = root->vertex.point.x + r * sin(0) * sin(3.14159);
                    root->third->vertex.point.y = root->vertex.point.y + r * cos(3.14159);
                    root->third->vertex.point.z = root->vertex.point.z + r * cos(0) * sin(3.14159);
                }

                if (root->second != nullptr)
                {
                    root->second->vertex.point.x = root->vertex.point.x + r * sin(4.18879) * sin(1.0472);
                    root->second->vertex.point.y = root->vertex.point.y + r * cos(1.0472);
                    root->second->vertex.point.z = root->vertex.point.z + r * cos(4.18879) * sin(1.0472);
                }
                if (root->first != nullptr)
                {
                    root->first->vertex.point.x = root->vertex.point.x + r * sin(0) * sin(1.0472);
                    root->first->vertex.point.y = root->vertex.point.y + r * cos(1.0472);
                    root->first->vertex.point.z = root->vertex.point.z + r * cos(0) * sin(1.0472);
                }
                return;
            }

            if ((root->vertex.point.x > root->parent->vertex.point.x) && (root->vertex.point.y < root->parent->vertex.point.y) && (root->vertex.point.z > root->parent->vertex.point.z))
            {
                if (root->third != nullptr)
                {
                    root->third->vertex.point.x = root->vertex.point.x + r * sin(0) * sin(3.14159);
                    root->third->vertex.point.y = root->vertex.point.y + r * cos(3.14159);
                    root->third->vertex.point.z = root->vertex.point.z + r * cos(0) * sin(3.14159);
                }

                if (root->second != nullptr)
                {
                    root->second->vertex.point.x = root->vertex.point.x + r * sin(2.0944) * sin(1.0472);
                    root->second->vertex.point.y = root->vertex.point.y + r * cos(1.0472);
                    root->second->vertex.point.z = root->vertex.point.z + r * cos(2.0944) * sin(1.0472);
                }

                if (root->first != nullptr)
                {
                    root->first->vertex.point.x = root->vertex.point.x + r * sin(0) * sin(1.0472);
                    root->first->vertex.point.y = root->vertex.point.y + r * cos(1.0472);
                    root->first->vertex.point.z = root->vertex.point.z + r * cos(0) * sin(1.0472);
                }
                return;
            }
        }

    }

}

void Converter::Tethrahedron(Element vertex)

{

    bool top = false;
    float degree;

    switch (vertex)
    {
    case Element::Carbon:

        top = true;
        degree = 1.9111355;  //109.5
        break;

    case Element::Phosphorus:
        degree = 1.623156; //93.5
        break;
    case Element::Nitrogen:
    default:
        degree = 1.8675023; //107.8
        break;

    }
    Point root, _1, _2, _3, _4;
    if (top)
    {
        root = { 0, 0, 0 };
        _1 = Spin(root, 0, 0);
        _2 = Spin(root, degree, 2.0944);
        _3 = Spin(root, degree, 4.18879);
        _4 = Spin(root, degree, 0);
    }
    else
    {
        root = { 0, 0, 0 };
        degree = cos(degree / 2);
        degree = pi - acos(sqrt(1 - 4 / 3 * degree * degree));
        _1 = Spin(root, degree, 2.0944);
        _2 = Spin(root, degree, 4.18879);
        _3 = Spin(root, degree, 0);
    }
    Vertex3D _point = { root, vertex };
    vertices3D.push_back(_point);
    _point.point = _1;
    _point.element = Element::Hydrogen;
    vertices3D.push_back(_point);
    _point.point = _2;
    vertices3D.push_back(_point);
    _point.point = _3;
    vertices3D.push_back(_point);
    if (top)
    {
        _point.point = _4;
        vertices3D.push_back(_point);
    }

}
void Converter::CentrifyCoordinates()
{
    float x, y, z;
    x = y = z = 0;
    for (unsigned int i = 0; i < vertices3D.size(); i++)
    {
        x += vertices3D[i].point.x;
        y += vertices3D[i].point.y;
        z += vertices3D[i].point.z;
    }
    x /= vertices3D.size();
    y /= vertices3D.size();
    z /= vertices3D.size();

    for (unsigned int i = 0; i < vertices3D.size(); i++)
    {
        vertices3D[i].point.x -= x;
        vertices3D[i].point.y -= y;
        vertices3D[i].point.z -= z;
    }
}
void Converter::Triangle(Element vertex)
{
    float degree;
    switch (vertex)
    {
    case Element::Sulfur:
        degree = 1.605703;
        break;
    default:
        degree = 1.81514242;
    }
    Vertex3D root = { {0, 0, 0}, vertex };
    vertices3D.push_back(root);
    float r = 0.8;
    Point _1{};
    _1.y = -r * cos(degree / 2);
    _1.z = 0;
    _1.x = r * sin(degree / 2);

    root.point = _1;
    root.element = Element::Hydrogen;
    vertices3D.push_back(root);
    Point _2;
    _2 = _1;
    _2.x = -_2.x;
    root.point = _2;
    vertices3D.push_back(root);
}

void Converter::Linear(Element vertex)
{
    Vertex3D _1 = { {-0.4, 0, 0}, vertex };
    vertices3D.push_back(_1);
    Vertex3D _2 = { {0.4, 0, 0}, Element::Hydrogen };
    vertices3D.push_back(_2);
}

bool Converter::IsSimple(std::vector<Vertex2D>& vertices2D)
{

    if (vertices2D.size() == 1)
    {

        switch (vertices2D[0].atom.element)
        {
        case Element::Carbon:
        case Element::Phosphorus:
        case Element::Nitrogen:
            
            Tethrahedron(vertices2D[0].atom.element);
            break;
        case Element::Sulfur:
        case Element::Oxygen:
            Triangle(vertices2D[0].atom.element);
            break;
        default:
            Linear(vertices2D[0].atom.element);
        }
        return true;
    }
    return false;

}