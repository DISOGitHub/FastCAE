#include "Smooth_Point.h"
#include <math.h>

Smooth_Point::Smooth_Point()
{
    normal[0] = 0;
    normal[1] = 0;
    normal[2] = 0;
    x = 0;
    y = 0;
    z = 0;
    w = 0;
}

Smooth_Point::Smooth_Point(double x_in, double y_in, double z_in, double w_in)
{
    x = x_in;
    y = y_in;
    z = z_in;
    w = w_in;
    normal[0] = 0;
    normal[1] = 0;
    normal[2] = 0;
}

Smooth_Point::Smooth_Point(const Smooth_Point &p)
{
    this->x = p.x;
    this->y = p.y;
    this->z = p.z;
    this->w = p.w;
    this->arrayDatas = p.arrayDatas;
    this->normal[0] = p.normal[0];
    this->normal[1] = p.normal[1];
    this->normal[2] = p.normal[2];
}

Smooth_Point& Smooth_Point::operator=(const double* p)
{
    x = *p;
    y = *(p+1);
    z = *(p+2);
    w = 0;
    return *this;
}

Smooth_Point &Smooth_Point::operator=(const Smooth_Point &p)
{
//    LOGFUNC();
    x = p.x;
    y = p.y;
    z = p.z;
    w = p.w;
    pF = p.pF;
    this->arrayDatas = p.arrayDatas;
    return *this;
}

bool Smooth_Point::operator==(const Smooth_Point& p)
{
    const double MINNUM = 0.0001;
    bool xEqu = ((p.x > (x - MINNUM)) && (p.x < (x + MINNUM)));
    bool yEqu = ((p.y > (y - MINNUM)) && (p.y < (y + MINNUM)));
    bool zEqu = ((p.z > (z - MINNUM)) && (p.z < (z + MINNUM)));
    return xEqu && yEqu && zEqu;
    //return ((p.x==x)&&(p.y==y)&&(p.z==z));
}

bool Smooth_Point::operator!=(const Smooth_Point& p)
{
    return !(*this == p);
}

Smooth_Point Smooth_Point::operator+(const Smooth_Point& p) const     ///两点相加
{
    Smooth_Point pt(*this);
    pt.x += p.x;
    pt.y += p.y;
    pt.z += p.z;
    return pt;
}


Smooth_Point Smooth_Point::operator-( const Smooth_Point& p )
{
    Smooth_Point pt(*this);
    pt.x -= p.x;
    pt.y -= p.y;
    pt.z -= p.z;
    return pt;
}

Smooth_Point Smooth_Point::operator-( void )
{
    return Smooth_Point(-x,-y,-z);
}

Smooth_Point Smooth_Point::operator+=(const Smooth_Point& p)     ///两点相加
{
    x += p.x;
    y += p.y;
    z += p.z;
    return *this;
}

Smooth_Point& Smooth_Point::operator/(const double p)
{
    x /= p;
    y /= p;
    z /= p;
    return *this;
}

void Smooth_Point::doCopy(Smooth_Point &a)
{
    x = a.x;
    y = a.y;
    z = a.z;
    w = a.w;
    number=a.number;
    pF = a.pF;
    mesh = a.mesh;
    this->arrayDatas = a.arrayDatas;
//    normal = a.normal;
}

std::set<unsigned int> Smooth_Point::getAdjacentFacets() const
{
    return pF;
}

int Smooth_Point::at( unsigned int tri_index ) const
{
    std::set<unsigned int>::iterator it = pF.begin();
    for (unsigned int i = 0; i < tri_index; i++) {it++;}
    return *it;
}

int Smooth_Point::getAdjacentFacetCount()
{
    return pF.size();
}

void Smooth_Point::remove( unsigned int tri )
{
    pF.erase(tri);
}

unsigned int Smooth_Point::adjacentFacetCount() const
{
    return pF.size();
}

void Smooth_Point::add( unsigned int tri )
{
    pF.insert(tri);
}

bool Smooth_Point::belongtoTriangle( unsigned int tri )
{
    return (pF.end() != pF.find(tri));
}

void Smooth_Point::clearadj()
{
    pF.clear();
}

std::set<unsigned int> Smooth_Point::getPf()
{
    return pF;
}

void Smooth_Point::setPf(std::set<unsigned int> &pf)
{
    pF = pf;
}

void Smooth_Point::Normalize()
{
    double magSq = x*x + y*y + z*z;
    if (magSq > 0.0f)
    { // check for divide-by-zero
        double oneOverMag = 1.0f / sqrt(magSq);
        x *= oneOverMag;
        y *= oneOverMag;
        z *= oneOverMag;
    }
}
