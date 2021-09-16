#include "helper.h"
#define Test9S

#ifdef Test1
/*
	Test: cekanje niti
*/

class TestThread : public Thread
{
public:

	TestThread(): Thread() {};
	~TestThread()
	{
		waitToComplete();
	}
protected:

	void run();

};

void TestThread::run()
{
	syncPrintf("Thread %d: loop1 starts\n", getId());

	for(int i=0;i<32000;i++)
	{
		for (int j = 0; j < 32000; j++);
	}

	syncPrintf("Thread %d: loop1 ends, dispatch\n",getId());

	dispatch();

	syncPrintf("Thread %d: loop2 starts\n",getId());

	for (int k = 0; k < 20000; k++);

	syncPrintf("Thread %d: loop2 ends\n",getId());


}

class WaitThread: public Thread
{
private:
	TestThread *t1_,*t2_;

public:
	WaitThread(TestThread *t1, TestThread *t2): Thread()
	{
		t1_ = t1;
		t2_ = t2;
	};

	~WaitThread()
		{
			waitToComplete();
		}

protected:

	void run()
	{
		syncPrintf("Starting tests...\n");
		t1_->waitToComplete();
		syncPrintf("Test 1 completed!\n");
		t2_->waitToComplete();
		syncPrintf("Test 2 completed!\n");
	}
};

void tick() {}

int userMain(int argc, char** argv)
{
	syncPrintf("User main starts\n");
	TestThread t1,t2;
	WaitThread w(&t1,&t2);
	t1.start();
	t2.start();
	w.start();
	syncPrintf("User main ends\n");
	return 16;
}
#endif

#ifdef Test2
/*
	Test: asinhrono preuzimanje
*/

class TestThread : public Thread
{
public:

	TestThread(): Thread() {};
	~TestThread()
	{
		waitToComplete();
	}
protected:

	void run();

};

void TestThread::run()
{
	syncPrintf("Thread %d: loop1 starts\n", getId());

	for(int i=0;i<32000;i++)
	{
		for (int j = 0; j < 32000; j++);
	}

	syncPrintf("Thread %d: loop1 ends, dispatch\n",getId());

	dispatch();

	//syncPrintf("Thread %d: loop2 starts\n",getId());

	for (int k = 0; k < 20000; k++);

	syncPrintf("Thread %d: loop2 ends\n",getId());

}

void tick(){}

int userMain(int argc, char** argv)
{
	syncPrintf("User main starts\n");
	TestThread t1,t2;
	t1.start();
	t2.start();
	syncPrintf("User main ends\n");
	return 16;
}
#endif

#ifdef Test3
/*
	Test: dohvatanje i pretraga po ID
*/

const int n = 16;

void tick(){}

class TestThread : public Thread
{
public:

	TestThread(): Thread(4096,2) {};
	~TestThread()
	{
		waitToComplete();
	}
protected:

	void run();

};

void TestThread::run()
{

	int buffer=2;

	for(int i=0;i<32000;i++)
	{
		buffer = 4096/2048;
		for (int j = 0; j < 1024; j++)
		{
			buffer = buffer*2;
			if(buffer%2)
				buffer = 2;
		}
	}

}

int userMain(int argc, char** argv)
{
	syncPrintf("Test starts: %d threads.\n",n);
	int i;
	TestThread threads[n];
	for(i=0;i<n;i++)
	{
		threads[i].start();
	}
	for(i=0;i<n;i++)
	{
		threads[i].waitToComplete();
		syncPrintf("%d. Done!\n",Thread::getThreadById(i+2)->getId());
	}
	syncPrintf("Test ends.\n");
	return 0;
}
#endif

#ifdef Test4
volatile Time ts;

void tick()
{
	syncPrintf("timeSlice=%d\n",ts);
}

class TestThread : public Thread
{
private:
	Time myTimeSlice;
public:

	TestThread(StackSize stackSize, Time timeSlice): Thread(stackSize,timeSlice), myTimeSlice(timeSlice) {};
	~TestThread()
	{
		waitToComplete();
	}
protected:

	void run();

};

void TestThread::run()
{
	for(unsigned i=0;i<32000;i++)
	{
		for(unsigned int j=0;j<32000;j++)
		{
			ts = myTimeSlice;
		}
	}
}

