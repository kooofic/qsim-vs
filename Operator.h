#ifndef OPERATOR_H_INCLUDED
#define OPERATOR_H_INCLUDED


namespace Qs
{

class Complex;

class Operator
{
protected:

public:

    Operator() {}

    virtual Complex Evaluate() = 0;
    virtual bool Simplify() = 0;

};

}

#endif // OPERATOR_H_INCLUDED
