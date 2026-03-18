#pragma once
#pragma once
#include <stddef.h>
#include <iostream>
#include <stdexcept>
#include <cassert> 
template <typename T>
class List {
private:
	struct Node {
		T data = T();
		struct Node* nxt = NULL;
		struct Node* pre = NULL;
	};
	struct Node* head;
	struct Node* tail;
	int length = 0;
public:
	class iterator {
	private:
		Node* ptr;
	public:
		iterator(Node* p = nullptr) : ptr(p) {}
		T& operator*() {
			return ptr->data;
		}
		Node* operator->() {
			return ptr;
		}
		iterator& operator++() {
			ptr = ptr->nxt;
			return *this;
		}
		iterator operator++(int) {
			iterator last = *this;
			ptr = ptr->nxt;
			return last;
		}
		iterator& operator--() {
			ptr = ptr->pre;
			return *this;
		}
		iterator operator--(int) {
			iterator last = *this;
			ptr = ptr->pre;
			return last;
		}
		bool operator!=(const iterator& y) const {
			return this->ptr != y.ptr;
		}
		bool operator==(const iterator& y) const {
			return this->ptr == y.ptr;
		}
		friend class List;
	};
	List() {
		head = new Node;
		tail = new Node;
		head->nxt = tail;
		tail->pre = head;
	}
	~List() {
		Node* x = head;
		while (x != NULL)
		{
			Node* next = x->nxt;
			delete x;
			x = next;
		}
	}
	iterator end() {
		return iterator(tail);
	}
	iterator begin() {
		return iterator(head->nxt);
	}
	iterator find(const T& x) {
		iterator p = begin();
		while (p != end())
		{
			if (*p == x)
			{
				return p;
			}
			++ p;
		}
		return end();
	}
	void insert(iterator pos, const T& x) {
		Node* cur = pos.ptr;
		if (cur == nullptr)
			return;
		Node* p = nullptr;
		try {
			p = new Node;
		}
		catch (std::bad_alloc& e) {
			std::cerr << "内存分配失败: " << e.what() << std::endl;
			return;
		}
		p->data = x;
		p->nxt = cur;
		p->pre = cur->pre;
		cur->pre->nxt = p;
		cur->pre = p;
		length++;
	}
	void push_back(const T& x) {
		insert(end(), x);
	}
	void push_front(const T& x) {
		insert(begin(), x);
	}
	void clear() {
		Node* p = head->nxt;
		while (p != tail) {
			Node* next = p->nxt;
			delete p;
			p = next;
		}
		length = 0;
		head->nxt = tail;
		tail->pre = head;
	}
	iterator erase(iterator pos) {
		Node* cur = pos.ptr;
		if (cur == tail || cur == nullptr)
			return end();
		Node* next = cur -> nxt;
		cur -> pre -> nxt = cur -> nxt;
		cur -> nxt -> pre = cur -> pre;
		delete cur;
		length --;
		return iterator(next);
	}
	int size() {
		return length;
	}
	bool empty() {
		return begin() == end(); // return length == 0;
	}
	T front() {
		return empty() ? T() : *begin();
	}
	T back() {
		return empty() ? T() : *(--end());
	}
	void remove(const T& x) {
		for (auto it = begin(); it != end(); ) {
			if (*it == x) {
				it = erase(it);
			}
			else {
				++ it;
			}
		}
	}
	void pop_front() {
		if (!empty())
			erase(begin());
	}
	void pop_back() {
		if (!empty()) {
			iterator it = end();
			it--;
			erase(it);
		}
	}
	T& at(int pos) {
		if (pos < 0 || pos >= length) {
			throw std::out_of_range("List::at() 索引越界");
		}
		iterator it = begin();
		for (int i = 0; i < pos; i++)
		{
			it++;
		}
		return *it;
	}
	T& operator[](int pos) {
		assert(pos >= 0 && pos < length);
		iterator it = begin();
		for (int i = 0; i < pos; i++)
		{
			++it;
		}
		return *it;
	}
	void print() {
		for (iterator it = begin(); it != end(); ++it)
		{
			std::cout << *it << " ";
		}
		std::cout << std::endl;
	}
};