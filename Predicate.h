//
//  Predicate.hpp
//  L1-Lexical-Analyzer
//
//  Created by Jake Hasler on 10/23/15.
//  Copyright © 2015 Jake Hasler. All rights reserved.
//

#ifndef Predicate_h
#define Predicate_h

#include <iostream>
#include <string>
#include <fstream>
#include <ostream>
#include <vector>
#include <cstdlib>
#include <cctype>
#include <sstream>

#include "Parameter.h"
#include "Scanner.h"

using namespace std;

class Predicate
{
public:
    Predicate(string name);
    ~Predicate();
    
    void setValue(string name);
    void addParameter(Parameter newParam);
    void addID(string newId);
    void addString(string newString);
    string toString();
    string getValue();
    
    vector<string> getSchema();
    vector<string> getParams();
    vector<Parameter> getParamList();
    
    bool ifId;
    bool ifString;
    bool ifParam;
    bool ifRule;
    
    
    vector<Parameter> paramList;
    vector<string> idList;
    vector<string> stringList;
    vector<string> ruleList;
    
private:
    string value;
    
};

#endif /* Predicate_h */
