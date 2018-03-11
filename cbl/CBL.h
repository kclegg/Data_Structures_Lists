#ifndef CBL_H_
#define CBL_H_
#include <stdexcept>
#include "List.h"

namespace cop3530{

template <typename E>
class CBL: public List<E>{

	private:
		E* CBL_list;
		size_t capacity;
		size_t headPosition;
		size_t tailPosition;
		void decreaseArray();
		void increaseArray();

	public:
		CBL();
		CBL(size_t cap);
		// add iterator constructor
		~CBL() override;

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
		class CBL_Iter
		{
			public:
			  // type aliases required for C++ iterator compatibility
			  using value_type = T;
			  using reference = T&;
			  using pointer = T*;
			  using difference_type = std::ptrdiff_t;
			  using iterator_category = std::forward_iterator_tag;

			  // type aliases for prettier code
			  using self_type = CBL_Iter;
			  using self_reference = CBL_Iter&;

			private:
			  T* here;

			public:
			  explicit CBL_Iter(T* start = nullptr) : here(start) {}
			  CBL_Iter(const CBL_Iter& src) : here(src.here) {}

			  reference operator*() const {
				  if(here == nullptr)
					  throw std::runtime_error("CBL_Iter<T>.operator*(): iterator pointer error");
				  return here->data;
			  }
			  pointer operator->() const {
				  if(here == nullptr)
				  	throw std::runtime_error("CBL_Iter<T>.operator*(): iterator pointer error");
				  return &(here->data);
			  }

			  self_reference operator=( CBL_Iter<T> const& src ) {
				  here=src.here;
				  return *this;
			  }

			  // preincrement
			  self_reference operator++() {
				  if(here == nullptr)
					  throw std::runtime_error("CBL_Iter<T>.operator++(): iterator pointer error");
				  here = here->next;
				  return *this;
			  }

			  // postincrement
			  self_type operator++(int) {
				  if(here == nullptr)
					  throw std::runtime_error("CBL_Iter<T>.operator++(int): iterator pointer error");
				  self_type val = *this;
				  here = here->next;
				  return val;
			  }

			  bool operator==( CBL_Iter<T> const& rhs ) const {
				  if(here == rhs.here)
					  return true;
				  return false;
			  }
			  bool operator!=( CBL_Iter<T> const& rhs) const {
				  if(here != rhs.here)
					  return true;
				  return false;
			  }
		};

		using value_type = E;
		using iterator = CBL_Iter<E>;
		using const_iterator = CBL_Iter<E const>;

		iterator begin() { return CBL_Iter<E>(headPosition); }
		iterator end() { return CBL_Iter<E>(tailPosition); }

