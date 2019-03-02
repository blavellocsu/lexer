//USAGE
//compile with g++ lexer.cpp -olexer
//run with ./lexer SampleInputFile1.txt

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <list>

using namespace std;


void testPrint(string *l);  //completed

int fsm[7][10] = {2,  4,  1,  1,  10, 10, 1,
                  2,  2,  2,  3,  3,  3,  1,
                  1,  1,  1,  1,  1,  1,  1,
                  5,  4,  5,  5,  5,  6,  1,
                  1,  1,  1,  1,  1,  1,  1,
                  7,  6,  7,  7,  7,  7,  1,
                  1,  1,  1,  1,  1,  1,  };

int main ( int argc, const char * argv[] ) {
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
  //ofstream outputFile.open("output.txt");

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

  testPrint(&charString);
  cout << "\n\n";





}//end main



void testPrint (string *l) {
    //Print Function
    for (int i = 0; i < l->length(); i++) {
        std::cout /*<< ' '*/ << l->at(i);
    } //end for
}//end testPrint
