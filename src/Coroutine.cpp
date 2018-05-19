#include "Coroutine.h"

bool stdFalse = false;
static unsigned long time = millis();

static routine_t routines[MAX_ROUTINES];

void setupManager()
{
	for (int i = 0; i < MAX_ROUTINES; i++)
	{
		routines[i].empty = true;
		routines[i].active = true;
		Scheduler.start(NULL, handler, i, &routines[i].active);
	}

	OCR0A = 0xAF;
	TIMSK0 |= _BV(OCIE0A);
}

int start(void (*func)(void*), void* param)
{
	for (int i = 0; i < MAX_ROUTINES; i++)
	{
		if (threadAvailable(i))
		{
			routines[i].execute = func;
			routines[i].empty = false;
			routines[i].param = param;
			routines[i].active = true;
			return i;
		}
	}

	return -1;
}

int kill(int id)
{
	routines[id].active = false;
	routines[id].empty = true;
}

void handler(int id)
{
	if (routines[id].empty)
	{
		Scheduler.yield();
		return;
	}
	routines[id].execute(routines[id].param);
	routines[id].empty = true;
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