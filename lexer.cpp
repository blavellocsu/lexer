//USAGE
//compile with g++ lexer.cpp -olexer
//run with ./lexer test.txt

#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <vector>
#include <list>
#include <ctype.h>
#include <sstream>
// just to make the code a little easier to read
#define newCharIterator vector<char>::iterator
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

bool startSymbols();
void parser();
bool S();
bool C();
bool A();
bool E();
bool T();
bool EPrime();
bool TPrime();
bool F();
bool If();
bool Else();
bool While();
void epsilon();

void printRule(string rule);
void printTL();

//GLOBAL DATA
string currentLexeme;
vector <string> lexemeVector;
vector <string> tokenVector;
int vIndex = 0;
int currentState = 1;
int currentIndex = 0;
string tokenString;
ofstream outputFile;
bool valid = true;

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

//struct Output {
//    string lexeme, token;
//    int index;
//} output;

//PrintSwitch
//If true, prints to command line and outputs to file.
//If false, only outputs to file.
bool printSwitch = true;
bool outputSwitch = true;


//==========================================================================================
// MAIN FUNCTION
//==========================================================================================

int main( int argc, const char * argv[] ) {
    handleFile (argc, argv);
    removeComments(&tokenString);
    cout << "------------------------" << endl;
    fillLexemeVector();
    printSVector(&lexemeVector);
    cout << endl;
    parser();
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
        //outputFile << "\nTOKENS\t\t\t\t\t" << "LEXEMES\n" << endl;
        
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
        //output.index = 0;
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
                    tokenVector.push_back("KEYWORD");
                }
                else {
                    tokenVector.push_back("IDENTIFIER");
                }
                lexemeVector.push_back(currentLexeme);
                currentLexeme = "";
                currentState = lexer(tokenStrPos, currentIndex);
                break;
                
            case 4:
                
                if (currentLexeme.size() > 0) {
                    lexemeVector.push_back(currentLexeme);
                    tokenVector.push_back("SEPARATOR");
                    currentLexeme = "";
                    //E();
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
                lexemeVector.push_back(currentLexeme);
                tokenVector.push_back("INTEGER");
                currentLexeme = "";
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
                tokenVector.push_back("REAL NUMBER");
                lexemeVector.push_back(currentLexeme);
                currentLexeme = "";
                currentState = lexer(tokenStrPos, currentIndex);
                break;
                
            case 9:
                currentState = lexer(tokenStrPos, currentIndex);
                currentIndex++;
                break;
                
            case 10:
                lexemeVector.push_back(currentLexeme);
                tokenVector.push_back("OPERATOR  ");
                currentLexeme = "";
                currentState = lexer(tokenStrPos, currentIndex);
                break;
                
            default:
                break;
        }
    }
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

//========================================================================================
// PARSER FUNCTIONS
//
// RULES:
//        S  -> If | Else | While | A
//        A  -> i = E
//        E  -> T E'
//        E' -> +TE' | -TE' | ε
//        T  -> FT'
//        T' -> *FT' | /FT' | ε
//        F -> i | (E)
//
//========================================================================================

void parser() {
    //checking to see if code starts with %%
    int start = 0;
    if (startSymbols()) {
        start += 2;
    }
    //loop through vector
    for(vIndex = 0 + start; vIndex < lexemeVector.size()-1; vIndex++) {
        cout << "LOOP" << endl;
        printTL();
        if( !S() ) {
            cout << "CODE INVALID\n";
            valid = false;
            break;
        }
    }

    if (valid) cout << "\nCODE VALID\n";
};

//   S  -> If | Else | While | A
bool S() {
    printTL();
    printRule("<Statement> -> <If> | <Else> | <While> | <A>");

    if (tokenVector[vIndex] == "IDENTIFIER"){
        A();
        return true;
    }
    else if (lexemeVector[vIndex] == "if") {
        If();
        return true;
    }
    else if (lexemeVector[vIndex] == "while") {
        While();
        return true;
    } else {
        return false;
    }
};

bool C() {
    printRule("<Condition> -> <Expression> <Relationship Operator> <Expression>");
    if(!E()) return false;
    
    if (lexemeVector[vIndex] == "=="    || lexemeVector[vIndex] == "!="
        || lexemeVector[vIndex] == "<"  || lexemeVector[vIndex] == ">"
        || lexemeVector[vIndex] == "<=" || lexemeVector[vIndex] == ">=") {
        vIndex++;
    } else return false;
    
    if(!E()) return false;

    return true;
};

//  A  -> i = E
bool A() {
//    printTL();
    printRule("<Assign> -> <Identifer> = <Expression>");
    
    if (tokenVector.at(vIndex) == "IDENTIFIER") {
        vIndex++;
        printTL();
        
        if (lexemeVector.at(vIndex) == "=") {
            vIndex++;
            if (!E()) return false;
        } else {
            std::cerr << "Expected =\n";
            return false;
        }
    } else {
        std::cerr << "Expected identifier\n";
        return false;
    }
    return true;
};

//  E  -> TE'
bool E() {
    
    printTL();
    printRule("<Expression> -> <Term> <Expression Prime>");
    
    if (tokenVector.at(vIndex) != "IDENTIFIER") {
        std::cerr << "Expected identifier\n";
        return false;
    }
    
    T();
    EPrime();
    return true;
};

