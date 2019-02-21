#include <iostream>
#include <fstream>
#include <string>
//#include <vector>
//#include <list>
using namespace std;

//Global data
const string keyword[] = { "int", "float", "bool", "if", "else", "then", "do", "while",
                       "whileend", "do", "doend", "for", "and", "or", "function"};
const string separators[] = {"'", "(", ")", "{", "}", "[", "]", ",", ".", ":", ";", "!"};
const string operators[] = {"*", "+", "-", "=", "/", ">", "<", "%"};

int main( int argc, const char * argv[] ) {
  //check for command line arguments
    string usage = "Usage: \"./<name> <filename.txt>\"";
    //check for correct usage: "./lexer sample.txt"
    if (argc != 2) {
      cout << "Invalid Arguments.\n" << usage << endl;
      exit(1);
    }

  //File handling
    //read filename in from argument
    string filename = argv[1];
    char currentChar;
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

        //NEXT STEP
        //add current char to some sort of data structure
        //possibly queue, list, vector.
      }

    } else {
     cout << "File Error: Could not open file.\n" << usage << endl;
     exit(1);
    }

  //Test Print
  cout << "End Program\n";
  return 0;
} //end main
