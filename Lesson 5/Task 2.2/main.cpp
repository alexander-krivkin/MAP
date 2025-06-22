#include <iostream>


int calc(int arr[], int size, int start)
{
	int left{}, right{ size - 1 }, middle{};

	while (left <= right)
	{
		middle = (left + right) / 2;
		if (start < arr[middle])
			right = middle - 1;
		else if (start >= arr[middle])
			left = middle + 1;
	}

	return size - right - 1;
}


int main()
{
	int arr[]{ 14, 16, 19, 32, 32, 32, 56, 69, 72 };
	int start{};

	std::cout << "Enter start point : ";
	std::cin >> start;

	std::cout << "Number of elements greater than " << start << ": ";
	std::cout << calc(arr, 9, start) << std::endl;

	return EXIT_SUCCESS;
}