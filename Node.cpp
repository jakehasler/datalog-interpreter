//
//  Node.cpp
//  DatalogInterpreter
//
//  Created by Jake Hasler on 12/8/15.
//  Copyright © 2015 Jake Hasler. All rights reserved.
//

#include "Node.h"


Node::Node(int value)
{
    // Default Constructor
    name = value;
    visited = false;
    hasNum = false;
    postNum = 0;
}

Node::~Node()
{
    // Deconstructor
}

Node::Node()
{
    // Empty Constructor
}

void Node::setName(int value)
{
    name = value;
}

void Node::setPON(int postNum)
{
    this->postNum = postNum;
}

void Node::setChildren(set<int> childs)
{
    children = childs;
}

void Node::addChild(int child)
{
    children.insert(child);
}

void Node::clearChildren()
{
    children.clear();
}

void Node::markVisited()
{
    visited = true;
}

int Node::getPON()
{
    return postNum;
}

int Node::getName()
{
    return name;
}

set<int> Node::getChildren()
{
    return children;
}

string Node::toString()
{
    stringstream ss;
    
    ss << "Node: " << name << ", PostNum: " << postNum << ", Visited: " << visited << ", NumChildren: " << children.size();
    
    return ss.str();
}