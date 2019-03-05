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


const string separators[] = {"'", "(", ")", "{", "}", "[", "]", ",", ".", ":", ";", "!"}; //don't need anymore
const string operators[] = {"*", "+", "-", "=", "/", ">", "<", "%"}; //don't need anymore

void removeComments(string *l); //completed
void printCharVector (vector <char> * v); //completed
bool isKeyword(string s);
bool isIdentifier(string s);
bool isSeparator (char c); //completed
bool isOperator (char c); //completed
void testIdentifyingChars (string * s); //completed
bool finalStateReached ();
void printSVector (vector <string> * v);
void addToLexeme ();
void handleFile (int ac, const char * av[]);
void runTests ();
void initKeywordsVec ();
void fillLexemeVector();
void removeSpaces ();
void getOutput ();
void seeASCIIofChars ();


int nextState (char theInput, int index);


vector<string> keywords;
vector <char> alphaVector;
vector <char> sepVector;
vector <char> opVector;
vector <char> digitVector;
vector <string> keywordVector;






string currentLexeme;
vector <string> lexemeVector;
int currentState = 1;
int currentIndex = 0;
string tokenString;



//==========================================================================================
//==========================================================================================

int main( int argc, const char * argv[] ) {
    
    char currentChar;
    
    handleFile (argc, argv);
    removeComments(&tokenString);
//    seeASCIIofChars ();

    runTests();
    fillLexemeVector();
//    removeSpaces();

    cout << "------------------------" << endl;
    printSVector(&lexemeVector);
//
    getOutput();

    return 0;
} //end main

//==========================================================================================
//==========================================================================================

void fillLexemeVector () {
    for (int i = 0; i < tokenString.size()+1 ; i++) {
    cout << "\nCurrent State: " << currentState;
    cout << "\nCurrent Lexeme: " << currentLexeme;
    cout << "\nInput" << tokenString[currentIndex];
    
    cout << "\nLexeme Vector Size: " << lexemeVector.size();
    cout << "\nLexeme Vector: ";
    printSVector(&lexemeVector);
    
    //check state
    currentState = nextState(tokenString[currentIndex], currentIndex);
    if (currentState == 3) {
        cout << "\nAccepting";
        // make sure it is not a space
        if (int(tokenString[currentIndex]) != 13 && int(tokenString[currentIndex]) != 10 && int(tokenString[currentIndex]) != 32 && int(tokenString[currentIndex]) != 9)
        currentLexeme.push_back(tokenString[currentIndex]);
        lexemeVector.push_back(currentLexeme);
    
        currentState = 1;
        currentIndex++;
        cout << "\nclearing lexeme";
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
                cout << "OPERATOR\t\t = \t\t" << *it << endl;
            }
            // if it is a separator
            if (isSeparator(it->at(0))) {
                cout << "SEPARATOR\t\t = \t\t" << *it << endl;
            }
        }
        else {
            if (isKeyword(*it)) {
                cout << "KEYWORD\t\t\t = \t\t" << *it << endl;
            }
            else {
                cout << "IDENTIFIER\t\t = \t\t" << *it << endl;
            }
            
            
        }
    }
    
}




void seeASCIIofChars () {
    
    for (int i = 0; i < tokenString.length(); i++) {
        cout << "\nCharacter: " << tokenString[i] << " ASCII: " << int(tokenString[i]);
    }
    
}






void initKeywordsVec () {
    
    //initializeKeywordsVector
    keywords.push_back("int");
    keywords.push_back("float");
    keywords.push_back("bool");
    keywords.push_back("if");
    keywords.push_back("else");
    keywords.push_back("then");
    keywords.push_back("do");
    keywords.push_back("while");
    keywords.push_back("whileend");
    keywords.push_back("doend");
    keywords.push_back("for");
    keywords.push_back("and");
    keywords.push_back("or");
    keywords.push_back("function");
    
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
        
        
    } else {
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
//  for (vector<string>::iterator it = keywords.begin(); it != keywords.end(); it++) {
//    if (s == *it)
//      return true;
//  } return false;
    if (s == "int" || s == "float" || s == "bool" || s == "if" || s == "else" || s == "then" || s == "do" || s == "while" || s == "whileend" || s == "do" || s == "doend" || s == "for" || s == "and" || s == "or" || s == "function") {
        return true;
    } else return false;
}