int userMain(int argc, char** argv)
{
	syncPrintf("Test starts.\n");
	TestThread t1(64,1), t2(4096,32), t3(1024,16), t4(4096,0);
	t1.start();
	t2.start();
	t3.start();
	t4.start();
	t1.waitToComplete();
	t2.waitToComplete();
	t3.waitToComplete();
	t4.waitToComplete();
	syncPrintf("Test ends.\n");
	return 0;
}
#endif

#ifdef Test5
/*
 	 Test: frekventni dispatch
*/

const int n = 10;

void tick(){}

class TestThread : public Thread
{
public:

	TestThread(): Thread(){}
	~TestThread()
	{
		waitToComplete();
	}
protected:

	void run();

};

void TestThread::run()
{
	for(int i=0;i<32000;i++)
	{
		for(int j=0;j<16;j++){
		dispatch();
		syncPrintf(".");
		}
	}
}

int userMain(int argc, char** argv)
{
	syncPrintf("Test starts.\n");
	TestThread t[n];
	int i;
	for(i=0;i<n;i++)
	{
		t[i].start();
	}
	for(i=0;i<n;i++)
	{
		t[i].waitToComplete();
	}
	syncPrintf("Test ends.\n");
	return 0;
}
#endif

#ifdef Test6
/*
 	 Test: idleThread
*/

void tick(){}

class TestThread : public Thread
{
private:
	TestThread *t;

public:

	TestThread(TestThread *thread): Thread(), t(thread){}
	~TestThread()
	{
		waitToComplete();
	}
protected:

	void run();

};

void TestThread::run()
{
	t->waitToComplete();
}

int userMain(int argc, char** argv)
{
	syncPrintf("Test starts.\n");
	TestThread *t1,*t2;
	t1 = new TestThread(t2);
	t2 = new TestThread(t1);
	t1->start();
	t2->start();
	delete t1;
	delete t2;
	syncPrintf("Test ends.\n");
	return 0;
}
#endif

#ifdef Test7
/*
 	 Test: Semafori bez realizacije spavanja, deo javnog testa
*/

const int n = 10;
int count = 100;
Semaphore mutex(1);

int mutex_glb = 1;

void mutex_glb_wait(){
 sloop:asm{
	mov ax, 0
	xchg ax, mutex_glb
}
 if(_AX ==0){
	dispatch();
	asm jmp sloop;
 }
}

#define intLock mutex_glb_wait();
#define intUnlock mutex_glb = 1;

class BoundedBuffer {
public:

	BoundedBuffer (unsigned size);
	virtual ~BoundedBuffer ();
	int append (char);
	char take ();
	int  fullCount(){return itemAvailable.val();};    // potrebno consumeru

private:
	unsigned Size;
	Semaphore mutexa, mutext;
	Semaphore spaceAvailable, itemAvailable;
	char* buffer;
	int head, tail;

};

BoundedBuffer::BoundedBuffer (unsigned size) : Size(size),
	mutexa(1), mutext(1), spaceAvailable(size), itemAvailable(0),
	head(0), tail(0) {
		buffer = new char[size];
		if (!buffer) exit(1);
	}

BoundedBuffer::~BoundedBuffer(){
	intLock
	delete [] buffer;
	intUnlock
}

int BoundedBuffer::append (char d) {
	spaceAvailable.wait(0);
	mutexa.wait(0);
		buffer[tail] = d;
		tail = (tail+1)%Size;
	mutexa.signal();
	itemAvailable.signal();
	return 0;
}

char BoundedBuffer::take () {
	itemAvailable.wait(0);
	mutext.wait(0);
		char d = buffer[head];
		head = (head+1)%Size;
	mutext.signal();
	spaceAvailable.signal();
	return d;
}

BoundedBuffer buffer(n/2);

class Producer: public Thread
{
public:
	Producer(): Thread() {};
	~Producer()
	{
		waitToComplete();
	}

protected:
	void run()
	{
		char c;
		while(count>0)
		{
			c = 65+(rand()%25);
			buffer.append(c);
			syncPrintf("%d. Producer %d puts '%c'\n",count,Thread::getRunningId(),c);
			mutex.wait(0);
			count--;
			mutex.signal();
		}
	}
};

