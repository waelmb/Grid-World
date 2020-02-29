/*
* Project 2: Grid World
* CS251 Summer 2019
* Wael Abdulelah Mobeirek
**********************/

#ifndef _GRID_WORLD_H
#define _GRID_WORLD_H

#include <vector>
#include <iostream>

using std::vector;

class Person {
	private:
		int row;
		int col;
		int id;
		bool isAlive;
		//Person* address; //FIXME: setters and getters
	public:
		//constructor
		Person() {
			row = -1;
			col = -1;
			id = -1;
			isAlive = false;
		}
		Person(int r, int c, int i) {
			row = r;
			col = c;
			id = i;
			isAlive = true;
		}
		
		//destructor
		~Person() {
			
		}
		
		//relocate
		void relocate(int r, int c) {
			row = r;
			col = c;
		}
		
		//set an id
		void setId(int i) {
			id = i;
		}
		
		//set isAlive
		void setIsAlive(bool b) {
			isAlive = b;
		}
		
		//returns location
		void location(int &r, int &c) {
			r = row;
			c = col;
		}
		
		//get id
		int getId() {
			return id;
		}
		
		//get isAlive
		bool getIsAlive() {
			return isAlive;
		}
};

class MyLNode
{
 private:
  Person person;
  MyLNode* next;
  MyLNode* prev;
  
  
 public:
  MyLNode (Person per){
  	person = per;
  	next = nullptr;
  	prev = nullptr;
  }
  MyLNode (Person per, MyLNode* n, MyLNode* p) {
  	person = per;
  	next = n;
  	prev = p;
  }
  Person getPerson () {
  	return person;
  }
  MyLNode* getNext() {
  	return next;
  }
  MyLNode* getPrev() {
  	return prev;
  }
  void setPerson(Person per) {
  	person = per;
  }
  void setNext (MyLNode* n) {
  	next = n;
  }
  void setPrev (MyLNode* p) {
  	prev = p;
  }
};

class Area {
	private:
		MyLNode* head;
		MyLNode* tail;
		int numPpl;
		
	public:
		//constructor
		Area() {
			head = nullptr;
			tail = nullptr;
			numPpl = 0;
		}
		
		//Increments the number of people
		void incPpl() {
			numPpl++;
		}
		
		//Decrements the number of people
		void decPpl() {
			numPpl--;
		}
		
		//Returns the number of people in a district
		int getNumPpl() {
			return numPpl;
		}
		
		//Returns the head of the list
		MyLNode* getHead() const {
			return head;
		}
		
		//Returns the tail of the list
		MyLNode* getTail() const {
			return tail;
		}
		
		//Push a person
		void pushPerson(MyLNode* tmp) {
			//insert the node to the end of the list
			//if list is empty
			if(head == nullptr) {
				head = tmp;
				tail = tmp;
			}
			//else
			else {
				tail->setNext(tmp);
				tmp->setPrev(tail);
				tail = tmp;
			}

			//increment numPpl
			numPpl++;
		}
		
		//Pop a perosn from the front of the list
		MyLNode* popFront() {
			//store address of head
			MyLNode* tmp = head;
			
			//update head
			head = head->getNext();
			
			//update the tail if needed
			if(head == nullptr) {
				tail = nullptr;
			}
			
			//update info return the popped address
			numPpl--;
			tmp->setNext(nullptr);
			tmp->setPrev(nullptr);
			if(numPpl == 0) {
				head = nullptr;
				tail = nullptr;
			}
			
			return tmp;
		}
		
