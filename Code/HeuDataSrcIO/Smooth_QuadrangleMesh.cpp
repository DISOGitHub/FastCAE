#include "Smooth_QuadrangleMesh.h"
#include "Smooth_Sparse_vector.h"
#include <fstream>

Smooth_QuadrangleMesh::Smooth_QuadrangleMesh()
{
    facetPointTree = kd_create(3);
}

Smooth_QuadrangleMesh::~Smooth_QuadrangleMesh()
{
    kd_free(facetPointTree);
}
#include <QDebug>
void Smooth_QuadrangleMesh::setData(const std::vector<Smooth_Point> &pts, const std::vector<Smooth_QuadrangleFacet> &fcs)
{
    vertices.clear();
    facets.clear();
    vertices.resize(pts.size());
    facets.resize(fcs.size());

    for(unsigned int i = 0; i < pts.size(); i++)
    {
        vertices[i] = pts[i];
        vertices[i].clearadj();

    }
    for(unsigned int i = 0; i < facets.size(); i++)
    {
        facets[i].v1 = fcs[i].v1;
        facets[i].v2 = fcs[i].v2;
        facets[i].v3 = fcs[i].v3;
        facets[i].v4 = fcs[i].v4;
        facets[i].arrayDatas = fcs[i].arrayDatas;
        Smooth_QuadrangleFacet& f = facets[i];
        for(unsigned int j = 0; j < 4; j++)
        {
            vertices[f.v[j]].add(i);
        }
    }
    removeRepetitivePoint();
    for(unsigned int i = 0; i < facets.size(); i++)
    {
        Smooth_QuadrangleFacet& f = facets[i];
        for(unsigned int j = 0; j < 4; j++)
        {
            vertices[f.v[j]].add(i);
        }
    }


    resetNormal();

}

void Smooth_QuadrangleMesh::setData(const std::vector<Smooth_Point> &pts)
{
    vertices = pts;
}

int Smooth_QuadrangleMesh::addPoint(Smooth_Point p)
{
    vertices.push_back(p);
    return vertices.size() - 1;
}


std::vector<Smooth_Point>& Smooth_QuadrangleMesh::getpoints()
{
    return vertices;
}

std::vector<Smooth_QuadrangleFacet> &Smooth_QuadrangleMesh::getFacets()
{
    return facets;
}

unsigned int Smooth_QuadrangleMesh::getFacetCount() const
{
    return facets.size();
}

Smooth_QuadrangleFacet &Smooth_QuadrangleMesh::getFacetRef(unsigned int i)
{
    return facets[i];
}

unsigned int Smooth_QuadrangleMesh::getPointCount() const
{
    return vertices.size();
}

Smooth_Point &Smooth_QuadrangleMesh::getPointRef(unsigned int i)
{
    return vertices[i];
}

bool Smooth_QuadrangleMesh::findNumber(unsigned int number)
{
    if (number >= vertices.size())
        return false;
    else
        return true;
}

void Smooth_QuadrangleMesh::clear()
{
    vertices.clear();
    facets.clear();
    kd_clear(facetPointTree);

}

void Smooth_QuadrangleMesh::resetData()
{
    std::vector<Smooth_Point> newVertices;
    std::vector<Smooth_QuadrangleFacet> newFacets;
    std::map<unsigned, unsigned> verticesMap;

    for(size_t i = 0; i < vertices.size(); i++)
    {
        Smooth_Point& currPoint = vertices[i];
        if(currPoint == Smooth_Point(0, 0, 0))
        {
            continue;
        }
        newVertices.push_back(currPoint);
        verticesMap[i] = newVertices.size() - 1;
    }
    newFacets = facets;
    for(size_t i = 0; i < newFacets.size(); i++)
    {
        Smooth_QuadrangleFacet& facet = newFacets[i];
        for(unsigned j = 0; j < 4; j++)
        {
            unsigned verticeIndex = facet.v[j];
            facet.v[j] = verticesMap[verticeIndex];
        }
    }
    this->setData(newVertices, newFacets);

	
}

