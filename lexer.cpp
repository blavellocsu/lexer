//USAGE
//compile with g++ lexer.cpp -olexer
//run with ./lexer SampleInputFile1.txt

#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <vector>
#include <list>
#include <ctype.h>
#include <sstream>
#define newCharIterator vector<char>::iterator // just to make the code a little easier to read
#define SIZE 9999999
#define newStringIterator vector<string>::iterator
#define tokenStrPos tokenString[currentIndex]

using namespace std;


//FUNCTION PROTOTYPES
void removeComments(string *l);
bool isKeyword(string s);
bool isSeparator (char c);
bool isOperator (char c);
void printSVector (vector <string> * v);
void handleFile (int ac, const char * av[]);
void fillLexemeVector();
int lexer (char theInput, int index);
void E();
void T();
void EPrime();
void TPrime();
void F();


//GLOBAL DATA
string currentLexeme;
vector <string> lexemeVector;
vector <string> tokenVector;
int currentState = 1;
int currentIndex = 0;
string tokenString;
ofstream outputFile;

//finite state machine
int fsm[10][6] = {
    2,5,9,4,10,9,
    2,2,2,3,3,9,
    1,1,1,1,1,1,
    1,1,1,1,1,1,
    9,5,9,6,6,7,
    1,1,1,1,1,1,
    9,7,9,8,8,9,
    1,1,1,1,1,1,
    1,1,1,1,1,1,
    1,1,1,1,1,1
};

struct Output {
    string lexeme, token;
} output;

//PrintSwitch
//If true, prints to command line and outputs to file.
//If false, only outputs to file.
bool printSwitch = true;


//==========================================================================================
// MAIN FUNCTION
//==========================================================================================

int main( int argc, const char * argv[] ) {
    
    handleFile (argc, argv);
    removeComments(&tokenString);
    cout << "------------------------" << endl;
    fillLexemeVector();
    cout << "\n" << endl;
    outputFile.close();
    cout << endl;
    return 0;
} //end main


//==========================================================================================
// LEXER FUNCTIONS
//==========================================================================================

void handleFile (int ac, const char * av[]) {
    //check for command line arguments
    string usage = "Usage: \"./<name> <filename.txt>\"";
    //check for correct usage: "./lexer sample.txt"
    if (ac != 2) {
        cout << "Invalid Arguments.\n" << usage << endl;
        exit(1);
    }
    
    //File handling
    //read filename in from command line argument
    //see top of file for correct command line usage
    string filename = av[1];
    
    //Create filesream
    ifstream file(filename);
    //check to see if file opened correctly
    if (file.is_open()) {
        cout << "File opened successfully." << endl;
        
        // read content of txt file (including white spaces) into a string
        string contents((istreambuf_iterator<char>(file)), istreambuf_iterator<char>());
        
        for (int i = 0; i < contents.length(); i++) {
            tokenString.push_back(contents[i]);
        }
        
        outputFile.open("output_" + filename);
        outputFile << "\nTOKENS\t\t\t\t\t" << "LEXEMES\n" << endl;
        
    }//end if
    else {
        cout << "File Error: Could not open file.\n" << usage << endl;
        exit(1);
    }//end error test
}//end handleFile

//==========================================================================================

//It iterates through the string and deletes the commented phrases
void removeComments(string *l) {
    //Remove Comments from tokenString
    //iterate through string
    // string length is going to change so we have to make it dynamic rather than putting into for loop directly
    int len = l->length();
    for (int i = 0; i < len; i++) {
        //check if current char is equal to '!'
        if (l->at(i) == '!') {
            //mark where ! starts in the string index
            int whereCommStart = i;
            //don't want i to change because we need to go back to there after we delete
            int j = i; // this will be where we are in the comments section
            j++;
            while (l->at(j) != '!'){
                j++;
            }
            // if we got here, that means we found the next !
            int whereCommEnd = j;
            
            // now we erase the substring between our two markers
            l->erase(whereCommStart, whereCommEnd-whereCommStart+1);
            len = l->length();
        }//end if '!'
        
    }//end for
}//end function

