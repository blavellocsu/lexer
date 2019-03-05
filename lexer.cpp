//USAGE
//compile with g++ lexer.cpp -olexer
//run with ./lexer SampleInputFile1.txt

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <list>
#include <ctype.h>
#define newCharIterator vector<char>::iterator // just to make the code a little easier to read
#define newStringIterator vector<string>::iterator


using namespace std;

//==========================================================================================
//Global data

// finite state machine
int fsm[6][6] = {
    2,4,6,3,3,3,
    2,2,2,3,3,3,
    1,1,1,1,1,1,
    6,4,6,3,3,5,
    6,5,6,3,3,6,
    1,1,1,1,1,1
};




void removeComments(string *l);
bool isKeyword(string s);
bool isSeparator (char c); 
bool isOperator (char c);
void printSVector (vector <string> * v);
void handleFile (int ac, const char * av[]);
void fillLexemeVector();
void getOutput ();
int nextState (char theInput, int index);


string currentLexeme;
vector <string> lexemeVector;
int currentState = 1;
int currentIndex = 0;
string tokenString;



//==========================================================================================
//==========================================================================================

int main( int argc, const char * argv[] ) {
    
    
    handleFile (argc, argv);
    removeComments(&tokenString);
    fillLexemeVector();
    cout << "------------------------" << endl;
    getOutput();
    cout << endl;
    
    return 0;
} //end main

//==========================================================================================
//==========================================================================================

void fillLexemeVector () {
    for (int i = 0; i < tokenString.size()+1 ; i++) {
        
        
        //check state
        currentState = nextState(tokenString[currentIndex], currentIndex);
        if (currentState == 3) {
            // make sure it is not a space
            if (int(tokenString[currentIndex]) != 13 && int(tokenString[currentIndex]) != 10 && int(tokenString[currentIndex]) != 32 && int(tokenString[currentIndex]) != 9) {
                
                if (isSeparator(tokenString[currentIndex]) || isOperator(tokenString[currentIndex])) {
                    // if its a separator or operator, add currentLexeme to lexemeVector
                    lexemeVector.push_back(currentLexeme);
                    currentLexeme = "";
                    
                }
                currentLexeme.push_back(tokenString[currentIndex]);
            }
            lexemeVector.push_back(currentLexeme);
            
            currentState = 1;
            currentIndex++;
            currentLexeme = "";
            
        }
        else {
            if (int(tokenString[currentIndex]) != 13 && int(tokenString[currentIndex]) != 10 && int(tokenString[currentIndex]) != 32 && int(tokenString[currentIndex]) != 9)
                currentLexeme.push_back(tokenString[currentIndex]);
            currentIndex++;
        }
    }
    
    
}

void getOutput () {
    
    for (vector<string>::iterator it = lexemeVector.begin(); it != lexemeVector.end(); it++) {
        
        // if the size is == 1, it must be an operator or a seperator
        if (it->size() == 1) {
            // if it is an operator
            if (isOperator(it->at(0))) {
                //print it as OPERATOR\T\T=\t\t\t\ it
                cout << endl;
                cout << "OPERATOR\t\t = \t\t" << *it;
            }
            // if it is a separator
            if (isSeparator(it->at(0))) {
                cout << endl;
                cout << "SEPARATOR\t\t = \t\t" << *it;
            }
        }
        else {
            if (isKeyword(*it)) {
                cout << endl;
                cout << "KEYWORD\t\t\t = \t\t" << *it;
            }
            else {
                if (it->size() != 0) {
                    cout << endl;
                    cout << "IDENTIFIER\t\t = \t\t" << *it;
                }
            }
        }
    }
    
}





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
        
        
    }
    else {
        cout << "File Error: Could not open file.\n" << usage << endl;
        exit(1);
    }
}



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


bool isKeyword(string s) {
    if (s == "int" || s == "float" || s == "bool" || s == "if" || s == "else" || s == "then" || s == "do" || s == "while" || s == "whileend" || s == "do" || s == "doend" || s == "for" || s == "and" || s == "or" || s == "function") {
        return true;
    }
    else return false;
}


bool isSeparator (char c) {
    if (c == '\'' || c == '(' || c == ')' || c == '{' || c == '}' || c == '[' || c == ']' || c == ',' ||
        c == '.' || c == ':' || c == ';' || c == '!' || isspace(c)) {
        return true;
    }
    else {
        return false;
    }
}

bool isOperator (char c) {
    
    if (c == '*' || c == '+' || c == '-' || c == '=' || c == '/' || c == '>' || c == '<' || c == '%') {
        return true;
    }
    else {
        return false;
    }
    
}



void printSVector (vector <string> * v) {
    
    for (newStringIterator it = v->begin(); it != v->end(); ++it) {
        
        cout << *it;
        
    }
    
}


int nextState (char theInput, int index) {
    //returns the state of the machine at the current index
    //returns -1 if the char is not allowed
    
    
    if (isalpha(theInput)) {
        return fsm[currentState-1][0];
    }
    if (isdigit(theInput)) {
        return fsm[currentState-1][1];
    }
    if (theInput   == '$') {
        return fsm[currentState-1][2];
    }
    if (isSeparator(theInput  )) {
        return fsm[currentState-1][3];
    }
    if (isOperator(theInput  )) {
        return fsm [currentState-1][4];
    }
    if (theInput   == '.') {
        // it is not going to reach this because '.' is part of the separator group. fix this later
        return fsm [currentState-1][5];
    }
    
    else return -1;
}








