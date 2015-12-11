//
//  DBProcessor.cpp
//  DatalogInterpreter
//
//  Created by Jake Hasler on 10/30/15.
//  Copyright © 2015 Jake Hasler. All rights reserved.
//

#include "DBProcessor.h"

DBProcessor::DBProcessor(Parser obj)
{
    theParser = obj;

    // Set domainList to that of the Parser object for ease of use.
    domainList = theParser.getDomain();
    schemeList = theParser.getSchemes();
    factList = theParser.getFacts();
    ruleList = theParser.getRules();
    queryList = theParser.getQueries();

    theGraph.setRules(ruleList);


    ruleCounter = 0;
}

DBProcessor::~DBProcessor()
{

}


Relation DBProcessor::createRelation(int index)
{
    Relation newRelation;
    // running through the Schemelist to create new Relations.

        //cout << schemeList[i].getValue() << endl;

        // Initialize relation object with name value from schemeList
        newRelation.setName(schemeList[index].getValue());

        // set temp schema vector to the head values of the scheme.
        vector<string> schemaList = schemeList[index].getSchema();

        // set schema to those values
        newRelation.setSchema(schemaList);

//    for(Predicate val : factList)
//    {
//        cout << "val: " << val.toString() << endl;
//    }

        for(int i = 0; i < factList.size(); i++)
        {
            // populate tuples of the relation with the facts from the same name value.
            if(factList[i].getValue() == newRelation.getName())
            {
                vector<string> paramVec = factList[i].getParams();

                newRelation.addTuple(paramVec);
            }
        }

    return newRelation;
}

Relation DBProcessor::evalQuery(int index, string evalType, int predIndex)
{
    map<string, Relation>::iterator iter;
    string queryName;

    vector<Parameter> paramList;

    // setting Parameter List depending on the type of evaluation
    if(evalType == "QUERY")
    {
        paramList = queryList[index].getParamList();
        queryName = queryList[index].getValue();
    }
    else if(evalType == "RULE")
    {
        paramList = ruleList[index].predicateList[predIndex].getParamList();
        queryName = ruleList[index].predicateList[predIndex].getValue();
    }

    // Can be done because of friend class.
    Relation relOriginal = theDB.theDatabase.find(queryName)->second;
    Relation relCopy;

    // Setting the name of the Relation Copy
    relCopy.setName(relOriginal.getName());
    relCopy.setSchema(relOriginal.getSchema());

    //cout << relOriginal.toString() << endl;

    // need to then go through parameters, and pass the relCopy to the functions.

    bool inIdList = false;
    int idCounter = 0;
    bool selectString = false;
    int vecCounter = 0;

    vector<string> stringVec;
    vector<int> stringPositions;
    vector<string> idVec;
    vector<int> idPositions;
    vector<string> renameList;
    vector<int> renamePositions;

    map<string, vector<int>> idMap;

    // iterates through each parameter of the query
    for (int i = 0; i < paramList.size(); i++)
    {
        // sets values to variables for tokenType and value - Better for complexity sake, so it doesn't getValue every time.
        string tokenType = paramList[i].getTokenType();
        string value = paramList[i].getValue();

        bool inMap = false;

        if(tokenType == "STRING")
        {
            // Select on the Relation
            stringVec.push_back(value);
            stringPositions.push_back(i);
            selectString = true;
            // Select will add the tuples meeting criteria the the new vector copy
        }
        else if(tokenType == "ID")
        {
            // If ID not in List
            // add to the list - projection
            // else if not in list, call ID Select

            // Map is officially FILLED from the following 20 lines
            for(auto& ids : idMap)
            {
                if(ids.first == value)
                {
                    inIdList = true;
                    idVec.push_back(value);
                    renameList.push_back(value);
                    idPositions.push_back(i);
                    ids.second.push_back(i);
                    inMap = true;
                }
            }
            if(!inMap)
            {
                int position = i;
                vector<int> positions;
                idVec.push_back(value);
                idPositions.push_back(i);
                // push the current position into the vector.
                positions.push_back(position);
                idMap.insert(pair<string, vector<int>> (value, positions));
            }

            // Project one time on an ID.
            // if haven't seen, add to list to project before
            // temporary lists with information for later operations.
        }
    }
    finalIdMap = idMap;
    if(selectString)
    {
        theDB.selectString(relOriginal, relCopy, stringVec, stringPositions, vecCounter);
    }
    if(inIdList || idCounter > 0)
    {
        theDB.selectID(relOriginal, relCopy, idMap, vecCounter, selectString);
    }
    else if(!inIdList && !selectString)
    {
        for(vector<string> singleTuple : relOriginal.getTupleSet())
        {
            vecCounter++;
            relCopy.addTuple(singleTuple);
        }

    }

    theDB.project(relCopy, idVec, idPositions, vecCounter);
    theDB.rename(relCopy, idVec, idPositions, renameList);
    // printing out the copy of the relation after the queryEvaluation
    if(evalType == "QUERY")
    {
        cout << queryResult(relCopy, idVec, index, vecCounter);
        // outfile << queryResult(relCopy, idVec, index, vecCounter);

    }
    //cout << relCopy.toString() << endl;
    newDB.addRelation(relCopy.getName(), relCopy);
    return relCopy;
}

