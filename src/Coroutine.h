#define MAX_ROUTINES 10
#include <Arduino.h>
#include "Scheduler.h"

typedef struct routine
{
	void (*execute)(void*);
	int id;
	bool empty;
	bool active;
	void* param;
} routine_t;

void setupManager();
int start(void (*func)(void*), void* param);
bool kill(int id);
void handler(int index);
bool threadAvailable(int index);
