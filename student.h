#ifndef student_H
#define student_H

#include <iostream>
#include <cstring>
#include <string>

using namespace std;

//student class
class student{
 public:
  student(float inGPA, string inName, int inID); //set GPA and name

  string getName(); //retrieve name
  float getGPA(); //retrieve gpa
  int getID(); //retrieve ID

  string name;
  float gpa;
  int id;
};

#endif
