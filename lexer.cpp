//USAGE
//compile with g++ lexer.cpp -olexer
//run with ./lexer SampleInputFile1.txt

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <list>
using namespace std;

//Global data
const string keyword[] = { "int", "float", "bool", "if", "else", "then", "do", "while",
                       "whileend", "do", "doend", "for", "and", "or", "function"};
const string separators[] = {"'", "(", ")", "{", "}", "[", "]", ",", ".", ":", ";", "!"};
const string operators[] = {"*", "+", "-", "=", "/", ">", "<", "%"};

bool isKeyword();
bool isOperatior;

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

      //create while loop to run through txt file
      while (!file.eof()) {

        //read each character into currentChar
        file >> currentChar;

        //test output
        cout << currentChar << " ";

        charList.push_back(currentChar);
      }

    } else {
     cout << "File Error: Could not open file.\n" << usage << endl;
     exit(1);
    }

cout << "\n\n\n";
    //Remove Comments from charList
    //iterate through list
    for (list<char>::iterator itr = charList.begin();
     itr != charList.end(); advance(itr,1)) {
      //check if current char is equal to '!'
      if (*itr == '!') {
        //delete until find next '!'
        do {
          //charList.erase returns next element
          //set itr = to list.erase
          itr = charList.erase(itr++);
        } while (*itr != '!');
        //jumps out of do loop at last '!', so delete last one
        itr = charList.erase(itr++);
      }//end if '!'
    }//end for


    //Print Function
    for (list<char>::iterator itr = charList.begin(); itr != charList.end(); ++itr) {
      std::cout << ' ' << *itr;
    }

  //Test Print
  cout << "\nEnd Program\n";
  return 0;
} //end main
