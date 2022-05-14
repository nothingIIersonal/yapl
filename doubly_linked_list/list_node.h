#pragma once


#include <memory>
#include <cinttypes>


template <typename T = int64_t>
class ListNode;

template <typename T = int64_t>
using ln_ptr = typename std::shared_ptr<ListNode<T>>;

template <typename T>
class ListNode
{
private:
	T _data;
	ln_ptr<T> _next;
	ln_ptr<T> _prev;

public:
	ListNode() 
	: _data(0), _next(nullptr), _prev(nullptr)
	 					   					{}
	ListNode(T data, 
		     ln_ptr<T> next,
		     ln_ptr<T> prev) 
	: _data(data), _next(next), _prev(prev) {}

	T get_data() 				   			{ return _data; }

	void set_data(T data) 		   			{ _data = data; }

	ln_ptr<T> next() 			  			{ return _next; }

	ln_ptr<T> prev() 			   			{ return _prev; }

	void set_next(ln_ptr<T> next)  			{ _next = next; }

	void set_prev(ln_ptr<T> prev) 			{ _prev = prev; }

};