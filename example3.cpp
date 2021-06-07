#include <algorithm>
#include <chrono>
#include <iostream>
#include <thread>
#include <vector>
#include <Windows.h>
#include <random>

#include "header.h"

void example3_1() {
	volatile int array[100000];
	for (int i = 0; i < 100000; i += 1) { array[i] = i; }
	// 按如下顺序：0 1 2 3 ... 99999
}

void example3_2() {
	volatile int array[100000];
	for (int i = 0; i < 100000; i += 5) { array[i] = i; }
	for (int i = 1; i < 100000; i += 5) { array[i] = i; }
	for (int i = 2; i < 100000; i += 5) { array[i] = i; }
	for (int i = 3; i < 100000; i += 5) { array[i] = i; }
	for (int i = 4; i < 100000; i += 5) { array[i] = i; }
	// 按如下顺序：
	//   0 5 10 15 ... 99995
	//   1 6 11 16 ... 99996
	//   2 7 12 17 ... 99997
	//   3 8 13 18 ... 99998
	//   4 9 14 19 ... 99999
}

void example3() {
	{
		auto start = std::chrono::high_resolution_clock::now();
		for (int idx = 0; idx < 20000; ++idx) { // 测试 20000 次
			example3_1();
		}
		auto finish = std::chrono::high_resolution_clock::now();
		printf("func 1 => %dms\n", (finish - start) / 1000 / 1000);
	}
	{
		auto start = std::chrono::high_resolution_clock::now();
		for (int idx = 0; idx < 20000; ++idx) { // 测试 20000 次
			example3_2();
		}
		auto finish = std::chrono::high_resolution_clock::now();
		printf("func 2 => %dms\n", (finish - start) / 1000 / 1000);
	}
}
