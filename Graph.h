//
//  Graph.hpp
//  DatalogInterpreter
//
//  Created by Jake Hasler on 12/8/15.
//  Copyright © 2015 Jake Hasler. All rights reserved.
//

#ifndef Graph_h
#define Graph_h

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

#include "Node.h"
#include "Rule.h"

using namespace std;

class Graph
{
friend class DBProcessor;
public:
    Graph();
    ~Graph();

    void setRules(vector<Rule> ruleList);

    void incrementNode();
    void incrementPON();

    void getGraph();
    void getReverseGraph();
    string toString(string which);
    void DFS(string whichGraph, int whichNode);
    void doDFS(string which);
    string printPON();
    string printSCC();
    string printPasses();

    // if only one child, then
    // check in graph if it has itself
    bool isTrivial(int index);

    stack<int> postOrderNums;
    stack<int> forwardStack;

private:

    int currentPostNum;
    int currentNode;

    vector<set<int>> theGraph;
    vector<Node> nodeGraph;
    vector<set<int>> reverseGraph;
    vector<Node> reverseNodeGraph;

    vector<Node> orderedList;
    vector<set<int>> SCC;
    vector<int> SCCPasses;

    vector<Rule> ruleList;

};

#endif /* Graph_h */
