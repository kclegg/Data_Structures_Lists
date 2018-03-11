// 1) insert	-> poolsize--
// 2) push_front-> poolsize--
// 3) push_back -> poolsize--

// 4) pop_front -> poolsize++
// 5) pop_back	-> poolsize++
// 6) remove	-> poolsize++

#ifndef PSLL_H_
#define PSLL_H_
#include <stdexcept>
#include "List.h"

namespace cop3530{

template <typename E>
class PSLL: public List<E>{

	private:
		struct Node{
			E data;
			Node* next;
		};
		Node* head = nullptr;
		Node* tail = nullptr;
		Node* unused = nullptr;
		size_t poolsize = 0;
		void reducePoolSize();

	public:
		PSLL();
		~PSLL() override;

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
		class PSLL_Iter
		{
			public:
			  // type aliases required for C++ iterator compatibility
			  using value_type = T;
			  using reference = T&;
			  using pointer = T*;
			  using difference_type = std::ptrdiff_t;
			  using iterator_category = std::forward_iterator_tag;

			  // type aliases for prettier code
			  using self_type = PSLL_Iter;
			  using self_reference = PSLL_Iter&;

			private:
			  Node* here;

			public:
			  explicit PSLL_Iter(Node* start = nullptr) : here(start) {}
			  PSLL_Iter(const PSLL_Iter& src) : here(src.here) {}

			  reference operator*() const {
				  if(here == nullptr)
					  throw std::runtime_error("PSLL_Iter<T>.operator*(): iterator pointer error");
				  return here->data;
			  }
			  pointer operator->() const {
				  if(here == nullptr)
					  throw std::runtime_error("PSLL_Iter<T>.operator*(): iterator pointer error");
				  return &(here->data);
			  }
			  self_reference operator=( PSLL_Iter<T> const& src ) {
				  here=src.here;
				  return *this;
			  }
			  // preincrement
			  self_reference operator++() {
				  if(here == nullptr)
				  	throw std::runtime_error("PSLL_Iter<T>.operator++(): iterator pointer error");
				  here = here->next;
				  return *this;
			  }
			  // postincrement
			  self_type operator++(int) {
				  if(here == nullptr)
				  	throw std::runtime_error("PSLL_Iter<T>.operator++(int): iterator pointer error");
				  self_type val = *this;
				  here = here->next;
				  return val;
			  }

			  bool operator==( PSLL_Iter<T> const& rhs ) const {
				  if(here == rhs.here)
					  return true;
				  return false;
			  }
			  bool operator!=( PSLL_Iter<T> const& rhs) const {
				  if(here != rhs.here)
					  return true;
				  return false;
			  }
		};

		using value_type = E;
		using iterator = PSLL_Iter<E>;
		using const_iterator = PSLL_Iter<E const>;

		iterator begin() { return PSLL_Iter<E>(head); }
		iterator end() { return PSLL_Iter<E>(tail); }

