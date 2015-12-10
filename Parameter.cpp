//
//  Parameter.cpp
//  L1-Lexical-Analyzer
//
//  Created by Jake Hasler on 10/23/15.
//  Copyright © 2015 Jake Hasler. All rights reserved.
//

#include "Parameter.h"

// STRING | ID | expression


// expression	-> 	LEFT_PAREN parameter operator parameter RIGHT_PAREN

Parameter::Parameter(string info, string t)
{
    setValue(info);
    setType(t);
    Expression expression(info, t, info);
}

Parameter::Parameter(Expression expression)
{
    value = "";
    tokenType = "";
    this->expression = expression;
}

Parameter::~Parameter()
{
    
}


string Parameter::toString()
{
    stringstream ss;
    //cout << value << " = Value" << endl;
    ss << value;
    //cout << value << " " << endl;
//    if(ifExpression == true)
//    {
//        ss << expression.toString();
//    }
//    else
//    {
//        //cout << value << " = Value" << endl;
//        ss << value;
//    }
    //cout << ss.str() << " ss" << endl;
    return ss.str();
}


void Parameter::setValue(string v)
{
    value = v;
}

void Parameter::setType(string t)
{
    tokenType = t;
}

string Parameter::getTokenType()
{
    return tokenType;
}

string Parameter::getValue()
{
    return value;
}