Relation DBProcessor::evalRule(int index)
{

    //cout << "Value of Rule: " << ruleList[index].getValue() << endl;
    // Getting the list of predicates from the
    vector<Predicate> rulePredicates = ruleList[index].predicateList;
    //cout << "This rule is: " << ruleList[index].toString() << endl;
    vector<Relation> relationList;

    Predicate headPred = rulePredicates[0];
    //cout << "HeadPred " << headPred.toString() << endl;

    int relCounter = 0;

    int factsAdded = 0;


    // For each predicate of the rule at the index passed into the function
    for(int i = 1; i < rulePredicates.size(); i++)
    {
        // starts at 1 beacause its after after the initial predicate.
        Relation temp = evalQuery(index, "RULE", i);
        //cout << temp.toString();
        relationList.push_back(temp);
        relCounter++;
    }

//    cout << "Size of Relation List" << endl;
//    cout << relationList.size() << endl;

    if(relCounter == 1)
    {
        //project
        Relation tempRel = relationList[0];
        theDB.fillTuple(tempRel, headPred, factsAdded);
        //if(tempRel.getTupleSet().size() == 0) tempRel.ifEmpty = true;
        return tempRel;
    }
    else if(relCounter == 2)
    {
        //cout << "The Rule to be joined is: " << ruleList[index].toString() << endl;
        Relation relResult = theDB.join(relationList[0], relationList[1]);
        theDB.fillTuple(relResult, headPred, factsAdded);
        //if(relResult.getTupleSet().size() == 0) relResult.ifEmpty = true;
        //cout << theDB.toString();
        return relResult;
    }
    else if(relCounter > 2)
    {
        Relation temp;
        Relation temp2;

        //Joining first two relations
        temp = theDB.join(relationList[0], relationList[1]);
        //cout << "Temp Relation: " << endl;
        //cout << temp.toString() << endl;
        for(int i = 2; i < relationList.size(); i++)
        {
            // for the rest in body, joining temp with next one, and so on.
            Relation newRel = relationList[i];
//            cout << "New Rel: " << endl;
//            cout << newRel.toString() << endl;
            //cout << temp.toString() << endl;
            temp = theDB.join(temp, newRel);
//            cout << "Joined Temp Rel: " << endl;
//            cout << temp.toString() << endl;
        }
        theDB.fillTuple(temp, headPred, factsAdded);
        return temp;
    }

    //cout << "Facts Added = " << factsAdded << endl;
    //if(factsAdded == 0) theDB.joined = true;

    Relation emptyRel;
    emptyRel.ifEmpty = true;
    // Just an empty relation if there aren't any predicates or some other error.
    return  emptyRel;

}


void DBProcessor::processGraph()
{
    theGraph.getGraph();
    theGraph.getReverseGraph();

    cout << "Dependency Graph" << endl;
    cout << theGraph.toString("forward");
    // outfile << "Dependency Graph" << endl;
    // outfile << theGraph.toString("forward");


    // DFS FOREST -> Start at first Node
    for(int i = 0; i < theGraph.reverseNodeGraph.size(); i++)
    {
        theGraph.DFS("reverse", i);
    }
    // will automatically be performed on postNums
    theGraph.doDFS("forward");
}


