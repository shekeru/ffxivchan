#pragma once
#include <chrono>
#include <functional>
#include <thread>

using namespace std;

class Interval {
	thread thread_obj;
	bool ending = false;
public:
	void Start(function<void(void)> func, int interval) {
		thread_obj = thread([&]() {
			while (!ending) {
				auto delta = chrono::steady_clock::now() 
					+ chrono::milliseconds(interval);
				func(); this_thread::sleep_until(delta);
			}
		}); thread_obj.detach();
	} void Stop() {
		ending = true; thread_obj.~thread();
	}
};