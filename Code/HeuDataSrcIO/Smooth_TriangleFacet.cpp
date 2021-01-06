#include "Smooth_TriangleFacet.h"

Smooth_TriangleFacet::Smooth_TriangleFacet()
{
    v1 = -1;
    v2 = -1;
    v3 = -1;
}

Smooth_TriangleFacet::~Smooth_TriangleFacet()
{
}

bool Smooth_TriangleFacet::hasVertice(unsigned findV)
{
    if(v1 == findV || v2 == findV || v3 == findV)
    {
        return true;
    } else {
        return false;
    }
}

bool Smooth_TriangleFacet::hasVertices(unsigned findV1, unsigned findV2)
{
    if(this->hasVertice(findV1) && this->hasVertice(findV2))
    {
        return true;
    } else {
        return false;
    }
}


bool Smooth_TriangleFacet::operator==(const Smooth_TriangleFacet& facet)
{
    if((this->v1 == facet.v1)&&(this->v2 == facet.v2)&&
       (this->v3 == facet.v3))
    {
        return true;
    } else {
        return false;
    }
}

Smooth_TriangleFacet &Smooth_TriangleFacet::operator=(const Smooth_TriangleFacet& facet)
{
//    LOGFUNC();
    v1 = facet.v1;
    v2 = facet.v2;
    v3 = facet.v3;
    normal = facet.normal;
    this->arrayDatas = facet.arrayDatas;
    return *this;
}
