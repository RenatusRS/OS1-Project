#ifndef _uthread_h_
#define _uthread_h_

#include "helper.h"

int userMain(int argc, char **argv);

class UserThread : public Thread {
public:
	int argc;
	char **argv;
	int res;

	UserThread(int argc, char **argv) : Thread(), argc(argc), argv(argv), res(0) {}

	~UserThread() { waitToComplete(); }

	void run() {
		res = userMain(argc, argv);
	}

	Thread *clone() const {
		return new UserThread(argc, argv);
	}

};

#endif
