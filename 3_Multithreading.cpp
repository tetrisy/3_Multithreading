#include "LRUCache.h"

#include <thread>
#include <vector>
#include <iostream>

int main() {
	LRUCache<int, int> cache(5);

	std::vector<std::thread> threads;

	for (int t = 0; t < 4; ++t) {
		threads.emplace_back([&cache, t]() {
			for (int i = 0; i < 10000; ++i) {
				cache.Put(i + t * 10000, i);

				int value;
				cache.Get(i, value);
			}
		});
	}

	for (auto& th : threads) {
		th.join();
	}

	std::cout << "Finished\n";
}