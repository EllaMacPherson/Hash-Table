#ifndef NODE_H
#define NODE_H
#include <iostream>
#include "student.h"

using namespace std;

class Node{
 public:
  //constructor
  Node(student*);
  //destructor
  ~Node();
  //get student
  student* getStudent();
  //set next node
  void setNext(Node*);
  //get next node
  Node* getNext();
 private:
  student* value;
  Node* next;
};
#endif
