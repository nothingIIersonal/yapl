#pragma once


#include <iostream>

#include "list_node.h"


template <typename T = int64_t>
class DoublyLinkedList
{
private:
	ln_ptr<T> _first;
	ln_ptr<T> _cur;

public:
	DoublyLinkedList() {}

	void add(T data);
	
	T get(uint64_t pos);
	
	void remove(uint64_t pos);

	void insert(T data, uint64_t pos);

};


template <typename T>
void DoublyLinkedList<T>::add(T data)
{
	if (!_first)
	{
		_first = std::make_shared<ListNode<T>>(ListNode<T>(data, nullptr, nullptr));
		_cur = _first;
		return;
	}

	ln_ptr<T> new_node = std::make_shared<ListNode<T>>(ListNode<T>(data, nullptr, _cur));
	_cur->set_next(new_node);

	_cur = new_node;
}


template <typename T>
T DoublyLinkedList<T>::get(uint64_t pos)
{
	auto searched = _first;

	while (searched && pos--)
		searched = searched->next();

	if (!searched)
		return 0;

	return searched->get_data();
}


template <typename T>
void DoublyLinkedList<T>::remove(uint64_t pos)
{
	auto searched = _first;

	while (searched && pos--)
		searched = searched->next();

	if (!searched)
		return;

	if (searched->next())
		searched->next()->set_prev(searched->prev());

	if (searched->prev())
		searched->prev()->set_next(searched->next());
}


template <typename T>
void DoublyLinkedList<T>::insert(T data, uint64_t pos)
{
	bool first = false;
	if (!pos)
		first = true;

	auto searched = _first;

	while (searched && pos--)
		searched = searched->next();

	if (!searched)
		return;

	ln_ptr<T> new_node = std::make_shared<ListNode<T>>(ListNode<T>(data, searched, nullptr));

	if (first)
	{
		_first->set_prev(new_node);
		new_node->set_next(_first);

		_first = new_node;

		return;
	}

	if (searched->prev())
	{
		searched->prev()->set_next(new_node);
		new_node->set_prev(searched->prev());
	}

	searched->set_prev(new_node);
}