#ifndef FRAC_H_INCLUDED
#define FRAC_H_INCLUDED

#include "Complex.h"
#include <stdexcept>
#include <ostream>

namespace Qs
{
class Frac : public Operator
{
private:
    Frac();
public:

    Operator * nom, den;

    Frac(Operator * n, Operator * d);

    bool Simplify()
    {
        return true;
    }


    Complex Evaluate()
    {
        return Complex();
    }

    Frac const operator+(const Frac& f);
    Frac const operator+(const int f);
    Frac const operator-(const Frac& f);
    Frac const operator-(const int f);
    Frac const operator*(const Frac& f);
    Frac const operator/(const Frac& f);
    Frac const operator*(const int f);
    Frac const operator/(const int f);


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
