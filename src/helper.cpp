#include "helper.h"

volatile int locks = 0;
volatile bool call = false;
volatile bool contextReady = false;

int main(int argc, char **argv) {
	PCB::running = new PCB();

	inic();
	UserThread user(argc, argv);
	user.start();
	user.waitToComplete();
	restore();

	return 0;
}
