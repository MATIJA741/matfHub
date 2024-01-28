#include "../include/parser.hpp"
#include "../include/parserEnum.hpp"
#include <cstring>
#include <cmath> 

//TODO refactor code

#define PI 3.14159265358979323846

Parser::Parser(){
    vars = new double[NUMVARS];
    int i;
    expressionPtr = nullptr;
    for (i = 0; i < NUMVARS; i++)
        vars[i] = 0.0;
    errorMessage[0] = '\0';
}

Parser::~Parser(){
    delete[] vars; vars = nullptr;
    std::cout << "Unisten: parser" << std::endl;
}

auto Parser::evalExpression(char *exp) -> double { 
    errorMessage[0] = '\0';
    double result;
    expressionPtr = exp;
    getToken();
    if (!*token) 
    {
        strcpy(errorMessage, "No Expression Present"); // no expression present
        return (double)0;
    }
    evalToken(result);
    if (*token) // last token must be null
        strcpy(errorMessage, "Syntax Error");
    return result;
}

void Parser::evalToken(double &result){
    int slot;
    char tmpToken[80];
    if (tokenType == VARIABLE)
    {
        // save old token
        char *tmpPtr = expressionPtr;
        strcpy(tmpToken, token);
        // compute the index of the variable
        slot = *token - 'A';
        getToken();
        if (*token != '=') 
        {
            expressionPtr = tmpPtr; // return current token
            strcpy(token, tmpToken); // restore old token
            tokenType = VARIABLE;
        }
        else {
            getToken(); // get next part of exp
            evalAdd(result);
            vars[slot] = result;
            return;
        }
    }
    evalAdd(result);
}
// Zbir
void Parser::evalAdd(double &result){
    char op;
    double temp;
    evalMultiply(result);
    while ((op = *token) == '+' || op == '-')
    {
        getToken();
        evalMultiply(temp);
        checkOperator(result, op, temp);
    }
}
// Mnozenje
void Parser::evalMultiply(double &result){
    char op;
    double temp;
    evalPow(result);
    while ((op = *token) == '*' || op == '/') 
    {
        getToken();
        evalPow(temp);
        checkOperator(result, op, temp);
    }
}
// Eksponent
void Parser::evalPow(double &result){
    double temp;
    evalUnar(result);
    while (*token == '^')
    {
        getToken();
        evalUnar(temp);
        result = pow(result, temp);
    }
}
// Unarni operatori
void Parser::evalUnar(double &result){
    char op;
    op = 0;
    if ((tokenType == DELIMITER && *token == '+') || *token == '-')
    {
        op = *token;
        getToken();
    }
    evalParantheses(result);
    if (op == '-')
        result = -result;
}
// Fja, zagrade, promenljiva
void Parser::evalParantheses(double &result){
    bool isfunc = (tokenType == FUNCTION);
    char tmpToken[80];
    if (isfunc)
    {
        strcpy(tmpToken, token);
        getToken();
    } 
    if (*token == '(')
    {
        getToken();
        evalAdd(result);
        if (*token != ')')
            strcpy(errorMessage, "Unbalanced Parentheses");
        if (isfunc)
            evalFunc(result, tmpToken);
        getToken();
    }
    else
        switch (tokenType)
        {
        case VARIABLE:
            result = vars[*token - 'A'];
            getToken();
            return;
        case NUMBER:
            result = atof(token);
            getToken();
            return;
        default:
            strcpy(errorMessage, "Syntax Error");
        }
}

void Parser::evalFunc(double &result, char* token){

    if (!strcmp(token, "SIN"))
        //PI / 180 * result
        result = sin(result);
    else if (!strcmp(token, "COS"))
        result = cos(result);
    else if (!strcmp(token, "TAN"))
        result = tan(result);
    else if (!strcmp(token, "ASIN"))
        result = asin(result);
    else if (!strcmp(token, "ACOS"))
        result = acos(result);
    else if (!strcmp(token, "ATAN"))
        result = atan(result);
    else if (!strcmp(token, "SINH"))
        result = sinh(result);
    else if (!strcmp(token, "COSH"))
        result = cosh(result);
    else if (!strcmp(token, "TANH"))
        result = tanh(result);
    else if (!strcmp(token, "ASINH"))
        result = asinh(result);
    else if (!strcmp(token, "ACOSH"))
        result = acosh(result);
    else if (!strcmp(token, "ATANH"))
        result = atanh(result);
    else if (!strcmp(token, "LN"))
        result = log(result);
    else if (!strcmp(token, "LOG"))
        result = log10(result);
    else if (!strcmp(token, "EXP"))
        result = exp(result);
    else if (!strcmp(token, "SQRT"))
        result = sqrt(result);
    else if (!strcmp(token, "SQR"))
        result = result*result;
    else if (!strcmp(token, "INT"))
        result = floor(result);
    else if (!strcmp(token, "ABS"))
        result = abs(result);
    else if (!strcmp(token, "INT"))
        result = floor(result);
    else if (!strcmp(token, "PI"))
        result = PI;
    else
        strcpy(errorMessage, "Unknown Function");
}

void Parser::getToken(){
    char *temp;
    tokenType = 0;
    temp = token;
    *temp = '\0';
    if (!*expressionPtr)  // at end of expression
        return;
    while (isspace(*expressionPtr))  // skip over white space
        ++expressionPtr;
    if (strchr("+-*/%^=()", *expressionPtr))
    {
        tokenType = DELIMITER;
        *temp++ = *expressionPtr++;  // advance to next char
    }
    else if (isalpha(*expressionPtr))
    {
        while (!strchr(" +-/*%^=()\t\r", *expressionPtr) && (*expressionPtr))
            *temp++ = toupper(*expressionPtr++);
        while (isspace(*expressionPtr))  // skip over white space
            ++expressionPtr;
        tokenType = (*expressionPtr == '(') ? FUNCTION : VARIABLE;
    }
    else if (isdigit(*expressionPtr) || *expressionPtr == '.')
    {
        while (!strchr(" +-/*%^=()\t\r", *expressionPtr) && (*expressionPtr))
            *temp++ = toupper(*expressionPtr++);
        tokenType = NUMBER;
    }
    *temp = '\0';
    if ((tokenType == VARIABLE) && (token[1]))
        strcpy(errorMessage, "Only first letter of variables is considered");
}

void Parser::checkOperator(double &result, char op, double temp){

    switch (op)
    {
    case '+':
        result = result + temp;
        break;
    case '-':
        result = result - temp;
        break;
    case '*':
        result = result * temp;
        break;
    case '/':
        result = result / temp;
        break;
    }

}




