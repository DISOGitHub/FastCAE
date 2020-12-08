#include "Smooth_QuadrangleFacet.h"

Smooth_QuadrangleFacet::Smooth_QuadrangleFacet()
{
    v1 = -1;
    v2 = -1;
    v3 = -1;
    v4 = -1;
}

Smooth_QuadrangleFacet::~Smooth_QuadrangleFacet()
{
}

bool Smooth_QuadrangleFacet::hasVertice(unsigned findV)
{
    if(v1 == findV || v2 == findV || v3 == findV || v4 == findV)
    {
        return true;
    } else {
        return false;
    }
}

bool Smooth_QuadrangleFacet::hasVertices(unsigned findV1, unsigned findV2)
{
    if(this->hasVertice(findV1) && this->hasVertice(findV2))
    {
        return true;
    } else {
        return false;
    }
}

bool Smooth_QuadrangleFacet::isDiagonal(unsigned findV1, unsigned findV2)
{
    if(findV1 == v1 && findV2 == v3)
    {
        return true;
    }
    if(findV1 == v2 && findV2 == v4)
    {
        return true;
    }
    if(findV1 == v3 && findV2 == v1)
    {
        return true;
    }
    if(findV1 == v4 && findV2 == v2)
    {
        return true;
    }
    return false;
}

bool Smooth_QuadrangleFacet::operator==(const Smooth_QuadrangleFacet& facet)
{
    if((this->v1 == facet.v1)&&(this->v2 == facet.v2)&&
       (this->v3 == facet.v3)&&(this->v4 == facet.v4))
    {
        return true;
    } else {
        return false;
    }
}

Smooth_QuadrangleFacet &Smooth_QuadrangleFacet::operator=(const Smooth_QuadrangleFacet& facet)
{
//    LOGFUNC();
    v1 = facet.v1;
    v2 = facet.v2;
    v3 = facet.v3;
    v4 = facet.v4;
    normal = facet.normal;
    this->arrayDatas = facet.arrayDatas;
    return *this;
}



