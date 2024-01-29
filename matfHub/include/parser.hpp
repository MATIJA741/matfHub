#pragma once

#include <iostream>
#include <string.h>
#include <vector>
class Parser {

#define NUMVARS 26

public:
  Parser();
  ~Parser();
  auto evalExpression(char *exp) -> double;
  char errorMessage[64];

protected:
  void evalToken(double &result);
  void evalAdd(double &result);
  void evalMultiply(double &result);
  void evalPow(double &result);
  void evalUnar(double &result);
  void evalParantheses(double &result);
  void evalFunc(double &result, char *token);
  void getToken();

  void checkOperator(double &result, char op, double temp);

  char *expressionPtr; // points to the expression
  char token[256];     // holds current token
  char tokenType;      // holds token's type
  double *vars;        // holds variable's values
};
