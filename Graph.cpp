//
//  Graph.cpp
//  DatalogInterpreter
//
//  Created by Jake Hasler on 12/8/15.
//  Copyright © 2015 Jake Hasler. All rights reserved.
//

#include "Graph.h"

Graph::Graph()
{
    currentPostNum = 0;
    currentNode = 0;

    for(int i = 0 ; i < nodeGraph.size(); i++)
    {
        set<int> emptySet;
        SCC.push_back(emptySet);
    }
}

Graph::~Graph()
{

}

void Graph::setRules(vector<Rule> ruleList)
{
    this->ruleList = ruleList;
}

void Graph::incrementNode()
{
    currentNode++;
}

void Graph::incrementPON()
{
    currentPostNum++;
}

void Graph::getGraph()
{
    // Go through each rule
    for (int i = 0; i < ruleList.size(); i++)
    {
        string currentRuleName = ruleList[i].getValue();
        set<int> dependencies;
        Node temp(i);
        vector<Predicate> rulePredicates = ruleList[i].predicateList;

        // I for not including head predicate
        for(int j = 1; j < rulePredicates.size(); j++)
        {
            for(int k = 0; k < ruleList.size(); k++)
            {
                // check the head predicate value of each rule if equal to any of the predicates
                if(rulePredicates[j].getValue() == ruleList[k].getValue())
                {
                    // putting index value of the rule into the set.
                    //temp.setName(k);
                    dependencies.insert(k);

                }
            }
        }

        temp.setChildren(dependencies);
        // pushing temporary set into the Graph.
        theGraph.push_back(dependencies);
        nodeGraph.push_back(temp);
    }

}

void Graph::getReverseGraph()
{
    for(int i = 0; i< theGraph.size(); i++)
    {
        reverseGraph.push_back(theGraph[i]);
        reverseGraph[i].clear();
    }
    for(int i = 0; i< nodeGraph.size(); i++)
    {
        reverseNodeGraph.push_back(nodeGraph[i]);
        reverseNodeGraph[i].clearChildren();
    }

    for(int i = 0; i < nodeGraph.size(); i++)
    {
        for(int child : nodeGraph[i].getChildren())
        {
            reverseNodeGraph[child].addChild(i);
        }
    }



}


void Graph::doDFS(string which)
{
    while(!postOrderNums.empty())
    {
        // DFS on just top value from stack
        DFS("forward", postOrderNums.top());
        postOrderNums.pop();
        if(!forwardStack.empty())
        {
            set<int> singleSCC;
            while(forwardStack.size())
            {
                singleSCC.insert(forwardStack.top());
                forwardStack.pop();
            }
            SCC.push_back(singleSCC);
        }
    }
}

void Graph::DFS(string whichGraph, int whichNode)
{
    if(whichGraph == "reverse")
    {
        // Go to child, mark as visited, give PON, return.
        reverseNodeGraph[whichNode].markVisited();

        // Recursive Algorithm - Checks for each child
        for(int adjacent : reverseNodeGraph[whichNode].getChildren())
        {
            // Base Case
            if(!reverseNodeGraph[adjacent].visited)
            {
                // DFS on adjacent node
                DFS("reverse", adjacent);
            }
        }

        /*** Only gets to this part if all of the children have been visited ***/

        // Assigning Post Order Number to the node if doesn't already have a post number
        if(!reverseNodeGraph[whichNode].hasNum)
        {
            //cout << "Assigning PON of: " << currentPostNum <<  "to: " << reverseNodeGraph[whichNode].toString() << endl;
            reverseNodeGraph[whichNode].setPON(currentPostNum);
            // Pushes PON to the stack
            postOrderNums.push(currentPostNum);

            reverseNodeGraph[whichNode].hasNum = true;
            incrementPON();
        }
        //cout << reverseNodeGraph[whichNode].toString() << endl;
        orderedList.push_back(reverseNodeGraph[whichNode]);

        // Advances PON to next value


    }
    else if(whichGraph == "forward")
    {
        /** Performing on forward graph in order of Post Order Nums. **/

        // iterates through all items of the stack
//        for(int child : nodeGraph[whichNode].getChildren())
//        {
//            set<int> singleSCC;
//            // Base Case
//            if(!nodeGraph[child].visited)
//            {
//
//                nodeGraph[child].markVisited();
//                singleSCC.insert(child);
//                // DFS on adjacent node
//                DFS("forward", child);
//            }
//
//            if(!nodeGraph[whichNode].visited)
//            {
//                nodeGraph[whichNode].markVisited();
//                singleSCC.insert(whichNode);
//            }
//
//            SCC.push_back(singleSCC);
//        }

        if(nodeGraph[whichNode].visited) return;

        nodeGraph[whichNode].markVisited();

        for(int child : nodeGraph[whichNode].getChildren())
        {
            DFS("forward", child);
        }

        forwardStack.push(whichNode);
    }

}

string Graph::printPON()
{
    stringstream ss;

    for(Node node : reverseNodeGraph)
    {
        ss << "POTN(R" << node.getName() << ") = " << node.getPON() << endl;
    }

    return ss.str();
}

string Graph::printSCC()
{
    stringstream ss;

    // For Every set of Components
    for(set<int> theSet : SCC)
    {
        if(!theSet.empty())
        {
            //cout << theSet.size() << endl;
            ss << "Components: ";
            for(int comp : theSet)
            {
                ss << "R" << comp << " ";
            }
            ss << endl;
        }
    }

    return ss.str();
}

string Graph::printPasses()
{
  stringstream ss;

  //
  for(int pass : SCCPasses)
  {
    if(!theSet.empty())
    {
      for(int comp : theSet)
      {
        ss << comp << " passes: " << "R" << comp;
        if(comp == theSet.size() -1)
        {
          ss << ","
        }
      }
      ss << endl;
    }
  }

  return ss.str();
}

string Graph::toString(string which)
{
    stringstream ss;

    vector<set<int>> tempGraph;
    bool ifNode = false;

    if(which == "forward") tempGraph = theGraph;
    else if(which == "reverse") tempGraph = reverseGraph;
    else if(which == "nodeForward")
    {

        ifNode = true;
        for(int i = 0; i < nodeGraph.size(); i++)
        {
            ss << "R" << nodeGraph[i].getName() << ": ";

            for(int val : nodeGraph[i].getChildren())
            {
                ss << "R" << val << " ";
            }

            ss << endl;
        }
    }
    else if(which == "nodeReverse")
    {

        ifNode = true;
        for(int i = 0; i < reverseNodeGraph.size(); i++)
        {
            ss << "R" << reverseNodeGraph[i].getName() << ": ";

            for(int val : reverseNodeGraph[i].getChildren())
            {
                ss << "R" << val << " ";
            }

            ss << endl;
        }
    }

    if(!ifNode)
    {
        for(int i = 0; i < tempGraph.size(); i++)
        {
            ss << "R" << i << ":";
            int index = 0;
            for(int val : tempGraph[i])
            {
                ss << "R" << val;
                if(index < tempGraph[i].size() - 1) ss << ",";
                index++;
            }

            ss << endl;
        }
    }



    return ss.str();
}