		//Pop a specific person
		void popPerson(MyLNode* person) {	
			//make prev node point to next node
			//if first element
			if(person->getPrev() == nullptr) {
				//update the head
				head = head->getNext();
				
				//update prev in the new head to make it null
				//if list isn't empty
				if(head != nullptr) {
					head->setPrev(nullptr);
				}
				//if list has become empty, update the tail
				else {
					tail = nullptr;
				}
			}
			//if last node
			else if(person->getNext() == nullptr) {
				MyLNode* tmpPrev = person->getPrev();
				
				//make the prev node point to null
				tmpPrev->setNext(nullptr);
				
				//update tail
				tail = tmpPrev;
			}
			//if in between
			else {
				MyLNode* tmpPrev = person->getPrev();
				MyLNode* tmpNext = person->getNext();

				tmpPrev->setNext(tmpNext);
				tmpNext->setPrev(tmpPrev);	
				
			}
			
			//update info
			person->setNext(nullptr);
			person->setPrev(nullptr);
			numPpl--;
			if(numPpl == 0) {
				head = nullptr;
				tail = nullptr;
			}
		}
		
		bool isEmpty() {
			if(head == nullptr && tail == nullptr) {
				return true;
			}
			else {
				return false;
			}
		}
		
};

class GridWorld {

  private:
  	Area** world;
    vector<MyLNode*> people;
    int idCap;
    int popul;
    Area dead;
    int rows;
    int cols;
    

  public:
    /**
    * constructor:  initializes a "world" with nrows and
    *    ncols (nrows*ncols districtcs) in which all 
    *    districtricts are empty (a wasteland!).
    */
    GridWorld(unsigned nrows, unsigned ncols)   {
    	//update size
		rows = nrows;
    	cols = ncols;
    	
    	//create grid
    	world = new Area*[cols];
    	for(int i = 0; i < cols; i++) {
    		world[i] = new Area[rows];
		}
		
		//initialize other variables
		idCap = 0;
		popul = 0;
    }
    
    ~GridWorld(){
        //FIXME: needs 
        
        //delete all areas
    	for(int i = 0; i < cols; i++) {
    		delete[] world[i];
		}
		delete[] world;
		
		//delete all nodes
		for(int i = 0; i < people.size(); i++) {
			delete people.at(i);
		}
    }
    
	/* function: isPersonAlive
	 * desc: determins if a given person id is alive in constant time
	 * returns: true or false
	 * status: DONE
	 */
	bool isPersonAlive(int id)const {
		if(id < idCap && id >= 0) {
			MyLNode* tmp = people.at(id);
			Person p = tmp->getPerson();
			return p.getIsAlive();
		}
		else {
			return false;
		}
	}
    /*
     * function: birth
     * description:  if row/col is valid, a new person is created
     *   with an ID according to rules in handout.  New person is
     *   placed in district (row, col)
     *
     * return:  indicates success/failure
     * Status: DONE.
     */
    bool birth(int row, int col, int &id){
    	MyLNode* tmp;
    	//Determine the appropriate id
    	if(dead.isEmpty()){ 
    		//assign according to numPeople
    		id = idCap;
    		
    		//Create person
			Person newPerson;
			newPerson.relocate(row, col);
			newPerson.setId(id);
			newPerson.setIsAlive(true);
		
    		//create a new node and push it
    		tmp = new MyLNode(newPerson);
    		
    		//add person to the people's vector
    		people.push_back(tmp);
    		
    		//update counters
			idCap++;
		}
		else {
			//get oldest unused id
			tmp = dead.popFront();
			
			//update id to pass back to calling function
			Person p = tmp->getPerson();
			id = p.getId();
			
			//update person's location and isAlive
			p.setIsAlive(true);
			p.relocate(row, col);
			tmp->setPerson(p);
		}
		
		//Add person to the specified area 
		world[row][col].pushPerson(tmp);
		
		//update counters
		popul++;
		
		return true;
    }

