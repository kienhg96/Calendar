#ifndef __COMMAND_MGR__
#define __COMMAND_MGR__
#include "Types.h"
#include "Collections.h"

struct CommandHandler {
	char * name;
	Callback callback;
	void * data;
};

class CommandMgr {
private:
	static CommandMgr * _instance;
	CommandMgr();
public:
	static CommandMgr * instance();
	~CommandMgr();
public:
	void init();
	void tick();
	void registerHandler(const char * name, Callback callback, void * data);
private:
	void processCommand();
private:
	char * _buffer;
	int _index;
	LinkedList * _handlers;
};

#endif
