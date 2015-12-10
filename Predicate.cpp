//
//  Predicate.cpp
//  L1-Lexical-Analyzer
//
//  Created by Jake Hasler on 10/23/15.
//  Copyright © 2015 Jake Hasler. All rights reserved.
//

#include "Predicate.h"


Predicate::Predicate(string name)
{
    value = name;
    
    ifId = false;
    ifString = false;
    ifParam = false;
}

Predicate::~Predicate()
{
    
}

void Predicate::addParameter(Parameter newParam)
{
    paramList.push_back(newParam);
}

void Predicate::addID(string newID)
{
    idList.push_back(newID);
}

void Predicate::addString(string newString)
{

    stringList.push_back(newString);
}

vector<string> Predicate::getSchema()
{
    if(ifId)
    {
        return idList;
    }
    else return ruleList;
}

vector<string> Predicate::getParams()
{
    vector<string> emptyVec;
    
    if(ifString)
    {
        return stringList;
    }
    else return emptyVec;
    
}

vector<Parameter> Predicate::getParamList()
{
//    if(ifId && ifString)
//    {
//        return paramList;
//    }
    
    return paramList;
}

// May need to reduce complexity of this function. 
string Predicate::toString()
{
//    if(idList.size() > 1) cout << "idList [1]: " << idList[1] << endl;
    if(ifId)
    {
        stringstream ss;
        ss << value << "(";
        
        for (int i = 0; i < idList.size(); i++)
        {
            // Prints the first and last item without commas
            if(idList.size() <= 1 || i == idList.size() - 1)
            {
                ss << idList[i];
            }
            else
            {
                ss << idList[i] << ",";

            }
            
        }
        ss << ")";
        return ss.str();

    }
    else if(ifString)
    {
        stringstream ss;
        ss << value << "(";
        
        for (int i = 0; i < stringList.size(); i++)
        {
            // Prints the first and last item without commas
            if(stringList.size() <= 1 || i == stringList.size() - 1)
            {
                ss << stringList[i];
            }
            else
            {
                // adds a comma to it.
                ss << stringList[i] << ",";
            }
            
        }
        ss << ")";
        return ss.str();
        
    }
    else
    {
        stringstream ss;
        ss << value << "(";
        
        for (int i = 0; i < paramList.size(); i++)
        {
            if(paramList.size() <= 1 || i == paramList.size() - 1)
            {
                ss << paramList[i].toString();
            }
            else
            {
                ss << paramList[i].toString() << ",";
            }
            
        }
        ss << ")";
        //cout << ss.str() << " SS" << endl;
        return ss.str();

    }
}

void Predicate::setValue(string name)
{
    value = name;
}

string Predicate::getValue()
{
    return value;
}