    /*
     * function: death 
     * description:  if given person is alive, person is killed and
     *   data structures updated to reflect this change.
     *
     * return:  indicates success/failure
     * Status: DONE.
     */
    bool death(int personID){
		if(isPersonAlive(personID)) {
			MyLNode* tmp = people.at(personID);
			Person p = tmp->getPerson();
			
			//Person is alive, "kill'em"
			p.setIsAlive(false);
			
			//get location
			int row = -1;
			int col = -1;
			p.location(row, col);
			
			//remove person from the area
			world[row][col].popPerson(tmp);
			
			//set location to default values
			row = -1;
			col = -1;
			p.relocate(row, col);
			
			//update person at tmp
			tmp->setPerson(p);
			
			//add to the dead
			dead.pushPerson(tmp);
			
			//update counters
			popul--;
			
			//return true
			return true;
		}
		else {
			return false;
		}
    }

    /*
     * function: whereis
     * description:  if given person is alive, his/her current residence
     *   is reported via reference parameters row and col.
     *
     * return:  indicates success/failure
     * Status: DONE.
     */
    bool whereis(int id, int &row, int &col)const{
    	if(isPersonAlive(id)) {
			MyLNode* tmp = people.at(id);
			Person p = tmp->getPerson();
			
			//get location
			int r = -1;
			int c = -1;
			p.location(r, c);
			row = r;
			col = c;
			
			//return true
			return true;
		}
		else {
			return false;
		}
    }

    /*
     * function: move
     * description:  if given person is alive, and specified target-row
     *   and column are valid, person is moved to specified district and
     *   data structures updated accordingly.
     *
     * return:  indicates success/failure
     *
     * comment/note:  the specified person becomes the 'newest' member
     *   of target district (least seniority) --  see requirements of members().
     *
     * Status: DONE.
     */
    bool move(int id, int targetRow, int targetCol){
    	if(isPersonAlive(id)) {
			MyLNode* tmp = people.at(id);
			Person p = tmp->getPerson();
			
			//get location
			int row = -1;
			int col = -1;
			p.location(row, col);
			
			//remove person from current area
			world[row][col].popPerson(tmp);
			
			//update person's location
			p.relocate(targetRow, targetCol);
			tmp->setPerson(p);
			
			//push person to new area
			world[targetRow][targetCol].pushPerson(tmp);
			
			//return true
			return true;
		}
		else {
			return false;
		}
    }

	/*
	* Status: DONE.
	*/
    std::vector<int> * members(int row, int col)const{
    	//allocate new vector
		vector<int>* membArea = new vector<int>();
		
    	//print the dead list
		if(row == -999 && col == -999) {
			MyLNode* hd = dead.getHead();
      		
			//traverse the list
			while(hd != nullptr) {
				//get the IDs of ppl in the district
				Person p = hd->getPerson();
				int id = p.getId();
				membArea->push_back(id);
				
				//advance to the next node
				hd = hd->getNext();
			}
		}
		//make sure row and col are in range
		else if(row < rows && row >= 0 && col < cols && col >= 0) {
      		MyLNode* hd = world[row][col].getHead();
      		
			//traverse the list
			while(hd != nullptr) {
				//get the IDs of ppl in the district
				Person p = hd->getPerson();
				int id = p.getId();
				membArea->push_back(id);
				
				//advance to the next node
				hd = hd->getNext();
			}
			
		}
		
		//return the pointer to the vector
		return membArea;
    }

    /*
     * function: population
     * description:  returns the current (living) population of the world.
     * Status: DONE.
     */
    int population()const{
      return popul;
    }
    
    /*
     * function: population(int,int)
     * description:  returns the current (living) population of specified
     *   district.  If district does not exist, zero is returned
     * Status: DONE.
     */
    int population(int row, int col)const{
      if(row < rows && row >= 0 && col < cols && col >= 0) {
      	return world[row][col].getNumPpl();
	  }
	  else {
	  	return -1;
	  }
    }

    /*
     * function: num_rows
     * description:  returns number of rows in world
     * Status: DONE.
     */
    int num_rows()const {
      return rows;
    }

    /*
     * function: num_cols
     * description:  returns number of columns in world
     * Status: DONE.
     */
    int num_cols()const {
      return cols;
    }



};

#endif