void Smooth_QuadrangleMesh::resetNormal()
{
    for(size_t i = 0; i < facets.size(); i++)
    {
        Smooth_QuadrangleFacet& f = facets[i];
        Smooth_Point& p1 = vertices[f.v1];
        Smooth_Point& p2 = vertices[f.v2];
        Smooth_Point& p3 = vertices[f.v3];
        Normal n1 = p1 - p2;
        Normal n2 = p3 - p2;
        Smooth_Point v = Smooth_Point::crossProduct(n2, n1);
        f.normal.x = v.x;
        f.normal.y = v.y;
        f.normal.z = v.z;
    }
    for(size_t i = 0; i < vertices.size(); i++)
    {
        Smooth_Point& p = vertices[i];
        int count = p.adjacentFacetCount();
        Normal normal(0,0,0);
        for(int j = 0; j < count; ++j)
        {
            Smooth_QuadrangleFacet& f = facets[p.at(j)];
            normal = normal + f.normal;
        }
        normal = normal / p.adjacentFacetCount();
        normal.Normalize();
        p.normal[0] = normal.x;
        p.normal[1] = normal.y;
        p.normal[2] = normal.z;
    }
}
//#include "QDebug"
void Smooth_QuadrangleMesh::smoothPoints()
{
    std::vector<std::vector<uint>> borders;
    findAllBorder(borders);
//    qDebug("borders.size = %d", borders.size());
    Smooth_Sparse_vector<bool> mark(false, getPointCount());

    // 所有边界不参与平滑
    for(size_t i = 0; i < borders.size(); i++)
    {
        std::vector<uint> border = borders[i];
        std::vector<Smooth_Point> points;
        for(size_t j = 0; j < border.size(); j++)
        {
            mark[border[j]] = true;
            Smooth_Point p = vertices[border[j]];
            points.push_back(p);
        }
//        if(i < 10)
//        {
//            QString ss = QString::number(i);
//            ss = "points" + ss + ".txt";
//            outPoint(points, ss.toStdString());
//        }
    }
    // 四边形是三角形的不参与平滑

    for(size_t i = 0; i < facets.size(); i++)
    {
        Smooth_QuadrangleFacet& facet = facets[i];
        if(facet.v1 == facet.v2 || facet.v1 == facet.v3 || facet.v1 == facet.v4)
        {
            mark[facet.v1] = true;
        } else if(facet.v2 == facet.v1 || facet.v2 == facet.v3 || facet.v2 == facet.v4) {
            mark[facet.v2] = true;
        } else if(facet.v3 == facet.v1 || facet.v3 == facet.v2 || facet.v3 == facet.v4) {
            mark[facet.v3] = true;
        } else if(facet.v4 == facet.v1 || facet.v4 == facet.v2 || facet.v4 == facet.v3){
            mark[facet.v4] = true;
        }
    }
    for(int i = 0; i < vertices.size(); i++)
    {
        Smooth_Point& p = vertices[i];
        if(mark[i])
        {
            continue;
        }

        std::set<unsigned int> adjc;
        findAdjacentPoints(i, adjc);
        Smooth_Point smoothP(0, 0, 0);
        for(auto iter = adjc.begin(); iter != adjc.end(); iter++)
        {
            Smooth_Point& pp = vertices[*iter];
            smoothP = smoothP + pp;
        }
        smoothP = smoothP/adjc.size();
        p.x = smoothP.x;
        p.y = smoothP.y;
        p.z = smoothP.z;
    }
    resetNormal();
}

void Smooth_QuadrangleMesh::outPoint(std::vector<Smooth_Point> pts, std::string path)
{
    std::ofstream outf(path, std::ios::trunc);
        for(unsigned int k = 0; k < pts.size(); ++k)
        {
            outf << pts[k].x << " " << pts[k].y << " " << pts[k].z << " " << 1 << std::endl;
        }
    outf.close();
}

void Smooth_QuadrangleMesh::findAdjacentPoints( unsigned int number, std::set<unsigned int>& adjc, unsigned int level )
{
    if(level <= 0)
    {
        return;
    }
    static Smooth_Sparse_vector<bool> processed(false, getPointCount());
    static unsigned int toplevel = 0;

    if(level > toplevel)
    {
        toplevel = level;
    }

    processed[number] = true;
    Smooth_Point& p = getPointRef(number);
    for (unsigned int i = 0; i < p.adjacentFacetCount(); i++)
    {
        Smooth_QuadrangleFacet& f = getFacetRef(p.at(i));
        for (unsigned int j = 0; j < 4; j++)
        {
            if(f.isDiagonal(number, f.v[j]))
            {
                continue;
            }
            if (false == processed[f.v[j]])
            {
                adjc.insert(f.v[j]);
                findAdjacentPoints(f.v[j], adjc, level-1);
            }
        }
    }

    if (level == toplevel)
    {
        adjc.erase(number);
        toplevel = 0;
        processed.clear();
    }
}

