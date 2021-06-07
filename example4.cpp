#include <algorithm>
#include <chrono>
#include <iostream>
#include <thread>
#include <vector>
#include <Windows.h>
#include <random>

#include "header.h"

volatile int arr[33];

void example4_1() {
	std::thread t1([&] { // 启动线程 t1
		for (int idx = 0; idx < 500000000; idx += 1) {
			arr[0] += idx;
		}
	});

	std::thread t2([&] { // 启动线程 t2
		for (int idx = 0; idx < 500000000; idx += 1) {
			arr[1] += idx;
		}
	});

	t1.join(); // 等待 t1 线程结束
	t2.join(); // 等待 t2 线程结束
}

void example4_2() {
	std::thread t1([&] { // 启动线程 t1
		for (int idx = 0; idx < 500000000; idx += 1) {
			arr[0] += idx;
		}
	});

	std::thread t2([&] { // 启动线程 t2
		for (int idx = 0; idx < 500000000; idx += 1) {
			arr[32] += idx;
		}
	});

	t1.join(); // 等待 t1 线程结束
	t2.join(); // 等待 t2 线程结束
}

void example4() {
	{
		auto start = std::chrono::high_resolution_clock::now();
		example4_1();
		auto finish = std::chrono::high_resolution_clock::now();
		printf("func 1 => %dms\n", (finish - start) / 1000 / 1000);
	}
	{
		auto start = std::chrono::high_resolution_clock::now();
		example4_2();
		auto finish = std::chrono::high_resolution_clock::now();
		printf("func 2 => %dms\n", (finish - start) / 1000 / 1000);
	}
}
