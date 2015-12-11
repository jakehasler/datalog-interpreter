//
//  DBProcessor.hpp
//  DatalogInterpreter
//
//  Created by Jake Hasler on 10/30/15.
//  Copyright © 2015 Jake Hasler. All rights reserved.
//

#ifndef DBProcessor_h
#define DBProcessor_h

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
#include <stack>

#include "Parser.h"
#include "Scanner.h"
#include "Token.h"
#include "Relation.h"
#include "Database.h"
#include "Rule.h"
#include "Predicate.h"
#include "Node.h"
#include "Graph.h"

using namespace std;

class DBProcessor
{

public:
    DBProcessor(Parser obj);
    ~DBProcessor();

    void process();
    Relation createRelation(int index);
    Relation evalQuery(int index, string evalType, int predIndex);
    Relation evalRule(int index);
    void processGraph();


    string queryResult(Relation relCopy, vector<string> idVec, int index, int vecCounter);

    string newResult();
    

private:
    Parser theParser;
    Database theDB;
    Database newDB;
    Graph theGraph;

    int ruleCounter;
    //bool ifAdded;

    vector<Predicate> schemeList;
    vector<Predicate> factList;
    vector<Rule> ruleList;
    vector<Predicate> queryList;
    set<string> domainList;



    map<string, vector<int>> finalIdMap;
};


#endif /* DBProcessor_h */
