/* Ella MacPherson 1/12/25
   Linked List part 2
*/

#include <iostream>
#include <cstring>
#include <iomanip>
#include "Node.h"
#include "Student.h"

using namespace std;

void add(Node*& head, Node* prev, Node* current, student* student);
void print(Node*& head, Node* next);
void Delete(Node*& head, Node* prev, Node* current, int ID);
void average(Node*& head, Node* current, float total, int it);

int main(){

  char input[10];
  bool go = true;

  //start empty
  Node* head = NULL;

  //loop forever
  while (go == true){
    input[0] = '\0';
    cout<<"Enter ADD, DELETE, PRINT, AVERAGE, or QUIT "<<endl;
    cin>>input;
    cin.ignore();
    //add and ask for inputs
    if(strcmp(input, "ADD") == 0) {

      float inGPA = 0;
      char inName[15];
      int inID;
  
      cout<<"Name of student? ";
      cin.get(inName, 14);
      cin.ignore();
      cout<<"ID of student? ";
      cin>>inID;      
      cin.ignore();
      cout<< "GPA of student? ";
      cin>>inGPA;
      cin.ignore();
      //create student pointer to assign to this node
      student* s = new student(inGPA, inName, inID);
      //assign that pointer
      add(head, NULL, head, s);
    }

    //print with recursion
    if(strcmp(input, "PRINT") == 0){
      print(head, head);
    }

    if(strcmp(input, "DELETE") == 0){
      cout<<"Enter student ID to delete"<<endl;
      int ID = 0;
      cin>>ID;
      
      Delete(head, NULL, head, ID);
    }
    if(strcmp(input, "AVERAGE") == 0){
      average(head, head, 0, 0);
    }
    if(strcmp(input, "QUIT") == 0){
      //iterate through
      while(head != NULL){
	Node* toDelete = head;
	head = head->getNext();
	delete toDelete;
      }
      
      return 0;
    }
  }
  
  return 0;
}


//add func DONE, do i need HEAD, yes to set new node
void add(Node*& head, Node* prev, Node* current, student* student){
  //head is always head, current is the one we are looking at at the moment.
  
  if(current == NULL){
    //add when list is empty
    head = new Node(student);
  }
  else{ //list has at least one node    
    if(current->getStudent()->getID() < student->getID()){
      if(current->getNext() == NULL){ //if student ID is still GREATER Than the one its currently on, but th enext is NULL, then we must add it at end of list!!! and NEVER let current become NULL excpet for when list is truly empty
	current->setNext(new Node(student));
	return;
      }
      prev = current;
      add(head, prev, current->getNext(), student);
    }else{ //must add this student IN FRONT of current
      if(prev == NULL){ //we are inserting at beginning of LIST must readjust head
	Node* newNode = new Node(student); //create new node
	newNode->setNext(current);
	head = newNode; //update head as this new node
	return;
      }
      //we are inserting BEFORE the CURRENT node!
      Node* newNode = new Node(student);
      newNode->setNext(current);
      prev->setNext(newNode);
      
    }    
  }
}

//print func DONE
void print(Node*& head, Node* current){
  if(current == head){ //on first one print 
    cout<<"List: ";
  }

  if(current != NULL){ //if we are not at last one in list
    cout<<current->getStudent()->getName()<<", ";
    cout<<current->getStudent()->getID()<<", ";
    cout<<current->getStudent()->getGPA()<<"  ";
    print(head,current->getNext()); //call func again but 1 more down list
  }
}

//delete func DONE ONLY need to delete first node with matching GPA in the list! USE DESTRUCTOR!!!!!!!!!!!
void Delete(Node*& head, Node* prev, Node* current, int ID){

  if(current == head){ //first call
    cout<<"Matches found: ";
  }

  if(current == NULL){ //when nothing in list
    cout<<"No students currently in list!"<<endl;
    return;
  }

  //if finds GPA --> RUN RECURSION AND DELETE
  if(current->getStudent()->getID() == ID){
    cout<<current->getStudent()->getName()<<" " <<endl;
    //RUN DELETION and exit func
    //DETERMINE WHERE WE ARE IN THE LIST
    if(prev == NULL){ // delete like beginning of list
      head = current->getNext();
      delete current;
      return;
    }
    else if(current->getNext() == NULL){ //delete like end of list
      if(prev == NULL){ //if only item in list
	delete current;
	head = NULL;
	return;
      }
      prev->setNext(NULL);
      delete current;
      return;
    }
    else{ //delete like middle of the list
      prev->setNext(current->getNext()); //set one before to one after current
      delete current;
      return;
    }

    return;
  }

  if(current->getNext() != NULL){ //look at next one if it's there!
    prev = current;
    Delete(head, prev, current->getNext(), ID);
  }
  else{ //if no ID in list truly
    cout<<"No matching ID in list, try again"<<endl;
  }
  
}

void average(Node*& head, Node* current, float total, int it){

  //if nothing in list
  if(head == NULL){
    cout<<"No students in list to average!"<<endl;
    return;
  }
  //if only one in list, average is that GPA
  if(current == head && current->getNext() == NULL){
    cout<<fixed;
    cout<<setprecision(2);
    cout<<"Average: " << current->getStudent()->getGPA() << endl;
    return;
  }

  //if reached end of list and collected all GPAs 
  if(current == NULL){
    float calculatedAverage = total/it;
    cout<<fixed;
    cout<<setprecision(2);
    cout<<"Average: " << calculatedAverage <<endl;
    return;
  }
  student* s = current->getStudent();
  total += s->getGPA();
  it++;
  average(head, current->getNext(), total, it);

  //

}
