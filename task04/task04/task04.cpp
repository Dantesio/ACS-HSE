#include <iostream>
#include <vector>
#include <thread>
#include <mutex>
#include <string>
#include <fstream>
#include <map>
#include <algorithm>
#include <unordered_set>
#include <omp.h>

// Assuming Yin team: 0, Yang team: 1.
struct Monk {
	bool team{ false };
	double power{ 0 };
	int id{ 0 };

	bool operator == (const Monk& o) const {
		return team == o.team &&
			power == o.power &&
			id == o.id;
	}
};

struct Hash {
	size_t operator() (const Monk& o) const {
		return o.id;
	}
};

#define TeamMatrix std::vector<Monk>


void getInput(std::istream& inp, TeamMatrix& matrix) {
	std::size_t sizeYin, sizeYang;
	inp >> sizeYin >> sizeYang;

	matrix.resize(sizeYin + sizeYang);

	for (auto i = 0; i < sizeYin; i++) {
		matrix[i].team = false;
		matrix[i].id = i;
		inp >> matrix[i].power;
	}

	for (auto i = 0; i < sizeYang; i++) {
		matrix[sizeYin + i].team = true;
		matrix[i].id = sizeYin + i;
		inp >> matrix[sizeYin + i].power;
	}
}

TeamMatrix merge(TeamMatrix& leftHalf, TeamMatrix& rightHalf) {
	std::unordered_set<Monk, Hash> result;
	for (const auto& leftOne : leftHalf)
		result.insert(leftOne);
	for (const auto& rightOne : rightHalf)
		result.insert(rightOne);

	TeamMatrix answer;
	for (const auto& a : result) {
		answer.push_back(a);
	}

	return answer;
}

// Alternative implementation
//std::recursive_mutex mutex;
struct Context {
	int l, r;
	TeamMatrix& matrix;
	std::map<int, int>& enemies;

	TeamMatrix result;
};

TeamMatrix eliminate(int l, int r, TeamMatrix& matrix, std::map<int, int>& enemies);

void wrapper(Context& context) {
	context.result = eliminate(context.l, context.r, context.matrix, context.enemies);
}

TeamMatrix eliminate(int l, int r, TeamMatrix& matrix, std::map<int, int>& enemies) {
	if (l == r) {
		// Alternative implementation
		 //mutex.lock();
		 //auto self = matrix[l];
		 //auto enemy = matrix[enemies[l]];
		 //mutex.unlock();
		Monk self;
		Monk enemy;
#pragma omp critical
		{
		self = matrix[l];
		enemy = matrix[enemies[l]];
		}

		if (self.power == enemy.power)
			return l < enemies[l] ? TeamMatrix{ self } : TeamMatrix{ enemy };
		else
			return self.power > enemy.power ? TeamMatrix{ self } : TeamMatrix{ enemy };
	}

	int mid = (l + r) / 2;
	Context a{ l, mid, matrix, enemies, {} },
		    b{ mid + 1, r, matrix, enemies, {} };

	// Alternative implementation
	//std::thread t1(wrapper, std::ref(a)),
	//	        t2(wrapper, std::ref(b));
	//t1.join();
	//t2.join();
#pragma omp parallel num_threads(2)
	{
		if (omp_get_thread_num() == 0) {
			wrapper(std::ref(a));
		} else {
			wrapper(std::ref(b));
		}
	}

	return merge(a.result, b.result);
}


int main() {
	TeamMatrix matrix;
	std::fstream fin("input.txt");
	getInput(fin, matrix);

	std::cout << "The data has been received. Let's start the tournament ..." << std::endl;

	while (matrix.size() != 1) {
		std::map<int, int> enemies;
		std::random_shuffle(matrix.begin(), matrix.end());
		for (int i = 0; i < matrix.size(); i += 2) {
			enemies[i] = i + 1;
			enemies[i + 1] = i;
		}

		Monk* last = nullptr;
		if (matrix.size() % 2 != 0) {
			last = &matrix.back();
			matrix.pop_back();
		}

		auto result = eliminate(0, matrix.size() - 1, matrix, enemies);
		if (last != nullptr) {
			result.push_back(*last);
		}

		matrix = result;
	}

	auto result = matrix.back();

	if (result.team)
		std::cout << "Team Kuan Yang wins and claims the Bodhisattva statue!" << std::endl;
	else
		std::cout << "Team Kuan Yin wins and claims the Bodhisattva statue!" << std::endl;

	return 0;
}