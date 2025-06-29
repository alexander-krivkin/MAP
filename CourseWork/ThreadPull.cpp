#include "ThreadPull.h"


namespace ak
{
	ThreadPull::ThreadPull(int threadsCount)
	{
		stopped_ = false;
		std::cout << std::endl << threadsCount << " threads pull started" << std::endl << std::endl;

		for (int idx{}; idx < threadsCount; idx++)
		{
			threads_.emplace_back([this]() {work_(); });
		}
	}

	ThreadPull::~ThreadPull()
	{
	}

	void ThreadPull::submit(UniqueFunction function)
	{
		safeThreadQueue_.push(std::move(function));
	}

	void ThreadPull::stop()
	{
		stopped_ = true;
		for (auto& thread : threads_)
		{
			thread.detach();
		}
		std::cout << std::endl << "Thread pull stopped" << std::endl << std::endl;
	}

	void ThreadPull::work_()
	{
		while (true)
		{
			auto func = safeThreadQueue_.pop();

			std::unique_lock<std::mutex> ulMtx(state_.mtx_);
			std::stringstream sstr;
			sstr << "Task " << std::setw(3) << ++state_.count_ << " ran in thread " << std::setw(7) << std::this_thread::get_id();
			sstr << " |  ";
			ulMtx.unlock();
			func(sstr.str());

			if (stopped_) { break; }
		}
	}
}