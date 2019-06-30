#ifndef FRAC_H_INCLUDED
#define FRAC_H_INCLUDED

#include <stdexcept>
#include <ostream>

namespace Qs
{
class Frac
{
private:
    Frac();
public:

    int nom, den;

    Frac(int n, int d = 1);

    void Simplify();

    Frac const operator+(const Frac& f);
    Frac const operator+(const int f);
    Frac const operator-(const Frac& f);
    Frac const operator-(const int f);
    Frac const operator*(const Frac& f);
    Frac const operator/(const Frac& f);
    Frac const operator*(const int f);
    Frac const operator/(const int f);

    double const evaluate();

};

Frac operator+(const int i, const Frac& f);
Frac operator-(const int i, const Frac& f);
Frac operator*(const int i, const Frac& f);
Frac operator/(const int i, const Frac& f);
std::ostream& operator<<(std::ostream& os, const Frac& f);
int GCD(int a, int b);
int LCM(int a, int b);

}
#endif // FRAC_H_INCLUDED
