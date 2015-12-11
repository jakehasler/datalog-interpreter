//
//  Database.cpp
//  DatalogInterpreter
//
//  Created by Jake Hasler on 10/29/15.
//  Copyright © 2015 Jake Hasler. All rights reserved.
//

#include "Database.h"



Database::Database()
{
    stringCount = 0;

    joined = false;
}

Database::~Database()
{


}

void Database::addRelation(string name, Relation newRelation)
{
    theDatabase.insert(pair<string, Relation> (name, newRelation));
}

void Database::oldSelectString(Relation &relOriginal, Relation &relCopy, string value, int index)
{
    // 'index' is indicative of index of the query param value
    // gets specific tuple meeting certain value at an index
    //cout << "Index Passed in = " << index << endl;
    //cout << "Value Passed in = " << value << endl;
    set<vector<string>> tuplesFromRelation = relOriginal.getTupleSet();

    for(vector<string> tuple : tuplesFromRelation)
    {
        if(tuple[index] == value)
        {
            relCopy.addTuple(tuple);
        }
        //relCopy.addTuple(relOriginal.getTuple(value, index));
    }

    // if tuple value is equal to index
    //relCopy.addTuple(relOriginal.getTuple(value, index));

    //cout << "RelCopy = " << relCopy.toString() << endl;
}

void Database::selectString(Relation &relOriginal, Relation &relCopy, vector<string> values, vector<int> indices, int &vecCounter)
{
    // 'index' is indicative of index of the query param value
    // gets specific tuple meeting certain value at an index
    set<vector<string>> tuplesFromRelation = relOriginal.getTupleSet();

    for(vector<string> tuple : tuplesFromRelation)
    {
        //int i = 0;
        bool valid = true;
        for(int i = 0; i < indices.size(); i++)
        {
            //cout << "Indices Value at " << i << ": "<< tuple[indices[i]] << endl;
            if(tuple[indices[i]] != values[i])
            {
                //cout << "Setting Valid false." << endl;
                valid = false;
                break;
            }
        }

        if(valid)
        {
            relCopy.addTuple(tuple);
            vecCounter++;
        }

    }

}

void Database::selectID(Relation &relOriginal, Relation &relCopy, map<string, vector<int>> idMap, int &vecCounter, bool selectString)
{
    set<vector<string>> tuplesFromRelation;
    if(selectString)
    {
        tuplesFromRelation = relCopy.getTupleSet();
    }
    else
    {
        tuplesFromRelation = relOriginal.getTupleSet();
    }

    vecCounter = 0;
    Relation relTemp;
    set<vector<string>> tuplesFromCopy = relCopy.getTupleSet();
    vector<string> schemaFromCopy = relCopy.getSchema();
    relTemp.setName(relCopy.getName());
    relTemp.setSchema(schemaFromCopy);

    for(vector<string> tuple : tuplesFromRelation)
    {
        bool valid = true;

        for(auto& ids : idMap)
        {
            if(ids.second.size() > 1)
            {
                for(int i = 0; i < ids.second.size(); i++)
                {
                    if(tuple[ids.second[0]] != tuple[ids.second[i]])
                    {
                        valid = false;
                        break;
                    }
                }
            }
        }

        if(valid)
        {
            relTemp.addTuple(tuple);
            vecCounter++;
        }

    }
    relCopy = relTemp;
}

void Database::project(Relation &relCopy, vector<string> idVec, vector<int> idPositions, int &vecCounter)
{

    Relation relTemp;
    set<vector<string>> tuplesFromCopy = relCopy.getTupleSet();
    vector<string> schemaFromCopy = relCopy.getSchema();
    relTemp.setName(relCopy.getName());
//    cout << "TuplesFromCopySize: " << tuplesFromCopy.size() << endl;
    for(vector<string> tuple : tuplesFromCopy)
    {

        vector<string> newTuple;
        vector<string> newSchema;

        for(int index : idPositions)
        {
//            cout << "TupleSize: " << tuple.size() << endl;
            //cout << tuple[index] << endl;
            if(tuple.size() > index)
            {
//                newSchema.push_back(schemaFromCopy[index]);
                newTuple.push_back(tuple[index]);
            }
            newSchema.push_back(schemaFromCopy[index]);
//            newTuple.push_back(tuple[index]);
        }
        relTemp.setSchema(newSchema);
        relTemp.addTuple(newTuple);
    }

    relCopy = relTemp;

}

void Database::rename(Relation &relCopy, vector<string> idVec, vector<int> idPositions, vector<string> renameList)
{
    vector<string> toRename;
    vector<string> schemaFromCopy = relCopy.getSchema();

    relCopy.setSchema(idVec);
}

