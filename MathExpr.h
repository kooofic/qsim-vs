#ifndef MATHEXPR_H_INCLUDED
#define MATHEXPR_H_INCLUDED

#include <iostream>
#include <stdexcept>
#include <stdlib.h>
#include <string>
#include <math.h>
#include <sstream>
#include "Frac.h"


namespace Qs {
struct Node
{
    char* value;
    int length;

    Node *left;
    Node *right;
    Node *parent;

    Node()
    {
        value = NULL;
        right = NULL;
        left = NULL;
        parent = NULL;
    }

    ~Node()
    {
        delete[] value;
        delete left;
        delete right;
    }

    void AddChild(Node* c, bool left);
    void DeleteChild(bool left);

};

struct SearchResult
{
    Node* srnode;
    int reverses;

    SearchResult()
    {
        srnode = NULL;
        reverses = 0;
    }
};

class MathExpr{
public:

    MathExpr();
    ~MathExpr();


    void CreateMathExpr(const char* expr);

    ///True  ==> Whole Expression
    ///False ==> NodeByNode
    void Print(bool fromat);
    void PrintOriginal() const;
    double Evaluate();

    MathExpr operator+(const MathExpr& me) const;
    MathExpr operator-(const MathExpr& me) const;
    MathExpr operator*(const MathExpr& me) const;
    MathExpr operator/(const MathExpr& me) const;
    MathExpr& operator=(const MathExpr& me);

    Node* NewNode(int val);
    Node* NewNode(char val);
    Node* NewNode(const char* val, int l);

    Node* AddNode(Node* parent, bool left, int val);
    Node* AddNode(Node* parent, bool left, char val);
    Node* AddNode(Node* parent, bool left, const char* val, int l);

    Node* ModifyNodeValue(Node* n, int val);
    Node* ModifyNodeValue(Node* n, char val);
    Node* ModifyNodeValue(Node* n, const char* val, int l);

    Node* SetupNode(Node* n, Node* le, Node* r, Node* p, int val);
    Node* SetupNode(Node* n, Node* le, Node* r, Node* p, char val);
    Node* SetupNode(Node* n, Node* l, Node* r, Node* p, const char* val, int le);

    Node* ResetNode(Node* n, Node* le, Node* r, Node* p, int val);
    Node* ResetNode(Node* n, Node* le, Node* r, Node* p, char val);
    Node* ResetNode(Node* n, Node* l, Node* r, Node* p, const char* val, int le);

	bool SimplifyAdd(Node* root, bool ret);
	bool SimplifyMin(Node* root, bool ret);
	bool SimplifyDiv(Node* root, bool ret);
	bool SimplifyMul(Node* root, bool ret);
	bool SimplifyPow(Node* root, bool ret);
	bool SimplifySqr(Node* root, bool ret);

	bool PrioritySimplifyAdd(Node* root, bool ret);
	bool PrioritySimplifyMin(Node* root, bool ret);
	bool PrioritySimplifyDiv(Node* root, bool ret);
	bool PrioritySimplifyMul(Node* root, bool ret);
	bool PrioritySimplifyPow(Node* root, bool ret);
	bool PrioritySimplifySqr(Node* root, bool ret);

    int NodeToInt(Node* n);

    void Simplify();

    int Length() const {return explength;}
    char* exp() const {return expression;}

private:

    Node* root = NULL;
    char* expression = NULL;
    int explength = 0;

    void build_tree(char* expr, int length, Node* n, Node* parent);

    void print_tree(Node* r);

    void print_node(Node* r, int lvl);

    double Evaluate(Node* r);

    ///Return:
    ///0 - OK
    ///-1 - BracketError
    ///-2 - OperationError, two operations after each other etc
    int SintaxCheck(char* expr, int length);

    ///Return false --> not needed brackets
    bool BracketNeeded(char* expr, int length);

    bool isNum(char c);
    ///negative sign (-)
    bool isUnary(char c);
    bool isBinaryOperator(char c);
    bool isBracket(char c);
    bool isImaginary(char c);

    bool isOnlyNumber(char* expr, int length);

    int OperatorPriority(char c);
    void destroy_tree();

    bool simp(Node* n);
    bool prioritySimp(Node* n);
    SearchResult searchForSimpable(Node* n, int op, int num, bool left, bool division, int r = 0);

    Node* deleteChildOtherIntoParent(Node* n, bool left);

    void build_string(std::string* s, Node* n);

    MathExpr Concatenate(const MathExpr& me, char c) const;
};



std::ostream& operator<<(std::ostream& os, const MathExpr& m);

}

#endif // MATHEXPR_H_INCLUDED
