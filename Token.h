/*
 * Token.h
 *
 *  Created on: Sep 23, 2015
 *      Author: jakehasler
 */

#ifndef TOKEN_H_
#define TOKEN_H_


#include <iostream>
#include <string>
#include <fstream>
#include <ostream>
#include <vector>
#include <cstdlib>
#include <cctype>
#include <sstream>

using namespace std;

class Token {

public:
	Token();
	Token(int num, string value, string type);
	virtual ~Token();

	// Organizes data output into one string
	// (Token_Type,"Value",Line_Number)
	string tokenize();

	string getTokenized();

	// Creates a string of current vector.
	void toString();

    string getType();
    
    int getNum();
    
    string getValue();

private:
	// Where the tokens will be pushed.
	int num;
	string value;
	string type;
	string tokenizedString;


};

#endif /* TOKEN_H_ */
