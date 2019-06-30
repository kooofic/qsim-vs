#include "MathExpr.h"

namespace Qs
{

MathExpr::MathExpr()
{
    root = NULL;
}

MathExpr::~MathExpr()
{
    delete[] expression;
    delete root;
}

void MathExpr::CreateMathExpr(const char* expr)
{
    root = new Node();

    int i = 0;

    ///We cut the closing zero
    while(*(expr+i) != '\0')
    {
        i++;
    }

    expression = new char[i];
    explength = i;

    for(int i = 0; i < explength; i++) *(expression + i) = *(expr + i);

    switch(SintaxCheck(expression, explength))
    {
    case 0:
        build_tree(expression, explength, this->root, NULL);
        break;
    case -1:
        throw std::invalid_argument( "The brackets in your expression are not right!");
        break;
    case -2:
        throw std::invalid_argument( "The expression you entered is not right! Maybe you put two binary operators next to each other?");
        break;
    default:
        throw std::invalid_argument( "Something went wrong with MathExpr::SintaxCheck");
        break;
    }
}

///Átad egy pointert az eredeti karakter tömber az elején, majd az módosítva adja tovább.
///Rekurzív építés
///Leszeded a felesleges zárójeleket (a belsõ rekurzióknál fontos pl) (a+b)*c, a második lépésben (a+b) lesz és a zárójel fölösleges
///Megkeresed a legkisebb prioritású mûveleletet. +-*/GYÖK^()
///Két részre bontod és átadod a következõknek.


///Egyelőre csak egész számokra működik!!!!! TODO --> Komplex számokra
void MathExpr::build_tree(char* expr, int length, Node* n, Node* parent)
{
    if(!BracketNeeded(expr, length)) ///Cut unnecessary brackets!
    {
        expr++;
        length--;
        length--;
    }

    if(isOnlyNumber(expr, length))
    {
        SetupNode(n, NULL, NULL, parent, expr, length);

        return;
    }

    int lowest_priority = 4;

    int lvl = 0;
    int index = -1;

    for(int i = 0; i < length; i++)
    {
        char c = *(expr + i);

        if(c == '(') lvl++;
        else if(c == ')') lvl--;
        else if(lvl == 0)
        {
            if(OperatorPriority(c) < lowest_priority)   ///Finding the break point of the expression;
            {
                index = i;
                lowest_priority = OperatorPriority(c);
            }
        }
    }

    if(index == -1) throw std::invalid_argument("Something bad");

    Node* r;
    Node* l;

    if(index == 0)
    {
        r = new Node();
        build_tree(expr+1, length-1, r, n);
        l = NULL;
    }
    else
    {
        r = new Node();
        l = new Node();

        build_tree(expr, index, l, n);
        build_tree(expr + index + 1, length - index - 1, r, n);
    }

    SetupNode(n, l, r, parent, *(expr + index));
}

///Priorities:
///0 - Lowest priority +-
///1 - */
///2 - PG (power, sqrroot)
///3 - numbers or imaginary op
int MathExpr::OperatorPriority(char c)
{
    switch(c)
    {
    case '+':
    case '-':
        return 0;
        break;
    case '*':
    case '/':
        return 1;
        break;
    case 'P':
    case 'G':
        return 2;
        break;
    case '0':
    case '1':
    case '2':
    case '3':
    case '4':
    case '5':
    case '6':
    case '7':
    case '8':
    case '9':
    case 'i':
        return 3;
        break;
    default:
        throw std::invalid_argument( std::string("Invalid character in expression:") + c);
    }
}

int MathExpr::SintaxCheck(char* expr, int length)
{
    int lvl = 0;
    char before = '\0';
    for(int i = 0; i < length; i++)
    {
        char c = *(expr + i);

        if(c == '(') lvl++;
        if(c == ')') lvl--;

        if(lvl < 0) return -1; ///Closing Bracket without Opening Bracket

        if(i != 0)
        {
            if( (isBinaryOperator(before) && isBinaryOperator(c)) ||  (before == '(' && isBinaryOperator(c)) || (isBinaryOperator(before) && c == ')')) return -2; ///Two binary operator cant be next to each other or next to bracket
            if( (before == '-') && (c == '(')) throw std::invalid_argument( " - before (, try -1*( " );
        }
        else
        {
            if(isBinaryOperator(c)) ///Proper expression can only begin with a bracket, number, or unary operator.
            {
                return -2;
            }
        }

        if(i == length - 1 && (isBinaryOperator(c) || isUnary(c))) return -2; ///Proper expression can't end with operator

        before = c;
    }

    if(lvl != 0) return -1; ///Opening Bracket without Closing Bracket

    return 0;
}

bool MathExpr::BracketNeeded(char* expr, int length)
{
    if(*(expr) != '(' || *(expr + length - 1) != ')') ///It can only be unnecessary if it encapsulates the whole
    {
        return true;
    }

    int lvl = 0;

    for(int i = 0; i < length; i++)
    {
        char c = *(expr + i);

        if(c == '(') lvl++;
        if(c == ')') lvl--;

        if(lvl == 0 && i != length - 1) return true;
    }

    return false;
}

bool MathExpr::isNum(char c)
{
    if(c == '1' || c == '2' || c == '3' || c == '4' || c == '5' || c == '6' || c == '7' || c == '8' || c == '9' || c == '0')
    {
        return true;
    }

    return false;
}

bool MathExpr::isUnary(char c)
{
    if(c == '-' || c == 'G')
    {
        return true;
    }

    return false;
}

bool MathExpr::isBinaryOperator(char c)
{
    if(c == '+' || c == 'P' || c == '*' || c == '/')
    {
        return true;
    }

    return false;
}

bool MathExpr::isBracket(char c)
{
    if(c == '(' || c == ')')
    {
        return true;
    }

    return false;
}


bool MathExpr::isImaginary(char c)
{
    if(c == 'i') return true;

    return false;

}

bool MathExpr::isOnlyNumber(char* expr, int length)
{
    for(int i = 0; i < length; i++)
    {
        char c = *(expr + i);

        if(!isNum(c) && !isImaginary(c)) return false;
    }

    return true;
}

void MathExpr::print_tree(Node* r)
{
    if(r->left != NULL)
    {
        print_tree(r->left);
    }

    for(int i = 0; i < r->length; i++) std::cout << *(r->value + i);
    ///std::cout << std::endl;

    if(r->right != NULL)
    {
        print_tree(r->right);
    }
}

void MathExpr::print_node(Node* r, int lvl)
{
    if(r->left != NULL)
    {
        print_node(r->left, lvl + 1);
    }
    else
    {
        std::cout << lvl << ": NULL" << std::endl;
    }

    std::cout << lvl << ": ";

    for(int i = 0; i < r->length; i++) std::cout << *(r->value + i);

    std::cout << std::endl;

    if(r->right != NULL)
    {
        print_node(r->right, lvl + 1);
    }
    else
    {
        std::cout << lvl << ": NULL" << std::endl;
    }
}


void MathExpr::PrintOriginal() const
{
    for(int i = 0; i < this->explength; i++)
    {
        std::cout << *(this->expression + i);
    }
}

void MathExpr::Print(bool format)
{
    if(format)
    {
        print_tree(root);
    }
    else
    {
        print_node(root, 0);
    }
    std::cout << std::endl;
}

double MathExpr::Evaluate()
{
    return this->Evaluate(this->root);
}

double MathExpr::Evaluate(Node* r)
{
    if(r->left == NULL && r->right == NULL)
    {
        return NodeToInt(r);
    }
    else if(r->left == NULL && r->right != NULL)
    {
        if(!this->isUnary(*(r->value)))
        {
            std::string op(r->value);
            throw std::invalid_argument( std::string("No left side value at operator: ")+op);
            return 0;
        }

        if(*(r->value) == '-')
        {
            return -1*this->Evaluate(r->right);
        }
        else if(*(r->value) == 'G')
        {
            return sqrt(this->Evaluate(r->right));
        }
        else
        {
            std::string op(r->value);
            throw std::invalid_argument( std::string("Unknown operator: " + op));
            return 0;
        }


    }
    else if(r->left != NULL && r->right == NULL)
    {

        std::string op(r->value);
        throw std::invalid_argument( std::string("Something is wrong, there is an operator that only has rightside value at: " + op));
        return 0;
    }
    else
    {
        switch(*(r->value))
        {
        case '+':
            return this->Evaluate(r->left) + this->Evaluate(r->right);
            break;
        case '-':
            return this->Evaluate(r->left) - this->Evaluate(r->right);
            break;
        case '*':
            return this->Evaluate(r->left) * this->Evaluate(r->right);
            break;
        case '/':
            return this->Evaluate(r->left) / this->Evaluate(r->right);
            break;
        case 'P':
            return pow(this->Evaluate(r->left),this->Evaluate(r->right));
            break;
        default:
            std::string op(r->value);
            throw std::invalid_argument( std::string("Unknown operator: " + op));
            return 0;
            break;
        }
    }
}

MathExpr MathExpr::operator+(const MathExpr& me) const
{
    return Concatenate(me, '+');
}

MathExpr MathExpr::operator-(const MathExpr& me) const
{
    return Concatenate(me, '-');
}

MathExpr MathExpr::operator*(const MathExpr& me) const
{
    return Concatenate(me, '*');
}

MathExpr MathExpr::operator/(const MathExpr& me) const
{
    return Concatenate(me, '/');
}

std::ostream& operator<<(std::ostream& os, const MathExpr& m)
{
    m.PrintOriginal();
    return os;
}

MathExpr& MathExpr::operator=(const MathExpr& me)
{
    if(expression != NULL)
    {
            delete[] expression;
            expression = NULL;
            delete root;
            root = NULL;
    }

    char* xp = new char[me.Length() + 1];

    int l = me.Length();

    for(int i = 0; i < l; i++)
    {
        xp[i] = me.exp()[i];
    }

    xp[me.Length()] = '\0';

    CreateMathExpr(xp);

    return *this;
}

void MathExpr::Simplify()
{
    while(simp(root))
    {
        while(prioritySimp(root));
    }
    while(prioritySimp(root));

    std::string * s = new std::string();

    *s = "";

    build_string(s, root);

    delete expression;

    explength = s->length();

    expression = new char[explength];

    for(int i = 0; i < explength; i++)
    {
        expression[i] = (*s)[i];
    }

    delete s;

}


void MathExpr::build_string(std::string* s, Node* n)
{
    if(n == NULL) return;

    build_string(s, n->left);
    s->append(n->value, n->length);
    build_string(s, n->right);

    return;
}

bool MathExpr::simp(Node* n)
{
    if(n == NULL) return false;
    if(isOnlyNumber(n->value, n->length)) return false;

    bool ret = (simp(n->right) || simp(n->left));

    switch(*(n->value))
    {
    case '+':

        if(isOnlyNumber(n->right->value, n->right->length) && isOnlyNumber(n->left->value, n->left->length))
        {
            int left = NodeToInt(n->left);
            int right = NodeToInt(n->right);
            int r = left + right;

            ResetNode(n, NULL, NULL, n->parent, r);

            return true;
        }
        else
        {
            return ret;
        }
        break;

    case '-':

        if(n->left == NULL)
        {
            return simp(n->right);
        }
        if(n->right == NULL)
        {
            return false;
        }
        if(isOnlyNumber(n->right->value, n->right->length) && isOnlyNumber(n->left->value, n->left->length))
        {

            int left = NodeToInt(n->left);
            int right = NodeToInt(n->right);

            int r = left - right;

            if(r >= 0)
            {
                ResetNode(n, NULL, NULL, n->parent, r);
            }
            else
            {
                ResetNode(n, NULL, NULL, n->parent, '-');
                Node* c = NewNode(-1*r);
                n->AddChild(c, false);
            }

            return true;

        }
        else
        {
            return ret;
        }
        break;

    case '*':

        if(isOnlyNumber(n->right->value, n->right->length) && isOnlyNumber(n->left->value, n->left->length))
        {
            int left = NodeToInt(n->left);
            int right = NodeToInt(n->right);

            int r = left * right;

            ResetNode(n, NULL, NULL, n->parent, r);

            return true;
        }
        else
        {
            return ret;
        }
        break;

    case '/':

        if(isOnlyNumber(n->right->value, n->right->length) && isOnlyNumber(n->left->value, n->left->length))
        {
            int left = NodeToInt(n->left);
            int right = NodeToInt(n->right);

            int gcd = GCD(left, right);

            if(gcd == right)
            {
                int r = left / right;

                ResetNode(n, NULL, NULL, n->parent, r);

                return true;
            }
            else if(gcd != 1)
            {
                ModifyNodeValue(n->left, left/gcd);
                ModifyNodeValue(n->right, right/gcd);

                return true;
            }
            else
            {
                return false;
            }
        }
        else
        {
            return ret;
        }
        break;

    case 'P':

        return ret;
        break;

    case 'G':

        if(isOnlyNumber(n->left->value, n->left->length))
        {
            int right = NodeToInt(n->right);

            double epsilon = 0.00001;

            double rd = sqrt((double)right);

            int ri = (int)rd;

            if(ri + epsilon > rd && ri - epsilon < rd)
            {
                ResetNode(n, NULL, NULL, n->parent, ri);
                return true;
            }
            else
            {
                return ret;
            }
        }
        else
        {
            return ret;
        }

        break;
    default:
        return false;
        break;
    }

    return false;
}


bool MathExpr::prioritySimp(Node* n)
{
    if(n == NULL) return false;

    if(isOnlyNumber(n->value, n->length)) return false;

    Node* left = n->left;
    Node* right = n->right;

    bool ret = ( prioritySimp(left) || prioritySimp(right) );

    bool leftnum = isOnlyNumber(left->value, left->length);
    bool rightnum = isOnlyNumber(right->value, right->length);

    if(leftnum && rightnum) return false;

    int op      = OperatorPriority(*(n->value));
    int leftop  = OperatorPriority(*(left->value));
    int rightop = OperatorPriority(*(left->value));

    ///Can only simplify by priority if one child is a number and the other is an operator of equal priority to the one in this note. A node that has an operator of equal priority on both size does not exist thanks to the build of the tree
    if(! ( ( leftnum && (rightop == op) ) || ( rightnum && (leftop == op) ) ) ) return false;


    switch(op)
    {
    case 0:
    case 1:
    {

        Node* numberNode = NULL;

        bool division = (*(n->value) == '/');

        if(leftnum)
        {
             numberNode = left;
        }
        else if (rightnum)
        {
             numberNode = right;
        }

        int orig = NodeToInt(numberNode);

        SearchResult searchr = searchForSimpable(numberNode, op, orig, leftnum, division);

        Node* foundNode = searchr.srnode;
        int revers = searchr.reverses;

        if(foundNode == NULL) return ret;

        Node* foundNumberNode;

        bool numberFoundOnLeft = false;

        if(isOnlyNumber(foundNode->left->value, foundNode->left->length))
        {
            foundNumberNode = foundNode->left;
            numberFoundOnLeft = true;
        }
        if(isOnlyNumber(foundNode->right->value, foundNode->right->length))
        {
            foundNumberNode = foundNode->right;
            numberFoundOnLeft = false;
        }

        if(foundNumberNode == NULL) throw std::invalid_argument( "Search found invalid number node" );

        int foundNum = NodeToInt(foundNumberNode);

        int result;

        switch(*(n->value))
        {

        case '+':

            if(*(foundNode->value) == '+')
            {
                result = orig + foundNum;

                ModifyNodeValue(numberNode, result);

                deleteChildOtherIntoParent(foundNode, numberFoundOnLeft);

            }
            else if(*(foundNode->value) == '-')
            {

                if(numberFoundOnLeft)
                {
                    result = orig + foundNum;

                    ModifyNodeValue(numberNode, result);

                    foundNumberNode->DeleteChild(true);
                }
                else
                {
                    result = orig - foundNum;

                    if(result >= 0)
                    {
                        ModifyNodeValue(numberNode, result);

                    }
                    else
                    {
                        ResetNode(n, NULL, NULL, n->parent, '-');
                        Node* c = NewNode(-1*result);
                        n->AddChild(c, false);
                    }

                    deleteChildOtherIntoParent(foundNode, false);
                }

            }
            else
            {
                throw std::invalid_argument( "Search found invalid priority operator for +" );
            }
            break;
        case '-':

            if(*(foundNode->value) == '+')
            {
                if(leftnum)
                {
                    result = orig + foundNum;

                    ModifyNodeValue(numberNode, result);

                    deleteChildOtherIntoParent(foundNode, numberFoundOnLeft);

                }
                else if(rightnum)
                {
                    result = -1*orig + foundNum;

                    if(result >= 0)
                    {
                        ModifyNodeValue(numberNode, result);
                    }
                    else
                    {
                        ResetNode(n, NULL, NULL, n->parent, '-');
                        Node* c = NewNode(-1*result);
                        n->AddChild(c, false);
                    }
                    deleteChildOtherIntoParent(foundNode, numberFoundOnLeft);
                }
                else
                {
                    throw std::invalid_argument( "Impossible" );
                }
            }

            else if(*(foundNode->value) == '-')
            {
                if(leftnum && numberFoundOnLeft)
                {
                    result = orig - foundNum;

                    if(result >= 0)
                    {
                        ModifyNodeValue(numberNode, result);
                    }
                    else
                    {
                        ResetNode(n, NULL, NULL, n->parent, '-');
                        Node* c = NewNode(-1*result);
                        n->AddChild(c, false);
                    }
                    foundNumberNode->DeleteChild(true);

                }
                else if(leftnum && !numberFoundOnLeft)
                {
                    result = orig + foundNum;

                    ModifyNodeValue(numberNode, result);

                    deleteChildOtherIntoParent(foundNode, false);
                }
                else if(rightnum && numberFoundOnLeft)
                {
                    result = -1*orig + foundNum;

                    if(result >= 0)
                    {
                        ModifyNodeValue(numberNode, result);
                    }
                    else
                    {
                        ResetNode(n, NULL, NULL, n->parent, '-');
                        Node* c = NewNode(-1*result);
                        n->AddChild(c, false);
                    }

                    foundNode->DeleteChild(left);

                }
                else if(rightnum && !numberFoundOnLeft)
                {
                    result = orig + foundNum;

                    ModifyNodeValue(numberNode, result);

                    deleteChildOtherIntoParent(foundNode, false);
                }
                else
                {
                    throw std::invalid_argument( "Impossible" );
                }
            }

            else
            {
                throw std::invalid_argument( "Search found invalid priority operator for -" );
            }
            break;

        case '*':

            if(*(foundNode->value) == '*')
            {
               result = orig * foundNum;

               ModifyNodeValue(numberNode, result);

               deleteChildOtherIntoParent(foundNode, numberFoundOnLeft);

            }
            else if(*(foundNode->value) == '/')
            {
                if(numberFoundOnLeft)
                {
                    result = orig * foundNum;
                }
                else
                {
                    result = orig / foundNum;
                }
            }
            else
            {
                throw std::invalid_argument( "Search found invalid priority operator for *" );
            }

            break;
        case '/':

            if(*(foundNode->value) == '*')
            {
                if(leftnum)
                {
                    result = orig / foundNum;
                }
                else if(rightnum)
                {
                    result = foundNum / orig;
                }
                else
                {
                    throw std::invalid_argument( "Impossible" );
                }
            }
            else if(*(foundNode->value) == '/')
            {
                if(leftnum && numberFoundOnLeft)
                {
                    result = orig / foundNum;
                }
                else if(leftnum && !numberFoundOnLeft)
                {
                    result = orig * foundNum;
                }
                else if(rightnum && numberFoundOnLeft)
                {
                    result = foundNum / orig;
                }
                else if(rightnum && !numberFoundOnLeft)
                {
                    result = orig * foundNum;
                }
                else
                {
                    throw std::invalid_argument( "Impossible" );
                }
            }
            else
            {
                throw std::invalid_argument( "Search found invalid priority operator for *" );
            }

            break;
        default:
                throw std::invalid_argument( "Invalid operator for priority simplification" );
            break;
        }
    }
    break;
    default:
        throw std::invalid_argument( "Got into strange branch of switch in priority simplification" );
        break;
    }

    return false;
}

Node* MathExpr::deleteChildOtherIntoParent(Node* n, bool left)
{
    if(left)
    {
        delete n->left;

        Node* nt = n->right;

        n->value = nt->value;
        n->length = nt->length;
        n->right = nt->right;
        n->left = nt->left;

        delete nt;
    }
    else
    {
        delete n->right;

        Node* nt = n->left;

        n->value = nt->value;
        n->length = nt->length;
        n->right = nt->right;
        n->left = nt->left;

        delete nt;
    }
    return n;
}

SearchResult MathExpr::searchForSimpable(Node* n, int op, int num, bool left, bool division, int r)
{
    if(n == NULL) throw std::invalid_argument( "Search for same priority operator reached NULL!" );

    SearchResult sr;
    sr.srnode = n;
    sr.reverses = r;

    if(isOnlyNumber(n->left->value, n->left->length)) ///LEFT
    {
        if(op == 0) return sr; ///Only need to get fancy with division and multiplication

        int number = NodeToInt(n->left);

        if(!division)
        {
            if(r % 2 == 0)
            {
                return sr; ///In multiplication it doesn't matter if the second is multiplication or division you will just multiply with the nominator
            }
            else
            {
                if(GCD(num, number) == number)
                {
                    return sr;
                }
            }
        }
        else
        {
            if(r % 2 == 0)
            {

            }
            else
            {

            }


            if(left)
            {
                if(GCD(num, number) == number) return sr;
            }
            else
            {
                if(GCD(num, number) == num) return sr;
            }
        }
    }

    if(isOnlyNumber(n->right->value, n->right->length)) ///RIGHT
    {
        if(op == 0) return sr;

        int number = NodeToInt(n->right);

        if(*(n->value) == '/')
        {
            r++;
            sr.reverses = r;
        }

        if(!division)
        {
            if(r % 2 == 0)
            {

            }
            else
            {

            }
            if(GCD(num, number) == number) return sr;
        }
        else
        {

            if(r % 2 == 0)
            {

            }
            else
            {

            }

            if(*(n->value) == '/')
            {
                return sr;
            }
            else
            {
                if(left)
                {
                    if(GCD(num, number) == number) return sr;
                }
                else
                {
                    if(GCD(num, number) == num) return sr;
                }
            }
        }

    }

    SearchResult le;
    SearchResult ri;

    if(n->left != NULL )
    {

        if(op == OperatorPriority(*(n->left->value)))
        {
            le = searchForSimpable(n->left, op, num, left, division, r);
        }
    }



    if(n->right != NULL )
    {
        if(op == OperatorPriority(*(n->right->value)))
        {
            if(op == 1 && *(n->value) == '/')
            {
                r++;
            }
            ri = searchForSimpable(n->right, op, num, left, division, r);
        }
    }

    if(le.srnode == NULL && ri.srnode == NULL)      ///In this case, there was either no appropriate priority children or they had none.
    {
        return le;
    }
    else if(le.srnode != NULL)
    {
        return le;
    }
    else
    {
        return ri;
    }
}

Node* MathExpr::ModifyNodeValue(Node* n, int val)
{
    if(n->value != NULL) free(n->value);

    std::stringstream strs;
    strs << val;
    std::string temp = strs.str();


    n->value = new char[temp.length()];

    n->length = temp.length();

    for(int i = 0; i < n->length; i++)
    {
        n->value[i] = temp[i];
    }

    return n;
}

Node* MathExpr::ModifyNodeValue(Node* n, char val)
{
    if(n->value != NULL) free(n->value);

    n->value = new char[1];

    n->length = 1;

    n->value[0] = val;

    return n;
}

Node* MathExpr::ModifyNodeValue(Node* n, const char* val, int l)
{
    if(n->value != NULL) free(n->value);

    n->value = new char[l];

    n->length = l;

    for(int i = 0; i < n->length; i++)
    {
        n->value[i] = val[i];
    }

    return n;
}

Node* MathExpr::NewNode(int val)
{
    Node* child = new Node;

    child->right = NULL;
    child->left = NULL;
    child->parent = NULL;
    child->value = NULL;

    ModifyNodeValue(child, val);

    return child;
}

Node* MathExpr::NewNode(char val)
{
    Node* child = new Node;

    child->right = NULL;
    child->left = NULL;
    child->parent = NULL;
    child->value = NULL;

    ModifyNodeValue(child, val);

    return child;
}

Node* MathExpr::NewNode(const char* val, int l)
{
    Node* child = new Node;

    child->right = NULL;
    child->left = NULL;
    child->parent = NULL;
    child->value = NULL;

    ModifyNodeValue(child, val, l);

    return child;
}


Node* MathExpr::AddNode(Node* parent, bool left, int val)
{
    Node* child = NewNode(val);

    child->parent = parent;

    if(parent != NULL)
    {
        if(left)
        {
            parent->left = child;
        }
        else
        {
            parent->right = child;
        }
    }

    return child;
}

Node* MathExpr::AddNode(Node* parent, bool left, char val)
{
    Node* child = NewNode(val);

    child->parent = parent;

    if(parent != NULL)
    {
        if(left)
        {
            parent->left = child;
        }
        else
        {
            parent->right = child;
        }
    }

    return child;
}

Node* MathExpr::AddNode(Node* parent, bool left, const char* val, int l)
{
    Node* child = NewNode(val, l);

    child->parent = parent;

    if(parent != NULL)
    {
        if(left)
        {
            parent->left = child;
        }
        else
        {
            parent->right = child;
        }
    }

    return child;
}

Node* MathExpr::SetupNode(Node* n, Node* le, Node* r, Node* p, int val)
{
    ModifyNodeValue(n, val);

    n->parent = p;
    n->left = le;
    n->right = r;

    return n;
}

Node* MathExpr::SetupNode(Node* n, Node* le, Node* r, Node* p, char val)
{
    ModifyNodeValue(n, val);

    n->parent = p;
    n->left = le;
    n->right = r;

    return n;
}

Node* MathExpr::SetupNode(Node* n, Node* le, Node* r, Node* p, const char* val, int l)
{
    ModifyNodeValue(n, val, l);

    n->parent = p;
    n->left = le;
    n->right = r;

    return n;
}

Node* MathExpr::ResetNode(Node* n, Node* le, Node* r, Node* p, int val)
{
    delete[] n->value;

    delete n->right;
    delete n->left;

    n->value = NULL;
    n->right = NULL;
    n->left = NULL;

    return SetupNode(n, le, r, p, val);
}

Node* MathExpr::ResetNode(Node* n, Node* le, Node* r, Node* p, char val)
{
    delete[] n->value;

    delete n->right;
    delete n->left;


    n->value = NULL;
    n->right = NULL;
    n->left = NULL;

    return SetupNode(n, le, r, p, val);
}

Node* MathExpr::ResetNode(Node* n, Node* le, Node* r, Node* p, const char* val, int l)
{
    delete[] n->value;

    delete n->right;
    delete n->left;

    n->value = NULL;
    n->right = NULL;
    n->left = NULL;

    return SetupNode(n, le, r, p, val, l);
}

int MathExpr::NodeToInt(Node* n)
{
    return atoi( std::string(n->value, n->length).c_str() );
}

MathExpr MathExpr::Concatenate(const MathExpr& me, char c) const
{
    MathExpr m;

    char* n = new char[Length() + me.Length() + 6]; ///Two pair of brackets and the operator

    n[0] = '(';
    int k = 1;
    for(int i = 0; i < this->Length(); i++)
    {
        n[k+i] = this->exp()[i];
    }

    k += this->Length();

    n[k] = ')';
    k++;
    n[k] = c;
    k++;
    n[k] = '(';
    k++;

    for(int i = 0; i < me.Length(); i++)
    {
        n[k+i] = me.exp()[i];
    }

    k += me.Length();

    n[k] = ')';
    n[k+1] = '\0';

    m.CreateMathExpr(n);

    delete n;

    return m;
}

void Node::AddChild(Node* c, bool left)
{
    c->parent = this;

    if(left)
    {
        left = c;
    }
    else
    {
        right = c;
    }
}


void Node::DeleteChild(bool left)
{
    if(left)
    {
        delete this->left;
        this->left = NULL;
    }
    else
    {
        delete this->right;
        this->right = NULL;
    }
}

}
