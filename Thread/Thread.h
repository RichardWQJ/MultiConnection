#ifndef THREAD_H_
#define THREAD_H_

#include <iostream>
#include <thread>
#include <atomic>
#include <functional>

class Thread
{
public:
	Thread();
	virtual ~Thread();

	void start();
	std::thread::id getId();
	void interrupt();
	bool isInterrupted();
	void join();
	virtual void run();

private:
	std::atomic<bool> isInterript;
	std::thread th;
};

#endif /* THREAD_H_ */