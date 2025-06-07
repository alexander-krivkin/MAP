#include <iostream>
#include <thread>
#include <mutex>


class Data
{
public:
	friend void swap1(Data& data1, Data& data2);
	friend void swap2(Data& data1, Data& data2);
	friend void swap3(Data& data1, Data& data2);

private:
	std::mutex mtx_{};
	int dataInt_{};
};


void swap1(Data& data1, Data& data2)
{
	std::lock(data1.mtx_, data2.mtx_);
	std::lock_guard<std::mutex> lock1(data1.mtx_, std::adopt_lock);
	std::lock_guard<std::mutex> lock2(data2.mtx_, std::adopt_lock);

	int tmp = data1.dataInt_;
	data1.dataInt_ = data2.dataInt_;
	data2.dataInt_ = tmp;
	std::this_thread::sleep_for(std::chrono::milliseconds(150));
	std::cout << "Swap1\n";
}

void swap2(Data& data1, Data& data2)
{
	std::unique_lock<std::mutex> lock1(data1.mtx_, std::defer_lock);
	std::unique_lock<std::mutex> lock2(data2.mtx_, std::defer_lock);
	std::lock(lock1, lock2);

	int tmp = data1.dataInt_;
	data1.dataInt_ = data2.dataInt_;
	data2.dataInt_ = tmp;
	std::this_thread::sleep_for(std::chrono::milliseconds(150));
	std::cout << "Swap2\n";
}

void swap3(Data& data1, Data& data2)
{
	std::scoped_lock lock(data1.mtx_, data2.mtx_);

	int tmp = data1.dataInt_;
	data1.dataInt_ = data2.dataInt_;
	data2.dataInt_ = tmp;
	std::this_thread::sleep_for(std::chrono::milliseconds(150));
	std::cout << "Swap3\n";
}


int main()
{
	Data data1{};
	Data data2{};

	std::thread thread1{ swap1, std::ref(data1), std::ref(data2) };
	std::thread thread2{ swap2, std::ref(data1), std::ref(data2) };
	std::thread thread3{ swap3, std::ref(data1), std::ref(data2) };
	std::thread thread4{ swap3, std::ref(data1), std::ref(data2) };
	std::thread thread5{ swap2, std::ref(data1), std::ref(data2) };
	std::thread thread6{ swap1, std::ref(data1), std::ref(data2) };

	thread1.join();
	thread2.join();
	thread3.join();
	thread4.join();
	thread5.join();
	thread6.join();

	return EXIT_SUCCESS;
}