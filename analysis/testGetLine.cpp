// c++ -o testGetLine testGetLine.cpp

#include <fstream> 
#include <iostream> 
#include <string> 

using namespace std ; 
  
int main (int argc, char ** argv) 
{ 
    string str ;
	ifstream infile ;
    infile.open (argv[1]) ;

    while (getline (infile, str))
      cout << str << endl ;
      
    return 0; 
} 