//
//  Expression.cpp
//  L1-Lexical-Analyzer
//
//  Created by Jake Hasler on 10/24/15.
//  Copyright © 2015 Jake Hasler. All rights reserved.
//

#include "Expression.h"


Expression::Expression()
{
    // Empty constructor
}

Expression::Expression(string param1, string theOperator, string param2)
{
    this->param1 = param1;
    this->theOperator = theOperator;
    this->param2 = param2;
}

Expression::~Expression()
{
    
}

string Expression::toString()
{
    stringstream ss;
    ss << param1 << theOperator << param2;
    
    return ss.str();
}