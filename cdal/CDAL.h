#ifndef CDAL_H_
#define CDAL_H_
#include <stdexcept>
#include "List.h"

namespace cop3530{

template <typename E>
class CDAL: public List<E>{

	private:
		struct Node{
			Node* next;
			E* CDAL_list;
			size_t tailPosition;

			Node(){
				CDAL_list = new E[50];
				tailPosition = 0;
				next = nullptr;
			}
		};

		Node* head;
		void deallocateArray();

	public:
		CDAL();
		CDAL(size_t cap);
		~CDAL() override;

		void insert(E element, size_t position) override;
		void push_back(E element) override;
		void push_front(E element) override;
		E replace(E element, size_t position) override;
		E remove(size_t position) override;
		E pop_back() override;
		E pop_front() override;
		E item_at(size_t position) override;
		E peek_back() override;
		E peek_front() override;
		bool is_empty() override;
		bool is_full() override;
		size_t length() override;
		void clear() override;
		typedef bool (*equals_function)(const E&, const E&);
		bool contains(const E& element, equals_function func) override;
		std::ostream& print(std::ostream& ostream);
		E* const contents() override;

		template <typename T>
		class CDAL_Iter
		{
			public:
			  // type aliases required for C++ iterator compatibility
			  using value_type = T;
			  using reference = T&;
			  using pointer = T*;
			  using difference_type = std::ptrdiff_t;
			  using iterator_category = std::forward_iterator_tag;

			  // type aliases for prettier code
			  using self_type = CDAL_Iter;
			  using self_reference = CDAL_Iter&;

			private:
			  T* here;

			public:
			  explicit CDAL_Iter(T* start = nullptr) : here(start) {}
			  CDAL_Iter(const CDAL_Iter& src) : here(src.here) {}

			  reference operator*() const {
				  if(here == nullptr)
					  throw std::runtime_error("CDAL_Iter<T>.operator*(): iterator pointer error");
				  return here->data;
			  }
			  pointer operator->() const {
				  if(here == nullptr)
				  	throw std::runtime_error("CDAL_Iter<T>.operator*(): iterator pointer error");
				  return &(here->data);
			  }

			  self_reference operator=( CDAL_Iter<T> const& src ) {
				  here=src.here;
				  return *this;
			  }

			  // preincrement
			  self_reference operator++() {
				  if(here == nullptr)
					  throw std::runtime_error("CDAL_Iter<T>.operator++(): iterator pointer error");
				  here = here->next;
				  return *this;
			  }

			  // postincrement
			  self_type operator++(int) {
				  if(here == nullptr)
					  throw std::runtime_error("CDAL_Iter<T>.operator++(int): iterator pointer error");
				  self_type val = *this;
				  here = here->next;
				  return val;
			  }

			  bool operator==( CDAL_Iter<T> const& rhs ) const {
				  if(here == rhs.here)
					  return true;
				  return false;
			  }
			  bool operator!=( CDAL_Iter<T> const& rhs) const {
				  if(here != rhs.here)
					  return true;
				  return false;
			  }
		};

		using value_type = E;
		using iterator = CDAL_Iter<E>;
		using const_iterator = CDAL_Iter<E const>;

		iterator begin() { return CDAL_Iter<E>(head); }
		iterator end() { return CDAL_Iter<E>(head); }

