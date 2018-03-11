#ifndef SDAL_H_
#define SDAL_H_
#include "List.h"
#include <stdexcept>

namespace cop3530{

template <typename E>
class SDAL: public List<E>{

	private:
		E* SDAL_list;
		size_t capacity;
		size_t tailPosition;
		void decreaseArray();
		void increaseArray();

	public:
		SDAL();
		SDAL(size_t cap);
		~SDAL() override;

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
		class SDAL_Iter
		{
			public:
			  // type aliases required for C++ iterator compatibility
			  using value_type = T;
			  using reference = T&;
			  using pointer = T*;
			  using difference_type = std::ptrdiff_t;
			  using iterator_category = std::forward_iterator_tag;

			  // type aliases for prettier code
			  using self_type = SDAL_Iter;
			  using self_reference = SDAL_Iter&;

			private:
			  T* here;

			public:
			  explicit SDAL_Iter(T* start = nullptr) : here(start) {}
			  SDAL_Iter(const SDAL_Iter& src) : here(src.here) {}

			  reference operator*() const {
				  if(here == nullptr)
					  throw std::runtime_error("SDAL_Iter<T>.operator*(): iterator pointer error");
				  return here->data;
			  }
			  pointer operator->() const {
				  if(here == nullptr)
				  	throw std::runtime_error("SDAL_Iter<T>.operator*(): iterator pointer error");
				  return &(here->data);
			  }

			  self_reference operator=( SDAL_Iter<T> const& src ) {
				  here=src.here;
				  return *this;
			  }

			  // preincrement
			  self_reference operator++() {
				  if(here == nullptr)
					  throw std::runtime_error("SDAL_Iter<T>.operator++(): iterator pointer error");
				  here = here->next;
				  return *this;
			  }

			  // postincrement
			  self_type operator++(int) {
				  if(here == nullptr)
					  throw std::runtime_error("SDAL_Iter<T>.operator++(int): iterator pointer error");
				  self_type val = *this;
				  here = here->next;
				  return val;
			  }

			  bool operator==( SDAL_Iter<T> const& rhs ) const {
				  if(here == rhs.here)
				  	return true;
				  return false;
			  }
			  bool operator!=( SDAL_Iter<T> const& rhs) const {
				  if(here != rhs.here)
					  return true;
				  return false;
			  }
		};

		using value_type = E;
		using iterator = SDAL_Iter<E>;
		using const_iterator = SDAL_Iter<E const>;

		iterator begin() { return SDAL_Iter<E>(SDAL_list[0]); }
		iterator end() { return SDAL_Iter<E>(SDAL_list[tailPosition]); }