class Consumer: public Thread
{
public:
	Consumer(): Thread() {};
	~Consumer()
	{
		waitToComplete();
	}

protected:
	void run()
	{
		char c;
		while(count>0)
		{
			c = buffer.take();
			syncPrintf("%d. Consumer %d gets '%c'\n",count,Thread::getRunningId(),c);
			mutex.wait(0);
			count--;
			mutex.signal();
		}
	}
};

void tick(){}

int userMain(int argc, char** argv)
{
	syncPrintf("Test starts.\n");
	Producer p;
	Consumer c;
	p.start();
	c.start();
	syncPrintf("Test ends.\n");
	return 0;
}
#endif

#ifdef Test8
/*
 	 Test: Semafori bez realizacije spavanja
*/

const int n = 5;
int count = 10;

Semaphore s(2);

class TestThread : public Thread
{
public:

	TestThread(): Thread(){}
	~TestThread()
	{
		waitToComplete();
	}
protected:

	void run();

};

void TestThread::run()
{
	s.wait(0);
	cout<<"Thread "<<getId()<<" in critical section."<<endl;
	for(unsigned int i=0;i<64000;i++)
		for(unsigned int j=0;j<64000;j++);
	s.signal();
}

void tick(){}

int userMain(int argc, char** argv)
{
	syncPrintf("Test starts.\n");
	TestThread t[n];
	int i;
	for(i=0;i<n;i++)
	{
		t[i].start();
	}
	for(i=0;i<n;i++)
	{
		t[i].waitToComplete();
	}
	syncPrintf("Test ends.\n");
	return 0;
}
#endif

#ifdef Test9
/*
 	 Test: Semafori sa spavanjem
*/

const int n = 1;
int t=-1;

Semaphore s(0);

class TestThread : public Thread
{
public:

	TestThread(): Thread(){}
	~TestThread()
	{
		waitToComplete();
	}
protected:

	void run();

};

void TestThread::run()
{
	syncPrintf("Thread waits for 10 units of time...\n");
	t=0;
	s.wait(10);
	syncPrintf("Thread finished.\n");
	s.signal();
}

void tick()
{
	t++;
	if(t)
		syncPrintf("%d\n",t);
}

int userMain(int argc, char** argv)
{
	syncPrintf("Test starts.\n");
	TestThread t[n];
	int i;
	for(i=0;i<n;i++)
	{
		t[i].start();
	}
	for(i=0;i<n;i++)
	{
		t[i].waitToComplete();
	}
	syncPrintf("Test ends.\n");
	return 0;
}
#endif

#ifdef Test10
/*
 	 Test: Semafori sa spavanjem 2
*/

int t=-1;

Semaphore s(0);

class TestThread : public Thread
{
private:
	Time waitTime;

public:

	TestThread(Time WT): Thread(), waitTime(WT){}
	~TestThread()
	{
		waitToComplete();
	}
protected:

	void run();

};

void TestThread::run()
{
	syncPrintf("Thread %d waits for %d units of time.\n",getId(),waitTime);
	int r = s.wait(waitTime);
	s.signal();
	syncPrintf("Thread %d finished: r = %d\n", getId(),r);
}

void tick()
{
	t++;
	if(t)
		syncPrintf("%d\n",t);
}

int userMain(int argc, char** argv)
{
	syncPrintf("Test starts.\n");
	TestThread t1(15),t2(3),t3(30);
	t1.start();
	t2.start();
	t3.start();
	s.wait(5);
	s.signal();
	s.wait(0);
	s.signal();
	syncPrintf("Test ends.\n");
	return 0;
}
#endif

#ifdef Test11
/*
 	 Test: Semafori sa spavanjem 3
*/

int t=-1;

Semaphore s(0);

class TestThread : public Thread
{
private:
	Time waitTime;

public:

	TestThread(Time WT): Thread(), waitTime(WT){}
	~TestThread()
	{
		waitToComplete();
	}
protected:

	void run();

};

void TestThread::run()
{
	syncPrintf("Thread %d waits for %d units of time.\n",getId(),waitTime);
	int r = s.wait(waitTime);
	s.signal();
	syncPrintf("Thread %d finished: r = %d\n", getId(),r);
}

void tick()
{
	/*
	t++;
	if(t)
		syncPrintf("%d\n",t);
		*/
}


