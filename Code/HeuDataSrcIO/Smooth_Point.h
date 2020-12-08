#ifndef SMOOTH_POINT_H
#define SMOOTH_POINT_H
#include <set>
#include <vector>
#include <string>
typedef unsigned int uint;
class Smooth_QuadrangleMesh;
class Smooth_Point
{
public:
    friend class Smooth_QuadrangleMesh;
    Smooth_Point();
    Smooth_Point(double x_in, double y_in, double z_in, double w_in = 0);
    Smooth_Point(const Smooth_Point &p);
    Smooth_Point& operator=(const double* p);
    Smooth_Point& operator=(const Smooth_Point &p);
    bool operator==(const Smooth_Point& p);
    bool operator!=(const Smooth_Point& p);
    Smooth_Point operator+(const Smooth_Point& p) const;
    Smooth_Point operator-( const Smooth_Point& p );
    Smooth_Point operator-( void );
    Smooth_Point operator+=(const Smooth_Point& p);
    Smooth_Point& operator/(const double p);
    void doCopy(Smooth_Point &a);
    std::set<unsigned int> getAdjacentFacets() const;
    int getAdjacentFacetCount();
    void remove( unsigned int tri );
    unsigned int /*Smooth_Point::*/adjacentFacetCount() const;
    int at( unsigned int tri_index ) const;
    void add( unsigned int tri );
    bool belongtoTriangle( unsigned int tri );
    void clearadj();
    std::set<unsigned int> getPf();
    void setPf(std::set<unsigned int> &pf);
    static Smooth_Point crossProduct(const Smooth_Point &a, const Smooth_Point &b)
    {
        return Smooth_Point(
            a.y*b.z - a.z*b.y,
            a.z*b.x - a.x*b.z,
            a.x*b.y - a.y*b.x
        );
    }
    void Normalize();

protected:
    Smooth_QuadrangleMesh* mesh;
    std::set<unsigned int> pF;/**< 点所在的三角片 */

public:
    struct POINT_ARRAY
    {
        std::string name;
        std::vector<double> arrayData;
    };

    double x;
    double y;
    double z;
    double w;
    std::vector<POINT_ARRAY> arrayDatas;
    double normal[3];/**< 顶点法向量 */
    int number;/**< 点在点集中的序号 */
};
typedef Smooth_Point Normal;


#endif // SMOOTH_POINT_H
