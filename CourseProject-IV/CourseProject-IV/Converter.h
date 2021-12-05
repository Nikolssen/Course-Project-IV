#pragma once
#include <vector>
#include <math.h>
#include <stdlib.h>
#include "Vertex2D.h"
#include "Vertex3D.h"
#include "ConverterNode.h"
class Converter
{
public:
    Converter();
    ~Converter();
    std::vector<Vertex3D> ConvertToCalotte(std::vector<Vertex2D>& vertices2D);


private:
    void Tethrahedron(Element vertex);
    void Triangle(Element vertex);
    void Linear(Element vertex);
    bool IsSimple(std::vector<Vertex2D>& vertices2D);
    Point Spin(Point& root, float phi, float theta);
    char GetPossibleBonds(Element activeElement);
    void MakeTree(ConverterNode* root, std::vector<Vertex2D>& vertices2D);
    void SortTree(ConverterNode* root);
    void InitCoordinates(ConverterNode* root);
    void ImplementCoordinates(ConverterNode* root);
    void ClearTree(ConverterNode* root);
    void CentrifyCoordinates();
    void AdvancedSpin(ConverterNode* root);

    std::vector<Vertex3D> vertices3D;
};

