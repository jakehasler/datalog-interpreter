//
//  Database.hpp
//  DatalogInterpreter
//
//  Created by Jake Hasler on 10/29/15.
//  Copyright © 2015 Jake Hasler. All rights reserved.
//

#ifndef Database_h
#define Database_h

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
#include <map>

#include "Parser.h"
#include "Scanner.h"
#include "Token.h"
#include "Relation.h"
#include "Predicate.h"
#include "Parameter.h"

using namespace std;

class Database
{
friend class DBProcessor;
public:
    Database();
    ~Database();
    
    void addRelation(string name, Relation newRelation);
    
    void selectID(Relation &relOriginal, Relation &relCopy, map<string, vector<int>> idMap, int &vecCounter, bool selectString);
    
    void oldSelectString(Relation &relOriginal, Relation &relCopy, string value, int index);
    
    void selectString(Relation &relOriginal, Relation &relCopy, vector<string> values, vector<int> indices, int &vecCounter);
    
    void project(Relation &relCopy, vector<string> idVec, vector<int> idPositions, int &vecCounter);
    
    void rename(Relation &relCopy, vector<string> idVec, vector<int> idPositions, vector<string> renameList);
    
    Relation join(Relation rel1, Relation rel2);
    
    void fillTuple(Relation tempRel, Predicate toMatch, int &factsAdded);
    
    Relation crossProduct(set<vector<string>> set1, set<vector<string>> set2, vector<string> newSchema);
    
    map<string, vector<vector<int>>> getMatchMap(vector<string> sche1,vector<string> sche2, bool &matching);
    
    void naturalJoin(Relation &tempRel, map<string, vector<vector<int>>> matchMap, set<vector<string>> set1, set<vector<string>> set2, vector<string> newSchema, vector<int> uniqueInd);
    
    vector<string> renameJoin(Relation rel, Relation rel2, vector<int> &uniqueInd);
    
    string toString();
    
    string printSet(set<vector<string>> set1);
    
private:
    map<string, Relation> theDatabase;
    
    bool joined;
    
    int stringCount;
};


#endif /* Database_h */
