/*
 * ThreadPool.h
 *
 *  Created on: 4 мая 2020 г.
 *      Author: Kirych
 */

#ifndef THREADPOOL_H_
#define THREADPOOL_H_

#include <thread>
#include <mutex>
#include <queue>
#include <vector>
#include <functional>
#include <condition_variable>
#include <atomic>

class ThreadPool {
public:
	ThreadPool();
	ThreadPool(int size);
	virtual ~ThreadPool();

	const int getAmountOfThreads() { return the_threads.size(); }
	void Add_Job(std::function<void()> New_Job);
	const bool isSomnethingToDo();

private:
	std::atomic_bool terminate_pool;
	std::mutex Queue_Mutex;
	std::mutex Print_Mutex;
	std::mutex Termination_Mutex;
	std::queue<std::function<void()> > Queue;
	std::vector<std::thread> the_threads;
	std::condition_variable condition;

	void Loop_function();

};

#endif /* THREADPOOL_H_ */
