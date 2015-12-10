//
//  Node.hpp
//  DatalogInterpreter
//
//  Created by Jake Hasler on 12/8/15.
//  Copyright © 2015 Jake Hasler. All rights reserved.
//

#ifndef Node_h
#define Node_h

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

using namespace std;


class Node {
public:
    
    Node(int value);
    Node();
    ~Node();
    
    void setName(int value);
    void setPON(int postNum);
    void setChildren(set<int> childs);
    void markVisited();
    
    void addChild(int child);
    
    void clearChildren();

    int getPON();
    int getName();
    set<int> getChildren();
    
    string toString();
    
    bool visited;
    bool hasNum;
    
private:
    int name;
    int postNum;
    set<int> children;
};

#endif /* Node_h */
