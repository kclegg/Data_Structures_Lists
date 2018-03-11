#ifndef LIST_H_
#define LIST_H_
#include <iostream>

namespace cop3530 {

template <typename E>
class List {
public:
	virtual ~List(){};

	virtual void insert(E element, size_t position) = 0;
	virtual void push_back(E element) = 0;
	virtual void push_front(E element) = 0;
	virtual E replace(E element, size_t position) = 0;
	virtual E remove(size_t position) = 0;
	virtual E pop_back() = 0;
	virtual E pop_front() = 0;
	virtual E item_at(size_t position) = 0;
	virtual E peek_back() = 0;
	virtual E peek_front() = 0;
	virtual bool is_empty() = 0;
	virtual bool is_full() = 0;
	virtual size_t length() = 0;
	virtual void clear() = 0;

	typedef bool (*equals_function)(const E&, const E&);								// define the data type "equals_function"
	virtual bool contains(const E& element, equals_function func) = 0;

	virtual std::ostream& print(std::ostream& ostream) = 0;
	virtual E* const contents() = 0;
};

} /* namespace cop3530 */

#endif /* ADTLIST_H_ */
