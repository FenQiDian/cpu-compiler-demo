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
		WaitForSingleObject(h_start21, INFINITE); // �ȴ����߳̿�����n��ѭ��
		while (rand() % 8 != 0); // ����ӳټ���ָ��

		x = 1;
		r1 = y;

		SetEvent(h_finish21); // ֪ͨ���̵߳�n��ѭ������
	}
}

void example2_2() {
	std::default_random_engine rand(221);
	for (int n = 0; n < 500000; ++n) {
		WaitForSingleObject(h_start22, INFINITE); // �ȴ����߳̿�����n��ѭ��
		while (rand() % 8 != 0); // ����ӳټ���ָ��

		y = 1;
		r2 = x;

		SetEvent(h_finish22); // ֪ͨ���̵߳�n��ѭ������
	}
}

void example2() {
	h_start21 = CreateEvent(nullptr, FALSE, FALSE, nullptr);
	h_finish21 = CreateEvent(nullptr, FALSE, FALSE, nullptr);

	h_start22 = CreateEvent(nullptr, FALSE, FALSE, nullptr);
	h_finish22 = CreateEvent(nullptr, FALSE, FALSE, nullptr);

	std::thread t1(example2_1); // �����߳�1
	std::thread t2(example2_2); // �����߳�2

	int count00 = 0;
	int count10 = 0;
	int count01 = 0;
	int count11 = 0;

	for (int iter = 0; iter < 500000; iter += 1) {
		x = 0;
		y = 0;

		SetEvent(h_start21); // ֪ͨ�߳�1��������n��ѭ��
		SetEvent(h_start22); // ֪ͨ�߳�2��������n��ѭ��

		WaitForSingleObject(h_finish21, INFINITE); // �ȴ��߳�1�ĵ�n��ѭ������
		WaitForSingleObject(h_finish22, INFINITE); // �ȴ��߳�2�ĵ�n��ѭ������

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
