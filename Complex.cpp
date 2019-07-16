#include "Complex.h"
#include <Math.h>

namespace Qs
{

double Complex::abs()
{
    return sqrt(r*r + i*i);
}

Complex Complex::operator+(const Complex& c)
{
    return Complex(r + c.r, i + c.i);
}

Complex Complex::operator-(const Complex& c)
{
    return Complex(r - c.r, i - c.i);
}

Complex Complex::operator*(const Complex& c)
{
    return Complex(r * c.r - (i * c.i), i*c.r + r*c.i);
}


}
