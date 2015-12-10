//
//  Rule.hpp
//  L1-Lexical-Analyzer
//
//  Created by Jake Hasler on 10/23/15.
//  Copyright © 2015 Jake Hasler. All rights reserved.
//

#ifndef Rule_h
#define Rule_h

#include <iostream>
#include <string>
#include <fstream>
#include <ostream>
#include <vector>
#include <cstdlib>
#include <cctype>
#include <sstream>

#include "Predicate.h"

using namespace std;

class Rule
{
public:
    
    Rule(string name);
    ~Rule();

    string toString();
    
    string getValue();
    
    void addPredicate(Predicate rulePred);
    
    vector<Predicate> predicateList;
    
private:
    string value;
};

#endif /* Rule_h */
