#ifndef SMOOTH_TRIANGLEMESH_H
#define SMOOTH_TRIANGLEMESH_H
#include <queue>
#include "Smooth_Point.h"
#include "Smooth_TriangleFacet.h"
#include "kdtree.h"

class Smooth_TriangleMesh
{
public:
    Smooth_TriangleMesh();
    ~Smooth_TriangleMesh();
    void setData(const std::vector<Smooth_Point> &pts, const std::vector<Smooth_TriangleFacet>& fcs);
    void setData(const std::vector<Smooth_Point>& pts);
    int addPoint(Smooth_Point p);
    std::vector<Smooth_Point> &getpoints();
    std::vector<Smooth_TriangleFacet>& getFacets();
    unsigned int getFacetCount() const;
    Smooth_TriangleFacet& getFacetRef(unsigned int i);
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
    void initKdtree();
    void findRangeIndexKdtree(Smooth_Point p, double radius, std::vector<unsigned int> &points);

    void smoothPoints(); // 平滑所有点边界不平滑
    void smoothBorder_Z(); // 所有边界按照Z方向进行平滑
    void outPoint(std::vector<Smooth_Point> pts, std::string path);
    Smooth_TriangleMesh& operator=( const Smooth_TriangleMesh& msh );


public:
    std::vector<Smooth_Point> vertices;
    std::vector<Smooth_TriangleFacet> facets;
    kdtree* facetPointTree;

};

#endif // TRIANGLEMESH_H
