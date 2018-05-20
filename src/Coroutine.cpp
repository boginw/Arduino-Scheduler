#include "Coroutine.h"

bool stdFalse = false;
static unsigned long time = millis();

static routine_t routines[MAX_ROUTINES];
static int uuid = 1;

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
			routines[i].id = uuid++;
			return routines[i].id;
		}
	}

	return -1;
}

bool kill(int id)
{
	for(int i = 0; i < MAX_ROUTINES; i++)
	{
		if(routines[i].id == id && routines[i].active) {
			routines[i].active = false;
			routines[i].empty = true;
			return true;
		}
	}
	return false;
}

void handler(int index)
{
	if (routines[index].empty)
	{
		Scheduler.yield();
		return;
	}
	routines[index].execute(routines[index].param);
	routines[index].empty = true;
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