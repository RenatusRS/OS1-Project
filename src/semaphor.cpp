#include "helper.h"

Semaphore::Semaphore(int init) {
	lock;
	myImpl = new KernelSem(init);
	unlock;
}

Semaphore::~Semaphore() {
	lock;
	delete myImpl;
	unlock;
}

int Semaphore::wait(Time maxTimeToWait) {
	return myImpl->wait(maxTimeToWait);
}

void Semaphore::signal() {
	myImpl->signal();
}

int Semaphore::val() const {
	return myImpl->val;
}
