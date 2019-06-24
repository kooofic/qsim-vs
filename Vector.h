#ifndef VECTOR_H_INCLUDED
#define VECTOR_H_INCLUDED

#include <ostream>
#include "MathExpr.h"

class Vector
{
    MathExpr* vec;
    int size;

public:

    Vector(int n);
    ~Vector();

    int Size() const { return size;}

    Vector operator+(const Vector& v) const;
    Vector operator-(const Vector& v) const;
    MathExpr operator*(const Vector& v) const;

    Vector operator*(const MathExpr& m) const;
    Vector operator/(const MathExpr& m) const;

    MathExpr& operator[](const int index) const;
};

std::ostream & operator << (std::ostream &out, const Vector &v);


#endif // VECTOR_H_INCLUDED
