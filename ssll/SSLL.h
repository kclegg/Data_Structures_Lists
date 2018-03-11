#ifndef SSLL_H_
#define SSLL_H_
#include <stdexcept>
#include "List.h"

namespace cop3530{

template <typename E>
class SSLL: public List<E>{

	private:
		struct Node{
			E data;
			Node* next;
		};
		Node* head = nullptr;
		Node* tail = nullptr;

	public:
		SSLL();
		~SSLL() override;

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
		class SSLL_Iter
		{
			public:
			  // type aliases required for C++ iterator compatibility
			  using value_type = T;
			  using reference = T&;
			  using pointer = T*;
			  using difference_type = std::ptrdiff_t;
			  using iterator_category = std::forward_iterator_tag;

			  // type aliases for prettier code
			  using self_type = SSLL_Iter;
			  using self_reference = SSLL_Iter&;

			private:
			  Node* here;

			public:
			  explicit SSLL_Iter(Node* start = nullptr) : here(start) {}
			  SSLL_Iter(const SSLL_Iter& src) : here(src.here) {}

			  reference operator*() const {
				  if(here == nullptr)
					  throw std::runtime_error("SSLL_Iter<T>.operator*(): iterator pointer error");
				  return here->data;
			  }
			  pointer operator->() const {
				  if(here == nullptr)
					  throw std::runtime_error("SSLL_Iter<T>.operator->(): iterator pointer error");
				  return &(here->data);
			  }
			  self_reference operator=( SSLL_Iter<T> const& src ) {
				  here=src.here;
				  return *this;
			  }
			  // preincrement
			  self_reference operator++() {
				  if(here == nullptr)
				  	throw std::runtime_error("SSLL_Iter<T>.operator++(): iterator pointer error");
				  here = here->next;
				  return *this;
			  }
			  // postincrement
			  self_type operator++(int) {
				  if(here == nullptr)
				  	throw std::runtime_error("SSLL_Iter<T>.operator++(int): iterator pointer error");
				  self_type val = *this;
				  here = here->next;
				  return val;
			  }

			  bool operator==( SSLL_Iter<T> const& rhs ) const {
				  if(here == rhs.here)
					  return true;
				  return false;
			  }
			  bool operator!=( SSLL_Iter<T> const& rhs) const {
				  if(here != rhs.here)
					  return true;
				  return false;
			  }
		};

		using value_type = E;
		using iterator = SSLL_Iter<E>;
		using const_iterator = SSLL_Iter<E const>;

		iterator begin() { return SSLL_Iter<E>(head); }
		iterator end() { return SSLL_Iter<E>(tail); }

