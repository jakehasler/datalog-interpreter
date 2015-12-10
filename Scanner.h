/*
 * Scanner.h
 *
 *  Created on: Sep 23, 2015
 *      Author: jakehasler
 */

#ifndef SCANNER_H_
#define SCANNER_H_

#include <iostream>
#include <string>
#include <fstream>
#include <ostream>
#include <vector>
#include <cstdlib>
#include <cctype>

using namespace std;

#include "Token.h"


class Scanner {
    friend class Lexer;
public:
	Scanner();
	virtual ~Scanner();

	void toString();

	void addToVector(Token newToken);
    
    int getListSize();

	int typeIndex(string type);
    
    bool checkMachines(int num, string idString);
    
    bool checkFacts(int num, string idString);
    
    bool checkSchemes(int num, string idString);
    
    bool checkRules(int num, string idString);
    
    bool checkQueries(int num, string idString);
    
    void checkID(int &num, char character, istream &input, stringstream &ss, Scanner &myScanner);
    
    void checkString(int startingLine, int &num, char character, istream &input, stringstream &ss);
    
    void finishString(int startingLine, string token, char character);

    void checkComment(int &num, char character, istream &input, stringstream &ss);
    
    void checkMultiComment(int &num, char character, istream &input, stringstream &ss);
    
    void compareAndCreate(int num, char character);
    
	void finish();
    
    int commentState;
    
    int idState;
    
    int multiCommentState;
    
    int stringState;
    
    int num;
    
    vector<Token> tokenVector;

private:
	vector<string> inputTypes;
//	vector<Token> tokenVector;
	int totalTokens;
    int firstUnread;
    
    int factsState;
    int schemesState;
    int rulesState;
    int queriesState;

};

class Lexer {
    friend class Scanner;
public:
    Lexer();
    virtual ~Lexer();
    
    void checkFile(ifstream &input, char* fileName, Scanner &myScanner);
    
    bool validate(char character, Scanner &myScanner, int num, int &currState);
    
    bool singleChecker(char currentChar);
    
    bool undefiner(char character, Scanner &myScanner);
    
private:
    
};


#endif /* SCANNER_H_ */



