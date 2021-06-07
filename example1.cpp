#include <random>
#include <thread>
#include <vector>
#include <Windows.h>

HANDLE h_start11;
HANDLE h_finish11;

HANDLE h_start12;
HANDLE h_finish12;

struct Data {
	int a;
	int b;
};

Data d1{1,1};
Data d2{9,9};

void example1_1() {
	std::default_random_engine rand(111);
	for (int n = 0; n < 10000; ++n) {
		WaitForSingleObject(h_start11, INFINITE); // �ȴ����߳̿�����n��ѭ��
		while (rand() % 8 != 0); // ����ӳټ���ָ��

		d1 = d2;

		SetEvent(h_finish11); // ֪ͨ���̵߳�n��ѭ������
	}
}

void example1_2() {
	std::default_random_engine rand(221);
	for (int n = 0; n < 10000; ++n) {
		WaitForSingleObject(h_start12, INFINITE); // �ȴ����߳̿�����n��ѭ��
		while (rand() % 8 != 0); // ����ӳټ���ָ��

		d2 = d1;

		SetEvent(h_finish12); // ֪ͨ���̵߳�n��ѭ������
	}
}

void example1() {
	h_start11 = CreateEvent(nullptr, FALSE, FALSE, nullptr);
	h_finish11 = CreateEvent(nullptr, FALSE, FALSE, nullptr);

	h_start12 = CreateEvent(nullptr, FALSE, FALSE, nullptr);
	h_finish12 = CreateEvent(nullptr, FALSE, FALSE, nullptr);

	std::thread t1(example1_1); // �����߳�1
	std::thread t2(example1_2); // �����߳�2

	for (int iter = 0; iter < 10000; iter += 1) {
		d1 = Data{1,1};
		d2 = Data{9,9};

		SetEvent(h_start11); // ֪ͨ�߳�1��������n��ѭ��
		SetEvent(h_start12); // ֪ͨ�߳�2��������n��ѭ��

		WaitForSingleObject(h_finish11, INFINITE); // �ȴ��߳�1�ĵ�n��ѭ������
		WaitForSingleObject(h_finish12, INFINITE); // �ȴ��߳�2�ĵ�n��ѭ������

		if (d1.a != d1.b) {
			printf("times %d | d1{a: %d, b: %d}\n", iter, d1.a, d1.b);
		}
		if (d2.a != d2.b) {
			printf("times %d | d2{a: %d, b: %d}\n", iter, d2.a, d2.b);
		}
	}

	t1.join();
	t2.join();
}