#include <iostream.h>

Semaphore* mutex = 0;

class Znak : public Thread
{
public:
	Znak(char znak, int n) : Thread(), znak(znak), n(n) {}
	virtual ~Znak() { waitToComplete(); }

	void run()
	{
		for (int i = 0; i < 30000; i++)
		{
			if (!mutex->wait(n)) {
				//cout << znak;
				mutex->signal();
			}


		}

		if (!mutex->wait(n)) {
			cout << endl << znak << " finished" << endl;
			mutex->signal();
		}
	}

private:
	char znak;
	int n;

};


int userMain(int argc, char* argv[]) {
	mutex = new Semaphore(1);

	Znak* a = new Znak('a', 10);
	Znak* b = new Znak('b', 15);
	Znak* c = new Znak('c', 30);

	a->start();
	b->start();
	c->start();

	delete a;
	delete b;
	delete c;

	if (mutex->wait(1)) {
		cout << endl << "userMain finished" << endl;
		mutex->signal();
	}

	delete mutex;



	return 0;
}
#endif

#ifdef Test12
/*
 	 Test: Semafori sa spavanjem 4
*/

int t=-1;

const int n=15;

Semaphore s(1);

class TestThread : public Thread
{
private:
	Time waitTime;

public:

	TestThread(Time WT): Thread(), waitTime(WT){}
	~TestThread()
	{
		waitToComplete();
	}
protected:

	void run();

};

void TestThread::run()
{
	syncPrintf("Thread %d waits for %d units of time.\n",getId(),waitTime);
	int r = s.wait(waitTime);
	if(getId()%2)
		s.signal();
	syncPrintf("Thread %d finished: r = %d\n", getId(),r);
}

void tick()
{
	t++;
	if(t)
		syncPrintf("%d\n",t);
}

int userMain(int argc, char** argv)
{
	syncPrintf("Test starts.\n");
	TestThread* t[n];
	int i;
	for(i=0;i<n;i++)
	{
		t[i] = new TestThread(5*(i+1));
		t[i]->start();
	}
	for(i=0;i<n;i++)
	{
		t[i]->waitToComplete();
	}
	delete t;
	syncPrintf("Test ends.\n");
	return 0;
}
#endif

#ifdef Test13
/*
	Test: dogadjaji
*/

#include <iostream.h>
#include <stdlib.h>

PREPAREENTRY(9,1);

Semaphore* mutex = 0;
Semaphore* sleepSem = 0;

void tick() {}

class Znak : public Thread
{
public:
	Znak(char znak, int n) : Thread(), znak(znak), n(n) {}
	virtual ~Znak() { waitToComplete(); }

	void run()
	{
		// for (long i = 0; i < 100000; i++)
		for (int i = 0; i < n; i++)
		{
			if (!mutex->wait(1)) {
				cout << znak;
				mutex->signal();
			}

			// for (int j = 0; j < 10000; j++)
				// for (int k = 0; k < 10000; k++);
			Time sleepTime = 2 + rand() % 8;
			sleepSem->wait(sleepTime);

			// dispatch();
		}

		if (!mutex->wait(1)) {
			cout << endl << znak << " finished" << endl;
			mutex->signal();
		}
	}

private:
	char znak;
	int n;

};

class Key : public Thread {
public:
	Key(int n) : Thread(), n(n) {}
	virtual ~Key() { waitToComplete(); }

	void run() {
		Event e(9);

		for (int i = 0; i < n; i++) {
			if (!mutex->wait(1)) {
				cout << endl << "key waiting " << (i + 1) << endl;
				mutex->signal();
			}

			e.wait();

			if (!mutex->wait(1)) {
				cout << endl << "key continue " << (i + 1) << endl;
				mutex->signal();
			}

			//sleepSem->wait(5);
		}

		if (!mutex->wait(1)) {
			cout << endl << "key finished" << endl;
			mutex->signal();
		}
	}

private:
	int n;

};

int userMain(int argc, char* argv[]) {
	mutex = new Semaphore(1);
	sleepSem = new Semaphore(0);

	Znak* a = new Znak('a', 10);
	Znak* b = new Znak('b', 15);
	Znak* c = new Znak('c', 40);
	Key* k = new Key(100);

	a->start();
	b->start();
	c->start();
	k->start();

	delete a;
	delete b;
	delete c;
	delete k;

	if (mutex->wait(1)) {
		cout << endl << "userMain finished" << endl;
		mutex->signal();
	}

	delete sleepSem;
	delete mutex;

	return 0;
}
#endif