//==========================================================================================

void fillLexemeVector () {
    
    for (int i = 0; i < SIZE ; i++) {
        switch (currentState) {
            case 1:
                currentState = lexer(tokenStrPos, currentIndex);
                if (int(tokenStrPos) != 13 && int(tokenStrPos) != 10 && int(tokenStrPos) != 32 && int(tokenStrPos) != 9 && int(tokenStrPos) != 0) {
                    currentLexeme.push_back(tokenString[currentIndex]);
                }
                currentIndex++;
                break;
                
            case 2:
                currentState = lexer(tokenStrPos, currentIndex);
                if (currentState!=3 && currentState != 9) {
                    currentLexeme.push_back(tokenString[currentIndex]);
                    currentIndex++;
                }
                break;
            
            case 3:
                if (isKeyword(currentLexeme)) {
//                    tokenVector.push_back("KEYWORD");
                    output.lexeme = currentLexeme;
                    output.token = "KEYWORD   ";
                    E();
                }
                else {
//                    tokenVector.push_back("IDENTIFIER");
                    output.lexeme = currentLexeme;
                    output.token = "IDENTIFIER";
                    E();
                }
//                lexemeVector.push_back(currentLexeme);
                currentLexeme = "";
                currentState = lexer(tokenStrPos, currentIndex);
                break;
                
            case 4:
                
                if (currentLexeme.size() > 0) {
//                    lexemeVector.push_back(currentLexeme);
//                    tokenVector.push_back("SEPARATOR");
                    output.lexeme = currentLexeme;
                    output.token = "SEPARATOR";
                    currentLexeme = "";
                    E();
                }
                currentState = lexer(tokenStrPos, currentIndex);
                break;
                
            case 5:
                currentState = lexer(tokenStrPos, currentIndex);
                if (currentState!=6 && currentState != 9) {
                    currentLexeme.push_back(tokenString[currentIndex]);
                    currentIndex++;
                }
                break;
                
            case 6:
//                lexemeVector.push_back(currentLexeme);
//                tokenVector.push_back("INTEGER");
                output.lexeme = currentLexeme;
                output.token = "INTEGER";
                currentLexeme = "";
                E();
                currentState = lexer(tokenStrPos, currentIndex);
                break;
                
            case 7:
                currentState = lexer(tokenStrPos, currentIndex);
                if (currentState!=8 && currentState != 9) {
                    currentLexeme.push_back(tokenString[currentIndex]);
                    currentIndex++;
                }
                break;
                
            case 8:
//                tokenVector.push_back("REAL NUMBER");
//                lexemeVector.push_back(currentLexeme);
                output.lexeme = currentLexeme;
                output.token = "REAL NUMBER";
                currentLexeme = "";
                E();
                currentState = lexer(tokenStrPos, currentIndex);
                break;
                
            case 9:
                currentState = lexer(tokenStrPos, currentIndex);
                currentIndex++;
                break;
                
            case 10:
//                lexemeVector.push_back(currentLexeme);
//                tokenVector.push_back("OPERATOR");
                output.lexeme = currentLexeme;
                output.token = "OPERATOR ";
                currentLexeme = "";
                E();
                currentState = lexer(tokenStrPos, currentIndex);
                break;
                
            default:
                break;
        }
    }
//    printSVector(&lexemeVector);
}

//==========================================================================================

int lexer (char theInput, int index) {
    //returns the state of the machine at the current index
    //returns -1 if the char is not allowed
    if (isalpha(theInput)) { return fsm[currentState-1][0]; }
    if (isdigit(theInput)) { return fsm[currentState-1][1]; }
    if (theInput == '$') { return fsm[currentState-1][2]; }
    if (isSeparator(theInput)) { return fsm[currentState-1][3]; }
    if (isOperator(theInput)) { return fsm [currentState-1][4]; }
    if (theInput == '.') {
        // it is not going to reach this because '.' is part of the separator group. fix this later
        return fsm [currentState-1][5];
    }
    else return -1;
} //end lexer


