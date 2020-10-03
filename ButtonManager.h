#ifndef __BUTTON_MANAGER__
#define __BUTTON_MANAGER__
#include <Arduino.h>
#include "Types.h"
#include "Collections.h"
#include "Pins.h"

#define ANALOG_NO_SIGNAL 10

struct Button {
	int min_val;
	int max_val;
	int timeout;
	unsigned long last_press;
	Callback callback;
	void * data;
};

class AnalogButton {
private:
	int _pin;
	LinkedList _buttons;
public:
	AnalogButton(int pin);
	~AnalogButton();
	
	void addButton(int min_val, int max_val, int timeout, Callback callback, void * data);
	void read();
};

class ButtonManager {
private:
	static ButtonManager * _instance;
	ButtonManager();
public:
	static ButtonManager * instance();
	~ButtonManager();
public:
	void setButtonCallback(int index, Callback callback, void * data);
	void tick();
private:
	AnalogButton * _buttonController;
};

#endif
