/* Ella MacPherson 2/23/25
   Hash Tables
*/

#include <iostream>
#include <cstring>
#include <iomanip>
#include "Node.h"
#include "Student.h"

using namespace std;

// remenant linked list code
void add(Node*& head, Node* prev, Node* current, student* student);
void print(Node*& head, Node* next);
void Delete(Node*& head, Node* prev, Node* current, int ID);

//for hashtable
void insert(Node**& hashtable, student* newStudent); 
int hashFunc(int ID, int size);


int main(){

  Node** hashtable; //type of array
  hashtable = new Node*[99]; //initial array -> DO I NEED TO PUT THIS IN A CLAsS/CONSTRCTOR??
  //how do i change and rehash it in the future

  //fill table with NULL head pointers
  for(int i = 0; i < 100; i++){

    hashtable[i] = NULL;
  }


  
  
  char input[10];
  bool go = true;

  //start with list empty
  Node* head = NULL;

  //loop forever
  while (go == true){
    //clear input
    
    input[0] = '\0';
    cout<<"Enter ADD, DELETE, PRINT or QUIT "<<endl;
    cin>>input;
    cin.ignore();
    
    // insert new manual data into hash table
    if(strcmp(input, "ADD") == 0) {
      //They want to add so collect all inputs
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
      insert(hashtable, s);
    }

    //print
    if(strcmp(input, "PRINT") == 0){
      print(head, head);
    }

    //delete
    if(strcmp(input, "DELETE") == 0){
      //gather input
      cout<<"Enter student ID to delete"<<endl;
      int ID = 0;
      cin>>ID;

      //delete function
      Delete(head, NULL, head, ID);
    }
    
    //Quit program
    if(strcmp(input, "QUIT") == 0){

      //iterate through
      while(head != NULL){
	Node* toDelete = head;
	head = head->getNext();
	delete toDelete; //delete every node
      }

      //exit main!
      return 0;
    }
  }
}

void insert(Node**& hashtable, student* s){
  cout<<"insert func running"<<endl;

  int valueToHash = s->getID();
  int i = hashFunc(valueToHash, 100);

  cout<<"index: " << i <<endl;

  
  Node* current = hashtable[i];
  if(current == NULL){ //if current bucket is EMPTY just insert it there
    hashtable[i] = new Node(s);
    cout<<"bucket: "<<i <<"content: " << s->getName()<< " " <<s->getID()<<endl; 
    return;
  }else{ //THERES A COLLOISION, LL it
    while(current->getNext() != NULL){
      current = current->getNext();
    }
  }
  current->setNext(new Node(s));
  return;
}

int hashFunc(int ID, int size){
  int index = ID % size; 
  return index;
}


//add func ->takes student pointer and puts it in correct list position
void add(Node*& head, Node* prev, Node* current, student* student){


  /*
  // just add when list is empty
  if(current == NULL){
    head = new Node(student);
  }
  else{ //list has at least one node    
    if(current->getStudent()->getID() < student->getID()){ //Check if ID is greater than, we must continue if so
      if(current->getNext() == NULL){ //if student ID is still GREATER Than the one its currently on, but th next is NULL, then we must add it at end of list!!! Don't want current to ever equal NULL excpet for when list is truly empty. So must check in advance here if we're at the end and the ID is still greater
	//If case is true, then we want our last node to be this one, as it is the greatest in the list
	current->setNext(new Node(student));
	return;
      }
      //If not true, recursively call add function again, store previous for storing in middle of the list later
      prev = current;
      add(head, prev, current->getNext(), student);
    }
    else{ //ID is LESS than next one, so we must insert it here
      if(prev == NULL){ //If previous is NULL, then we are at head of list and must reset head
	Node* newNode = new Node(student); //create new node
	newNode->setNext(current); //push current node back
	head = newNode; //update head as this new node
	return; //exit function
      }

      //If program is here - We are inserting in the middle of the list

      Node* newNode = new Node(student); //create placeholder for our node
      newNode->setNext(current); //set next as current (we are 1 ahead of where it should be)
      prev->setNext(newNode); //set previous as linked to this node now!
      
    }    
  }
  */
}

//print func
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

//delete func
void Delete(Node*& head, Node* prev, Node* current, int ID){

  if(current == head){ //first call
    cout<<"Match found & deleted: ";
  }

  if(current == NULL){ //when nothing in list
    cout<<"No students currently in list!"<<endl;
    return;
  }

  //check if ID matches if it does --> run recursion and delete
  if(current->getStudent()->getID() == ID){
    //Print name of student found
    cout<<current->getStudent()->getName()<<" " <<endl;

    //Determine position of deletion
    if(prev == NULL){ // delete beginning of list
      head = current->getNext();
      delete current;
      return;
    }
    else if(current->getNext() == NULL){ //delete end of list
      if(prev == NULL){ //if only item in list
	delete current;
	head = NULL; //change head
	return;
      }
      //end delete but its only one in list
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

  //ID does not match, lets recursively call again
  if(current->getNext() != NULL){
    prev = current; //store prev
    Delete(head, prev, current->getNext(), ID);
  }
  else{ //if no ID matches (only other possible case)
    cout<<"No matching ID in list, try again"<<endl;
  }
  
}



