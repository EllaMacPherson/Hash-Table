#include "student.h"
#include <iostream>
#include <cstring>


//constructur
student::student(float inGPA, char* inName, int inID){
  gpa = inGPA;
  id = inID;
  name = new char[50];
  strcpy(name, inName);
}

//give name
char* student::getName(){
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
