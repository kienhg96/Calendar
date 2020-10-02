#include "Scheduler.h"
#include "TemperatureReader.h"
#include "TimeController.h"
#include "DisplayManager.h"
#include "CommandMgr.h"
#include "LunarConverter.h"
#include "ButtonManager.h"

Scheduler * scheduler = Scheduler::instance();
TemperatureReader * temperatureReader = TemperatureReader::instance();
TimeController * timeController = TimeController::instance();
DisplayManager * displayManager = DisplayManager::instance();
CommandMgr * commandMgr = CommandMgr::instance();
LunarConverter * conv = LunarConverter::instance();
ButtonManager * buttonMgr = ButtonManager::instance();

void setup() {
	Serial.begin(9600);
	scheduler->init();
	temperatureReader->init(scheduler);
	timeController->init(scheduler, buttonMgr);
	displayManager->init(scheduler, timeController, temperatureReader, conv);
	commandMgr->init();
}

void loop() {
	scheduler->tick();
	commandMgr->tick();
	buttonMgr->tick();
}
