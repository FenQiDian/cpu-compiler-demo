#include <algorithm>
#include <chrono>
#include <iostream>
#include <thread>
#include <vector>
#include <Windows.h>
#include <random>

#include "header.h"

void example5_1(volatile int array[100000]) {
	for (int i = 0; i < 100000; i += 1) {
		if (array[i] < 128) {
			array[i] = (array[i] + 5) % 256;
		} else {
			array[i] = (array[i] + 10) % 256;
		}
	}
}

void example5() {
	volatile int array1[100000];
	volatile int array2[100000];

	for (int idx = 0; idx < 100000; ++idx) {
		array1[idx] = 1;
		array2[idx] = rand() % 256;
	}

	{
		auto start = std::chrono::high_resolution_clock::now();
		for (int idx = 0; idx < 10000; ++idx) { // ²âÊÔ 10000 ´Î
			example5_1(array1);
		}
		auto finish = std::chrono::high_resolution_clock::now();
		printf("func 1 => %dms\n", (finish - start) / 1000 / 1000);
	}
	{
		auto start = std::chrono::high_resolution_clock::now();
		for (int idx = 0; idx < 10000; ++idx) { // ²âÊÔ 10000 ´Î
			example5_1(array2);
		}
		auto finish = std::chrono::high_resolution_clock::now();
		printf("func 2 => %dms\n", (finish - start) / 1000 / 1000);
	}
}
