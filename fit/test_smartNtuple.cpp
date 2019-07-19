/*
c++ -o test_smartNtuple `root-config --glibs --cflags` smartNtuple.cc -lm test_smartNtuple.cpp
*/

#include "smartNtuple.h"

int main () 
{
  
  smartNtuple ("","") ;
  return 0 ;
}
