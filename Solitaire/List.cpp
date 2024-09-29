#pragma once
#include <iostream>

using namespace std;

template <typename T> class DLL {
private:
	class Node {
	public:
		T data;
		Node* prev;
		Node* next;

		Node(T data, Node* prev, Node* next) : data(data), prev(prev), next(next) {}
	};
	
	Node* head;
	Node* tail;

public:
	class ListIterator {
	private:
		Node* iter;

	public:

		ListIterator(Node* iter = nullptr) : iter(iter) {}
		
		ListIterator& operator ++ () {
			(!iter || !iter->next) ? iter = nullptr : iter = iter->next;
			return *this;
		}

		ListIterator& operator -- () {
			(!iter || !iter->prev) ? iter = nullptr : iter = iter->prev;
			return *this;
		}

		T& operator *() {
			return iter->data;
		}

		bool operator == (const ListIterator& iter2) const {
			return (this->iter == iter2.iter);
		}

		bool operator != (const ListIterator& iter2) const {
			return (this->iter != iter2.iter);
		}
	};

	DLL() : head(nullptr), tail(nullptr) {}
	DLL(Node* head, Node* tail) : head(head), tail(tail) {}

	T getHead() {
		return head ? head->data : nullptr;
	}

	T getTail() {
		return tail ? tail->data : nullptr;
	}

	void insertHead(const T element) {
		if (!head) {
			head = tail = new Node(element, nullptr, nullptr);
			return;
		}

		head->prev = new Node(element, nullptr, head);
		head = head->prev;
		if (!tail) tail = head;
	}

	void insertTail(const T element) {
		if (!head) {
			head = tail = new Node(element, nullptr, nullptr);
			return;
		}

		tail->next = new Node(element, tail, nullptr);
		tail = tail->next;
		if (!head) head = tail;
	}

	void deleteHead() {
		if (!head) return;
		Node* temp = head;
		head = head->next;
		delete temp;
		if(head) head->prev = nullptr;
	}

	void deleteTail() {
		if (!tail) return;
		Node* temp = tail;
		tail = tail->prev;
		delete temp;
		if(tail) tail->next = nullptr;
	}

	Node*& removeTail() {
		Node* temp = tail;
		tail = tail->prev;
		if (tail) tail->next = nullptr;
		return temp;
	}

	void printForward() const {
		if (!head) return;
		for (auto iter = fbegin(); iter != fend(); ++iter) {
			(iter == ListIterator(tail)) ? cout << *iter : cout << *iter << " -> ";
		}
		cout << endl;
	}

	void printReverse() const {
		if (!head) return;
		for (auto iter = rbegin(); iter != rend(); --iter) {
			(iter == ListIterator(head)) ? cout << *iter : cout << *iter << " -> ";
		}
		cout << endl;
	}

	bool isEmpty() const {
		return head == nullptr;
	}

	int size() const {
		int size = 0;
		for (Node* iter = head; iter; iter = iter->next, size++);
		return size;
	}

	Node* accessNode(int positionFromTail) {
		if (!head) return nullptr;

		Node* iter = tail;
		for (int index = 1; iter && index < positionFromTail; index++, iter = iter->prev);
		return iter;
	}

	Node*& extract(int numberOfNodes) {
		Node* node = accessNode(numberOfNodes);
		if (!node) return node;

		if (head == tail) {
			head = tail = nullptr;
			return node;
		}

		tail = node->prev;
		node->prev->next = nullptr;
		return node;
	}

	void append(Node*& list, int length) {
		if (!list) return;

		if (!head) {
			head = tail = list;
			list->prev = nullptr;
			for (int i = 1; i < length; i++, tail = tail->next);
			return;
		}
		
		tail->next = list;
		list->prev = tail;
		for (int i = 0; i < length; i++, tail = tail->next);
	}

	typename ListIterator fbegin() const {
		return ListIterator(head);
	}

	typename ListIterator fend() const {
		return ListIterator(nullptr);
	}

	typename ListIterator rbegin() const {
		return ListIterator(tail);
	}

	typename ListIterator rend() const {
		return ListIterator(nullptr);
	}

	~DLL() {
		for (Node* iter = head; iter;) {
			Node* temp = iter->next;
			delete iter;
			iter = temp;
		}
		head = tail = nullptr;
	}

};