		const_iterator begin() const { return SDAL_Iter<E>(SDAL_list[0]); }
		const_iterator end() const { return SDAL_Iter<E>(SDAL_list[tailPosition]); }
};

template <typename E>
SDAL<E>::SDAL(){
	capacity = 50;
	SDAL_list = new E[50];
	tailPosition = -1;
}

template <typename E>
SDAL<E>::~SDAL(){
	clear();
}

template <typename E>
SDAL<E>::SDAL(size_t cap){
	capacity = cap;
	SDAL_list = new E[capacity];
	tailPosition = -1;
}

template <typename E>
void
SDAL<E>::increaseArray(){
	if(is_full()){
		size_t newlength = capacity*1.5;
		E* tempArray = new E[newlength];
		for(unsigned int i = 0; i < length(); i++){
			tempArray[i] = SDAL_list[i];
		}
		delete[] SDAL_list;
		capacity = newlength;
		SDAL_list = tempArray;
	}
}

template <typename E>
void
SDAL<E>::decreaseArray(){
	if(capacity >= 100 && (capacity/2) > length()){
		size_t newlength = capacity*0.75;
		E* tempArray = new E[newlength];
		for(unsigned int i = 0; i < length(); i++){
			tempArray[i] = SDAL_list[i];
		}
		delete[] SDAL_list;
		capacity = newlength;
		SDAL_list = tempArray;
	}
}

template <typename E>
void
SDAL<E>::insert(E element, size_t position){
	if(position < 0 || position > length())
		throw std::runtime_error("SDAL<E>.insert(): position does not exist");
	else if(is_full()){
		throw std::runtime_error("SDAL<E>.insert(): full list");
	}
	else{
		if(position == 0){
			push_front(element);
		}
		else if(position == length()){
			push_back(element);
		}
		else{
			for(unsigned int i = length(); i > position; i--){
				SDAL_list[i] = SDAL_list[i-1];
			}
			SDAL_list[position] = element;
			increaseArray();
			tailPosition++;
		}
	}
}

template <typename E>
void
SDAL<E>::push_back(E element){
	if(is_empty()){
		SDAL_list[0] = element;
	}
	else if(is_full()){
		throw std::runtime_error("SDAL<E>.push_back(): full list");
	}
	else{
		SDAL_list[length()] = element;
	}
	tailPosition++;
	increaseArray();
}

template <typename E>
void
SDAL<E>::push_front(E element){
	if(is_empty()){
		SDAL_list[0] = element;
	}
	else if(is_full()){
		throw std::runtime_error("SDAL<E>.push_front(): full list");
	}
	else{
		for(int i = length(); i > 0; i--){
			SDAL_list[i] = SDAL_list[i-1];	// start from back of the list and push existing values forward
		}
		SDAL_list[0] = element;
	}
	tailPosition++;
	increaseArray();
}

template <typename E>
E
SDAL<E>::replace(E element, size_t position){
	if(is_empty())
		throw std::runtime_error("SDAL<E>.replace(): empty list");
	else if(position < 0 || position > (length()-1)){
		throw std::runtime_error("SDAL<E>.replace(): position does not exist");
	}
	else{
		E val = SDAL_list[position];
		SDAL_list[position] = element;
		return val;
	}
}

template <typename E>
E
SDAL<E>::remove(size_t position){
	if(is_empty())
		throw std::runtime_error("SDAL<E>.remove(): empty list");
	else if((length()-1) < position || position < 0)
		throw std::runtime_error("SDAL<E>.remove(): position doesn't exist");
	else{

		if(position == 0){
			E pop = pop_front();
			decreaseArray();
			return pop;
		}
		else if(position == length()-1){
			E pop = pop_back();
			decreaseArray();
			return pop;
		}
		else{
			E val = SDAL_list[position];
			size_t index = position;
			while(index != length()){
				SDAL_list[index] = SDAL_list[index+1];
				index++;
			}
			decreaseArray();
			if(tailPosition != -1){tailPosition--;}
			return val;
		}
	}
}

template <typename E>
E
SDAL<E>::pop_back(){
	if(is_empty())
		throw std::runtime_error("SDAL<E>.pop_back(): empty list");
	else{
		E val = SDAL_list[length()-1];
		//SDAL_list[length()-1] = nullptr;
		//delete SDAL_list[length()-1];
		decreaseArray();
		if(tailPosition != -1){tailPosition--;}
		return val;
	}
}

template <typename E>
E
SDAL<E>::pop_front(){
	if(is_empty())
		throw std::runtime_error("SDAL<E>.pop_front(): empty list");
	else{
		E val = SDAL_list[0];
		size_t index = 0;
		while(index != length()){
			SDAL_list[index] = SDAL_list[index+1];
			index++;
		}
		decreaseArray();
		if(length() == 1){
			tailPosition = -1;
		}
		else{
			if(tailPosition != -1){tailPosition--;}
		}
		return val;
	}
}

template <typename E>
E
SDAL<E>::item_at(size_t position){
	if(is_empty())
		throw std::runtime_error("SDAL<E>.item_at(): empty list");
	else if((length()-1) < position || position < 0)
		throw std::runtime_error("SDAL<E>.item_at(): position doesn't exist");
	else{
		return SDAL_list[position];
	}
}

template <typename E>
E
SDAL<E>::peek_back(){
	if(is_empty())
		throw std::runtime_error("SDAL<E>.peek_back(): empty list");
	return SDAL_list[length()-1];
}

template <typename E>
E
SDAL<E>::peek_front(){
	if(is_empty())
		throw std::runtime_error("SDAL<E>.peek_front(): empty list");
	return SDAL_list[0];
}

template <typename E>
bool
SDAL<E>::is_empty(){
	if(tailPosition == -1){
		return true;
	}
	return false;
}

template <typename E>
bool
SDAL<E>::is_full(){
	if(length() == capacity){
		return true;
	}
	return false;
}

template <typename E>
size_t
SDAL<E>::length(){
	if(!is_empty()){
		return (tailPosition+1);
	}
	return 0;
}

template <typename E>
void
SDAL<E>::clear(){
	tailPosition = -1;
}

template <typename E>
bool
SDAL<E>::contains(const E& element, equals_function func){
	if(is_empty()){
		throw std::runtime_error("SDAL<E>.contains(): empty list");
	}
	else{
		size_t index = 0;
		while(index != length()){
			if(func(SDAL_list[index], element))
				return true;
			index++;
		}
		return false;
	}
}

template <typename E>
std::ostream&
SDAL<E>::print(std::ostream& ostream){
	if(is_empty()){throw std::runtime_error("SDAL<E>.print(): empty list");}

	size_t index = 0;
	ostream << "[";
	while(index != length()){
		ostream << SDAL_list[index];
		index++;

		if(index != tailPosition+1){ostream << ",";}	// add a comma unless its the last element
		else{ostream << "]";}
	}
	return ostream;
}

template <typename E>
E* const
SDAL<E>::contents(){
	if(is_empty()){throw std::runtime_error("SDAL<E>.contents(): empty list");}
	else{
		E* const array = new E[length()];
		size_t index = 0;
		while(index != tailPosition+1){
			array[index] = SDAL_list[index];
			index++;
		}
		return array;
	}
}

}
#endif /* SDAL_H_ */
