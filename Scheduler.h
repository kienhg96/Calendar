#ifndef __SCHEDULER_H__
#define __SCHEDULER_H__
#include "Collections.h"
#include <Arduino.h>
#include "Types.h"

#define MAX_TODO_SCHEDULE_CALLBACK_SIZE 100

struct ScheduleTask {
	long time;
	long current;
	bool loop;
	void * data;
	Callback callback;
};

class Scheduler {
private:
	static Scheduler * _instance;
	Scheduler();
public:
	static Scheduler * instance();
	~Scheduler();
public:
	void init();
	void emit();

	ScheduleTask * schedule(Callback callback, long time);
	ScheduleTask * schedule(Callback callback, void * data, long time);
	ScheduleTask * scheduleOnce(Callback callback, long time);
	ScheduleTask * scheduleOnce(Callback callback, void * data, long time);
	bool unschedule(ScheduleTask * task);
	void tick();
private:
	LinkedList _scheduleTasks;
	LinkedList _todoTasks;
};

#endif
