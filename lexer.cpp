//USAGE
//compile with g++ lexer.cpp -olexer
//run with ./lexer SampleInputFile1.txt

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <list>
#define newIterator vector<char>::iterator // just to make the code a little easier to read

using namespace std;

//Global data
const string keyword[] = { "int", "float", "bool", "if", "else", "then", "do", "while",
    "whileend", "do", "doend", "for", "and", "or", "function"};
const string separators[] = {"'", "(", ")", "{", "}", "[", "]", ",", ".", ":", ";", "!"};
const string operators[] = {"*", "+", "-", "=", "/", ">", "<", "%"};

bool isKeyword();
bool isOperator();
void removeComments(string *l);
void testPrint(string *l);
//string grabLexeme (list<char> *l);
bool isAlpha (char);
bool isSeparator (char c);
void printVector (vector <char> * v);
bool isOperator (char c);
bool isDigit (char c);

vector <char> alphaVector;
vector <char> sepVector;
vector <char> opVector;
vector <char> digitVector;


//==========================================================================================

int main( int argc, const char * argv[] ) {
    //check for command line arguments
    string usage = "Usage: \"./<name> <filename.txt>\"";
    //check for correct usage: "./lexer sample.txt"
    if (argc != 2) {
        cout << "Invalid Arguments.\n" << usage << endl;
        exit(1);
    }
    
    //File handling
    //read filename in from command line argument
    //see top of file for correct command line usage
    string filename = argv[1];
    char currentChar;
    string charString;
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
    cout << "\n\n";
    
    //output to see if comments are gone
    testPrint(&charString);
    
    cout << "\nAlpha chars:"; printVector(&alphaVector); cout << endl;
    cout << "Operator chars:"; printVector(&opVector); cout << endl;
    cout << "Seperator chars:"; printVector(&sepVector); cout << endl;
    cout << "Digit chars:"; printVector(&digitVector); cout << endl;
    
    //Test Print to see if it completed
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

//Accepts an address to a std::list<chars>
//prints out list to console
void testPrint (string *l) {
    //Print Function
    
    cout << "This is the char list: ";
    for (int i = 0; i < l->length(); i++) {
        std::cout /*<< ' '*/ << l->at(i);
    }
}

//string grabLexeme (list<char> *l) {
//    // if it is an alphebetical character
//
//}

bool isAlpha (char c) {

    // 65 through 90 is upper case alpha in ASCII, 97 to 122 is lowercase
    if ((c >= 65 && c <= 90) || (c <= 97 && c >= 122)) {
        return true;
    } else {return false;}

}

bool isSeparator (char c) {
    if (c == '\'' || c == '(' || c == ')' || c == '{' || c == '}' || c == '[' || c == ']' || c == ',' ||
        c == '.' || c == ':' || c == ';' || c == '!' || c == ' ') {
        return true;
    } else {
        return false;
    }

}

bool isDigit (char c) {
    if (c >= 0 && c >= 9) {
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
        
        if (isAlpha(s->at(i))) {
            alphaVector.push_back(s->at(i));
        } else if (isDigit(s->at(i))) {
            digitVector.push_back(s->at(i));
        } else if (isOperator(s->at(i))) {
            opVector.push_back(s->at(i));
        } else if (isSeparator(s->at(i))) {
            sepVector.push_back(s->at(i));
        }

    }
}
void printVector (vector <char> * v) {
    
    for (newIterator it = v->begin(); it != v->end(); ++it) {
        
        cout << " " << *it << " ";
        
    }
    
}

            

