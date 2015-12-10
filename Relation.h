//
//  Relation.hpp
//  DatalogInterpreter
//
//  Created by Jake Hasler on 10/29/15.
//  Copyright © 2015 Jake Hasler. All rights reserved.
//

#ifndef Relation_h
#define Relation_h

#include <stdio.h>
#include <iostream>
#include <string>
#include <fstream>
#include <ostream>
#include <vector>
#include <cstdlib>
#include <cctype>
#include <sstream>
#include <set>

#include "Parser.h"
#include "Scanner.h"
#include "Token.h"

using namespace std;

class Relation
{
public:
    Relation(string value);
    Relation();
    ~Relation();
    
    void setName(string value);
    void setSchema(vector<string> singleSchema);
    void addTuple(vector<string> singleTuple);
    
    vector<string> getTuple(string value, int index);
    vector<string> getSchema();
    
    void resetTuples(set<vector<string>> newTuples);
    
    set<vector<string>> getTupleSet();
    
    string getName();
    
    string toString();
    
    bool ifEmpty;
    
private:
    string name;
    vector<string> schema;
    
    // The set wasn't inserting all of the values - changed to vector.
    set<vector<string>> tuples;
    
    
};


#endif /* Relation_h */
