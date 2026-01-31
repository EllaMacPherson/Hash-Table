/* Ella MacPherson 2/13/25
   Hash Tables
   Student-list but with hashtable
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

// remenant linked list code declartions;
void add(Node*& head, Node* prev, Node* current, student* student); //reused in HT insertion
void Delete(Node*& head, Node* prev, Node* current, int ID); //reused in HT deletion

// hashtable function declartion
void insert(Node**& hashtable, student* newStudent, int& size); 
int hashFunc(int ID, int& size);
void printHT(Node**& hashtable, int& size);
void rehash(Node**& hashtable, student* newStudent, int& size);
void randomAdd(int num, int& size, Node**& hashtable, int& j);
void deleteHT(Node**& hashtable, int& size, int xID);


int main(){

  //initalize hashtable array
  Node** hashtable;
  hashtable = new Node*[100];

  //creat variable to hold current size of hashtable
  int size = 100;

  //fill table with NULL head pointers
  for(int i = 0; i < size; i++){
    hashtable[i] = NULL;
  }

  //input string, loop var
  string input;
  bool go = true;
  
  //counter for # times random has been called so it doesnt add the same numbers over and over and cause rehash mayham
  int j = 0;

  //running loop
  while (go == true){

    //clear input
    input.clear();

    cout<<"Enter ADD, DELETE, PRINT, RANDOM or QUIT "<<endl;

    //get input
    getline(cin, input);

    // manual data add
    if(input == "ADD") {
      
      // Initialize all input
      float inGPA = 0;
      string inName;
      int inID;

      // Gather...
      cout<<"Name of student? ";
      getline(cin, inName);
      cout<<"ID of student? ";
      cin>>inID;      
      cin.ignore();
      cout<< "GPA of student? ";
      cin>>inGPA;
      cin.ignore();
      
      //create student pointer to insert
      student* s = new student(inGPA, inName, inID);
      //call insert function
      insert(hashtable, s, size);
    }

    //print table
    if(input =="PRINT"){
      printHT(hashtable, size);
    }

    //Randomly add X students
    if(input == "RANDOM"){
      //store how many they want to add
      int num;
      cout<<"How many students would you like to add?"<<endl;
      cin>>num;
      cin.ignore();

      //generate and insert them!
      randomAdd(num, size, hashtable, j);
    }
    
    //delete
    if(input == "DELETE" ){
      //gather ID 
      cout<<"Enter student ID to delete"<<endl;
      int ID = 0;
      cin>>ID;
      cin.ignore();
      
      //delete function
      deleteHT(hashtable, size, ID);
    }
    
    //Quit program
    if(input == "QUIT"){

      //iterate through
      for(int i = 0; i > size; i++){
	if(hashtable[i] != NULL){ //if theres something there delete it
	  if(hashtable[i]->getNext() != NULL){ //
	    if(hashtable[i]->getNext()->getNext() != NULL){
	      delete hashtable[i]->getNext()->getNext();
	      delete hashtable[i]->getNext();
	      delete hashtable[i];
	    }
	    delete hashtable[i]->getNext();
	    delete hashtable[i];
	  }
	}
	delete hashtable[i];
      }

      delete[] hashtable; //delete array once all Node* deleted

      //exit main!
      return 0;
    }
  }
}

// delete from hashtable
void deleteHT(Node**& hashtable, int& size, int deleteID){

  //find index ID will be stored at
  int index = hashFunc(deleteID, size);

  //iterate through to that index
  for(int i = 0; i < size; i++){
    if(i == index){
      //delete from linked list
      Delete(hashtable[i], NULL, hashtable[i], deleteID);
    }
  }
  
  return; //mission accomplished
}


//randomly adds x number of students
void randomAdd(int num, int& size, Node**& hashtable, int& j){

  //repeat this for # of people wanted to be added to program
  for(int i; i < num; i++){

    //reset file definitions so getline doesnt save
    ifstream firstFile("first.txt");
    ifstream lastFile("last.txt");

    //random generator initialize
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<int> idDist(1, 99);
    uniform_real_distribution<float> GPAdist(0.0f, 4.0f);

    
    //generate name indexs
    int first = idDist(gen);
    int last = idDist(gen);

    //utilize those beautiful strings, NOT cstrings
    string firstname = "";
    string lastname = "";

    //index of file
    int current = 0;

    //get to correct line
    while (current < first && getline(firstFile, firstname)) {
        ++current;
    }
    //save in first name once there
    if (current == first) {
      getline(firstFile, firstname);
    }

    //same thing for last name
    int currentLast = 0;
    while (currentLast < last && getline(lastFile, lastname)) {
        ++currentLast;
    }

    if (currentLast == last) {
      getline(lastFile, lastname);
    }

    //smoosh em together
    string fullName = "";
    fullName = firstname + " " + lastname;
    
    //incremental ID
    int ID = 400201 + (3*i) + (7*j);
    
    //randomly generate a GPA
    float GPA = GPAdist(gen);


    //make a new student with all this info
    student* s = new student(GPA, fullName, ID);

    //feed into insert func
    insert(hashtable, s, size);
  }

  //change j so in future doesnt repeat ID's exactly
  j++;
  return;

}


//insert a student into the hashtable 
void insert(Node**& hashtable, student* s, int& size){

  //get index to store at
  int valueToHash = s->getID();
  int i = hashFunc(valueToHash, size);

  //create a current Node* to look at current index
  Node* current = hashtable[i];

  //if current bucket is EMPTY just insert it there
  if(current == NULL){
    //create a new node for that student at that location
    hashtable[i] = new Node(s);
    return;
    
  }else{
    //if 2nd slot open
    if(current->getNext() == NULL){
      add(hashtable[i], NULL, hashtable[i], s); //add like a LL (this sorts by ID which is unnecessary but not detrimental
      return;
    }
    else{
      //if 3rd slot opem
      if(current->getNext()->getNext() == NULL){
	add(hashtable[i], NULL, hashtable[i], s); //add like a LL
	return;
      }
      //if all slots FULL!! REHASH TIME!
      else{
	rehash(hashtable, s, size); //makes a new hashtable, sets hashtable = to it, reinitializes everything, adds our new student
	return;
      }
    }
    return;
  }
}

//quick lil hash func
int hashFunc(int ID, int& size){
  int index = ID % size; 
  return index;
}

//rehash function
void rehash(Node**& currentHT, student* newStudent, int& size){
  //store old size
  int oldSize = size;

  //create new size
  size = (size * 2) + 1;
  cout<<"rehashing to size: "<<size<<endl; //keeping debug comment cause it makes it clear to user

  //create NEW hashtable of NEW size!
  Node** newHT;
  newHT = new Node*[size];
  
  //fill it will null pointers
  for(int i = 0; i < size; i++){
    newHT[i] = NULL;
  }

  //fill it with elements in old HT
  for(int i = 0; i < oldSize; i++){
    if(currentHT[i] != NULL){    //if theres something there
      student* s = currentHT[i]->getStudent(); //take the student thats there
      insert(newHT, s, size); //insert it in new hashtable
      if(currentHT[i]->getNext() != NULL){ //do again if possible
	s = currentHT[i]->getNext()->getStudent();
	insert(newHT, s, size);
	if(currentHT[i]->getNext()->getNext() != NULL){ //do thrice if possible
	  s = currentHT[i]->getNext()->getNext()->getStudent();
	  insert(newHT, s, size);
	}
      }
    }
  }


  //add student that needed to be added initially...! into the new HT of course
  insert(newHT, newStudent, size);

  delete[] currentHT;
  //cout<<"setting currentHT equal to the new one js created"<<endl;
  //update current hashtable to be this new hashtable
  currentHT = newHT;
  //  cout<<"rehash done"<<endl;

}

//print!
void printHT(Node**& hashtable, int& size){
  
  for(int i = 0; i < size; i++){

    //make GPAs prettier #thank you imanip
    cout << fixed << setprecision(2);

    //create current so last one is less messy
    Node* current;
    if(hashtable[i] != NULL){ //if theres something there print it!
      cout<<i<<" ";
      cout<<"1.  ";
      cout<< hashtable[i]->getStudent()->getName()<<" "<<
	hashtable[i]->getStudent()->getID()<< " " <<
	hashtable[i]->getStudent()->getGPA();
      if(hashtable[i]->getNext() != NULL){ //2nd one in LL
	cout<<"   2.  ";
	cout<<hashtable[i]->getNext()->getStudent()->getName()<< " "<<
	  hashtable[i]->getNext()->getStudent()->getID()<< " " <<
	  hashtable[i]->getNext()->getStudent()->getGPA();
	current = hashtable[i]->getNext();
	if(current->getNext() != NULL){ //3rd one in LL
	  cout<<"  3.  ";
	  cout<<current->getNext()->getStudent()->getName()<<" " <<
	    hashtable[i]->getNext()->getStudent()->getID()<< " " <<
	    hashtable[i]->getNext()->getStudent()->getGPA();
	}
      }
      cout<<endl;
    }
    
  }
}



//add func a little extra for this project but its left over from LL and it works dandy
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

//delete func also left over from LL but also works dandy
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



