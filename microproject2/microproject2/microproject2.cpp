#include <iostream>
#include <vector>
#include <mutex>

// Udachin Daniil, BSE191, Variant 2

std::mutex g_mutex;
static bool isRunning = true;

static class Pot {
private:
	int capacity{ 0 };
	int currentVolume{ 0 };
	std::mutex p_mutex;

	void empty() {
		currentVolume = 0;
		isRunning = false;
	}

public:
	void increase() {
		p_mutex.lock();
		if (!isRunning) {
			p_mutex.unlock();
			return;
		}

		currentVolume++;
		std::cout << "Bee #" << std::this_thread::get_id() << " has increased pot's volume by 1 to: "
			<< currentVolume << " from " << (currentVolume - 1) << std::endl;

		if (currentVolume >= capacity) {
			std::cout << "Bee #" << std::this_thread::get_id() << " has triggered a bear" << std::endl;
			empty();
		}

		p_mutex.unlock();
	}

	void resize(int capacity) {
		p_mutex.lock();
		this->capacity = capacity;
		if (currentVolume > capacity) {
			empty();
		}
		p_mutex.unlock();
	}

	Pot(int capacity) : capacity(capacity) {}
} pot{ 0 };


bool checkIsRunning() {
	g_mutex.lock();
	bool result = isRunning;
	g_mutex.unlock();
	return result;
}

int main()
{
	bool yaTopCoder = true;
	int capacity, beeCount;
	std::cout << "In one forest there live X bees and one bear, which use one pot of honey with a capacity of Y sips." << std::endl;
	std::cout << "At first the pot is empty. Until the pot is full, the bear sleeps. As soon as the pot is full," << std::endl;
	std::cout << "the bear wakes up and eats all the honey, and then goes back to sleep." << std::endl;
	std::cout << "Each bee repeatedly collects one sip of honey and puts it in a pot." << std::endl;
	std::cout << "The bee that brings the last portion of honey wakes up the bear.\n" << std::endl;
	std::cout << "Please, type in pot's capacity and bees count" << std::endl;
	std::cin >> capacity >> beeCount;
	pot.resize(capacity);

	while (yaTopCoder) {
		std::vector<std::thread> threads;
		for (int i = 0; i < beeCount; i++) {
			threads.emplace_back((
				[]() {
				while (checkIsRunning())
					pot.increase();
			}));
		}

		while (checkIsRunning()) {
			for (auto& thread : threads)
				thread.join();
		}

		std::cout << "Pot has been emptyied by the Bear." << std::endl;
		std::cout << "Continue work? Print no in case you want to end bees' torture." << std::endl;

		std::string str;
		std::cin >> str;
		isRunning = str != "no";

		for (auto& thread : threads)
			if (thread.joinable())
				thread.join();

		threads.clear();
		yaTopCoder = isRunning;
	}

	std::cout << "The bees have realized their class affiliation!" << std::endl;
	std::cout << "They no longer want to serve the oppressor beargeois!" << std::endl;
	std::cout << "The bees scattered across the hives, the pot is broken, the bear is stung and is sad from hunger." << std::endl;
	std::cout << "Glory to the bee Soviet socialist republic!" << std::endl;
	std::cout << "Long live the revolution!\n" << std::endl;

	return 0;
}