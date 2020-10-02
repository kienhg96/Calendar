#include "TemperatureReader.h"

TemperatureReader * TemperatureReader::_instance = 0;

TemperatureReader * TemperatureReader::instance() {
	if (!_instance) {
		_instance = new TemperatureReader();
	}
	return _instance;
}

TemperatureReader::TemperatureReader() {
	_temperature = 0;
	_sum = 0;
	_counter = 0;
}

TemperatureReader::~TemperatureReader() {}

int TemperatureReader::temperature() {
	return _temperature;
}

void TemperatureReader::init(Scheduler * scheduler) {
	Serial.println("TemperatureReader::init");
	pinMode(TEMPERATURE_INPUT_PIN, INPUT);
	scheduler->schedule(&update, this, 200);
}

void TemperatureReader::update(void * data) {
	TemperatureReader * self = (TemperatureReader *) data;

	int val = analogRead(TEMPERATURE_INPUT_PIN);
	self->_sum += val;
	self->_counter++;
	if (self->_counter == 10) {
		self->_temperature = floor((1.0 * self->_sum / self->_counter) * 500 / 1024.0);
		self->_counter = 0;
		self->_sum = 0;
	}
}