bool isIdentifier(string s) {
  return false;
}

bool isSeparator (char c) {
    if (c == '\'' || c == '(' || c == ')' || c == '{' || c == '}' || c == '[' || c == ']' || c == ',' ||
        c == '.' || c == ':' || c == ';' || c == '!' || isspace(c)) {
        return true;
    } else {
        return false;
    }
}

bool isOperator (char c) {

    if (c == '*' || c == '+' || c == '-' || c == '=' || c == '/' || c == '>' || c == '<' || c == '%') {
        return true;
    } else {
        return false;
    }

}





void printCharVector (vector <char> * v) {

    for (newCharIterator it = v->begin(); it != v->end(); ++it) {

        cout << " " << *it << " ";

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

// ==========================================================================
// Not using right now

//void addToLexeme () {
//
//        nextState(tokenString);
//        cout << "adding " << tokenString.at(currentIndex) << " to our lexeme" << endl;
//        currentLexeme.push_back(tokenString.at(currentIndex));
//
//}

//bool finalStateReached () {
//    if (currentState == 3)
//        return false;
//    else return true;
//}

    



void runTests() {
    // =================================================================================
    // Testing Section
    
    //    for (int i = 0; i < tokenString.length(); i++) {
    //
    //    cout << "Current state = " << currentState << endl;
    //    currentState = nextState(tokenString);
    //
    //    }
    //    for (int m = 0; m < 4; m++) {
    //        for (int t = 0; t < 10; t++) {
    //            addToLexeme();
    //            cout << "This is our lexeme" << currentLexeme << endl;
    //            if (finalStateReached()) {
    //                cout << "our first lexeme has size: " << currentLexeme.length() << " is done: " << currentLexeme << endl;
    //                lexemeVector.push_back(currentLexeme);
    //                currentLexeme = "";
    //                break;
    //            }
    //        }
    //    }
    
    //    cout << "\nAlpha chars:"; printCharVector(&alphaVector); cout << endl;
    //    cout << "Operator chars:"; printCharVector(&opVector); cout << endl;
    //    cout << "Seperator chars:"; printCharVector(&sepVector); cout << endl;
    //    cout << "Digit chars:"; printCharVector(&digitVector); cout << endl;
    
    //Test Print to see if it completed
    
    //    cout << "This is our vector: ";
    //    printSVector(&lexemeVector);
    //    cout << "\nEnd Program\n";
    
    
    // string is at state 1 and index 0
    // gets next character and its state changes
    // is it at state 6?
    // if yes, go to state 1
    // if no, then store that character in the currentLexeme string
    // increase index by 1
    // is it at state 3?
    // if yes, add currentLexeme to lexemeVector
    // if no, repeat
    
    //    while (currentIndex < tokenString.length()) {
    //        cout << "Index: " << currentIndex << ", ";
    //        cout << "Token: " << tokenString.at(currentIndex) << endl;
    //        currentIndex++;
    //
    //    }
    
    
    
}

void testIdentifyingChars (string * s) {
    for (int i = 0; i < s->length(); i++) {
        
        if (isalpha(s->at(i)) || s->at(i) == ' ') {
            alphaVector.push_back(s->at(i));
        } if (isdigit(s->at(i))) {
            digitVector.push_back(s->at(i));
        } if (isOperator(s->at(i))) {
            opVector.push_back(s->at(i));
        } if (isSeparator(s->at(i))) {
            sepVector.push_back(s->at(i));
        }
        
    }
}

