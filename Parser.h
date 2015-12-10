//
//  Parser.hpp
//  L1-Lexical-Analyzer
//
//  Created by Jake Hasler on 10/22/15.
//  Copyright © 2015 Jake Hasler. All rights reserved.
//

#ifndef Parser_h
#define Parser_h

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

#include "Token.h"
#include "Predicate.h"
#include "Parameter.h"
#include "Rule.h"
#include "Expression.h"

using namespace std;


class Parser {
    
public:
    Parser(vector<Token> tokenVector);
    Parser();
    
    virtual ~Parser();
    
    void parse();
    bool match(string tokenType);
    void advanceIndex();
    
    // 4 Main Functions for Parsing
    void checkSchemeList();
    void checkFactList();
    void checkRuleList();
    void checkQueryList();
    
    void checkPredicateList();
    
    void checkParamList();
    
    void scheme();
    void idList(Predicate &newPred);
    void fact();
    void stringList(Predicate &factPred);
    void rule();
    void predicate();
    void rulePredicate(Rule &newRule);
    void queryPredicate(Predicate &queryPred);
    void predicateList();
    void rulePredicateList(Rule &newRule);
    bool parameter(vector<Parameter> &paramList);
    void paramList(vector<Parameter> &paramList);
    void headPredicate(Rule &newRule);
    bool expression(vector<Parameter> &paramList);
    void query();
    
    bool isOperator();
    
    void addScheme(Predicate pred);
    void addFact(Predicate pred);
    void addRule(Rule newRule);
    void addQuery(Predicate pred);
    void addDomain(string domain);
    
    set<string> getDomain();
    vector<Predicate> getSchemes();
    vector<Predicate> getQueries();
    vector<Predicate> getFacts();
    vector<Rule> getRules();
    
    string toString();
    
    // Temporary
    //Predicate newPredicate;
    
    bool success;
    
    // Main Index Counter
    int theIndex;
    
    // Scheme Counter
    int schemeCount;
    
    
    
private:
    vector<Token> tokenVector;
    
    vector<Predicate> schemeList;
    vector<Predicate> factList;
    vector<Rule> ruleList;
    vector<Predicate> queryList;
    
    set<string> domainList;
};

#endif /* Parser_h */
