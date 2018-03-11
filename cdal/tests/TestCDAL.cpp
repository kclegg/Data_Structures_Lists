#include <iostream>

#include "CDAL.h"

using namespace std;

int main(){
	cop3530::CDAL<int> cdal;

	cdal.push_front(3);		// 3
	cdal.push_front(6);		// 6 3
	cdal.push_back(2);		// 6 3 2
	cdal.push_front(1);		// 1 6 3 2
	cdal.pop_back();		// 1 6 3
	cdal.push_front(7);		// 7 1 6 3
	cdal.remove(3);			// 7 1 6
	cdal.pop_front();		// 1 6
	cdal.insert(87,2);		// 1 6 87
	cdal.replace(53,1);		// 1 53 6 87

	cout << "FRONT  " << cdal.peek_front() << endl;
	cout << "BACK   " << cdal.peek_back() << endl;
	cout << "POS 1  " << cdal.item_at(1) << endl;
	cout << "LENGTH " << cdal.length() << endl;

	cdal.clear();

	cdal.push_front(24);	// 24
	cdal.push_front(29);	// 29 24
	cdal.push_front(45);	// 45 29 24
	cdal.pop_back();		// 45 29
	cdal.push_front(232);	// 232 45 29

	int* cont = cdal.contents();

	cout << cont[0];	// 232
	cout << endl;
	cout << cont[1];	// 45
	cout << endl;
	cout << cont[2];	// 29
	cout << endl;

	cout << "FRONT  " << cdal.peek_front() << endl;
	cout << "BACK   " << cdal.peek_back() << endl;
	cout << "POS 1  " << cdal.item_at(1) << endl;
	cout << "LENGTH " << cdal.length() << endl;

	cdal.print(std::cout);	// print ostream method
	cout << endl;

	cdal.~CDAL();

	return 0;

}