void Smooth_QuadrangleMesh::findAllBorder(std::vector<std::vector<unsigned int> > &borders)
{
    Smooth_Sparse_vector<bool> borderMark(false, getPointCount());
    Smooth_Sparse_vector<bool> mark(false, getPointCount());
    for(size_t i = 0; i < getPointCount(); i++)
    {
        mark[i] = true;
        std::set<unsigned int> adjPt;
        this->findAdjacentPoints(i, adjPt);
        for(std::set< unsigned int>::iterator it = adjPt.begin(); it != adjPt.end(); it++)
        {
            int adjIndex = *it;
            if(mark[adjIndex])
            {
                continue;
            }
            std::vector< unsigned int> fs;
            int count = findEdgeFacets(i, adjIndex, fs);

            if(count == 1)
            {
                borderMark[i] = true;
                borderMark[adjIndex] = true;
            }

        }
    }
    Smooth_Sparse_vector<bool> mark2(false, getPointCount());
    for(size_t i = 0; i < getPointCount(); i++)
    {
        if(mark2[i])
        {
            continue;
        }
        if(borderMark[i]) // 边界点
        {
            std::vector< unsigned int> border;
            border.push_back(i);
            mark2[i] = true;

            int theIndex = i;
            bool v = true;
            while(v)
            {
                v = false;
                std::set< unsigned int> adjPt;
                findAdjacentPoints(theIndex, adjPt);
                for(std::set< unsigned int>::iterator it = adjPt.begin(); it != adjPt.end(); it++)
                {
                    if(mark2[*it])
                    {
                        continue;
                    }
                    if(borderMark[*it])
                    {
                        std::vector<uint> fs;
                        int count = findEdgeFacets(theIndex, *it, fs);
                        if(count == 1)
                        {
                            border.push_back(*it);
                            mark2[*it] = true;
                            theIndex = *it;
                            v = true;
                            break;
                        }
                    }
                }
            }

            bool anticlockwise = false;

            if(border.size() > 1)
            {
                int index1 = border[0];
                int index2 = border[1];
                std::vector<uint> fs;
                findEdgeFacets(index1, index2, fs);
                Smooth_QuadrangleFacet& f = this->getFacetRef(fs[0]);
                if(index1 == f.v1 && index2 == f.v2)
                {
                    anticlockwise = true;
                } else if(index1 == f.v2 && index2 == f.v3) {
                    anticlockwise = true;
                } else if(index1 == f.v3 && index2 == f.v4) {
                    anticlockwise = true;
                } else if(index1 == f.v4 && index2 == f.v1) {
                    anticlockwise = true;
                } else {
                    anticlockwise = false;
                }
            }
            if(anticlockwise)
            {
                borders.push_back(border);
            } else {
                std::vector<unsigned> tempBorder;
                for(int i = border.size() - 1; i >= 0; i--)
                {
                    tempBorder.push_back(border[i]);
                }
                borders.push_back(tempBorder);
            }

        }
    }

}

void Smooth_QuadrangleMesh::findMaxBorder(std::vector<unsigned int> &border)
{
    std::vector<std::vector<uint> > borders;
    findAllBorder(borders);
    int maxCount = 0;
    for(size_t i = 0; i < borders.size(); i++)
    {
        std::vector<uint>& points = borders[i];
        if(points.size() > maxCount)
        {
            maxCount = points.size();
            border = points;
        }
    }
}

int Smooth_QuadrangleMesh::findEdgeFacets(int startP, int endP, std::vector< unsigned int> &fs)
{
    Smooth_Sparse_vector<bool> mark(false, getFacetCount());
    Smooth_Point& p1 = getPointRef(startP);
    Smooth_Point& p2 = getPointRef(endP);
    for(size_t i = 0; i < p1.adjacentFacetCount(); i++)
    {
        mark[p1.at(i)] = true;
    }
    for(size_t i = 0; i < p2.adjacentFacetCount(); i++)
    {
        if(mark[p2.at(i)])
        {
            fs.push_back(p2.at(i));
        }
    }
    return fs.size();

}

void Smooth_QuadrangleMesh::removeRepetitivePoint()
{
//    qDebug("vertices = %d", vertices.size());
    std::vector<Smooth_Point> newPoints;
    kd_clear(facetPointTree);
    std::map<uint, uint> newOldMap;
    for(size_t i = 0; i < vertices.size(); i++)
    {
        Smooth_Point& p = vertices[i];
        int newIndex = -1;
        kdres *res = kd_nearest3(facetPointTree,p.x,p.y,p.z);
        if(NULL != res)
        {
//            unsigned int pi = (unsigned int)kd_res_item_data(res);
			void *pivoid = kd_res_item_data(res);
			int *pi = static_cast<int*>(pivoid);
            Smooth_Point pp = newPoints.at(*pi);
            double p2 = (p.x - pp.x)*(p.x - pp.x) + (p.y - pp.y)*(p.y - pp.y) + (p.z - pp.z)*(p.z - pp.z);
            if(p2 < 0.0001)
            {
                newIndex = *pi;
            } else {
                newIndex = newPoints.size();
                kd_insert3(facetPointTree,p.x,p.y,p.z,(void*)newPoints.size());
                newPoints.push_back(p);
            }
            kd_res_free(res);
        } else {
            newIndex = newPoints.size();
            kd_insert3(facetPointTree,p.x,p.y,p.z,(void*)newPoints.size());
            newPoints.push_back(p);
        }
        newOldMap[i] = newIndex;
    }
    for(size_t i = 0; i < facets.size(); i++)
    {
        Smooth_QuadrangleFacet& f = facets[i];
        for(int j = 0; j < 4; j++)
        {
            f.v[j] = newOldMap[f.v[j]];
        }
    }
    vertices = newPoints;
//    qDebug("newPoints = %d", newPoints.size());
}

Smooth_QuadrangleMesh& Smooth_QuadrangleMesh::operator=( const Smooth_QuadrangleMesh& msh )
{
//    LOGFUNC();
    vertices = msh.vertices;
    facets = msh.facets;
    return *this;
}