		const_iterator begin() const { return CBL_Iter<E>(headPosition); }
		const_iterator end() const { return CBL_Iter<E>(tailPosition); }
};

template <typename E>
CBL<E>::CBL(){
	CBL_list = new E[50];
	capacity = 50;
	headPosition = -1;
	tailPosition = -1;
}

template <typename E>
CBL<E>::CBL(size_t cap){
	CBL_list = new E[capacity];
	capacity = cap;
	headPosition = -1;
	tailPosition = -1;
}

template <typename E>
CBL<E>::~CBL(){
	clear();
}

/*template <typename E>
CBL<E>::CBL(iterator i){

}*/

template <typename E>
void
CBL<E>::increaseArray(){
	if(is_full()){
		size_t newlength = capacity*1.5;
		E* tempArray = new E[newlength];
		for(unsigned int i = 0; i < length(); i++){
			tempArray[i] = CBL_list[i];
		}
		delete[] CBL_list;
		capacity = newlength;
		CBL_list = tempArray;
	}
}

template <typename E>
void
CBL<E>::decreaseArray(){
	if(capacity >= 100 && (capacity/2) > length()){
		size_t newlength = capacity*0.75;
		E* tempArray = new E[newlength];
		for(unsigned int i = 0; i < length(); i++){
			tempArray[i] = CBL_list[i];
		}
		delete[] CBL_list;
		capacity = newlength;
		CBL_list = tempArray;
	}
}

template <typename E>
void
CBL<E>::insert(E element, size_t position){
	if(position < 0 || position > capacity || (position < headPosition && position > (tailPosition+1)) || ((position > headPosition && position > (tailPosition+1)) && headPosition < tailPosition))
		throw std::runtime_error("CBL<E>.insert(): position does not exist");
	else if(is_full()){
		throw std::runtime_error("CBL<E>.insert(): full list");
	}
	else if(is_empty()){
		CBL_list[0] = element;
		headPosition = 0;
		tailPosition = 0;
	}
	else{
		if(position == headPosition){
			push_front(element);
		}
		else if(position == tailPosition+1){
			push_back(element);
		}
		else{
			// always start from the TAIL !!!
			if(headPosition > tailPosition){
				if(position > headPosition){
					size_t loop = (capacity-position)+(tailPosition+1);
					size_t index = tailPosition+1;

					while(loop > 0){
						if(index == 0){
							CBL_list[index] = CBL_list[capacity-1];
							index = capacity-1;
						}
						else{
							CBL_list[index] = CBL_list[index-1];
							index--;
						}
						loop--;
					}
					CBL_list[position] = element;
				}
				else{
					size_t loop = (tailPosition-position)+1;
					size_t index = tailPosition+1;
					while(loop > 0){
						CBL_list[index] = CBL_list[index-1];
						index--;
						loop--;
					}
					CBL_list[position] = element;
				}
			}
			else if(headPosition < tailPosition){
				size_t loop = (tailPosition-position)+1;
				size_t index = tailPosition+1;

				while(loop > 0){
					CBL_list[index] = CBL_list[index-1];
					index--;
					loop--;
				}
				CBL_list[position] = element;
			}

			increaseArray();
			tailPosition++;
		}
	}
}

template <typename E>
void
CBL<E>::push_back(E element){
	if(is_empty()){
		CBL_list[0] = element;
		headPosition = 0;
		tailPosition = 0;
	}
	else if(is_full()){
		throw std::runtime_error("CBL<E>.push_back(): full list");
	}
	else{
		if(tailPosition != capacity-1){
			CBL_list[tailPosition+1] = element;
			tailPosition++;
		}
		else{
			CBL_list[0] = element;
			tailPosition = 0;
		}
	}
	increaseArray();
}

template <typename E>
void
CBL<E>::push_front(E element){
	if(is_empty()){
		CBL_list[0] = element;
		headPosition = 0;
		tailPosition = 0;
	}
	else if(is_full()){
		throw std::runtime_error("CBL<E>.push_front(): full list");
	}
	else{
		if(headPosition > tailPosition){
			CBL_list[headPosition-1] = element;
			headPosition--;
		}
		else if(headPosition < tailPosition){
			if(headPosition == 0){
				CBL_list[capacity-1] = element;
				headPosition = capacity-1;
			}
			else{
				CBL_list[headPosition-1] = element;
				headPosition--;
			}
		}
	}
	increaseArray();
}

template <typename E>
E
CBL<E>::replace(E element, size_t position){
	if(is_empty())
		throw std::runtime_error("CBL<E>.replace(): empty list");
	else if(position < 0 || position > (length()-1)){
		throw std::runtime_error("CBL<E>.replace(): position does not exist");
	}
	else{
		E val = CBL_list[position];
		CBL_list[position] = element;
		return val;
	}
}

template <typename E>
E
CBL<E>::remove(size_t position){
	if(is_empty())
		throw std::runtime_error("CBL<E>.remove(): empty list");
	else if((capacity-1) < position || position < 0 || (position < headPosition && position > (tailPosition+1)) || ((position > headPosition && position > (tailPosition+1)) && headPosition < tailPosition))
		throw std::runtime_error("CBL<E>.remove(): position doesn't exist");
	else{
		E val = CBL_list[position];
		size_t index = position;

		if(position == headPosition && position == tailPosition){
			headPosition = -1;
			tailPosition = -1;
		}
		else if(headPosition > tailPosition){
			if(headPosition == capacity-1){headPosition = 0;}
			else{headPosition++;}
		}
		else if(tailPosition > headPosition){
			if(tailPosition > 0){tailPosition--;}
			else{tailPosition = -1;}
		}

		if(headPosition > tailPosition){
			size_t loop = (capacity-position)+tailPosition;
			while(loop > 0){
				CBL_list[index] = CBL_list[index+1];
				if(index!=capacity-1){index++;}
				else{index = 0;}
				loop--;
			}
		}
		else if(tailPosition > headPosition){
			size_t loop = length()-position;
			while(loop > 0){
				CBL_list[index] = CBL_list[index+1];
				index++;
				loop--;
			}
		}

		decreaseArray();
		return val;
	}
}

template <typename E>
E
CBL<E>::pop_back(){
	if(is_empty())
		throw std::runtime_error("CBL<E>.pop_back(): empty list");
	else{
		E val = CBL_list[tailPosition];

		if(length() == 1){
			headPosition = -1;
			tailPosition = -1;
		}
		else{
			if(tailPosition > headPosition){
				if(tailPosition != -1){tailPosition--;}
			}
			else if(headPosition > tailPosition){
				if(tailPosition == 0){tailPosition = capacity-1;}
				else{tailPosition--;}
			}
		}

		decreaseArray();
		return val;
	}
}

template <typename E>
E
CBL<E>::pop_front(){
	if(is_empty())
		throw std::runtime_error("CBL<E>.pop_front(): empty list");
	else{
		E val = CBL_list[headPosition];

		if(length() == 1){
			headPosition = -1;
			tailPosition = -1;
		}
		else{
			if(headPosition != -1){
				if(headPosition < tailPosition){tailPosition--;}
				else if(headPosition > tailPosition){
					if(headPosition == capacity-1){headPosition = 0;}
					else{headPosition = headPosition+1;}
				}
			}
		}

		size_t loop = length();
		size_t index = headPosition;

		if(headPosition > tailPosition){
			while(loop > 0){
				// edit below
				if(index != tailPosition){
					if(index != capacity-1){
						CBL_list[index] = CBL_list[index+1];
						index++;}
					else{
						CBL_list[index] = CBL_list[0];
						index = 0;
					}
				}
				loop--;
			}
		}
		else if(tailPosition > headPosition){
			while(loop > 0){
				if(index != tailPosition){
					CBL_list[index] = CBL_list[index+1];
				}
				loop--;
			}
		}
		else{}

		decreaseArray();
		return val;
	}
}

template <typename E>
E
CBL<E>::item_at(size_t position){
	if(is_empty())
		throw std::runtime_error("CBL<E>.item_at(): empty list");
	else if((length()-1) < position || position < 0)
		throw std::runtime_error("CBL<E>.item_at(): position doesn't exist");
	else{
		return CBL_list[position];
	}
}

template <typename E>
E
CBL<E>::peek_back(){
	if(is_empty())
		throw std::runtime_error("CBL<E>.peek_back(): empty list");
	return CBL_list[tailPosition];
}

template <typename E>
E
CBL<E>::peek_front(){
	if(is_empty())
		throw std::runtime_error("CBL<E>.peek_front(): empty list");
	return CBL_list[headPosition];
}

template <typename E>
bool
CBL<E>::is_empty(){
	if(tailPosition == -1 || headPosition == -1){
		return true;
	}
	return false;
}

template <typename E>
bool
CBL<E>::is_full(){
	if(length() == capacity){
		return true;
	}
	return false;
}

template <typename E>
size_t
CBL<E>::length(){
	if(!is_empty()){
		if(tailPosition > headPosition){return ((tailPosition-headPosition)+1);}
		else if(tailPosition < headPosition){return ((capacity-headPosition)+(tailPosition+1));}
		else{return 1;}
	}
	return 0;
}

template <typename E>
void
CBL<E>::clear(){
	headPosition = -1;
	tailPosition = -1;
}

template <typename E>
bool
CBL<E>::contains(const E& element, equals_function func){
	if(is_empty()){
		throw std::runtime_error("CBL<E>.contains(): empty list");
	}
	else{
		size_t index = headPosition;
		size_t loop = length();
		while(loop > 0){
			if(func(CBL_list[index], element)){return true;}

			if(index != tailPosition){
				if(index != capacity-1){index++;}
				else{index = 0;}
			}
			loop--;
		}
		return false;
	}
}

template <typename E>
std::ostream&
CBL<E>::print(std::ostream& ostream){
	if(is_empty()){throw std::runtime_error("CBL<E>.print(): empty list");}

	size_t index = headPosition;
	size_t loop = length();
	ostream << "[";
	while(loop > 0){
		ostream << CBL_list[index];
		if(index != tailPosition){
			if(index != capacity-1){index++;}
			else{index = 0;}
		}

		if(index != tailPosition+1){ostream << ",";}	// add a comma unless its the last element
		else{ostream << "]";}
		loop--;
	}
	return ostream;
}

template <typename E>
E* const
CBL<E>::contents(){
	if(is_empty()){throw std::runtime_error("CBL<E>.contents(): empty list");}
	else{
		E* const array = new E[length()];
		size_t index = headPosition;
		size_t loop = length();
		while(loop > 0){
			array[index] = CBL_list[index];
			if(index != tailPosition){
				if(index != capacity-1){index++;}
				else{index = 0;}
			}
			loop--;
		}
		return array;
	}
}

}
#endif /* CBL_H_ */
