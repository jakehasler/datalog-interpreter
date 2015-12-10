//
//  Lexer.cpp
//  L1-Lexical-Analyzer
//
//  Created by Jake Hasler on 10/12/15.
//  Copyright © 2015 Jake Hasler. All rights reserved.
//

#include "Scanner.h"


Lexer::Lexer()
{
    
}

Lexer::~Lexer()
{
    
}


void Lexer::checkFile(ifstream &input, char* fileName, Scanner &myScanner)
{
    
    input.open(fileName);
    
    if(input.is_open())
    {
        char c;
        while (input.get(c) && !input.eof())
        {
            char peeked = (char)(input.peek());
            
            switch(c)
			{
				case '#': { if(peeked == '|')
							{
								stringstream ss;
								ss << c;
								myScanner.multiCommentState++;
								myScanner.checkMultiComment(myScanner.num, c, input, ss);
							}
							else
							{
								stringstream ss;
								myScanner.commentState++;
								ss << c;
								myScanner.checkComment(myScanner.num, c, input, ss);
							}
					break; }
				case ':': { if(peeked == '-')
							{
								Token colonDashToken(myScanner.num, ":-", "COLON_DASH");
								myScanner.addToVector(colonDashToken);
								input.get();
							}
							else
							{
								Token colonToken(myScanner.num, ":", "COLON");
								myScanner.addToVector(colonToken);
							}
					break;}
				case '\'': { 
					myScanner.stringState++;
					stringstream ss;
					int startingLine = myScanner.num;
					
					ss << c;
					myScanner.checkString(startingLine, myScanner.num, c, input, ss);
					myScanner.stringState = 0;
					break; }
				case '\n': { myScanner.num++;
					break; }
				case '\r': { myScanner.num++;
					break; }
				default: { 
					if(isalpha(c))
					{
						stringstream ss;
						ss << c;
						myScanner.idState++;
						myScanner.checkID(myScanner.num, c, input, ss, myScanner);
					}
					else
					{
						int currState;
						validate(c, myScanner, myScanner.num, currState);
					}
					break; }
					
			}
            

        }
        
    }
    else
    {
        cout << "The file could not be opened." << endl;
    }
        Token eofToken(myScanner.num, "", "EOF");
        myScanner.addToVector(eofToken);
        input.close();

    //myScanner.toString();
}


bool Lexer::validate(char character, Scanner &myScanner, int num, int &currState)
{
    switch(character)
    {
        case ',': { Token commaToken(myScanner.num, ",", "COMMA");
            myScanner.addToVector(commaToken); return true;
            break; }
        case '.': { Token periodToken(myScanner.num, ".", "PERIOD");
            myScanner.addToVector(periodToken); return true;
            break;}
        case '?': { Token qmarkToken(myScanner.num, "?", "Q_MARK");
            myScanner.addToVector(qmarkToken); return true;
            break; }
        case '(': { Token leftParenToken(myScanner.num, "(", "LEFT_PAREN");
            myScanner.addToVector(leftParenToken); return true;
            break; }
        case ')': { Token rightParenToken(myScanner.num, ")", "RIGHT_PAREN");
            myScanner.addToVector(rightParenToken); return true;
            break; }
        case ':': { Token colonToken(myScanner.num, ":", "COLON");
            myScanner.addToVector(colonToken); return true;
            break; }
        case '*': { Token multiplyToken(myScanner.num, "*", "MULTIPLY");
            myScanner.addToVector(multiplyToken); return true;
            break; }
        case '+': { Token addToken(myScanner.num, "+", "ADD");
            myScanner.addToVector(addToken); return true;
            break; }
        case '$': { Token dollarToken(myScanner.num, "$", "UNDEFINED");
            myScanner.addToVector(dollarToken);
            break; }
        case '&': { Token amperToken(myScanner.num, "&", "UNDEFINED");
            myScanner.addToVector(amperToken);
            break; }
        case '^': { Token carrotToken(myScanner.num, "^", "UNDEFINED");
            myScanner.addToVector(carrotToken);
            break; }
        case ' ': break;
        case '\t': break;
        default: undefiner(character, myScanner); return false; break;

    }
    return false;

}

bool Lexer::undefiner(char character, Scanner &myScanner)
{
    stringstream ss;
    ss << character;
    string charStr = ss.str();
    Token undefToken(myScanner.num, charStr, "UNDEFINED");
    myScanner.addToVector(undefToken);
    
    return true;
}

bool Lexer::singleChecker(char character)
{
    switch(character)
    {
        case ',': { return true;
            break; }
        case '.': { return true;
            break;}
        case '?': { return true;
            break; }
        case '(': { return true;
            break; }
        case ')': { return true;
            break; }
        case ':': { return true;
            break; }
        case '*': { return true;
            break; }
        case '+': { return true;
            break; }
        default: return false; break;
            
    }
    return false;
    
}
