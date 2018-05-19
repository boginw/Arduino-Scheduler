#define MAX_ROUTINES 10
#include <Arduino.h>
#include "Scheduler.h"

typedef struct routine
{
	void (*execute)(void*);
	bool empty;
	bool active;
	void* param;
} routine_t;

bool* shouldDie(int id);
void setupManager();
int start(void (*func)(void*), void* param);
int kill(int id);
void handler(int id);
bool threadAvailable(int index);
