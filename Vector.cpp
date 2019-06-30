#include "Vector.h"

namespace Qs
{

Vector::Vector(int n)
{
    vec = (MathExpr*)malloc(n*sizeof(MathExpr));

    size = n;
}

Vector::~Vector()
{
    free(vec);
}

Vector Vector::operator+(const Vector& v) const
{
    Vector r(size);
    for(int i = 0; i < size; i++)
    {
        r.vec[i] = this->vec[i]+v.vec[i];
    }
    return r;
}

Vector Vector::operator-(const Vector& v) const
{
    Vector r(size);
    for(int i = 0; i < size; i++)
    {
        r.vec[i] = this->vec[i]-v.vec[i];
    }
    return r;
}

MathExpr Vector::operator*(const Vector& v) const
{ ///TODO
    MathExpr m;
    Vector r(size);
    for(int i = 0; i < size; i++)
    {
        r.vec[i] = this->vec[i]+v.vec[i];
    }
    return m;
}


Vector Vector::operator*(const MathExpr& m) const
{
    Vector r(size);

    for(int i = 0; i < size; i++)
    {
        r.vec[i] = this->vec[i]*m;
    }
    return r;
}

Vector Vector::operator/(const MathExpr& m) const
{
    Vector r(size);
    for(int i = 0; i < size; i++)
    {
        r.vec[i] = this->vec[i]/m;
    }
    return r;
}

MathExpr& Vector::operator[](const int index) const
{
    return vec[index];
}

std::ostream& operator<<(std::ostream &out, const Vector &v)
{
    out << '(';
    for(int i = 0; i < v.Size(); i++)
    {
        v[i].PrintOriginal();
        if(i != v.Size() - 1 )out << ',';
    }

    out << ')';

    return out;
}

}
