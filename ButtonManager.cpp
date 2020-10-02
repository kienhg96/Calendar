#include "ButtonManager.h"
#include <math.h>

AnalogButton::AnalogButton(int pin) {
	_pin = pin;
}

AnalogButton::~AnalogButton() {}

void AnalogButton::addButton(int min_val, int max_val, int timeout, Callback callback, void * data) {
	Button * button = new Button();
	button->min_val = min_val;
	button->max_val = max_val;
	button->timeout = timeout;
	button->callback = callback;
	button->data = data;
	button->last_press = 0;
	_buttons.add(button);
}

void AnalogButton::read() {
	int val = analogRead(_pin);
	if (val < ANALOG_NO_SIGNAL) return;
	LinkedListIterator it = _buttons.iterator();
	unsigned long current = millis();
	while (it.hasNext()) {
		Button * button = (Button *) it.next();
		if (button->min_val <= val && val <= button->max_val) {
			int delta = current - button->last_press;
			if (abs(current - button->last_press) >= button->timeout) {
				button->last_press = current;
				button->callback(button->data);
			}
			break;
		}
	}
}

ButtonManager * ButtonManager::_instance = 0;

ButtonManager * ButtonManager::instance() {
	if (!_instance) {
		_instance = new ButtonManager();
	}
	return _instance;
}

ButtonManager::ButtonManager() {
	_buttonController = new AnalogButton(BUTTON_PIN);
}

ButtonManager::~ButtonManager() {
	delete _buttonController;
}

void ButtonManager::setButtonCallback(int index, Callback callback, void * data) {
	if (index == 1) {
		_buttonController->addButton(679, 699, 250, callback, data);
	} else if (index == 2) {
		_buttonController->addButton(970, 986, 250, callback, data);
	} else if (index == 3) {
		_buttonController->addButton(1011, 1131, 250, callback, data);
	} else {
		Serial.print("Invalid setButtonCallback index: ");
		Serial.println(index);
	}
}

void ButtonManager::tick() {
	_buttonController->read();
}
