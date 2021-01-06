#ifndef SMOOTH_QUADRANGLEMESH_H
#define SMOOTH_QUADRANGLEMESH_H

#include <queue>
#include "Smooth_Point.h"
#include "Smooth_QuadrangleFacet.h"
#include "kdtree.h"

class Smooth_QuadrangleMesh
{
public:
    Smooth_QuadrangleMesh();
    ~Smooth_QuadrangleMesh();
    void setData(const std::vector<Smooth_Point> &pts, const std::vector<Smooth_QuadrangleFacet>& fcs);
    void setData(const std::vector<Smooth_Point>& pts);
    int addPoint(Smooth_Point p);
    std::vector<Smooth_Point> &getpoints();
    std::vector<Smooth_QuadrangleFacet>& getFacets();
    unsigned int getFacetCount() const;
    Smooth_QuadrangleFacet& getFacetRef(unsigned int i);
    unsigned int getPointCount() const;
    Smooth_Point &getPointRef(unsigned int i);
    bool findNumber(unsigned int number);
    void clear();
    void resetData();

    void findAdjacentPoints( unsigned int number, std::set<unsigned int>& adjc, unsigned int level = 1);
    void findAllBorder(std::vector<std::vector<unsigned int>>& borders);
    void findMaxBorder(std::vector< unsigned int>& border);
    int findEdgeFacets(int startP, int endP, std::vector<unsigned int> &fs); // 查找边共用的两个面

    void removeRepetitivePoint(); // 删除重复点
    void resetNormal();

    void smoothPoints();
    void outPoint(std::vector<Smooth_Point> pts, std::string path);
    Smooth_QuadrangleMesh& operator=( const Smooth_QuadrangleMesh& msh );


public:
    std::vector<Smooth_Point> vertices;
    std::vector<Smooth_QuadrangleFacet> facets;
    kdtree* facetPointTree;

};

#endif // SMOOTH_QUADRANGLEMESH_H
