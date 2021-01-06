#ifndef SMOOTH_TRIANGLEFACET_H
#define SMOOTH_TRIANGLEFACET_H
#include "Smooth_Point.h"

class Smooth_TriangleFacet
{
public:
    Smooth_TriangleFacet();
    ~Smooth_TriangleFacet();
    bool hasVertice(unsigned findV);
    bool hasVertices(unsigned findV1, unsigned findV2);
    bool operator==(const Smooth_TriangleFacet& facet);
    Smooth_TriangleFacet &operator=(const Smooth_TriangleFacet& facet);
    union
    {
        unsigned int v[3];
        struct
        {
            unsigned int v1;
            unsigned int v2;
            unsigned int v3;
        };
    };
    Normal normal; // 面片的单位法矢
    struct POINT_ARRAY
    {
        std::string name;
        std::vector<double> arrayData;
    };
    std::vector<POINT_ARRAY> arrayDatas;

};

#endif // TRIANGLEFACET_H
