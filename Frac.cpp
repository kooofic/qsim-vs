#include "Frac.h"

namespace Qs
{
Frac::Frac(int n, int d)
{
    this->nom = n;

    if(d != 0)
    {
        this->den = d;
    }
    else
    {
            throw std::invalid_argument("Denominator cannot be zero!");
    }
}
/*
void Frac::Simplify()
{
    int gcd = GCD(this->nom, this->den);

    this->nom = this->nom/gcd;
    this->den = this->den/gcd;
}*/

int GCD(int a, int b)
{
    int temp;
    while(b != 0)
    {
        temp = b;
        b = a%b;
        a = temp;
    }

    return a;
}

int LCM(int a, int b)
{
    int gcd = GCD(a,b);
    if(gcd == 0)
    {
        throw std::invalid_argument( "GCD is zero so LCM divides by zero!");
        return 0;
    }
    return a*b / gcd;
}


Frac const Frac::operator+(const Frac& f)
{
    int lcm = LCM(this->den, f.den);

    int onemult = lcm/f.den;
    int twomult = lcm/this->den;

    Frac ret(onemult*this->nom + twomult*f.nom, lcm);
    ret.Simplify();

    return ret;
}

Frac const Frac::operator+(const int f)
{
    Frac iff(f);

    return *(this) + iff;

}

Frac const Frac::operator-(const Frac& f)
{
    int lcm = LCM(this->den, f.den);

    int onemult = lcm/f.den;
    int twomult = lcm/this->den;

    Frac ret(onemult*this->nom - twomult*f.nom, lcm);

    ret.Simplify();

    return ret;
}


Frac const Frac::operator-(const int f)
{
    Frac iff(f);

    return *(this) - iff;

}

Frac const Frac::operator*(const Frac& f)
{
    Frac ret(this->nom*f.nom, this->den*f.den);

    ret.Simplify();

    return ret;
}

Frac const Frac::operator/(const Frac& f)
{

    if(f.nom == 0)
    {
        throw std::invalid_argument( "Nominator is zero so you divide by zero!");
        return 0;
    }

    Frac ret(this->nom*f.den, this->den*f.nom);

    ret.Simplify();

    return ret;
}

Frac const Frac::operator*(const int f)
{
    Frac ret(this->nom * f, this->den);

    ret.Simplify();

    return ret;
}

Frac const Frac::operator/(const int f)
{
    if(f == 0)
    {
        throw std::invalid_argument( "Divider is zero so you divide by zero!");
        return 0;
    }
    Frac ret(this->nom, this->den * f);

    ret.Simplify();

    return ret;
}

/*
double const Frac::evaluate()
{
    return (double)nom/(double)den;
}*/

Frac operator+(const int i, const Frac& f)
{
    Frac ret(f.den*i + f.nom, f.den);
    ret.Simplify();

    return ret;
}

Frac operator-(const int i, const Frac& f)
{
    Frac ret(f.den*i - f.nom, f.den);
    ret.Simplify();

    return ret;
}

Frac operator*(const int i, const Frac& f)
{
    Frac ret(f.nom*i, f.den);

    ret.Simplify();

    return ret;
}

Frac operator/(const int i, const Frac& f)
{
    if(f.nom == 0)
    {
        throw std::invalid_argument( "Nominator is zero so you divide by zero!");
        return 0;
    }

    Frac ret(f.den*i, f.nom);

    ret.Simplify();

    return ret;
}


std::ostream& operator<<(std::ostream& os, const Frac& f)
{
    os << f.nom << '/' << f.den;
    return os;
}


}
