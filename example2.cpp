#include <random>
#include <thread>
#include <vector>
#include <Windows.h>

int x = 0, y = 0;
int r1 = 0, r2 = 0;

HANDLE h_start21;
HANDLE h_finish21;

HANDLE h_start22;
HANDLE h_finish22;

void example2_1() {
	std::default_random_engine rand(111);
	for (int n = 0; n < 500000; ++n) {
		WaitForSingleObject(h_start21, INFINITE); // 等待主线程开启第n次循环
		while (rand() % 8 != 0); // 随机延迟几个指令

		x = 1;
		r1 = y;

		SetEvent(h_finish21); // 通知主线程第n次循环结束
	}
}

void example2_2() {
	std::default_random_engine rand(221);
	for (int n = 0; n < 500000; ++n) {
		WaitForSingleObject(h_start22, INFINITE); // 等待主线程开启第n次循环
		while (rand() % 8 != 0); // 随机延迟几个指令

		y = 1;
		r2 = x;

		SetEvent(h_finish22); // 通知主线程第n次循环结束
	}
}

void example2() {
	h_start21 = CreateEvent(nullptr, FALSE, FALSE, nullptr);
	h_finish21 = CreateEvent(nullptr, FALSE, FALSE, nullptr);

	h_start22 = CreateEvent(nullptr, FALSE, FALSE, nullptr);
	h_finish22 = CreateEvent(nullptr, FALSE, FALSE, nullptr);

	std::thread t1(example2_1); // 启动线程1
	std::thread t2(example2_2); // 启动线程2

	int count00 = 0;
	int count10 = 0;
	int count01 = 0;
	int count11 = 0;

	for (int iter = 0; iter < 500000; iter += 1) {
		x = 0;
		y = 0;

		SetEvent(h_start21); // 通知线程1，启动第n次循环
		SetEvent(h_start22); // 通知线程2，启动第n次循环

		WaitForSingleObject(h_finish21, INFINITE); // 等待线程1的第n次循环结束
		WaitForSingleObject(h_finish22, INFINITE); // 等待线程2的第n次循环结束

		if (r1 == 0 && r2 == 0) {
			count00 += 1;
		} else if (r1 == 1 && r2 == 0) {
			count10 += 1;
		} else if (r1 == 0 && r2 == 1) {
			count01 += 1;
		} else if (r1 == 1 && r2 == 1) {
			count11 += 1;
		}
	}

	printf("r1 == 0 && r2 == 0 | %d times\n", count00);
	printf("r1 == 1 && r2 == 0 | %d times\n", count10);
	printf("r1 == 0 && r2 == 1 | %d times\n", count01);
	printf("r1 == 1 && r2 == 1 | %d times\n", count11);

	t1.join();
	t2.join();
}
