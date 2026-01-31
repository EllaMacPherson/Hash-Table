/* Ella MacPherson 2/23/25
   Hash Tables
*/

#include <iostream>
#include <cstring>
#include <string>
#include <iomanip>
#include <fstream>
#include <random>
#include "Node.h"
#include "Student.h"

using namespace std;

// remenant linked list code
void add(Node*& head, Node* prev, Node* current, student* student);
void print(Node*& head, Node* next);
void Delete(Node*& head, Node* prev, Node* current, int ID);

//for hashtable
void insert(Node**& hashtable, student* newStudent, int& size); 
int hashFunc(int ID, int& size);
void printHT(Node**& hashtable, int& size);
void rehash(Node**& hashtable, student* newStudent, int& size);
void randomAdd(int num, int size, Node**& hashtable);



int main(){

  Node** hashtable; //type of array
  hashtable = new Node*[100]; //initial array -> DO I NEED TO PUT THIS IN A CLAsS/CONSTRCTOR??
  int size = 100;
  //how do i change and rehash it in the future

  //fill table with NULL head pointers
  for(int i = 0; i < size; i++){
    hashtable[i] = NULL;
  }


  
  
  string input;
  bool go = true;

  //start with list empty
  Node* head = NULL;

  //loop forever
  while (go == true){
    //clear input
    input.clear();

    cout<<"Enter ADD, DELETE, PRINT, RANDOM or QUIT "<<endl;

    getline(cin, input);

    
    // insert new manual data into hash table
    if(input == "ADD") {
      //They want to add so collect all inputs
      float inGPA = 0;
      string inName;
      int inID;

      
      cout<<"Name of student? ";

      getline(cin, inName);

      cout<<"ID of student? ";
      cin>>inID;      
      cin.ignore();
      cout<< "GPA of student? ";
      cin>>inGPA;
      cin.ignore();
      
      //create student pointer to assign to this node
      student* s = new student(inGPA, inName, inID);
      insert(hashtable, s, size);
    }

    //print
    if(input =="PRINT"){
      //      print(head, head);
      //      cout<<"got print command.. about to call print"<<endl;
      printHT(hashtable, size);
    }

    if(input == "RANDOM"){
      int num;
      cout<<"How many students would you like to add?"<<endl;
      cin>>num;
      cin.ignore();
      randomAdd(num, size, hashtable);
    }
    
    //delete
    if(input == "DELETE" ){
      //gather input
      cout<<"Enter student ID to delete"<<endl;
      int ID = 0;
      cin>>ID;

      //delete function
      Delete(head, NULL, head, ID);
    }
    
    //Quit program
    if(input == "QUIT"){

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

void randomAdd(int num, int size, Node**& hashtable){
  //repeat this for # of people wanted to be added to program



  for(int i; i < num; i++){
    //randomly generate 1 # for first name and another # for last
    //random generator set up..
    ifstream firstFile("first.txt");
    ifstream lastFile("last.txt");

    random_device rd;
    mt19937 gen(rd());


    uniform_int_distribution<int> idDist(1, 100);
    uniform_real_distribution<float> GPAdist(0.0f, 4.0f);

    //generate name!!!
    int first = idDist(gen);
    int last = idDist(gen);
    //    cout<<"first random number index: " <<first<<endl;
    //    cout<<"second random number index: " <<last<<endl;

    //keeping it in cstring cause whole project built around it..

    string firstname = "";
    string lastname = "";


    int current = 0;

    // If you want 1-based indexing (line 1 = first line), keep this
    // If you want 0-based (line 0 = first line), change to current < targetIndex
    while (current < first && getline(firstFile, firstname)) {
        ++current;
    }

    if (current == first) {
      getline(firstFile, firstname);
    }

    //    cout<<"current: "<<current<<endl; 
    //cout<<"first name with that number : " <<firstname<<endl;
    
    int currentLast = 0;
    while (currentLast < last && getline(lastFile, lastname)) {
        ++currentLast;
    }

    if (currentLast == last) {
      getline(lastFile, lastname);
    }

    //    cout<<"lastindex: "<<currentLast<<endl; 

    //    cout<<"lastname with that number: " <<lastname<<endl;


    
    string fullName = "";
    fullName = firstname + " " + lastname;
    //    cout<<"Full name: " <<fullName<<endl;
    
    //incremental ID
    int ID = 423000 + i;
    
    //randomly generate a GPA
    float GPA = GPAdist(gen);

    //    cout<<"ADDING STUDENT"<<endl;
    //make a new student with all this info
    student* s = new student(GPA, fullName, ID);

    //feed into insert func
    insert(hashtable, s, size);
  }
  return;

}


void insert(Node**& hashtable, student* s, int& size){
  //  cout<<"insert func running"<<endl;

  int valueToHash = s->getID();
  int i = hashFunc(valueToHash, size);

  //  cout<<"index: " << i <<endl;

  
  Node* current = hashtable[i];
  if(current == NULL){ //if current bucket is EMPTY just insert it there
    hashtable[i] = new Node(s);
    //  cout<<"bucket: "<<i <<"content: " << s->getName()<< " " <<s->getID()<<endl; 
    return;
  }else{
    //if 2nd slot open
    if(current->getNext() == NULL){
      add(hashtable[i], NULL, hashtable[i], s);
      return;
    }
    else{
      //if 3rd slot opem
      if(current->getNext()->getNext() == NULL){
	add(hashtable[i], NULL, hashtable[i], s);
	return;
      }
      //if all slots FULL!! REHASHA TIMEEE
      else{
	//	cout<<"calling rehash"<<endl;
	rehash(hashtable, s, size); //makes a new hashtable, sets hashtable = to it, reinitializes everything, adds our new student
	return;
      }
    }
    
    return;
  }
}

int hashFunc(int ID, int& size){
  int index = ID % size; 
  return index;
}

void rehash(Node**& currentHT, student* newStudent, int& size){
  int oldSize = size;
  //  cout<<"rehash RUNING"<<endl;
  size = (size * 2) + 1;
  //cout<<"new array size: " << size<< endl;
  
  Node** newHT;
  newHT = new Node*[size];
  //fill it will null pointers
  for(int i = 0; i < size; i++){
    newHT[i] = NULL;
  }

  //iniialize it with old current HT
  //cout<<"reinitalizing array"<<endl;
  
  for(int i = 0; i < oldSize; i++){
    if(currentHT[i] != NULL){    //if theres something there
      student* s = currentHT[i]->getStudent(); //take the student thats there
      insert(newHT, s, size); //insert it in new hashtable
      if(currentHT[i]->getNext() != NULL){
	s = currentHT[i]->getNext()->getStudent();
	insert(newHT, s, size);
	if(currentHT[i]->getNext()->getNext() != NULL){
	  s = currentHT[i]->getNext()->getNext()->getStudent();
	  insert(newHT, s, size);
	}
      }
    }
  }


  // cout<<"adding new student manually"<<endl;
  //add student that needed to be added initially...!
  insert(newHT, newStudent, size);

  //cout<<"setting currentHT equal to the new one js created"<<endl;
  //update current hashtable to be this new hashtable
  currentHT = newHT;
  cout<<"rehash done"<<endl;

}


void printHT(Node**& hashtable, int& size){
  //cout<<"running printHT"<<endl;
  //cout<<"size: "<<size<<endl;
  
  for(int i = 0; i < size; i++){
    //  cout<<"for loop iteration: " << i <<endl;
    //THERE MUST BE A BETTER WAY????? do i have to use recursion??
    //    cout<<"iteration: "<< i <<endl;
    Node* current;
    if(hashtable[i] != NULL){ //if theres something there print it!
      //cout<<"SOMETHING HERE!"<<endl;

      
      cout<< hashtable[i]->getStudent()->getName()<<" "<<
	hashtable[i]->getStudent()->getID()<< " " <<
	hashtable[i]->getStudent()->getGPA();
      if(hashtable[i]->getNext() != NULL){ //2nd one in LL 
	cout<<hashtable[i]->getNext()->getStudent()->getName()<< " ";
	current = hashtable[i]->getNext();
	if(current->getNext() != NULL){ //3rd one in LL
	  cout<<current->getNext()->getStudent()->getName();
	}
      }
      cout<<endl;
    }
    
  }
}


//does this work when IDS are equal??? i swear it worked in LL
//add func ->takes student pointer and puts it in correct list position
void add(Node*& head, Node* prev, Node* current, student* student){


  if(current->getStudent()->getID() < student->getID()){ //Check if ID is greater than, we must continue if so
    if(current->getNext() == NULL){ //if student ID is still GREATER Than the one its currently on, but th next is NULL, then we must add it at end of list!!! Don't want current to ever equal NULL excpet for when list is truly empty. So must check in advance here if we're at the end and the ID is still greater
      //If case is true, then we want our last node to be this one, as it is the greatest in the list
      current->setNext(new Node(student));
      //      cout<<"set in same bucket, prev: " << prev->getStudent()->getName()<<endl;
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
      //cout<<"set in same bucket, prev: " << prev->getStudent()->getName()<<endl;
      return; //exit function
    }
    
    //If program is here - We are inserting in the middle of the list
    
    Node* newNode = new Node(student); //create placeholder for our node
    newNode->setNext(current); //set next as current (we are 1 ahead of where it should be)
    prev->setNext(newNode); //set previous as linked to this node now!
    //cout<<"set in same bucket, prev: " << prev->getStudent()->getName()<<endl;
    
  }    
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