string Database::printSet(set<vector<string>> set1)
{
    stringstream ss;
    ss << "Printing Set: " << endl;

    for(vector<string> vec : set1)
            {
                //cout << vec.size() << endl;
                for(string item : vec)
                {
                    ss << item << endl;
                }

            }
    return ss.str();
}


Relation Database::join(Relation rel1, Relation rel2)
{
    //cout << "Current DB: " << endl << toString() << endl;
    //cout << "Joining the Relations passed into the function" << endl;

    //cout << "Rel 1: " << rel1.toString() << endl;
    //cout << "Rel 2: " << rel2.toString() << endl;
    set<vector<string>> set1 = rel1.getTupleSet();
    set<vector<string>> set2 = rel2.getTupleSet();
    vector<string> sche1 = rel1.getSchema();
    vector<string> sche2 = rel2.getSchema();

    vector<int> uniqueInd;

    Relation tempRel;

    // Getting the resulting schema from both relations
    vector<string> newSchema = renameJoin(rel1, rel2, uniqueInd);

//    cout << printSet(rel1.getTupleSet()) << endl;

    // Get position values from each relation where ID's Match
    // if no matches, then get cross product - CHECK :HT
    // - for each tuple in rel1, addTuple(tuple+rel2(eachTuple))
    // if matches

    // If there are no matches in either of the schema
    bool matching = false;

    // Send schemas to get map of the matches.
    map<string, vector<vector<int>>> matchMap = getMatchMap(sche1, sche2, matching);

    // matching = true;
    if(matching == false)
    {

        //cout << "CP" << endl;
        tempRel = crossProduct(set1, set2, newSchema);
        //cout << tempRel.toString() << endl;
    }
    else
    {

        //cout << "NJ" << endl;
        // Will join both relations
        naturalJoin(tempRel, matchMap, set1, set2, newSchema, uniqueInd);
    }

    return tempRel;

}

void Database::fillTuple(Relation tempRel, Predicate toMatch, int &factsAdded)
{
    vector<string> resultVec;
    resultVec.push_back("'0'");
    resultVec.push_back("'1'");

    //cout << "The Relation filling " << "Pred: " << toMatch.toString() << endl;
    //cout << tempRel.toString() << endl;
    //cout  << endl;
    //cout << "New Schema: " << endl;


    vector<string> paramsToMatch;
    string relationName = toMatch.getValue();
    vector<string> schemaFromTemp = tempRel.getSchema();
    vector<vector<int>> positions;

    if(toMatch.ifId) paramsToMatch = toMatch.idList;
    if(toMatch.ifString) paramsToMatch = toMatch.stringList;

    for(int i = 0; i < paramsToMatch.size(); i++)
    {
        for(int j = 0; j < schemaFromTemp.size(); j++)
        {
            if(paramsToMatch[i] == schemaFromTemp[j])
            {
                vector<int> newVec;
                positions.push_back(newVec);
                positions[i].push_back(i);
                positions[i].push_back(j);
            }
        }
    }


    // Looping through the matches
    for(vector<string> tuples : tempRel.getTupleSet())
    {
        vector<string> newTuple;
        // Go through each position value.
        for(vector<int> posVec : positions)
        {
            // The tuple is only added if the values at each index are equal.
            newTuple.push_back(tuples[posVec[1]]);
        }

        // Adding tuple to the matching name of the predicate
        theDatabase.find(relationName)->second.addTuple(newTuple);
    }
    factsAdded++;

}

void Database::naturalJoin(Relation &tempRel, map<string, vector<vector<int>>> matchMap, set<vector<string>> set1, set<vector<string>> set2, vector<string> newSchema, vector<int> uniqueInd)
{

    // Questions: Account for multiple matching IDs in a Join?
    // setting the schema of the new Relation.
    // Go through each tuple of first set
    for(vector<string> tuple1 : set1)
    {
        for(vector<string> tuple2 : set2)
        {
            // Get index values of matching IDs
            // currently only accounting for one match.
            vector<string> newTuple;
            vector<string> extraTuple;
            bool add = true;
            for(auto& id : matchMap)
            {

                // Only if second vector has at least one position value in it.
                if(id.second[1].size() > 0)
                {

                    int i1 = id.second[0][0];
                    int i2 = id.second[1][0];

                    string idfirst = id.first;
                    // Ensures that the match map vector sizes aren't less than the indices
                    // 	that are being accessed.

                    if(tuple1.size() > i1 && tuple2.size() > i2)
                    {

                        if(tuple1[i1] != tuple2[i2])
                        {

                            add = false;
                        }

					}

                }
            } // end match map loop

            if(add)
            {

                // set newTuple to the value of the tuple.
                for(string val : tuple1)
                {
                    newTuple.push_back(val);
                }

               ///unsigned long sizeDif = newSchema.size() - newTuple.size();
                //cout << "here. " << endl;
                for(int j = 0; j < uniqueInd.size(); j++)
                {
                    newTuple.push_back(tuple2[uniqueInd[j]]);
                }

                if(newTuple.size() > 0) tempRel.addTuple(newTuple);

            }

        }
    }// end relation set loops

    tempRel.setSchema(newSchema);
}

