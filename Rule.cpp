//
//  Rule.cpp
//  L1-Lexical-Analyzer
//
//  Created by Jake Hasler on 10/23/15.
//  Copyright © 2015 Jake Hasler. All rights reserved.
//

#include "Rule.h"

Rule::Rule(string name)
{
    value = name;
}

Rule::~Rule()
{
    
}

string Rule::getValue()
{
    return value;
}

void Rule::addPredicate(Predicate rulePred)
{
    predicateList.push_back(rulePred);
}

string Rule::toString()
{
    stringstream ss;
    for (int i = 0; i < predicateList.size(); i++)
    {
        ss << predicateList[i].toString();
        if(i < 1)
        {
            ss << " :- ";
        }
        else if(i < predicateList.size() - 1)
        {
            ss << ",";
        }

    }
    return ss.str();
}