#ifdef Test14
/*
	Test: brojac sekundi
*/

unsigned t=18;
unsigned seconds = 5;

void secondPast()
{
	if(seconds)
		syncPrintf("%d\n",seconds);
	seconds--;
}

void tick()
{
	t--;
	if(t==0){
		t = 18;
		secondPast();
	}
}

int userMain(int argc, char** argv)
{
	syncPrintf("Starting test\n");
	while(seconds);
	syncPrintf("Test ends\n");
	return 0;
}
#endif

#ifdef Test15
/*
	Test: Niti maxStack velicine
*/

const int n = 12;

void tick(){}

class TestThread : public Thread
{
public:

	TestThread(): Thread(32768,2) {};
	~TestThread()
	{
		waitToComplete();
	}
protected:

	void run();

};

void TestThread::run()
{

	int buffer=2;

	for(int i=0;i<32000;i++)
	{
		buffer = 4096/2048;
		for (int j = 0; j < 1024; j++)
		{
			buffer = buffer*2;
			if(buffer%2)
				buffer = 2;
		}
	}

}

int userMain(int argc, char** argv)
{
	syncPrintf("Test starts: %d threads.\n",n);
	int i;
	TestThread threads[n];
	for(i=0;i<n;i++)
	{
		threads[i].start();
	}
	for(i=0;i<n;i++)
	{
		threads[i].waitToComplete();
		syncPrintf("%d. Done!\n",Thread::getThreadById(i+2)->getId());
	}
	syncPrintf("Test ends.\n");
	return 0;
}
#endif

#ifdef Test3S
/**
 * USER3.CPP
 *
 * Tests threads which simply echo the message passed a specified amount of
 * times in specified intervals.
 */
#include <helper.h>

 void dumbSleep(int delay) {
      for (int i = 0; i < 1000; ++i) {
          for (int j = 0; j < delay; ++j);
      }
  }

class EchoThread : public Thread {
    public:
        EchoThread(const char* message, unsigned times=1, unsigned delay=1) :
            Thread(1024, 20), message(message), times(times), delay(delay) {}
        virtual void run();
        ~EchoThread() {
            waitToComplete();
        }
    private:
        const char* message;
        unsigned times;
        unsigned delay;
};

void EchoThread::run() {
    syncPrintf("First message from thread %d\n", getId());
    for (unsigned i = 0; i < times; ++i) {
        syncPrintf("%s from thread %d\n", message, getId());
        dumbSleep(delay * 1000);
    }
    syncPrintf("Last message from thread %d\n", getId());
}

void tick() {}

int userMain(int argc, char* argv[]) {
    (void) argc;
    (void) argv;
    lock;
    EchoThread** threads = new EchoThread*[256];
    unlock;
    for (unsigned i = 0; i < 256; ++i) {
        lock;
        threads[i] = new EchoThread("ECHO", i % 4, i % 10);
        unlock;
        threads[i]->start();
        if (i % 20 == 0) {
            syncPrintf("Dispatching main\n");
            dispatch();
        }
    }
    for (unsigned j = 0; j < 256; ++j) {
        // We cannot wait for a thread to complete while interrupts are locked!
        threads[j]->waitToComplete();
        lock;
        delete threads[j];
        unlock;
    }
    lock;
    delete[] threads;
    unlock;
    return 0;
}

#endif

#ifdef Test5S
/**
 * USER5.CPP
 *
 * Tests whether a simple producer-consumer chain works.
 */
#include <helper.h>

Semaphore sem;

  void dumbSleep(int delay) {
      for (int i = 0; i < 1000; ++i) {
          for (int j = 0; j < delay; ++j);
      }
  }

class Producer : public Thread {
    public:
        Producer() : Thread(1024, 10) {}
        virtual void run();
        ~Producer() {
            waitToComplete();
        }
};

void Producer::run() {
    while (true) {
        syncPrintf("Prodooc\n");
        sem.signal();
        dumbSleep(rand() % 1000);
    }
}

