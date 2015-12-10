/*
 * Scanner.cpp
 *
 *  Created on: Sep 23, 2015
 *      Author: jakehasler
 */

#include "Scanner.h"


Scanner::Scanner() {

	//this->inputTypes = inputTypes;

	inputTypes.push_back("COMMA");
	inputTypes.push_back("PERIOD");
	inputTypes.push_back("Q_MARK");
	inputTypes.push_back("LEFT_PAREN");
	inputTypes.push_back("RIGHT_PAREN");
	inputTypes.push_back("COLON");
	inputTypes.push_back("COLON_DASH");
	inputTypes.push_back("MULTIPLY");
	inputTypes.push_back("ADD");
	inputTypes.push_back("SCHEMES");
	inputTypes.push_back("FACTS");
	inputTypes.push_back("RULES");
	inputTypes.push_back("QUERIES");
	inputTypes.push_back("ID");
	inputTypes.push_back("STRING");
	inputTypes.push_back("COMMENT");
	inputTypes.push_back("WHITESPACE");
	inputTypes.push_back("UNDEFINED");
	inputTypes.push_back("EOF");
    
    // for simple index comparison for precedence.
    
    firstUnread = 0;

	totalTokens = 0;
    
    commentState = 0;
    
    multiCommentState = 0;
    
    stringState = 0;
    
    idState = 0;
    
    num = 1;


}

Scanner::~Scanner() {
}

void Scanner::toString()
{
	for(int i = 0; i < tokenVector.size(); i++)
	{
		cout << tokenVector[i].getTokenized() << endl;
	}
    cout << "Total Tokens = " << tokenVector.size() << endl;
}

int Scanner::getListSize()
{
    return totalTokens;
}

// Returns index of parameter passed in.
int Scanner::typeIndex(string type)
{
	for(int i = 0; i < inputTypes.size(); i++)
	{
		if(type == inputTypes[i])
		{
			return i;
		}

	}
	cout << "This type was misspelled, or isn't a valid type." << endl;
    
    return -1;
}

void Scanner::addToVector(Token newToken)
{
    newToken.tokenize();
	tokenVector.push_back(newToken);

}


bool Scanner::checkMachines(int num, string idString)
{
    bool id = true;
    switch(idString[0])
    {
        case 'F':
            if(checkFacts(num, idString))
            {
                id = false;
            } break;
        case 'S':
            if(checkSchemes(num, idString))
            {
                id = false;
            }break;
        case 'Q':
            if(checkQueries(num, idString))
            {
                id = false;
            }break;
        case 'R':
            if(checkRules(num, idString))
            {
                id = false;
            } break;
        default : id = true; break;
    }

    return id;

}


bool Scanner::checkFacts(int num, string idString)
{
    factsState = 0;
    
    if(idString == "Facts")
    {
        Token factsToken(this->num, "Facts", "FACTS");
        addToVector(factsToken);
        idState = 0;
        return true;
    }else return false;
    

}
    



bool Scanner::checkSchemes(int num, string idString)
{

    schemesState = 0;
    
    if(idString == "Schemes")
    {
        Token schemesToken(this->num, "Schemes", "SCHEMES");
        idState = 0;
        addToVector(schemesToken);
        return true;
    }
    else return false;
    
    
}


bool Scanner::checkRules(int num, string idString)
{
    rulesState = 0;

    if(idString == "Rules")
    {
        Token rulesToken(this->num, "Rules", "RULES");
        idState = 0;
        addToVector(rulesToken);
        return true;
    }
    else return false;

        
}


bool Scanner::checkQueries(int num, string idString)
{
    queriesState = 0;
    
    if(idString == "Queries") // If all states are fulfilled.
    {

        Token queriesToken(this->num, "Queries", "QUERIES");
        this->idState = 0;
        addToVector(queriesToken);
        return true;
    }
    else return false;
    
}


