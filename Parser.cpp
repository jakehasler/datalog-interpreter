//
//  Parser.cpp
//  L1-Lexical-Analyzer
//
//  Created by Jake Hasler on 10/22/15.
//  Copyright © 2015 Jake Hasler. All rights reserved.
//

#include "Parser.h"


Parser::Parser(vector<Token> tokenVector)
{
    this->tokenVector = tokenVector;
    success = true;
    theIndex = 0;
    schemeCount = 0;
}

Parser::Parser()
{
    
}

Parser::~Parser()
{
    
}

void Parser::parse()
{
    
    try
    {
        match("SCHEMES");
        match("COLON");
        checkSchemeList();
        
        if(schemeCount < 1)
        {
            throw(tokenVector[theIndex]);
        }
        
        match("FACTS");
        match("COLON");
        checkFactList();
        match("RULES");
        match("COLON");
        checkRuleList();
        match("QUERIES");
        match("COLON");
        checkQueryList();
        
        success = true;
        
        // commenting Out the toString so that I can do it in the processor.
        // cout << toString();
    }
    catch(Token thrownToken)
    {
        cout << "Failure!" << endl << "  ";
        thrownToken.toString();
    }

}


bool Parser::match(string tokenType)
{
    if(tokenType == tokenVector[theIndex].getType())
    {
        advanceIndex();
        return true;
    }
    else
    {
        success = false;
        throw(tokenVector[theIndex]);
    }
}

void Parser::advanceIndex()
{
    theIndex++;
}


void Parser::checkSchemeList()
{
    string topType = tokenVector[theIndex].getType();
    
    // Condition takes care of Follow Set
    if(topType != "FACTS")
    {
        scheme();
        checkSchemeList();
    }

}

void Parser::checkFactList()
{
    // Condition takes care of Follow Set
    if(tokenVector[theIndex].getType() != "RULES")
    {
        fact();
        checkFactList();
    }
    
}

void Parser::checkRuleList()
{
    
    // Condition takes care of Follow Set
    if(tokenVector[theIndex].getType() != "QUERIES")
    {
        rule();
        checkRuleList();
    }

}

void Parser::checkQueryList()
{
    // Condition takes care of Follow Set
    if(tokenVector[theIndex].getType() != "EOF")
    {
        query();
        checkQueryList();
    }
}


void Parser::scheme()
{
    Predicate newPredicate(tokenVector[theIndex].getValue());
    match("ID");
    match("LEFT_PAREN");
    match("ID");
    newPredicate.addID(tokenVector[theIndex - 1].getValue());
    idList(newPredicate);
    newPredicate.ifId = true;
    match("RIGHT_PAREN");
    addScheme(newPredicate);
    schemeCount++;
}


void Parser::idList(Predicate &newPred)
{
    // Condition takes care of Follow Set
    if(tokenVector[theIndex].getType() != "RIGHT_PAREN")
    {
        match("COMMA");
        match("ID");
        newPred.addID(tokenVector[theIndex - 1].getValue());
        newPred.ifId = true;
        idList(newPred);
    }

}

void Parser::fact()
{
    Predicate factPred(tokenVector[theIndex].getValue());
    match("ID");
    match("LEFT_PAREN");
    match("STRING");
    //cout << tokenVector[theIndex - 1].getValue() << endl;
    factPred.addString(tokenVector[theIndex - 1].getValue());
    factPred.ifString = true;
    addDomain(tokenVector[theIndex - 1].getValue());
    stringList(factPred);
    match("RIGHT_PAREN");
    match("PERIOD");
    addFact(factPred);
}

void Parser::stringList(Predicate &factPred)
{
    // Condition takes care of Follow Set
    if(tokenVector[theIndex].getType() != "RIGHT_PAREN")
    {
        match("COMMA");
        match("STRING");
        factPred.addString(tokenVector[theIndex - 1].getValue());
        addDomain(tokenVector[theIndex - 1].getValue());
        factPred.ifString = true;
        stringList(factPred);
    }
    
}

void Parser::rule()
{
    Rule newRule(tokenVector[theIndex].getValue());
    headPredicate(newRule);
    match("COLON_DASH");
    rulePredicate(newRule);
    rulePredicateList(newRule);
    match("PERIOD");
    addRule(newRule);
    
}

void Parser::predicate()
{

    Predicate newPred(tokenVector[theIndex].getValue());
    match("ID");
    match("LEFT_PAREN");
    parameter(newPred.paramList);
    paramList(newPred.paramList);
    match("RIGHT_PAREN");
    
}

void Parser::rulePredicate(Rule &newRule)
{
    Predicate newPred(tokenVector[theIndex].getValue());
    match("ID");
    match("LEFT_PAREN");
    parameter(newPred.paramList);
    paramList(newPred.paramList);
    match("RIGHT_PAREN");
    newRule.addPredicate(newPred);
    
}

void Parser::queryPredicate(Predicate &queryPred)
{
    match("ID");
    match("LEFT_PAREN");
    //queryPred.ifParam = true;
    parameter(queryPred.paramList);
    paramList(queryPred.paramList);
    for (int i = 0; i < queryPred.paramList.size(); i++) {
        //cout << "Param: " << queryPred.paramList[i].toString() << endl;
    }
    match("RIGHT_PAREN");
}