		const_iterator begin() const { return CDAL_Iter<E>(head); }
		const_iterator end() const { return CDAL_Iter<E>(head); }
};

template <typename E>
CDAL<E>::CDAL(){
	head = new Node();
	head->tailPosition = -1;
}

template <typename E>
CDAL<E>::~CDAL(){
	clear();
}

template <typename E>
void
CDAL<E>::deallocateArray(){
	if(is_empty()){throw std::runtime_error("CDAL<E>.deallocateArray(): empty list");}
	else{
		Node* tempHead = head;
		size_t tail;
		size_t numEmpty = 0;
		size_t numLists = 0;
		while(tempHead){
			tail = tempHead->tailPosition;
			numLists++;
			if(tail == -1){
				numEmpty++;
			}
			tempHead = tempHead->next;
		}

		if((numLists/2) < numEmpty){
			size_t len = 0;
			for(unsigned int i = 0; i < (numEmpty/2); i++){
				tempHead = head;
				tail = tempHead->tailPosition;

				for(unsigned int j = i; j < numLists; j++){
					tempHead = tempHead->next;
					tail = tempHead->tailPosition;
				}

				if(tail == -1){
					delete[] tempHead->CDAL_list;
					delete tempHead;
				}
			}
		}
	}
}

template <typename E>
void
CDAL<E>::insert(E element, size_t position){
	if(position < 0 || position > length())
		throw std::runtime_error("CDAL<E>.insert(): position does not exist");
	else if(is_empty()){
		if(!head){
			head = new Node();
		}
		head->CDAL_list[0] = element;
		head->tailPosition = 0;
	}
	else{
		if(position == 0){
			push_front(element);
		}
		else if(position == length()){
			push_back(element);
		}
		else{
			Node* tempHead = head;
			bool loop = true;
			while(loop){				// first loop to the last node
				if(tempHead->next && tempHead->next->tailPosition!=-1){
					tempHead = tempHead->next;
				}
				else{
					loop = false;
				}
			}
			size_t tail = tempHead->tailPosition;

			if(tail == 49){						// only checks for current last node
				Node* newNode = new Node();
				newNode->CDAL_list[0] = tempHead->CDAL_list[49];
				newNode->tailPosition = 0;
				tempHead->next = newNode;
			}
			else{
				tempHead->CDAL_list[tail+1] = tempHead->CDAL_list[tail];
				tempHead->tailPosition = (tail+1);
			}

			tempHead = head;
			tail = tempHead->tailPosition;
			loop = true;

			for(unsigned int i = 49; i < position; i+=50){
				tempHead = tempHead->next;
			}
			size_t index = position;
			while(index > 49){			// adjust position index to get a value in [0,49] range
				index = index - 50;
			}

			tail = tempHead->tailPosition;

			E* tempList = new E[tail+1]; // temp store all of the elements of current node we are working on
			E* lastTempList;			// temp list for the last node (not guaranteed to be 49 elements, avoid leftovers)

			E lastVal = tempHead->CDAL_list[tail];

			for(unsigned int i = tail; i > index; i--){
				tempHead->CDAL_list[i] = tempHead->CDAL_list[i-1];
			}
			tempHead->CDAL_list[index] = element;

			if(tail != 49){tempHead->CDAL_list[tail+1] = lastVal;}
			else{
				while(loop){

					if(tempHead->next && tempHead->next->tailPosition!=-1){

						tempHead = tempHead->next;
						tail = tempHead->tailPosition;
						tempList = new E[tail+1];

						for(unsigned int i = 1; i < (tail+1); i++){
							tempList[i] = tempHead->CDAL_list[i-1];
						}
						tempList[0] = lastVal;

						if(tempHead->next && tempHead->next->tailPosition!=-1){
							lastVal = tempHead->CDAL_list[49];
							tempHead->CDAL_list = tempList;
						}
						else{
							tempHead->CDAL_list = tempList;
							loop = false;
						}
					}
					else{
						lastTempList = new E[tail+1];
						lastTempList[0] = lastVal;

						for(unsigned int i = 1; i < (tail+1); i++){
							lastTempList[i] = tempHead->CDAL_list[i-1];
						}
						tempHead->CDAL_list = lastTempList;
						loop = false;
					}
				}
			}
		}
	}
}

template <typename E>
void
CDAL<E>::push_back(E element){
	if(is_empty()){
		if(!head){
			head = new Node();
		}
		head->CDAL_list[0] = element;
		head->tailPosition = 0;
	}
	else{
		Node* tempHead = head;
		bool loop = true;
		while(loop){					// loop to last node of cdal
			if(tempHead->next && tempHead->next->tailPosition!=-1){
				tempHead = tempHead->next;
			}
			else{
				loop = false;
			}
		}
		size_t tail = tempHead->tailPosition;
		if(tail == 49){
			Node* newNode = new Node();
			newNode->CDAL_list[0] = element;
			newNode->tailPosition = 0;
			tempHead->next = newNode;
		}
		else{
			tempHead->CDAL_list[tail+1] = element;
			tempHead->tailPosition = tail+1;
		}
	}
}

template <typename E>
void
CDAL<E>::push_front(E element){
	if(is_empty()){
		if(!head){
			head = new Node();
		}
		head->CDAL_list[0] = element;
		head->tailPosition = 0;
	}
	else{
		Node* tempHead = head;
		bool loop = true;
		while(loop){				// first loop to the last node
			if(tempHead->next && tempHead->next->tailPosition!=-1){
				tempHead = tempHead->next;
			}
			else{
				loop = false;
			}
		}
		size_t tail = tempHead->tailPosition;

		if(tail == 49){						// only checks for current last node
			Node* newNode = new Node();
			newNode->CDAL_list[0] = tempHead->CDAL_list[49];
			newNode->tailPosition = 0;
			tempHead->next = newNode;
		}
		else{
			tempHead->CDAL_list[tail+1] = tempHead->CDAL_list[tail];
			tempHead->tailPosition = (tail+1);
		}

		tempHead = head;
		tail = tempHead->tailPosition;
		loop = true;

		E* tempList = new E[tail+1]; // temp store all of the elements of current node we are working on
		E* lastTempList;			// temp list for the last node (not guaranteed to be 49 elements, avoid leftovers)
		tempList[0] = element;

		while(loop){

			if(tempHead->next && tempHead->next->tailPosition!=-1){
				E lastElement = tempHead->CDAL_list[49];

				for(unsigned int i = 1; i < (tail+1); i++){
					tempList[i] = tempHead->CDAL_list[i-1];
				}
				tempHead->CDAL_list = tempList;

				tempHead = tempHead->next;
				tail = tempHead->tailPosition;
				tempList = new E[tail+1];
				tempList[0] = lastElement;
			}
			else{
				lastTempList = new E[tail+1];
				lastTempList[0] = tempList[0];
				for(unsigned int i = 1; i < (tail+1); i++){
					lastTempList[i] = tempHead->CDAL_list[i-1];
				}
				tempHead->CDAL_list = lastTempList;
				loop = false;
			}
		}
	}
}

template <typename E>
E
CDAL<E>::replace(E element, size_t position){
	if(is_empty())
		throw std::runtime_error("CDAL<E>.replace(): empty list");
	else if(position < 0 || position > (length()-1)){
		throw std::runtime_error("CDAL<E>.replace(): position does not exist");
	}
	else{
		E val;
		Node* tempHead = head;

		for(unsigned int i = 49; i < position; i+=50){
			tempHead = tempHead->next;
		}
		size_t index = position;
		while(index > 49){			// adjust position index to get a value in [0,49] range
			index = index - 50;
		}
		val = tempHead->CDAL_list[index];
		tempHead->CDAL_list[index] = element;
		return val;
	}
}

template <typename E>
E
CDAL<E>::remove(size_t position){
	if(is_empty())
		throw std::runtime_error("CDAL<E>.remove(): empty list");
	else if((length()-1) < position || position < 0)
		throw std::runtime_error("CDAL<E>.remove(): position doesn't exist");
	else{
		if(position == 0){
			E pop = pop_front();
			deallocateArray();
			return pop;
		}
		else{
			E val;
			Node* tempHead = head;

			for(unsigned int i = 49; i < position; i+=50){
				tempHead = tempHead->next;
			}

			size_t tempPos = position;
			while(tempPos > 49){
				tempPos = tempPos - 50;
			}

			val = tempHead->CDAL_list[tempPos];			// element we are removing

			size_t tail;
			bool loop = true;

			while(loop){
				tail = tempHead->tailPosition;

				while(tempPos != tail){
					tempHead->CDAL_list[tempPos] = tempHead->CDAL_list[tempPos+1];
					tempPos++;
				}

				if(tempHead->next && tempHead->next->tailPosition!=-1){
					tempHead->CDAL_list[49] = tempHead->next->CDAL_list[0];
					tempHead->tailPosition = 49;
					tempHead = tempHead->next;
					tempPos = 0;
				}
				else{
					tempHead->CDAL_list[tail] = NULL;
					tempHead->tailPosition = tail-1;
					loop = false;
				}
			}
			deallocateArray();
			return val;
		}
	}
}

template <typename E>
E
CDAL<E>::pop_back(){
	if(is_empty())
		throw std::runtime_error("CDAL<E>.pop_back(): empty list");
	else{
		E val;
		Node* tempHead = head;
		bool loop = true;
		while(loop){					// loop to last node of cdal
			if(tempHead->next && tempHead->next->tailPosition!=-1){
				tempHead = tempHead->next;
			}
			else{
				loop = false;
			}
		}
		size_t tail = tempHead->tailPosition;
		val = tempHead->CDAL_list[tail];
		tempHead->CDAL_list[tail] = NULL;		// set the last element of the last node to nullptr
		tempHead->tailPosition = tail-1;
		deallocateArray();
		return val;
	}
}

template <typename E>
E
CDAL<E>::pop_front(){
	if(is_empty())
		throw std::runtime_error("CDAL<E>.pop_front(): empty list");
	else{
		E val = head->CDAL_list[0];
		Node* tempHead = head;
		size_t tail;
		size_t index = 0;
		bool loop = true;

		while(loop){
			tail = tempHead->tailPosition;
			if(tail >= 0){
				while(index < tail){
					// error below
					tempHead->CDAL_list[index] = tempHead->CDAL_list[index+1];
					index++;
				}
			}
			if(tempHead->next && tempHead->next->tailPosition!=-1){
				tempHead->CDAL_list[49] = tempHead->next->CDAL_list[0];
				tempHead->tailPosition = 49;
				tempHead = tempHead->next;
				index = 0;
			}
			else{
				tempHead->CDAL_list[tail] = NULL;
				tempHead->tailPosition = tail-1;
				loop = false;
			}
		}
		deallocateArray();
		return val;
	}
}

template <typename E>
E
CDAL<E>::item_at(size_t position){

	if(is_empty())
		throw std::runtime_error("CDAL<E>.item_at(): empty list");
	else if((length()-1) < position || position < 0)
		throw std::runtime_error("CDAL<E>.item_at(): position doesn't exist");
	else{
		Node* tempHead = head;

		for(unsigned int i = 49; i < position; i+=50){
			tempHead = tempHead->next;
		}
		size_t index = position;
		while(index > 49){
			index = index - 50;
		}
		//0-49, 50-99, 100-149, 150-199
		return tempHead->CDAL_list[index];
	}
}

template <typename E>
E
CDAL<E>::peek_back(){
	if(is_empty())
		throw std::runtime_error("CDAL<E>.peek_back(): empty list");
	else{
		Node* tempHead = head;
		bool loop = true;
		while(loop){
			if(tempHead->next && tempHead->next->tailPosition!=-1){
				tempHead = tempHead->next;
			}
			else{
				loop = false;
			}
		}
		size_t tail = tempHead->tailPosition;
		return tempHead->CDAL_list[tail];
	}
}

template <typename E>
E
CDAL<E>::peek_front(){
	if(is_empty())
		throw std::runtime_error("CDAL<E>.peek_front(): empty list");
	return head->CDAL_list[0];
}

template <typename E>
bool
CDAL<E>::is_empty(){
	if((!head) || head->tailPosition == -1){
		return true;
	}
	return false;
}

template <typename E>
bool
CDAL<E>::is_full(){
	return false;
}

template <typename E>
size_t
CDAL<E>::length(){
	if(is_empty()){return 0;}
	else{
		size_t num = 0;
		Node* tempHead = head;
		while(tempHead){
			size_t tail = tempHead->tailPosition;
			num+=(tail+1);
			tempHead = tempHead->next;
		}
		return num;
	}
	return 0;
}

template <typename E>
void
CDAL<E>::clear(){
	Node* tempHead = head;
	while(tempHead){
		head = head->next;				// set head node equal to next node
		tempHead->tailPosition = -1;
		delete[] tempHead->CDAL_list;
		delete tempHead;				// delete previous head node
		tempHead = head;				// reset temp variable to the new head
	}
	head = nullptr;
}

template <typename E>
bool
CDAL<E>::contains(const E& element, equals_function func){
	if(is_empty()){
		throw std::runtime_error("CDAL<E>.contains(): empty list");
	}
	else{
		Node* tempHead = head;
		bool loop = true;
		while(loop){
			size_t tail = tempHead->tailPosition;
			for(unsigned int i = 0; i < tail; i++){
				if(func(tempHead->CDAL_list[i], element))
					return true;
			}
			if((tempHead->next) && tempHead->next->tailPosition != -1){
				tempHead = tempHead->next;
			}
			else{loop = false;}
		}
		return false;
	}
}

template <typename E>
std::ostream&
CDAL<E>::print(std::ostream& ostream){
	if(is_empty()){throw std::runtime_error("CDAL<E>.print(): empty list");}

	Node* tempHead = head;
	ostream << "[";
	while(tempHead){
		size_t tail = tempHead->tailPosition;
		for(unsigned int i = 0; i < (tail+1); i++){
			// error below when more than 1 node
			ostream << tempHead->CDAL_list[i];

			if((tempHead->next) && tempHead->next->tailPosition != -1){ostream << ",";}	// add a comma unless its the last element
			else{if((tail) != i){ostream << ",";}}
		}
		tempHead = tempHead->next;
	}
	ostream << "]";
	return ostream;
}

template <typename E>
E* const
CDAL<E>::contents(){
	if(is_empty()){throw std::runtime_error("CDAL<E>.contents(): empty list");}
	else{
		E* const array = new E[length()];	// array of number of elements
		Node* tempHead = head;
		size_t index = 0;
		bool loop = true;
		while(loop){
			size_t tail = tempHead->tailPosition;
			for(unsigned int i = 0; i < tail+1; i++){
				array[index] = tempHead->CDAL_list[i];
				index++;
			}
			if((tempHead->next) && tempHead->next->tailPosition != -1){
				tempHead = tempHead->next;
			}
			else{loop = false;}
		}
		return array;
	}
}

}
#endif /* CDAL_H_ */
