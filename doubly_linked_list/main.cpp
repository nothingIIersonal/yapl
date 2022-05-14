#include <iostream>

#include "doubly_linked_list.h"

int main()
{
	DoublyLinkedList<int> list_1 = DoublyLinkedList<int>(); 
	list_1.add(1);
	list_1.add(2);
	list_1.add(3);
	list_1.add(4);

	std::cout << list_1.get(0) << list_1.get(1) << list_1.get(2) << list_1.get(3) << list_1.get(4) << "\n";

	list_1.remove(2);

	std::cout << list_1.get(0) << list_1.get(1) << list_1.get(2) << list_1.get(3) << list_1.get(4) << "\n";
 
	list_1.insert(5, 1);

	std::cout << list_1.get(0) << list_1.get(1) << list_1.get(2) << list_1.get(3) << list_1.get(4) << "\n";

	list_1.insert(6, 3);

	std::cout << list_1.get(0) << list_1.get(1) << list_1.get(2) << list_1.get(3) << list_1.get(4) << "\n";	

	list_1.insert(9, 0);

	std::cout << list_1.get(0) << list_1.get(1) << list_1.get(2) << list_1.get(3) << list_1.get(4) << "\n";	

	return 0;
}