void Parser::predicateList()
{
    // Condition takes care of Follow Set
    if(tokenVector[theIndex].getType() != "PERIOD")
    {
        match("COMMA");
        predicate();
        predicateList();
    }

}

void Parser::rulePredicateList(Rule &newRule)
{
    // Condition takes care of Follow Set
    if(tokenVector[theIndex].getType() != "PERIOD")
    {
        match("COMMA");
        rulePredicate(newRule);
        rulePredicateList(newRule);
    }
    
}

bool Parser::parameter(vector<Parameter> &paramList)
{
    //cout << "entered parameter()" << endl;
    //cout << "Value of the Token: " << tokenVector[theIndex].getValue() << endl;
    
    string topType = tokenVector[theIndex].getType();
    
    if(topType == "STRING" || topType == "ID")
    {
        Parameter newParam(tokenVector[theIndex].getValue(), topType);
        //cout << newParam.toString() << endl;
        paramList.push_back(newParam);
        advanceIndex();
        return true;
    }
    else if(expression(paramList))
    {
        string param1;
        string theOperator;
        string param2;
        Expression paramExpression(param1, theOperator, param2);
        Parameter expParam(paramExpression);
        expParam.ifExpression = true;
        paramList.push_back(expParam);
        return true;
    }
    else
    {
        throw(tokenVector[theIndex]);
        return false;
    }
    
    return false;
}

void Parser::paramList(vector<Parameter> &parameterList)
{
    //cout << "entered paramList()" << endl;
    
    string topType = tokenVector[theIndex].getType();

    if(topType != "RIGHT_PAREN")
    {
        match("COMMA");
        parameter(parameterList);
        paramList(parameterList);
    }

}

void Parser::headPredicate(Rule &newRule)
{
    Predicate rulePred(tokenVector[theIndex].getValue());
    match("ID");
    match("LEFT_PAREN");
    match("ID");
    rulePred.addID(tokenVector[theIndex - 1].getValue());
    idList(rulePred);
    rulePred.ifId = true;
    match("RIGHT_PAREN");
    newRule.addPredicate(rulePred);
}


bool Parser::expression(vector<Parameter> &paramList)
{
    match("LEFT_PAREN");
    parameter(paramList);
    isOperator();
    parameter(paramList);
    match("RIGHT_PAREN");
    
    return true;
}

void Parser::query()
{
    Predicate queryPred(tokenVector[theIndex].getValue());
    queryPredicate(queryPred);
    queryPred.ifParam = true;
    //queryPred.addParameter(queryParam);
    match("Q_MARK");
    addQuery(queryPred);
    for (int i = 0; i < queryPred.paramList.size(); i++) {
        //cout << "queryPred.paramList: " << queryPred.paramList[i].toString() << endl;
    }
    
}

bool Parser::isOperator()
{
    string topType = tokenVector[this->theIndex].getType();
    
    if(topType == "ADD" || topType == "MULTIPLY")
    {
        advanceIndex();
        return true;
    }
    else
    {
        throw(tokenVector[theIndex]);
        return false;
    }
}

void Parser::addScheme(Predicate pred)
{
    schemeList.push_back(pred);
}

void Parser::addFact(Predicate pred)
{
    factList.push_back(pred);
}

void Parser::addRule(Rule newRule)
{
    ruleList.push_back(newRule);
}

void Parser::addQuery(Predicate pred)
{
    queryList.push_back(pred);
}

void Parser::addDomain(string domain)
{
   domainList.insert(domain);
}

set<string> Parser::getDomain()
{
    return domainList;
}

vector<Predicate> Parser::getQueries()
{
    return queryList;
}

vector<Predicate> Parser::getFacts()
{
    return factList;
}

vector<Predicate> Parser::getSchemes()
{
    return schemeList;
}

vector<Rule> Parser::getRules()
{
    return ruleList;
}

string Parser::toString()
{
    stringstream ss;
    
    if(success == true)
    {
        ss << "Success!" << endl;
        ss << "Schemes(" << schemeList.size() << "):" << endl;
        for (int i = 0; i < schemeList.size(); i++)
        {
            ss << "  " << schemeList[i].toString() << endl;
        }
        ss << "Facts(" << factList.size() << "):" << endl;
        for (int i = 0; i < factList.size(); i++)
        {
            ss << "  " << factList[i].toString()<< "." << endl;
        }
        ss << "Rules(" << ruleList.size() << "):" << endl;
        for (int i = 0; i < ruleList.size(); i++)
        {
            ss << "  " << ruleList[i].toString() << "." << endl;
        }
        ss << "Queries(" << queryList.size() << "):" << endl;
        for (int i = 0; i < queryList.size(); i++)
        {
            ss << "  " << queryList[i].toString() << "?" << endl;
        }
        ss << "Domain(" << domainList.size() << "):" << endl;
        for(string domain : domainList)
        {
            ss << "  " << domain << endl;
        }
    }
    
    return ss.str();
    
}