class Consumer : public Thread {
    public:
        Consumer() : Thread(1024, 10) {}
        virtual void run();
        ~Consumer() {
            waitToComplete();
        }
};

void Consumer::run() {
    while (true) {
        int waitResult = sem.wait(0);
        if (waitResult) {
            syncPrintf("Consoomed %d\n", getId());
        } else {
            syncPrintf("CONSOOM FAILED %d\n", getId());
        }
        dumbSleep(rand() % 1000);
    }
}

void tick() {}

int userMain(int argc, char* argv[]) {
    (void) argc;
    (void) argv;
    Producer p;
    Consumer c[10];
    lock;
    p.start();
    for (unsigned i = 0; i < 10; ++i) {
        c[i].start();
    }
    unlock;
    return 0;
}

#endif

#ifdef Test6S
/**
 * USER6.CPP
 *
 * Tests whether a simple keyboard event listener works.
 */
#include <helper.h>

PREPAREENTRY(0x9, true)

void tick() {}

int userMain(int argc, char* argv[]) {
    (void) argc;
    (void) argv;
    Event evt(0x9);
    for (unsigned i = 0; i < 64; ++i) {
        syncPrintf("Key %d\n", i);
        evt.wait();
    }
    return 0;
}

#endif

#ifdef Test7S
/**
 * USER7.CPP
 *
 * Creates the maximum amount of threads possible within the system memory
 * and frees them, hoping nothing will go wrong.
 */
#include <helper.h>

class OveruseThread : public Thread {
    public:
        OveruseThread() : Thread(1024, 20) {}
        virtual void run() {
            syncPrintf("This should not happen.\n");
        }
        ~OveruseThread() {
            waitToComplete();
        }
};

void tick() {}

Thread* threads[500];

int userMain(int argc, char* argv[]) {
    (void) argc;
    (void) argv;
    unsigned i = 0;
    for (; i < 500; ++i) {
        syncPrintf("Creating %d\n", i);
        lock;
        threads[i] = new OveruseThread();
        unlock;
        if (threads[i] == nullptr || threads[i]->getId() == -1) {
            syncPrintf("Failed at index %d\n", i);
            if (threads[i] != nullptr) {
                lock;
                delete threads[i];
                unlock;
            }
            break;
        }
    }
    for (unsigned j = 0; j < i; ++j) {
        lock;
        delete threads[j];
        unlock;
    }
    syncPrintf("Done\n");
    return 0;
}

#endif

#ifdef Test8S
/**
 * USER8.CPP
 *
 * Waits for a few threads to exit and then explicitly exits the program.
 */
#include <helper.h>

  void dumbSleep(int delay) {
      for (int i = 0; i < 1000; ++i) {
          for (int j = 0; j < delay; ++j);
      }
  }

class ExitThread : public Thread {
    public:
        ExitThread() : Thread(1024, 1) {}
        virtual void run();
        ~ExitThread() {
            waitToComplete();
        }
};

void ExitThread::run() {
    syncPrintf("Thread %d sleeping\n", getId());
    dumbSleep(rand() % 1000);
    syncPrintf("Thread %d exiting\n", getId());
    exit();
}

void tick() {}

int userMain(int argc, char* argv[]) {
    (void) argc;
    (void) argv;
    ExitThread threads[20];
    for (unsigned i = 0; i < 20; ++i) {
        threads[i].start();
    }
    dispatch();
    dispatch();
    dispatch();
    dispatch();
    syncPrintf("You've had your chance!\n");
    Thread::exit();
    return 0;
}

#endif

#ifdef Test9S
/**
 * USER9.CPP
 *
 * Creates as many child threads as possible and then exits them all.
 * Child threads are never actually removed from memory, only terminated.
 */
#include <helper.h>

  void dumbSleep(int delay) {
      for (int i = 0; i < 1000; ++i) {
          for (int j = 0; j < delay; ++j);
      }
  }

class ForkThread : public Thread {
    public:
        ForkThread() : Thread(1024, 1) {}
        virtual void run();
        virtual Thread* clone() const {
            return new ForkThread();
        }
        ~ForkThread() {
            waitToComplete();
        }
        static volatile int failedFork;
};

volatile int ForkThread::failedFork = false;

