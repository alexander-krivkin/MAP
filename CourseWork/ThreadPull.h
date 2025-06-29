#pragma once

#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
#include <thread>
#include <functional>
#include <future>
#include <chrono>

#include "SafeThreadQueue.h"


namespace ak
{
	//using UniqueFunction = std::packaged_task<void()>;
	using UniqueFunction = std::function<void(std::string)>;

	class ThreadPull final
	{
	public:
		ThreadPull(int threadsCount = 1);
		~ThreadPull();

		void submit(UniqueFunction function);
		void stop();

	private:
		void work_();

		std::vector<std::jthread> threads_{};
		SafeThreadQueue<UniqueFunction> safeThreadQueue_{};
		bool stopped_{ true };
		struct
		{
			int count_{};
			mutable std::mutex mtx_{};
		} state_;
	};
}