#include <iostream>

#include "PSLL.h"

using namespace std;

int main(){
	cop3530::PSLL<int> psll;

	psll.push_front(3);		// 3
	psll.push_front(6);		// 6 3
	psll.push_back(2);		// 6 3 2
	psll.push_front(1);		// 1 6 3 2
	psll.pop_back();		// 1 6 3
	psll.push_front(7);		// 7 1 6 3
	psll.remove(3);			// 7 1 6
	psll.pop_front();		// 1 6
	psll.insert(87,2);		// 1 6 87
	psll.replace(53,1);		// 1 53 6 87

	cout << "FRONT  " << psll.peek_front() << endl;
	cout << "BACK   " << psll.peek_back() << endl;
	cout << "POS 1  " << psll.item_at(1) << endl;
	cout << "LENGTH " << psll.length() << endl;

	psll.clear();

	psll.push_front(24);	// 24
	psll.push_front(29);	// 29 24
	psll.push_front(45);	// 45 29 24
	psll.pop_back();		// 45 29
	psll.push_front(232);	// 232 45 29

	int* cont = psll.contents();

	cout << cont[0];	// 232
	cout << endl;
	cout << cont[1];	// 45
	cout << endl;
	cout << cont[2];	// 29
	cout << endl;

	cout << "FRONT  " << psll.peek_front() << endl;
	cout << "BACK   " << psll.peek_back() << endl;
	cout << "POS 1  " << psll.item_at(1) << endl;
	cout << "LENGTH " << psll.length() << endl;

	psll.print(std::cout);	// print ostream method
	cout << endl;

	psll.~PSLL();

	return 0;

}
