#include "Coroutine.h"

static unsigned long time = millis();

static routine_t routines[MAX_ROUTINES];

void setupManager()
{
	for (int i = 0; i < MAX_ROUTINES; i++)
	{
		routines[i].empty = true;
		Scheduler.start(NULL, handler, i);
	}

	OCR0A = 0xAF;
	TIMSK0 |= _BV(OCIE0A);
}

int start(void (*func)(int), int param)
{
	for (int i = 0; i < MAX_ROUTINES; i++)
	{
		if (routines[i].empty)
		{
			routines[i].execute = func;
			routines[i].empty = false;
			routines[i].param = param;
			return i;
		}
	}

	return -1;
}

int kill(int routineIndex)
{
	// TODO: this
}

void handler(int routineIndex)
{
	if (routines[routineIndex].empty)
	{
		Scheduler.yield();
		return;
	}
	routines[routineIndex].execute(routines[routineIndex].param);
	routines[routineIndex].empty = true;
	Scheduler.yield();
}

bool threadAvailable(int index)
{
	return routines[index].empty;
}

SIGNAL(TIMER0_COMPA_vect)
{
	if (millis() - time >= 1)
	{
		time = millis();
		yield();
	}
}