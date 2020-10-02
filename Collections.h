#ifndef __COLLECTIONS_H__
#define __COLLECTIONS_H__

struct Node {
	Node * next;
	Node * prev;
	void * data;
};

class LinkedList;
class LinkedListIterator;

class LinkedList {
public:
	LinkedList();
	~LinkedList();

public:
	Node * add(void * data);
	Node * remove(Node * node);

	Node * head();
	Node * tail();
	int size();
	LinkedListIterator iterator();
	void setHead(Node * node);
private:
	Node * _head;
	Node * _tail;
	int _size;
};

class LinkedListIterator {
public:
	LinkedListIterator(LinkedList * list);
	~LinkedListIterator();

public:
	bool hasNext();
	void * next();
	void remove();

private:
	Node * _head;
	Node * _current;
	LinkedList * _list;
};

#endif