		const_iterator begin() const { return SSLL_Iter<E>(head); }
		const_iterator end() const { return SSLL_Iter<E>(tail); }
};

template <typename E>
SSLL<E>::SSLL(){}

template <typename E>
SSLL<E>::~SSLL(){
	clear();
}

template <typename E>
void
SSLL<E>::insert(E element, size_t position){
	if(is_full())
		throw std::runtime_error("SSLL<E>.insert(): full list");
	else if(position < 0 || position > length())
		throw std::runtime_error("SSLL<E>.insert(): position does not exist");
	else{
		Node* tempNode = new Node();	// Create our new front node to be pushed
		Node* tempHead = head;			// Create a temp node to store head
		size_t loop = 1;
		tempNode->data = element;		// set Nodes data value to element parameter

		if(position == 0){				// insert new node as head of list
			push_front(element);
		}
		else if(position == length()){	// insert new node as tail of list
			push_back(element);
		}
		else{
			while(tempHead && loop > 0){					// start at head of list
				if(loop != position){
					tempHead = tempHead->next;	// continue to loop until appropriate node is reached
				}
				else{
					tempNode->next = tempHead->next;	// set the value of the next node to value of the next head node
					tempHead->next = tempNode;			// adjust the value of previous spot for insertion
					loop = -1;							// break out of loop
				}
				loop++;
			}
		}
	}
}

template <typename E>
void
SSLL<E>::push_back(E element){
	if(is_full())
		throw std::runtime_error("SSLL<E>.push_back(): full list");
	else{
		Node* tempNode = new Node();	// Create our new tail node to be pushed
		tempNode->data = element;		// set data value = element parameter
		if(head != nullptr){			// if a head node exists
			tail->next = tempNode;		// set next value = the current tail
			tail = tempNode;			// set head = our new front node
		}
		else{							// set the head/tail = only value in linked list
			head = tempNode;
			tail = tempNode;
			tail->next = nullptr;
		}
	}
}

template <typename E>
void
SSLL<E>::push_front(E element){
	if(is_full())
		throw std::runtime_error("SSLL<E>.push_front(): full list");
	else{
		Node* tempNode = new Node();	// Create our new front node to be pushed
		tempNode->data = element;		// set data value = element parameter
		if(head != nullptr){			// if a head node exists
			tempNode->next = head;		// set next value = the current head
			head = tempNode;			// set head = our new front node
		}
		else{							// set the head/tail = only value in linked list
			head = tempNode;
			tail = tempNode;
			head->next = nullptr;
		}
	}
}

template <typename E>
E
SSLL<E>::replace(E element, size_t position){
	if(is_empty())
		throw std::runtime_error("SSLL<E>.replace(): empty list");
	else if(position < 0 || position > (length()-1)){
		throw std::runtime_error("SSLL<E>.replace(): position does not exist");
	}
	else{
		Node* tempHead = head;					// Create a temp node to start at head

		for(size_t i = 0; i < position; i++){	// loop to node we want to replace
			tempHead = tempHead->next;
		}
		E val = tempHead->data;					// store current value of node in 'val'
		tempHead->data = element;				// set that node to new element value
		return val;
	}
}

template <typename E>
E
SSLL<E>::remove(size_t position){
	if(is_empty())
		throw std::runtime_error("SSLL<E>.remove(): empty list");
	else if((length()-1) < position || position < 0)
		throw std::runtime_error("SSLL<E>.remove(): position doesn't exist");
	else{
		if(position == 0){
			E pop = pop_front();
			return pop;
		}
		else if(position == length()-1){
			E pop = pop_back();
			return pop;
		}
		else{
			Node* tempNode = new Node();
			Node* tempHead = head;

			for(size_t i = 0; i < position-1; i++){		// loop to node just before the node we want to remove
				tempHead = tempHead->next;
			}
			tempNode = tempHead->next;
			E val = tempNode->data;
			tempHead->next = tempHead->next->next;
			delete tempNode;
			tail->next = nullptr;
			return val;
		}
	}
}

template <typename E>
E
SSLL<E>::pop_back(){
	if(is_empty())
		throw std::runtime_error("SSLL<E>.pop_back(): empty list");
	else{
		if(length() == 1){
			Node* tempHead = head;
			E val = tempHead->data;
			delete head;
			head = nullptr;
			tail = nullptr;
			return val;
		}
		else{
			Node* tempHead = head;
			Node* tempTail = tail;
			E val = tempTail->data;

			for(size_t i = 0; i < (length()-2); i++){	// loop to node we want to be our new tail
				tempHead = tempHead->next;
			}

			delete tail;			// delete current tail node
			tail = tempHead;		// set tail to our new last node
			tail->next = nullptr;	// set node past tail = null pointer
			return val;				// return deleted tail node value
		}
	}
}

template <typename E>
E
SSLL<E>::pop_front(){
	if(is_empty())
		throw std::runtime_error("SSLL<E>.pop_front(): empty list");
	else{
		Node* tempHead = head;
		head = head->next;
		return tempHead->data;
	}
}

template <typename E>
E
SSLL<E>::item_at(size_t position){

	if(is_empty())
		throw std::runtime_error("SSLL<E>.item_at(): empty list");
	else if((length()-1) < position || position < 0)
		throw std::runtime_error("SSLL<E>.item_at(): position doesn't exist");
	else{
		Node* tempNode = head;
		for(size_t i = 0; i < position; i++){
			tempNode = tempNode->next;
		}
		return tempNode->data;
	}
}

template <typename E>
E
SSLL<E>::peek_back(){
	if(is_empty())
		throw std::runtime_error("SSLL<E>.peek_back(): empty list");
	return tail->data;
}

template <typename E>
E
SSLL<E>::peek_front(){
	if(is_empty())
		throw std::runtime_error("SSLL<E>.peek_front(): empty list");
	return head->data;
}

template <typename E>
bool
SSLL<E>::is_empty(){
	if(head == nullptr)
		return true;
	return false;
}

template <typename E>
bool
SSLL<E>::is_full(){
	return false;		// an SSLL will never be full since it does not have a capacity
}

template <typename E>
size_t
SSLL<E>::length(){
	size_t num = 0;
	if(!is_empty()){
		Node* tempHead = head;		// start at head
		while(tempHead){			// while an element exists keep incrementing num
			num++;
			tempHead = tempHead->next;
		}
	}
	return num;
}

template <typename E>
void
SSLL<E>::clear(){
	Node* tempHead = head;
	while(tempHead){
		head = head->next;	// set head node equal to next node
		delete tempHead;	// delete previous head node
		tempHead = head;	// reset temp variable to the new head
	}
	head = nullptr;
}

template <typename E>
bool
SSLL<E>::contains(const E& element, equals_function func){
	if(is_empty()){
		throw std::runtime_error("SSLL<E>.contains(): empty list");
	}
	else{
		Node* tempNode = head;
		tail->next = nullptr;
		while(tempNode != nullptr){
			if(func(tempNode->data, element))
				return true;
			tempNode = tempNode->next;
		}
		return false;
	}
}

template <typename E>
std::ostream&
SSLL<E>::print(std::ostream& ostream){
	if(is_empty()){throw std::runtime_error("SSLL<E>.print(): empty list");}

	Node* tempNode = head;
	ostream << "[";
	while(tempNode != nullptr){
		ostream << tempNode->data;
		tempNode = tempNode->next;

		if(tempNode != nullptr){ostream << ",";}	// add a comma unless its the last element
		else{ostream << "]";}
	}
	return ostream;
}

template <typename E>
E* const
SSLL<E>::contents(){
	if(is_empty()){throw std::runtime_error("SSLL<E>.contents(): empty list");}
	else{
		E* const array = new E[length()];
		Node* tempNode = head;
		tail->next = nullptr;
		size_t loop = 0;
		while(tempNode != nullptr){
			array[loop] = tempNode->data;
			tempNode = tempNode->next;
			loop++;
		}
		return array;
	}
}

}
#endif /* SSLL_H_ */