		const_iterator begin() const { return PSLL_Iter<E>(head); }
		const_iterator end() const { return PSLL_Iter<E>(tail); }
};

template <typename E>
PSLL<E>::PSLL(){}

template <typename E>
PSLL<E>::~PSLL(){
	clear();
}

template <typename E>
void
PSLL<E>::reducePoolSize(){
	size_t listsize = length();
	int loop = poolsize - (listsize/2);

	if(listsize >= 100 && poolsize > (listsize/2)){
		for(int i = 0; i < loop; i++){
			Node* tempNode1 = unused;
			Node* tempNode2 = nullptr;
			poolsize--;

			while(tempNode1->next != nullptr){
				tempNode1 = tempNode1->next;
			}

			tempNode2 = tempNode1;
			tempNode1 = nullptr;

			delete tempNode2;
		}
	}
}

template <typename E>
void
PSLL<E>::insert(E element, size_t position){
	if(is_full())
		throw std::runtime_error("PSLL<E>.insert(): full list");
	else if(position < 0 || position > length())
		throw std::runtime_error("PSLL<E>.insert(): position does not exist");
	else{
		Node* tempNode;					// Create our new front node to be pushed
		Node* tempHead = head;			// Create a temp node to store head
		size_t loop = 1;

		if(unused != nullptr){
			tempNode = unused;
			unused = unused->next;
			unused->next = nullptr;
			poolsize--;
		}
		else{
			tempNode = new Node();
		}

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
PSLL<E>::push_back(E element){
	if(is_full())
		throw std::runtime_error("PSLL<E>.push_back(): full list");
	else{
		Node* tempNode;
		if(unused != nullptr){
			tempNode = unused;
			unused = unused->next;
			tempNode->next = nullptr;
			poolsize--;
		}
		else{
			tempNode = new Node();
		}

		tempNode->data = element;

		if(head != nullptr){
			tail->next = tempNode;
			tail = tempNode;
		}
		else{
			head = tempNode;
			tail = tempNode;
			tail->next = nullptr;
		}
	}
}

template <typename E>
void
PSLL<E>::push_front(E element){
	if(is_full())
		throw std::runtime_error("PSLL<E>.push_front(): full list");
	else{
		Node* tempNode;
		if(unused != nullptr){
			tempNode = unused;
			unused = unused->next;
			tempNode->next = nullptr;
			poolsize--;
		}
		else{						// a pool node doesn't exist
			tempNode = new Node();
		}

		tempNode->data = element;

		if(head != nullptr){
			tempNode->next = head;
			head = tempNode;
		}
		else{
			head = tempNode;
			tail = tempNode;
			tail->next = nullptr;
		}
	}
}

template <typename E>				// NO CHANGE NEEDED FROM SAME METHOD OF SSLL CLASS
E
PSLL<E>::replace(E element, size_t position){
	if(is_empty())
		throw std::runtime_error("PSLL<E>.replace(): empty list");
	else if(position < 0 || position > (length()-1)){
		throw std::runtime_error("PSLL<E>.replace(): position does not exist");
	}
	else{
		Node* tempHead = head;

		for(size_t i = 0; i < position; i++){
			tempHead = tempHead->next;
		}
		E val = tempHead->data;
		tempHead->data = element;
		return val;
	}
}

template <typename E>
E
PSLL<E>::remove(size_t position){
	if(is_empty())
		throw std::runtime_error("PSLL<E>.remove(): empty list");
	else if((length()-1) < position || position < 0)
		throw std::runtime_error("PSLL<E>.remove(): position doesn't exist");
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

			for(size_t i = 0; i < position-1; i++){
				tempHead = tempHead->next;
			}
			tempNode = tempHead->next;
			E val = tempNode->data;
			tempHead->next = tempHead->next->next;

			if(unused == nullptr){
				unused = tempNode;
				tempNode->next = nullptr;
			}
			else{
				Node* tempUnused = unused->next;
				while(tempUnused != nullptr){
					tempUnused = tempUnused->next;
				}
				tempUnused->next = tempNode;
				tempNode->next = nullptr;
			}

			delete tempNode;
			tail->next = nullptr;
			poolsize++;						// pool size will always increase if removing correct element
			reducePoolSize();				// reduce if conditions in method are met
			return val;
		}
	}
}

template <typename E>
E
PSLL<E>::pop_back(){
	if(is_empty())
		throw std::runtime_error("PSLL<E>.pop_back(): empty list");
	else{
		Node* tempHead = head;
		Node* tempTail = tail;
		Node* tempUnused = unused;

		if(length() == 1){
 			E val = tempHead->data;

			if(unused != nullptr){
				unused = tempHead;
				tempHead->next = tempUnused;
			}
			else{
				unused = tempHead;
			}

			if(tempTail != nullptr){tempUnused->next = tempTail;}
			//delete tempHead;

			head = nullptr;
			tail = nullptr;
			poolsize++;						// pool size will always increase if removing tail that exists
			reducePoolSize();				// reduce if conditions in method are met
			return val;
		}
		else{
			E val = tempTail->data;

			while(tempHead->next->next != nullptr){
				tempHead = tempHead->next;
			}

			if(unused != nullptr){
				unused = tempTail;
				tempHead->next = tempUnused;
			}
			else{
				tempUnused = tail;
			}

			if(tempTail != nullptr){tempUnused->next = tempTail;}
			tempTail->next = nullptr;
			tail = tempHead;
			poolsize++;						// pool size will always increase if removing tail that exists
			reducePoolSize();				// reduce if conditions in method are met
			return val;
		}
	}
}

template <typename E>
E
PSLL<E>::pop_front(){
	if(is_empty())
		throw std::runtime_error("PSLL<E>.pop_front(): empty list");
	else{
		E val = head->data;
		Node* tempHead = head;

		if(length() == 1){
			head = nullptr;
			tail = nullptr;
		}
		else{
			head = head->next;
		}

		if(unused != nullptr){
			unused = tempHead;
			tempHead->next = unused;
		}
		else{
			unused->next = tempHead;
			tempHead->next = nullptr;
		}

		poolsize++;						// pool size will always increase if removing tail that exists
		reducePoolSize();				// reduce if conditions in method are met
		return val;
	}
}

template <typename E>
E
PSLL<E>::item_at(size_t position){

	if(is_empty())
		throw std::runtime_error("PSLL<E>.item_at(): empty list");
	else if((length()-1) < position || position < 0)
		throw std::runtime_error("PSLL<E>.item_at(): position doesn't exist");
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
PSLL<E>::peek_back(){
	if(is_empty())
		throw std::runtime_error("PSLL<E>.peek_back(): empty list");
	return tail->data;
}

template <typename E>
E
PSLL<E>::peek_front(){
	if(is_empty())
		throw std::runtime_error("PSLL<E>.peek_front(): empty list");
	return head->data;
}

template <typename E>
bool
PSLL<E>::is_empty(){
	if(head == nullptr)
		return true;
	return false;
}

template <typename E>
bool
PSLL<E>::is_full(){
	return false;
}

template <typename E>
size_t
PSLL<E>::length(){
	size_t num = 0;
	if(!is_empty()){
		Node* tempHead = head;
		while(tempHead){
			num++;
			tempHead = tempHead->next;
		}
	}
	return num;
}

template <typename E>
void
PSLL<E>::clear(){
	Node* tempHead = head;
	Node* tempUnused = unused;
	while(tempHead){
		head = head->next;	// set head node equal to next node
		delete tempHead;	// delete previous head node
		tempHead = head;	// reset temp variable to the new head
	}
	head = nullptr;
	while(unused){
		unused = unused->next;	// set head node equal to next node
		delete tempUnused;	// delete previous head node
		tempUnused = unused;	// reset temp variable to the new head
	}
	unused = nullptr;
}

template <typename E>
bool
PSLL<E>::contains(const E& element, equals_function func){
	if(is_empty()){
		throw std::runtime_error("PSLL<E>.contains(): empty list");
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
PSLL<E>::print(std::ostream& ostream){
	if(is_empty()){throw std::runtime_error("PSLL<E>.print(): empty list");}

	Node* tempNode = head;
	ostream << "[";
	while(tempNode != nullptr){
		ostream << tempNode->data;
		tempNode = tempNode->next;

		if(tempNode != nullptr){ostream << ",";}
		else{ostream << "]";}
	}
	return ostream;
}

template <typename E>
E* const
PSLL<E>::contents(){
	if(is_empty()){throw std::runtime_error("PSLL<E>.contents(): empty list");}
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
#endif /* PSLL_H_ */