void ForkThread::run() {
    while (!failedFork) {
        ID forked = fork();
        if (forked < 0) {
            syncPrintf("Failed to fork in thread %d!\n", getRunningId());
            failedFork = true;
            break;
        } else if (forked == 0) {
            syncPrintf("We are in child %d\n", getRunningId());
        } else {
            syncPrintf("Cloned thread: %d\n", forked);
            dumbSleep(10000);
        }
    }
    waitForForkChildren();
}

void tick() {}

int userMain(int argc, char* argv[]) {
    (void) argc;
    (void) argv;
    ForkThread t;
    t.start();
    return 0;
}

#endif

#ifdef Test10S
/**
 * USER10.CPP
 *
 * Creates as many child threads as possible and then exits them all.
 * Same as USER9.CPP but forks the main thread instead.
 */
#include <helper.h>

volatile int failedFork = false;

void tick() {}

int userMain(int argc, char* argv[]) {
    (void) argc;
    (void) argv;
    while (!failedFork) {
        ID forked = Thread::fork();
        if (forked < 0) {
            syncPrintf("Failed to fork in thread %d!\n", Thread::getRunningId());
            failedFork = true;
            break;
        } else if (forked == 0) {
            syncPrintf("We are in child %d\n", Thread::getRunningId());
        } else {
            syncPrintf("Cloned thread: %d\n", forked);
        }
    }
    Thread::waitForForkChildren();
    return 0;
}

#endif

#ifdef Test11S
/**
 * USER11.CPP
 *
 * Producer/consumer example but with fork().
 */
#include <helper.h>

void tick() {}

Semaphore sem;

  void dumbSleep(int delay) {
      for (int i = 0; i < 1000; ++i) {
          for (int j = 0; j < delay; ++j);
      }
  }

void produce() {
    while (true) {
        syncPrintf("Prodooc\n");
        sem.signal();
        dumbSleep(rand() % 1000);
    }
}

void consume() {
    while (true) {
        int waitResult = sem.wait(0);
        if (waitResult) {
            syncPrintf("Consoomed %d\n", Thread::getRunningId());
        } else {
            syncPrintf("CONSOOM FAILED %d\n", Thread::getRunningId());
        }
        dumbSleep(rand() % 1000);
    }
}

class ForkThread : public Thread {
    public:
        ForkThread() : Thread(1024, 1) {}
        ~ForkThread() {
            waitToComplete();
        }
        virtual Thread* clone() const {
            return new ForkThread();
        }
        virtual void run();
};

void ForkThread::run() {
    ID lastId;
    for (unsigned i = 0; i < 11; ++i) {
        lastId = fork();
        if (lastId < 0) {
            syncPrintf("Failed to fork on index %u!\n", lastId);
            return;
        } else if (lastId > 0) {
            if (i == 0) {
                produce();
            } else {
                consume();
            }
        }
    }
    waitForForkChildren();
}

int userMain(int argc, char* argv[]) {
    (void) argc;
    (void) argv;
    ForkThread t;
    t.start();
    return 0;
}

#endif

#ifdef Test12S
/**
 * USER12.CPP
 *
 * 3rd task on first colloquium, march 2015.
 */
#include <helper.h>

void tick() {}

void writer(char* c, int* flag) {
    while (1) {
        while (*flag == 1);
        cin >> (*c);
        *flag = 1;
    }
}

void reader(char* c, int* flag) {
    while (1) {
        while (*flag == 0);
        cout << (*c);
        *flag = 0;
    }
}

void pipe() {
    static char c;
    static int flag = 0;
    if (Thread::fork()) {
        writer(&c,&flag);
    } else {
        reader(&c,&flag);
    }
}

class ForkThread : public Thread {
    public:
        ForkThread() : Thread(1024, 1) {}
        ~ForkThread() {
            waitToComplete();
        }
        virtual Thread* clone() const {
            return new ForkThread();
        }
        virtual void run() {
            pipe();
        }
};

int userMain(int argc, char* argv[]) {
    (void) argc;
    (void) argv;
    ForkThread t;
    t.start();
    return 0;
}

#endif


int main(int argc, char **argv) {

	PCB::running = new PCB();

	inic();

	UserThread user(argc, argv);

	user.start();

	int ret = user.res;
	user.waitToComplete();

	restore();

	return 0;
}
