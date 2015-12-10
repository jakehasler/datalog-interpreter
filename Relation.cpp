//
//  Relation.cpp
//  DatalogInterpreter
//
//  Created by Jake Hasler on 10/29/15.
//  Copyright © 2015 Jake Hasler. All rights reserved.
//

#include "Relation.h"


Relation::Relation(string value)
{
    name = value;
    ifEmpty = false;
}

// Default Constructor
Relation::Relation()
{
    ifEmpty = false;
}

Relation::~Relation()
{
    
}

void Relation::setName(string value)
{
    name = value;
}

void Relation::setSchema(vector<string> singleSchema)
{
    schema = singleSchema;
}

void Relation::addTuple(vector<string> singleTuple)
{
    tuples.insert(singleTuple);
}

vector<string> Relation::getSchema()
{
    return schema;
}

void Relation::resetTuples(set<vector<string> > newTuples)
{
    tuples = newTuples;
}

set<vector<string>> Relation::getTupleSet()
{
    return this->tuples;
}

vector<string> Relation::getTuple(string value, int index)
{
    //cout << "Index Passed in = " << index << endl;
    //cout << "Value Passed in = " << value << endl;
    cout << "entered" << endl;
    vector<string> currentTuple;
    
    //cout << "currentTuple[index] = " << currentTuple[index] << endl;
    
    //  loop through set, if value at index of tuple is equal to param value, returns just that tuple.
    
    int i = 0;
    for(vector<string> tuple : tuples)
    {
        //cout << "i = "<< i << endl;
        //cout << "Tuple["<< index << "] = " << tuple[index] << endl;
        if(tuple[index] == value)
        {
            //cout << "Tuple["<< index << "] = " << tuple[index] << endl;
            currentTuple = tuple;
            //return currentTuple;
        }
        i++;
    }
    
//    cout << currentTuple.size() << " currentTupleSize" << endl;
//    for(string i : currentTuple)
//    {
//        cout << "i =" << i << "at index:" << index << endl;
//    }
    //cout << "returned = " << currentTuple[index] << endl;
    return currentTuple;
}

string Relation::getName()
{
    return name;
}


string Relation::toString()
{
    stringstream ss;
    
    ss << "Name: " << name << endl;
    
    for(string colHead : schema)
    {
        ss << " " << colHead << "  ";
    }
    ss << endl;
    
    // For each vector in the tuple set
    for(vector<string> tuple : tuples)
    {
        
        for(string val : tuple)
        {
            ss << val << " ";
        }
        ss << endl;
    }
    
    return ss.str();
}