void Scanner::checkID(int &num, char character, istream &input, stringstream &ss, Scanner &myScanner)
{

    Lexer myLexer;
    bool validId = true;
    bool toBreak = false;
    string totalID;
    idState = 1;
    
    int line = this->num;
    
    while(toBreak == false && idState < 2)
    {
        char currentChar = input.get();
        char peeked = (char)(input.peek());
        //cout << "Current Char =" << currentChar << endl;
        if(isalpha(currentChar))
        {
            ss << currentChar;       
        }
        else if(isalnum(currentChar) && idState > 0)
        {
            ss << currentChar;
            //idState++;
        }
        else if(!isalnum(currentChar))
        {
            //cout << currentChar << endl;
            input.putback(currentChar);
            idState++;
            toBreak = true;
        }
        if(peeked == '\n' || peeked == '\r')
        {
            idState++;
        }
    }

    totalID = ss.str();
    
    if(checkMachines(line, totalID) == true && validId == true)
    {
        Token idToken(line, totalID, "ID");
        addToVector(idToken);
        idState = 0;
    }
    else if(validId == false)
    {
        // create undefined token
        Token undefToken(line, totalID, "UNDEFINED");
    }
 
}

void Scanner::finishString(int startingLine, string token, char character)
{
	
	
    if(stringState == 1)
    {
        Token undefToken(startingLine, token, "UNDEFINED");
        addToVector(undefToken);
        stringState = 0;
    }
        
    
    if(stringState == 2)
    {
		if(character == '\n' || character == '\r') this->num++;
        Token stringToken(startingLine, token, "STRING");
        addToVector(stringToken);
        stringState = 0;
    }
}


void Scanner::checkString(int startingLine, int &num, char character, istream &input, stringstream &ss)
{
    // if char == '\'' && peeked == '\'' -> continue reading
    // don't count white spaces between two apostrophes
    // if reaches end of file before final apostrophe, then send to undefined function
    int totalChars = 0;
    Lexer myLexer;
    while(input.get(character) && stringState < 2)
    {
        char peeked = (char)(input.peek());
        totalChars++;

        if(character == '\n' || character == '\r')
        {
            ss << character;
            this->num++;
        }
        else if(isalnum(character))
        {
            ss << character;
        }
        else if(character == '\'' && peeked == '\'')
        {
            ss << character;
            ss << peeked;
            input.get();
        }
        else if(character == '\'' && peeked != '\'')
        {
            
            ss << character;
            stringState++;
        }
        else
        {
            ss << character;
        }
        
    }

    if(character != '\n' && character != '\r')
    {
        input.putback(character);
    }
    string token = ss.str();

    finishString(startingLine, token, character);
}


void Scanner::checkComment(int &num, char character, istream &input, stringstream &ss)
{

    int startingLine = this->num;
    
    if(character == '\n' || character == '\r') this->num++;

    while (input.get(character) && commentState < 2)
    {
        //char peeked = (char)(input.peek());
        if(character == '\n' || character == '\r')
        {
            this->num++;
            input.putback(character);
            commentState++;
        }
        else
        {
           ss << character;
        }
        
    }
    
    if(commentState == 2 || ss.str().length() == 1)
    {
        Token commentToken(startingLine, ss.str(), "COMMENT");
        //addToVector(commentToken);
        commentState = 0;  
    }
    
}

void Scanner::checkMultiComment(int &num, char character, istream &input, stringstream &ss)
{
    //cout << "Entered checkMultiComments" << endl;
    int startingLine = num;
    
    while(input.get(character) && multiCommentState != 3)
    {
       
        char peeked = (char)(input.peek());
        if(character != '#' && character != '|')
        {
            if(character == '\n' || character == '\r')
            {
                this->num++;
            }
            
            ss << character;

        }
        else if(character == '|' && peeked == '#')
        {
            ss << character;
            ss << peeked;
            multiCommentState = 3;
        }
        else ss << character;
        
    }
    
    if(multiCommentState < 3)
    {
        Token undefCommentToken(startingLine, ss.str(), "UNDEFINED");
        //addToVector(undefCommentToken);
        multiCommentState = 0;
    }
    
    if(multiCommentState == 3)
    {
        Token multiCommentToken(startingLine, ss.str(), "COMMENT");
        //addToVector(multiCommentToken);
        multiCommentState = 0;
    }
}

void Scanner::finish()
{
	cout << "Total Tokens = " << totalTokens << endl;
}

