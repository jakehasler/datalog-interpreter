/*
 * Token.cpp
 *
 *  Created on: Sep 23, 2015
 *      Author: jakehasler
 */

#include "Token.h"


Token::Token()
{

}

Token::Token(int num, string value, string type) {
	this->num = num;
	this->value = value;
	this->type = type;
	tokenizedString = "";

}



Token::~Token() {
	// TODO Auto-generated destructor stub
}


string Token::tokenize()
{
	stringstream ss;

	ss << "(" << type << ",\"" << value << "\"," << num << ")";

	tokenizedString = ss.str();

	return tokenizedString;
}

void Token::toString()
{
	cout << tokenizedString << endl;
}

string Token::getTokenized()
{
	return tokenizedString;
}


string Token::getType()
{
    return type;
}

int Token::getNum()
{
    return num;
}

string Token::getValue()
{
    return value;
}