Relation Database::crossProduct(set<vector<string>> set1, set<vector<string>> set2, vector<string> newSchema)
{
    Relation tempRel;
    unsigned long vecLength = 0;

    // Go through each tuple
    for(vector<string> tuple : set1)
    {
        for(vector<string> tuple2 : set2)
        {

        vector<string> tempVec;
        // Go through each item in the tuple
        for(string ID : tuple)
        {
            // Pushing each ID from vec1 onto temp vec
            tempVec.push_back(ID);
        }

        // Go through set2, append each to tempVec
        // Only need to add from same tuple
        // currently adding from every tuple to the one vector.

        for(string ID2 : tuple2)
        {
                //cout << "Adding " << ID2 << endl;
            tempVec.push_back(ID2);

        }
            vecLength = tempVec.size();
            tempRel.addTuple(tempVec);
        }

    }

//    for(string temp : tempVec)
//    {
//
//    }
    //cout << tempRel.toString() << endl;
    //cout << vecLength << endl;
    //if(vecLength == newSchema.size())
    //{
        tempRel.setSchema(newSchema);
        //cout << "Result of Cross Product: " << tempRel.toString() << endl;
    //}

    //cout << tempRel.toString() << endl;
    return tempRel;
}


map<string, vector<vector<int>>> Database::getMatchMap(vector<string> sche1,vector<string> sche2, bool &matching)
{
    map<string, vector<vector<int>>> matchMap;
    int matchCount = 0;


    // Testing things out.
    vector<vector<int>> matchVec;

    for(int i = 0; i < sche1.size(); i++)
    {
        bool inMatchVec = false;

        for(int i = 0; i < matchVec.size(); i++)
        {

        }

        if(!inMatchVec)
        {

        }
    }


    // Put each item from first schema in the map
    // If value if first schema appears more than once, then appends another
    //      position value to the first vector.
    for(int i = 0; i < sche1.size(); i++)
    {
        bool inMatchMap = false;

        for(auto& id : matchMap)
        {
            if(id.first == sche1[i])
            {
                // Adding another position to the first vector if that's the case.
                inMatchMap = true;
                id.second[0].push_back(i);
            }
        }

        if(!inMatchMap)
        {
            string ID = sche1[i];
            vector<vector<int>> posVec;
            vector<int> vec1;
            vector<int> vec2;
            vec1.push_back(i);
            posVec.push_back(vec1);
            posVec.push_back(vec2);

            matchMap.insert(pair<string, vector<vector<int>>> (ID, posVec));
        }

    }

    // Check each item in second schema if matches, push to second posVector.
    for(int j = 0; j < sche2.size(); j++)
    {
        for(auto& id : matchMap)
        {
            if(id.first == sche2[j])
            {
                matchCount++;
                //matching = true;
                // pushing position to the second vector in the map.
                id.second[1].push_back(j);
            }
        }

    }

    if(matchCount > 0) matching = true;
    // matching = false;
    return matchMap;
}


vector<string> Database::renameJoin(Relation rel1, Relation rel2, vector<int> &uniqueInd)
{
    vector<string> temp;

    vector<string> schema1 = rel1.getSchema();
    vector<string> schema2 = rel2.getSchema();



    set<string> tempSet;

    for (int i = 0; i < schema1.size(); i++)
    {
        temp.push_back(schema1[i]);
    }

    for(int i = 0; i < schema2.size(); i++)
    {
        string tempStr = schema2[i];
        int inVec = 0;
        for(string item1 : schema1)
        {
            if(schema2[i] == item1)
            {
                inVec++;
            }
        }
        if(inVec == 0)
        {
            temp.push_back(tempStr);
            uniqueInd.push_back(i);
        }
    }


    // returning the filled vector.
    return temp;

}



string Database::toString()
{
    stringstream ss;

    ss << "The Database" << endl;
    for(auto& nameRelation : theDatabase)
    {
        ss << nameRelation.second.toString() << endl;
    }

    return ss.str();
}
