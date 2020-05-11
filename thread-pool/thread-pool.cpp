
#include <stdio.h>
#include <chrono>
#include <thread>

#include <pthread.h>
#include <iostream>
#include <iomanip>
#include <ctime>
#include <chrono>

#include "ThreadPool.h"

void print_message() {

	pthread_t thId = pthread_self();
	//fprintf(stdout, "Thread %ul is working!\n", thId);
	/*auto current_time = std::chrono::high_resolution_clock::now();
	std::time_t now_c = std::chrono::system_clock::to_time_t(current_time);
	std::cout << std::put_time(std::localtime(&now_c), "%F %T") << ": Thread " << thId << " is working!" << std::endl;*/

	auto now = std::chrono::high_resolution_clock::now();
	auto ms = std::chrono::duration_cast<std::chrono::microseconds>(now.time_since_epoch()) % 1000000;
	auto timer = std::chrono::system_clock::to_time_t(now);
	std::tm bt = *std::localtime(&timer);
	std::ostringstream oss;

    oss << std::put_time(&bt, "%H:%M:%S"); // HH:MM:SS
    oss << '.' << std::setfill('0') << std::setw(3) << ms.count();

	std::cout << std::put_time(std::localtime(&timer), "%F") << " " << oss.str() << ": Thread " << thId << " is working!" << std::endl;
}

int main(int argc, char **argv) {

	fprintf(stdout, "Started\n");

	{
		ThreadPool pool;

		fprintf(stdout, "Thread pool initialized with %d threads\n", pool.getAmountOfThreads());

		for(int i=0; i<10; ++i)
			pool.Add_Job(print_message);

		std::this_thread::sleep_for(std::chrono::seconds(2));
	}
	return 0;
}

