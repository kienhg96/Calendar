#include "Collections.h";

LinkedList::LinkedList() {
	_head = 0;
	_tail = 0;
	_size = 0;
}

LinkedList::~LinkedList() {
	Node * p = _head;
	Node * _p;
	while (p) {
		_p = p;
		p = p->next;
		delete _p;
	}
}

Node * LinkedList::add(void * data) {
	Node * node = new Node();
	node->data = data;
	if (_size == 0) {
		_head = node;
		_tail = node;
	}
	else {
		_tail->next = node;
		node->prev = _tail;
		_tail = node;
	}
	_size++;
	return node;
}

Node * LinkedList::remove(Node * node) {
	if (node->prev) {
		node->prev->next = node->next;
	} else {
		_head = _head->next;
	}
	if (node->next) {
		node->next->prev = node->prev;
	} else {
		_tail = _tail->prev;
	}
	_size--;
	Node * result = node->prev;
	delete node;
	return result;
}

int LinkedList::size() {
	return _size;
}

Node * LinkedList::tail() {
	return _tail;
}

Node * LinkedList::head() {
	return _head;
}

void LinkedList::setHead(Node * node) {
	_head = node;
	if (!_head) {
		_tail = 0;
	}
	else if (!_head->next) {
		_tail = _head;
	}
}

LinkedListIterator LinkedList::iterator() {
	LinkedListIterator iterator(this);
	return iterator;
}

LinkedListIterator::LinkedListIterator(LinkedList * list) {
	_list = list;
	_head = list->head();
	_current = 0;
}

LinkedListIterator::~LinkedListIterator() {}

bool LinkedListIterator::hasNext() {
	return (_head != 0) && (_current == 0 || _current->next != 0);
}

void * LinkedListIterator::next() {
	if (_current == 0) {
		_current = _head;
	}
	else {
		_current = _current->next;
	}
	return _current->data;
}

void LinkedListIterator::remove() {
	_current = _list->remove(_current);
}
