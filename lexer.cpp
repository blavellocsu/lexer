//USAGE
//compile with g++ lexer.cpp -olexer
//run with ./lexer SampleInputFile1.txt

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <list>
#include <ctype.h>
#define newIterator vector<char>::iterator // just to make the code a little easier to read
#define newSIterator vector<string>::iterator


using namespace std;

//Global data

// finite state machine
int fsm[6][6] = {
    2,4,6,3,3,3,
    2,2,2,3,3,3,
    1,1,1,1,1,1,
    6,4,6,3,3,5,
    6,5,6,3,3,6,
};


//const string keywordArray[] = { "int", "float", "bool", "if", "else", "then", "do", "while",
  //  "whileend", "do", "doend", "for", "and", "or", "function"};
vector<string> keywords;

const string separators[] = {"'", "(", ")", "{", "}", "[", "]", ",", ".", ":", ";", "!"};
const string operators[] = {"*", "+", "-", "=", "/", ">", "<", "%"};

void removeComments(string *l); //completed
void testPrint(string *l);  //completed
void printVector (vector <char> * v); //completed

//string grabLexeme (list<char> *l);
bool isKeyword(string s);
bool isIdentifier(string s);
bool isSeparator (char c); //completed
bool isOperator (char c); //completed
void testIdentifyingChars (string * s); //completed
bool finalStateReached ();

vector <char> alphaVector;
vector <char> sepVector;
vector <char> opVector;
vector <char> digitVector;
vector <string> keywordVector;
vector <string> lexemeVector;

void printSVector (vector <string> * v);

void addToLexeme ();


int currentState = 1;

string currentLexeme;
string charString;



//==========================================================================================
// this function is going to take the whole string of the whole text and it is also going to take the index we want to start (or pick up from)
int nextState (string wholeString, int index);

// this is the current idex we are working with.
// every time we use the getState function, we are going to walk through the string until we see a white space. then we are going to stop and change this current index to 
int currentIndex = 0;

//==========================================================================================

int main( int argc, const char * argv[] ) {
    //check for command line arguments
    string usage = "Usage: \"./<name> <filename.txt>\"";
    //check for correct usage: "./lexer sample.txt"
    if (argc != 2) {
        cout << "Invalid Arguments.\n" << usage << endl;
        exit(1);
    }

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

    //File handling
    //read filename in from command line argument
    //see top of file for correct command line usage
    string filename = argv[1];
    char currentChar;
    
    //Create filesream
    ifstream file(filename);
    //check to see if file opened correctly
    if (file.is_open()) {
        cout << "File opened successfully." << endl;

        // read content of txt file (including white spaces) into a string
        string contents((istreambuf_iterator<char>(file)), istreambuf_iterator<char>());


        for (int i = 0; i < contents.length(); i++) {
            charString.push_back(contents[i]);
          }


    } else {
        cout << "File Error: Could not open file.\n" << usage << endl;
        exit(1);
    }

    //remove comments from code
    removeComments(&charString);

    //spacing
    //cout << "\n\n";

    //output to see if comments are gone
    //testPrint(&charString);

    testIdentifyingChars(&charString);
    
// =================================================================================
    // Testing Section
    
//    for (int i = 0; i < charString.length(); i++) {
//
//    cout << "Current state = " << currentState << endl;
//    currentState = nextState(charString, currentIndex);
//
//    }
    for (int m = 0; m < 4; m++) {
    for (int t = 0; t < 10; t++) {
    addToLexeme();
    cout << "This is our lexeme" << currentLexeme << endl;
        if (finalStateReached()) {
            cout << "our first lexeme has size: " << currentLexeme.length() << " is done: " << currentLexeme << endl;
            lexemeVector.push_back(currentLexeme);
            currentLexeme = "";
            break;
        }
    }
    }
    
//    cout << "\nAlpha chars:"; printVector(&alphaVector); cout << endl;
//    cout << "Operator chars:"; printVector(&opVector); cout << endl;
//    cout << "Seperator chars:"; printVector(&sepVector); cout << endl;
//    cout << "Digit chars:"; printVector(&digitVector); cout << endl;

    //Test Print to see if it completed
    
    cout << "This is our vector: ";
    printSVector(&lexemeVector);
    cout << "\nEnd Program\n";

    return 0;
} //end main

//==========================================================================================

//This function accepts an address to a std::list<char>
//It iterates through the list and deletes the commented phrases
void removeComments(string *l) {
    //Remove Comments from charString
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

//string grabLexeme (list<char> *l) {
//    // if it is an alphebetical character
//
//}

bool isKeyword(string s) {
  for (vector<string>::iterator it = keywords.begin(); it != keywords.end(); it++) {
    if (s == *it)
      return true;
  }
  return false;
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
void printVector (vector <char> * v) {

    for (newIterator it = v->begin(); it != v->end(); ++it) {

        cout << " " << *it << " ";

    }

}

void printSVector (vector <string> * v) {
    
    for (newSIterator it = v->begin(); it != v->end(); ++it) {
        
        cout << *it;
        
    }
    
}



//Accepts an address to a std::list<chars>
//prints out list to console
void testPrint (string *l) {
    //Print Function

    cout << "This is the char list: ";
    for (int i = 0; i < l->length(); i++) {
        std::cout /*<< ' '*/ << l->at(i);
    }
}



int nextState (string wholeString, int index) {
    char thisChar = wholeString[index];
    cout << "This char is: " << thisChar << endl;

    if (isalpha(thisChar)) {
        currentIndex++;
            return fsm[currentState-1][0];
        }
     if (isdigit(thisChar)) {
        currentIndex++;
            return fsm[currentState-1][1];
        }
     if (thisChar == '$') {
        currentIndex++;
            return fsm[currentState-1][2];
        }
     if (isSeparator(thisChar)) {
        currentIndex++;
            return fsm[currentState-1][3];
        }
     if (isOperator(thisChar)) {
        currentIndex++;
            return fsm [currentState-1][4];
        }
     if (thisChar == '.') {
        currentIndex++;
            // it is not going to reach this because '.' is part of the separator group. fix this later
            return fsm [currentState-1][5];
        }
    
    else return -1;
    }

void addToLexeme () {
    
        nextState(charString, currentIndex);
        cout << "adding " << charString.at(currentIndex) << " to our lexeme" << endl;
        currentLexeme.push_back(charString.at(currentIndex));
    
        
        
}

bool finalStateReached () {
    if (currentState == 3)
        return false;
    else return true;
}

    




