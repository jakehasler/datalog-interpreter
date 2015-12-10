//
//  Expression.hpp
//  L1-Lexical-Analyzer
//
//  Created by Jake Hasler on 10/24/15.
//  Copyright © 2015 Jake Hasler. All rights reserved.
//

#ifndef Expression_h
#define Expression_h

#include <iostream>
#include <string>
#include <fstream>
#include <ostream>
#include <vector>
#include <cstdlib>
#include <cctype>
#include <sstream>

using namespace std;

class Expression
{
public:
    Expression();
    Expression(string param1, string theOperator, string param2);
    ~Expression();
    
    string toString();
    
private:
    string param1;
    string theOperator;
    string param2; 
    
};

#endif /* Expression_h */
