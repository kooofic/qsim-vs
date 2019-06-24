#ifndef COMPLEX_H_INCLUDED
#define COMPLEX_H_INCLUDED

class Complex{

    double r;
    double i;

    public :
        Complex();
		Complex(double re, double im = 0) { r = re; i = im;  }

        double abs();
        Complex operator+(const Complex& c);
        Complex operator-(const Complex& c);
        Complex operator*(const Complex& c);


};

Complex operator*(const double d, const Complex& c);
Complex operator*(const Complex& c, const double d);
Complex operator*(const float d, const Complex& c);
Complex operator*(const Complex& c, const float d);
Complex operator*(const int d, const Complex& c);
Complex operator*(const Complex& c, const int d);

#endif // COMPLEX_H_INCLUDED
