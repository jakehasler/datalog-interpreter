//
//  Parameter.hpp
//  L1-Lexical-Analyzer
//
//  Created by Jake Hasler on 10/23/15.
//  Copyright © 2015 Jake Hasler. All rights reserved.
//

#ifndef Parameter_h
#define Parameter_h

#include <iostream>
#include <string>
#include <fstream>
#include <ostream>
#include <vector>
#include <cstdlib>
#include <cctype>
#include <sstream>

#include "Token.h"
#include "Scanner.h"
#include "Expression.h"

using namespace std;

class Parameter
{
public:
    
    // Okay to have two constructors?
    Parameter(string info, string type);
    Parameter(Expression expression);
    ~Parameter();
    
    string toString();
    
    void setValue(string v);
    void setType(string t);
    
    string getTokenType();
    string getValue();
    
    bool ifExpression;

private:
    string value;
    string tokenType;
    Expression expression;
    
};


#endif /* Parameter_h */