//  E' -> +TE' | -TE' | ε
bool EPrime() {
    
    printRule("<Expression Prime> -> + <Term> <Expression Prime> | - <Term> <Expression Prime> | ε");

    //logic (RULES)
    if (lexemeVector.at(vIndex) == "+" || lexemeVector.at(vIndex) == "-") {
        
        vIndex++;
        printTL();
        
        if(!T()) return false;
        EPrime();
        return true;
    } else {
        printTL();

        epsilon();
    }
    return false;
};

//  T  -> FT'
bool T() {
    
    printRule("<Term> -> <Factor> <Term Prime>");
    if(tokenVector.at(vIndex) != "IDENTIFIER") {
        std::cerr << "Expected identifier\n";
        return false;
    }
    F();
    TPrime();
    
    return true;
};

//  T' -> *FT' | /FT' | ε
bool TPrime() {
    
    printRule("<Term Prime> -> * <Factor> <Term Prime> | / <Factor> <Term Prime> | ε");

    if (lexemeVector.at(vIndex) == "*" || lexemeVector.at(vIndex) == "/") {
        //vIndex++;
        F();
        TPrime();
        return true;
    } else {
        epsilon();
    }
    return false;
};

//  F -> i | (E)
bool F() {
    
    printRule("<Factor> -> " + lexemeVector.at(vIndex));

    vIndex++;
    printTL();

    //cout << "\nToken: " << tokenVector.at(vIndex) << "\tLexeme: " << lexemeVector.at(vIndex) << endl;

    return false;
};


bool If() {
    printRule("<If> -> If ( <Condition ) { <Statement> }");
    
    if (lexemeVector.at(vIndex) == "if") {
        vIndex++;
        printTL();
        
        if (lexemeVector.at(vIndex) == "(") {
            vIndex++;
            
            if (!C()) return false;
            //vIndex++;
            
            if (lexemeVector.at(vIndex) == ")") {
                vIndex++;
                printTL();

                if (lexemeVector.at(vIndex) == "{") {
                    vIndex++;
                    printTL();
                    
                    if ( !S() ) return false;
                    vIndex++;
                    
                    if (lexemeVector.at(vIndex) == "}") {
                        vIndex++;
                        Else();
                        return true;
                    }
                }
            }
        }
    }
    return false;
};


bool Else() {
    printTL();
    printRule("<Else> -> else { <Statement> }");

    if (lexemeVector.at(vIndex) == "else") {
        vIndex++;
        printTL();
        
        if (lexemeVector.at(vIndex) == "{") {
            vIndex++;
            if(!S()) return false;
            //vIndex++;
            
            if (lexemeVector.at(vIndex) == "}") {
                vIndex++;
                return true;
            }
        }
    } else {
        vIndex--;
        epsilon();
    }
    return false;
};

bool While() {
    printTL();
    printRule("<While> -> While ( <Condition> ) { <Statement> }");
    
    if (lexemeVector.at(vIndex) == "while") {
        vIndex++;
        printTL();
        
        if (lexemeVector.at(vIndex) == "(") {
            vIndex++;
            printTL();

            if (!C()) return false;
            //vIndex++;
            if (lexemeVector.at(vIndex) == ")"){
                vIndex++;
                //cout << "TESTING!!!!!!!!!!\n";
                if (lexemeVector.at(vIndex) == "{") {
                    printTL();
                    vIndex++;
                    if (!S()) return false;

                    vIndex++;
                    if (lexemeVector.at(vIndex) == "}") return true;
                }
            }
        }
    }
    return false;
};


void epsilon () {
    printRule("<Empty> -> ε case");
//    if (lexemeVector[vIndex] == ";") {
//        vIndex++;
//        cout << "lexeme: " << lexemeVector[vIndex] << endl;
//    }
};















void printRule(string rule) {
    //Print to CMD Line
    if (printSwitch) {
        cout << rule << endl;
    }
    //Print to file
    if (outputSwitch) {
        outputFile << "\t" << rule << endl;
    }
}
void printTL(){
    if (printSwitch) {
        cout << "\nToken: " << tokenVector.at(vIndex) << "\tLexeme: " << lexemeVector.at(vIndex) << endl;
    }
    //Print to file
    if (outputSwitch) {
        outputFile << "\nToken: " << tokenVector.at(vIndex) << "\tLexeme: " << lexemeVector.at(vIndex) << endl;
    }
}


//Just checking if starting symbols %% or $$ are used to indicate starting.
//if so, output them and then disregard first 2 elements
bool startSymbols(){
    if (lexemeVector.at(0) == "%" && lexemeVector.at(1) == "%") {
        if (printSwitch) {
            cout << "\nToken: " << tokenVector.at(0) << "\tLexeme: "
            << lexemeVector.at(0) << lexemeVector.at(1) << endl;
        }
        //Print to file
        if (outputSwitch) {
            outputFile << "\nToken: " << tokenVector.at(0) << "\tLexeme: "
            << lexemeVector.at(0) << lexemeVector.at(1) << endl;
        }
        return true;
    }
    return false;
}

//==========================================================================================
// TESTING TEMPORARY FUNCTIONS
//==========================================================================================

void printSVector (vector <string> * v) {
    for (newStringIterator it = v->begin(); it != v->end(); ++it) {
        cout << *it;
    }
}//end printSVector
