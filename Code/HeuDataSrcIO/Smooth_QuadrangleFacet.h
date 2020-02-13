#ifndef SMOOTH_QUADRANGLEFACET_H
#define SMOOTH_QUADRANGLEFACET_H
#include "Smooth_Point.h"
#include <set>

class Smooth_QuadrangleFacet
{
public:
    Smooth_QuadrangleFacet();
    ~Smooth_QuadrangleFacet();
    bool hasVertice(unsigned findV);
    bool hasVertices(unsigned findV1, unsigned findV2);
    bool isDiagonal(unsigned findV1, unsigned findV2); // 两个点是否是对角线
    bool operator==(const Smooth_QuadrangleFacet& facet);
    Smooth_QuadrangleFacet &operator=(const Smooth_QuadrangleFacet& facet);
    union
    {
        unsigned int v[4]; // 四边形面片的四个顶点在vector中的位置
        struct
        {
            unsigned int v1;
            unsigned int v2;
            unsigned int v3;
            unsigned int v4;
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

#endif // SMOOTH_QUADRANGLEFACET_H