string DBProcessor::queryResult(Relation relCopy, vector<string> idVec, int index, int vecCounter)
{
    stringstream ss;
    set<vector<string>> setFromCopy = relCopy.getTupleSet();
    vector<string> schemaFromCopy = relCopy.getSchema();
    bool emptyTuple = false;

    for(vector<string> vec : setFromCopy)
    {
        if(vec.size() == 0)
        {
            emptyTuple = true;
        }
    }

    ss << queryList[index].toString() << "? ";
    if(vecCounter > 0)
    {
        ss << "Yes(" << setFromCopy.size() << ")" << endl;

        for(vector<string> tuple : setFromCopy)
        {
            vector<string> visited;
            for(int i = 0; i < schemaFromCopy.size(); i++)
            {

                bool match = false;
                for(string id : visited)
                {
                    if(id == schemaFromCopy[i])
                    {
                        match = true;
                    }
                }

                if(match == false)
                {
                    if(i != 0) ss << ", ";
                    else ss << "  ";
                    ss << schemaFromCopy[i] << "=" << tuple[i];
                    visited.push_back(schemaFromCopy[i]);
                }


            }
            if(idVec.size() != 0) ss << endl;
        }

    }
    else if(vecCounter == 0) ss << "No" << endl;

    return ss.str();
}


string DBProcessor::newResult()
{
   return "yes";
}



void DBProcessor::process()
{
    // Where Left off
    // DFS("forward", 0)
    // Actually inserting the strongly connected components?
    // What to do with them after they are inserted?
    // Trivial?

    //cout << theParser.toString();

    // adds relations to the map for each scheme.
    for (int i = 0; i < schemeList.size(); i++)
    {
        Relation createdRelation = createRelation(i);
        theDB.addRelation(createdRelation.getName(), createdRelation);
    }


    // Just printing out the DB for visual verification
    //cout << theDB.toString() << endl;


    /** LAB 5 GRAPH OPERATIONS **/
    processGraph();
    cout << endl;
    cout << "Rule Evaluation" << endl;
    // outfile << "Rule Evaluation" << endl;



    // for each SCC set
    // for each component
    // evalRule("RULE", component);
    // store data member of passes in each rule?

    /** LAB 5 TESTING RUN THROUGH **/
    for(int i = 0; i < theGraph.SCC.size(); i++)
    {
      theDB.joined = false;
      int passCounter = 0;
      int count = 0;
      while(!theDB.joined)
      { /*!theDB.joined*/
        passCounter++;
        theDB.joined = true;
        for(int comp : theGraph.SCC[i])
        {
          string ruleName = ruleList[comp].getValue();
          unsigned long origSize = theDB.theDatabase.find(ruleName)->second.getTupleSet().size();
          Relation evaluated = evalRule(comp);
          unsigned long secondSize = theDB.theDatabase.find(ruleName)->second.getTupleSet().size();
          if(origSize != secondSize)
          {
              theDB.joined = false;
          }
          // If trivial, then break out of loop.
          if(theGraph.isTrivial(comp))
          {
            // cout << ruleName << "is trivial" << endl;
            passCounter = 1;
            theDB.joined = true;
          }
        }
      }
      theGraph.SCCPasses.push_back(passCounter);
    }

    cout << theGraph.printPasses() << endl;
    // outfile << theGraph.printPasses() << endl;
    /** LAB 4 RULE OPERATIONS **/
    // Always have to access rulePredicateList at index > 0, then add joined preds to the
    // For each Rule in the ruleList.
    // theDB.joined = false;
    // while(!theDB.joined)
    // {
    //
    //     theDB.joined = true;
    //     ruleCounter++;
    //     for (int i = 0; i < ruleList.size(); i++)
    //     {
    //         string ruleName = ruleList[i].getValue();
    //         unsigned long origSize = theDB.theDatabase.find(ruleName)->second.getTupleSet().size();
    //
    //         //Relation evaluated = evalRule(i);
    //         unsigned long secondSize = theDB.theDatabase.find(ruleName)->second.getTupleSet().size();
    //
    //         if(origSize != secondSize)
    //         {
    //             theDB.joined = false;
    //         }
    //     }
    // }


    //cout << "Schemes populated after " << ruleCounter << " passes through the Rules." << endl;
   //cout << theDB.toString() << endl;
    cout << "Query Evaluation " << endl;
    /** LAB 3 QUERY OPERATIONS **/
    for (int i = 0; i < queryList.size(); i++)
    {

        evalQuery(i, "QUERY", 0);

    }

    // outfile.close();

    //cout << newDB.toString() << endl;


}
