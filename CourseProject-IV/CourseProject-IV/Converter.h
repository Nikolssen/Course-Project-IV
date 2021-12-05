#pragma once
#include <vector>
#include <math.h>
#include <stdlib.h>
#include "Vertex2D.h"
#include "Vertex3D.h"
#include "ConverterNode.h"
#include "State.h"
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
    void MakeTree(ConverterNode* root, std::vector<Vertex2D>& vertices2D);
    void SortTree(ConverterNode* root);
    void InitCoordinates(ConverterNode* root);
    void ImplementCoordinates(ConverterNode* root);
    void ClearTree(ConverterNode* root);
    void CentrifyCoordinates();
    void AdvancedSpin(ConverterNode* root);

    std::vector<Vertex3D> vertices3D;
    const double pi = 3.14159265358979323846;
};

