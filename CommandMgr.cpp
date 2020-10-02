#include "CommandMgr.h"
#include "string.h"
#include <Arduino.h>

#define BUFFER_SIZE 256

CommandMgr * CommandMgr::_instance = nullptr;

CommandMgr * CommandMgr::instance() {
	if (!_instance) {
		_instance = new CommandMgr();
	}
	return _instance;
}

CommandMgr::CommandMgr() {
	_buffer = new char[BUFFER_SIZE];
	_index = 0;
	_handlers = new LinkedList();
}

CommandMgr::~CommandMgr() {
	delete[] _buffer;
	delete _handlers;
}

void CommandMgr::init() {}

void CommandMgr::tick() {
	int size = Serial.available();
	while (size > 0 && _index < BUFFER_SIZE) {
		size--;
		char input = Serial.read();
		if (input == '\n') {
			_buffer[_index] = 0;
			processCommand();
			_index = 0;
		} else {
			_buffer[_index] = input;
			_index++;
		}
	}
}

void CommandMgr::registerHandler(const char * name, Callback callback, void * data) {
	
}

void CommandMgr::processCommand() {
	Serial.print("Command received: ");
	Serial.println(_buffer);

	LinkedListIterator it = _handlers->iterator();
	while (it.hasNext()) {
		CommandHandler * handler = (CommandHandler *) it.next();
		if (strcmp(handler->name, _buffer) == 0) {
			handler->callback(handler->data);
			break;
		}
	}
}