//==========================================================================================
// LEXER CHAR IDENTIFICATION
//==========================================================================================

bool isKeyword(string s) {
    if (s == "int" || s == "float" || s == "bool" || s == "if" || s == "else" || s == "then" || s == "do" || s == "while" || s == "whileend" || s == "do" || s == "doend" || s == "for" || s == "and" || s == "or" || s == "function") {
        return true;
    } else { return false; }
}//end isKeyword


bool isSeparator (char c) {
    //removed .
    if (c == '\'' || c == '(' || c == ')' || c == '{' || c == '}' || c == '[' || c == ']' || c == ',' ||
        c == ':' || c == ';' || c == '!' || int(c) == 13 || int(c) == 10 || int(c) == 32 || int(c) == 9) {
        return true;
    } else { return false; }
}//end isSeparator

bool isOperator (char c) {
    if (c == '*' || c == '+' || c == '-' || c == '=' || c == '/' || c == '>' || c == '<' || c == '%') {
        return true;
    } else { return false; }
}//end isOperator





//==========================================================================================
// PARSER FUNCTIONS
//
// RULES:
//        E  -> TE'
//        E' -> +TE' | -TE' | ε
//        T  -> FT'
//        T' -> +FT' | /FT' | ε
//        F -> i | (E)
//
//==========================================================================================


//  E  -> TE'
void E() {
    //Print to CMD Line
    if (printSwitch) {
        cout << "\nToken: " << output.token << "\tLexeme: " << output.lexeme << endl;
        cout << "<Expression> -> <Term> <Expression Prime>" << endl;
    }
    //write to file
    outputFile << "\nToken: " << output.token << "\t\tLexeme: " << output.lexeme << endl;
    outputFile << "\t<Expression> -> <Term> <Expression Prime>" << endl;
    
    T();
    EPrime();
}

//  E' -> +TE' | -TE' | ε
void EPrime() {
    //Print to CMD Line
    if (printSwitch) {
        //cout << "<Expression Prime> -> + <Term> <Expression Prime> | - <Term> <Expression Prime> | ε" << endl;
     }
    
    //write to file
    //outputFile << "<Expression Prime> -> + <Term> <Expression Prime> | - <Term> <Expression Prime> | ε" << endl;
    
    //logic (RULES)
    if (output.lexeme == "+") {
//        cout << "It's a +" << endl;
//        T();
//        EPrime();
    }
    
    if (output.lexeme == "-") {
//        cout << "It's a " << endl;
//        T();
//        EPrime();
    }
    
};

//  T  -> FT'
void T() {
    //Print to CMD Line
    if (printSwitch) {
        cout << "<Term> -> <Factor> <Term Prime>" << endl;
    }
    
    //write to file
    outputFile << "\t<Term> -> <Factor> <Term Prime>" << endl;

};

//  T' -> +FT' | /FT' | ε
void TPrime() {
    //Print to CMD Line
    if (printSwitch) {
        cout << "<Term Prime> -> NOT SURE <Factor> <Term Prime> | / <Factor> <Term Prime> | ε" << endl;
    }
    
    //write to file
    outputFile << "\t<Term Prime> -> NOT SURE <Factor> <Term Prime> | / <Factor> <Term Prime> | ε" << endl;

};

//  F -> i | (E)
void F() {
    //Print to CMD Line
    if (printSwitch) {
        cout << "<Factor> -> i | <Expression> | ε" << endl;
    }

    //write to file
    outputFile << "\t<Factor> -> i | <Expression> | ε" << endl;

};








//==========================================================================================
// TESTING TEMPORARY FUNCTIONS
//==========================================================================================

void printSVector (vector <string> * v) {
    for (newStringIterator it = v->begin(); it != v->end(); ++it) {
        cout << *it;
    }
}//end printSVector
