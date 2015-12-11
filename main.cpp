/*
 * main.cpp
 *
 *  Created on: Sep 23, 2015
 *      Author: jakehasler
 */


#include <iostream>
#include <string>
#include <fstream>
#include <istream>
#include <ostream>
#include <vector>
#include <cstdlib>
#include <cctype>


#include "Token.h"
#include "Scanner.h"
#include "Parser.h"
#include "Relation.h"
#include "Database.h"
#include "DBProcessor.h"
#include "Graph.h"


using namespace std;

// Functions Go Here

//ofstream outfile("output.txt");

int main(int argc, char* argv[]) {

	Scanner myScanner;
    Lexer myLexer;


	ifstream input;
    char* fileName = argv[1];

    myLexer.checkFile(input, fileName, myScanner);

    // Stores vector of tokens from Scan Result
    vector<Token> tokenVector = myScanner.tokenVector;

    Parser myParser(tokenVector);

    myParser.parse();

    // Passing in Parser Object with all vectors of data -
    DBProcessor theProcessor(myParser);

    // Using Process function to keep main clean and consolidate code in one place.
    theProcessor.process();


	return 0;

}
