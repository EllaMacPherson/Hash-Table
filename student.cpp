#include "student.h"
#include <iostream>

#include<string>


//constructur
student::student(float inGPA, string inName, int inID){
  gpa = inGPA;
  id = inID;
  name = inName;
}

//give name
string student::getName(){
  return name;
}

//give GPA
float student::getGPA(){
  return gpa;
}

//give ID
int student::getID(){
  return id;
}
