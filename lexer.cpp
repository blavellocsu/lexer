//USAGE
//compile with g++ lexer.cpp -olexer
//run with ./lexer SampleInputFile1.txt

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <list>
#define newIterator list<char>::iterator // just to make the code a little easier to read

using namespace std;

//Global data
const string keyword[] = { "int", "float", "bool", "if", "else", "then", "do", "while",
                       "whileend", "do", "doend", "for", "and", "or", "function"};
const string separators[] = {"'", "(", ")", "{", "}", "[", "]", ",", ".", ":", ";", "!"};
const string operators[] = {"*", "+", "-", "=", "/", ">", "<", "%"};

bool isKeyword();
bool isOperator();
void removeComments(list<char> *l);
void testPrint(list<char> *l);
//string grabLexeme (list<char> *l);
bool isAlpha (char);


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
    list <char> charList;
    //Create filesream
    ifstream file(filename);
    //check to see if file opened correctly
    if (file.is_open()) {
      cout << "File opened successfully." << endl;
        
      // read content of txt file (including white spaces) into a string
        string contents((std::istreambuf_iterator<char>(file)),
                        std::istreambuf_iterator<char>());
        
        for (int i = 0; i < contents.length(); i++) {
            charList.push_back(contents[i]);
        }


    } else {
     cout << "File Error: Could not open file.\n" << usage << endl;
     exit(1);
    }

    //remove comments from code
    removeComments(&charList);

    //spacing
    cout << "\n\n";

    //output to see if comments are gone
    testPrint(&charList);

  //Test Print to see if it completed
  cout << "\nEnd Program\n";

  return 0;
} //end main


//This function accepts an address to a std::list<char>
//It iterates through the list and deletes the commented phrases
void removeComments(list<char> *l) {
  //Remove Comments from charList
  //iterate through list
  for (newIterator itr = l->begin();
   itr != l->end(); advance(itr,1)) {
    //check if current char is equal to '!'
    if (*itr == '!') {
      //delete until find next '!'
      do {
        //charList.erase returns next element
        //set itr = to list.erase
        itr = l->erase(itr++);
      } while (*itr != '!');
      //jumps out of do loop at last '!', so delete last one
      itr = l->erase(itr++);
    }//end if '!'
  }//end for
}//end function

//Accepts an address to a std::list<chars>
//prints out list to console
void testPrint (list<char> *l) {
  //Print Function
    
    cout << "This is the char list: ";
  for (newIterator itr = l->begin(); itr != l->end(); ++itr) {
    std::cout /*<< ' '*/ << *itr;
  }
}

//string grabLexeme (list<char> *l) {
//    // if it is an alphebetical character
//
//}

//bool isAlpha (char c) {
//
//    // 65 through 90 is upper case alpha in ASCII, 97 to 122 is lowercase
//    if ((c >= 65 && <= 90) || (c <= 97 && >= 122)) {
//        return true;
//    } else {return false}
//
//}
