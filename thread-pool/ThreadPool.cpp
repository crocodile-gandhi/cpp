/*
 * ThreadPool.cpp
 *
 *  Created on: 4 мая 2020 г.
 *      Author: Kirych
 */

#include "ThreadPool.h"
#include <stdio.h>


ThreadPool::ThreadPool():
terminate_pool(ATOMIC_VAR_INIT(false))
{
	int size = std::thread::hardware_concurrency();
	for (int i = 0; i < size; ++i )
		the_threads.push_back( std::thread(&ThreadPool::Loop_function, this) );
}

ThreadPool::ThreadPool(int proposed_size):
terminate_pool(ATOMIC_VAR_INIT(false))
{
	for (int i = 0; i < proposed_size; ++i )
		the_threads.push_back( std::thread(&ThreadPool::Loop_function, this) );
}

ThreadPool::~ThreadPool() {
	{
		//std::unique_lock<std::mutex> lock(Queue_Mutex);
		terminate_pool = true;
	}
	condition.notify_all();

	fprintf(stdout, "Joining threads\n");
	for(std::thread &some_thread : the_threads) {
		some_thread.join();
	}

	the_threads.clear();
	fprintf(stdout, "Thread pool has been destroyed\n");
}

void ThreadPool::Loop_function() {

    while(!terminate_pool) // some mutex?
    {
    	std::function<void()> Job;
        {
            std::unique_lock<std::mutex> lock(Queue_Mutex);

            condition.wait(lock, [this](){return !this->Queue.empty() || this->terminate_pool;});
            if (!terminate_pool) {
            	Job = Queue.front();
            	Queue.pop();
            }
        }
        if (!terminate_pool) {
        	std::lock_guard<std::mutex> guard(Print_Mutex);
        	Job();
        }
    }
    std::lock_guard<std::mutex> guard(Print_Mutex);
    fprintf(stdout, "Exiting from thread\n");
}

void ThreadPool::Add_Job(std::function<void()> New_Job)
{
    {
        std::unique_lock<std::mutex> lock(Queue_Mutex); // cannot get the lock after the class initialization
        Queue.push(New_Job);
    }
    condition.notify_one();
}

const bool ThreadPool::isSomnethingToDo() {
	return !Queue.empty() || terminate_pool;
}
