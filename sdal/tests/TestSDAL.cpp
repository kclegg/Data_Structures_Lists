#include <iostream>

#include "SDAL.h"

using namespace std;

int main(){
	cop3530::SDAL<int> sdal;

	sdal.push_front(3);		// 3
	sdal.push_front(6);		// 6 3
	sdal.push_back(2);		// 6 3 2
	sdal.push_front(1);		// 1 6 3 2
	sdal.pop_back();		// 1 6 3
	sdal.push_front(7);		// 7 1 6 3
	sdal.remove(3);			// 7 1 6
	sdal.pop_front();		// 1 6
	sdal.insert(87,2);		// 1 6 87
	sdal.replace(53,1);		// 1 53 6 87

	cout << "FRONT  " << sdal.peek_front() << endl;
	cout << "BACK   " << sdal.peek_back() << endl;
	cout << "POS 1  " << sdal.item_at(1) << endl;
	cout << "LENGTH " << sdal.length() << endl;

	sdal.clear();

	sdal.push_front(24);	// 24
	sdal.push_front(29);	// 29 24
	sdal.push_front(45);	// 45 29 24
	sdal.pop_back();		// 45 29
	sdal.push_front(232);	// 232 45 29

	int* cont = sdal.contents();

	cout << cont[0];	// 232
	cout << endl;
	cout << cont[1];	// 45
	cout << endl;
	cout << cont[2];	// 29
	cout << endl;

	cout << "FRONT  " << sdal.peek_front() << endl;
	cout << "BACK   " << sdal.peek_back() << endl;
	cout << "POS 1  " << sdal.item_at(1) << endl;
	cout << "LENGTH " << sdal.length() << endl;

	sdal.print(std::cout);	// print ostream method
	cout << endl;

	sdal.~SDAL();

	return 0;

}
