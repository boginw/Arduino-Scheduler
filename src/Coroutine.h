#define MAX_ROUTINES 10
#include <Arduino.h>
#include "Scheduler.h"

typedef struct routine
{
	void (*execute)(int);
	bool empty;
	int param;
} routine_t;

void setupManager();
int start(void (*func)(int), int param);
int kill(int routineIndex);
void handler(int routineIndex);
bool threadAvailable(int index);
