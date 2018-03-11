#include <iostream>
#include "SSLL.h"

using namespace std;

int main(){
	cop3530::SSLL<int> ssll;

	ssll.push_front(3);		// 3
	ssll.push_front(6);		// 6 3
	ssll.push_back(2);		// 6 3 2
	ssll.push_front(1);		// 1 6 3 2
	ssll.pop_back();		// 1 6 3
	ssll.push_front(7);		// 7 1 6 3
	ssll.remove(3);			// 7 1 6
	ssll.pop_front();		// 1 6
	ssll.insert(87,2);		// 1 6 87
	ssll.replace(53,1);		// 1 53 6 87

	cout << "FRONT  " << ssll.peek_front() << endl;
	cout << "BACK   " << ssll.peek_back() << endl;
	cout << "POS 1  " << ssll.item_at(1) << endl;
	cout << "LENGTH " << ssll.length() << endl;

	ssll.clear();

	ssll.push_front(24);	// 24
	ssll.push_front(29);	// 29 24
	ssll.push_front(45);	// 45 29 24
	ssll.pop_back();		// 45 29
	ssll.push_front(232);	// 232 45 29

	int* cont = ssll.contents();

	cout << cont[0];	// 232
	cout << endl;
	cout << cont[1];	// 45
	cout << endl;
	cout << cont[2];	// 29
	cout << endl;

	cout << "FRONT  " << ssll.peek_front() << endl;
	cout << "BACK   " << ssll.peek_back() << endl;
	cout << "POS 1  " << ssll.item_at(1) << endl;
	cout << "LENGTH " << ssll.length() << endl;

	ssll.print(std::cout);	// print ostream method
	cout << endl;

	ssll.~SSLL();

	return 0;

}
