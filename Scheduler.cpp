#include <SoftwareSerial.h>
#include "Scheduler.h"
#include <avr/interrupt.h>

#define TIMER_COUNTER_START_VAL 65286

Scheduler * Scheduler::_instance = 0;

Scheduler * Scheduler::instance() {
	if (!_instance) {
		_instance = new Scheduler();
	}
	return _instance;
}

Scheduler::Scheduler() {}

Scheduler::~Scheduler() {}

void Scheduler::init() {
	Serial.println("Scheduler::init");
	cli();

	TCCR1A = 0;
	TCCR1B = 0;
	TIMSK1 = 0;

	TCCR1B |= (1 << CS11) | (1 << CS10); // Prescale 64
	TCNT1 = TIMER_COUNTER_START_VAL;
	TIMSK1 = (1 << TOIE1);

	sei();
}

static Scheduler * scheduler = Scheduler::instance();

ISR (TIMER1_OVF_vect) {
	TCNT1 = TIMER_COUNTER_START_VAL;
	scheduler->emit();
}

void Scheduler::emit() {
	LinkedListIterator it = _scheduleTasks.iterator();
	while (it.hasNext()) {
		ScheduleTask * task = it.next();
		task->current--;
		if (task->current == 0) {
			if (_todoTasks.size() < MAX_TODO_SCHEDULE_CALLBACK_SIZE) {
				_todoTasks.add(task);
			} else {
				Serial.println("Task is dropped");
			}

			if (task->loop) {
				task->current = task->time;
			} else {
				it.remove();
				delete task;
			}
		}
	}
}

ScheduleTask * Scheduler::schedule(Callback callback, long time) {
	schedule(callback, nullptr, time);
}

ScheduleTask * Scheduler::schedule(Callback callback, void * data, long time) {
	ScheduleTask * task = new ScheduleTask();
	task->callback = callback;
	task->time = time;
	task->current = time;
	task->loop = true;
	task->data = data;
	_scheduleTasks.add(task);
	return task;
}

ScheduleTask * Scheduler::scheduleOnce(Callback callback, long time) {
	scheduleOnce(callback, nullptr, time);
}

ScheduleTask * Scheduler::scheduleOnce(Callback callback, void * data, long time) {
	ScheduleTask * task = new ScheduleTask();
	task->callback = callback;
	task->time = time;
	task->current = time;
	task->loop = false;
	_scheduleTasks.add(task);
	return task;
}

bool Scheduler::unschedule(ScheduleTask * task) {
	LinkedListIterator it = _scheduleTasks.iterator();
	while (it.hasNext()) {
		if (task == it.next()) {
			it.remove();
			delete task;
			return true;
		}
	}
	return false;
}

void Scheduler::tick() {
	while (_todoTasks.size() > 0) {
		Node * node = _todoTasks.head();
		ScheduleTask * task = (ScheduleTask *) node->data;
		_todoTasks.remove(node);
		task->callback(task->data);
	}
}
