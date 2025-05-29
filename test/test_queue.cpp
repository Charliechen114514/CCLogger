#include "cached_queue/logger_queue.h"
#include <cassert>
#include <iostream>
#include <string>
#include <thread>
#include <vector>

// 测试常量
constexpr int THREAD_COUNT = 10;
constexpr int OPERATIONS_PER_THREAD = 100000;

// 压力测试：多线程并发操作
void stress_test() {
	LoggerQueue queue;
	std::vector<std::thread> threads;

	// 并发入队测试
	for (int i = 0; i < THREAD_COUNT; ++i) {
		threads.emplace_back([&queue, i]() {
			for (int j = 0; j < OPERATIONS_PER_THREAD; ++j) {
				queue.enqueue("Thread" + std::to_string(i) + "_Msg" + std::to_string(j));
			}
		});
	}

	// 等待所有入队线程完成
	for (auto& t : threads) {
		t.join();
	}

	// 验证队列大小
	assert(queue.size() == THREAD_COUNT * OPERATIONS_PER_THREAD);
	std::cout << "Enqueue stress test passed. Queue size: " << queue.size() << std::endl;

	threads.clear();

	// 并发出队测试
	std::atomic<int> dequeue_count { 0 };
	for (int i = 0; i < THREAD_COUNT; ++i) {
		threads.emplace_back([&queue, &dequeue_count]() {
			while (true) {
				try {
					auto msg = queue.dequeue();
					++dequeue_count;
				} catch (...) {
					break; // 队列为空时退出
				}
			}
		});
	}

	// 等待所有出队线程完成
	for (auto& t : threads) {
		t.join();
	}

	// 验证所有元素都已出队
	assert(queue.size() == 0);
	assert(dequeue_count == THREAD_COUNT * OPERATIONS_PER_THREAD);
	std::cout << "Dequeue stress test passed. Dequeued count: " << dequeue_count << std::endl;
}

#include <chrono>

// 性能测试
void performance_test() {
	LoggerQueue queue;
	constexpr int TOTAL_OPERATIONS = 1000000;

	// 单线程入队性能
	auto start = std::chrono::high_resolution_clock::now();
	for (int i = 0; i < TOTAL_OPERATIONS; ++i) {
		queue.enqueue("Message" + std::to_string(i));
	}
	auto end = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double> elapsed = end - start;
	std::cout << "Enqueue performance: " << TOTAL_OPERATIONS / elapsed.count()
	          << " ops/sec" << std::endl;
	assert(queue.size() == TOTAL_OPERATIONS);

	// 单线程出队性能
	start = std::chrono::high_resolution_clock::now();
	for (int i = 0; i < TOTAL_OPERATIONS; ++i) {
		queue.dequeue();
	}
	end = std::chrono::high_resolution_clock::now();
	elapsed = end - start;
	std::cout << "Dequeue performance: " << TOTAL_OPERATIONS / elapsed.count()
	          << " ops/sec" << std::endl;
	assert(queue.size() == 0);

	// 多线程性能测试
	std::vector<std::thread> threads;
	constexpr int THREADS = 16;
	constexpr int OPS_PER_THREAD = TOTAL_OPERATIONS / THREADS;

	start = std::chrono::high_resolution_clock::now();
	for (int i = 0; i < THREADS; ++i) {
		threads.emplace_back([&queue]() {
			for (int j = 0; j < OPS_PER_THREAD; ++j) {
				queue.enqueue("ConcurrentMsg");
			}
		});
	}
	for (auto& t : threads) {
		t.join();
	}
	end = std::chrono::high_resolution_clock::now();
	elapsed = end - start;
	std::cout << "Multi-thread enqueue performance: " << TOTAL_OPERATIONS / elapsed.count()
	          << " ops/sec" << std::endl;
	assert(queue.size() == TOTAL_OPERATIONS);
}

void functional_test() {
	LoggerQueue queue;

	// 测试空队列
	assert(queue.size() == 0);
	assert(queue.current_left().empty());

	// 测试入队
	queue.enqueue("Test1");
	assert(queue.size() == 1);
	assert(queue.current_left().front() == "Test1");

	// 测试出队
	assert(queue.dequeue() == "Test1");
	assert(queue.size() == 0);

	// 测试清空
	queue.enqueue("Test2");
	queue.enqueue("Test3");
	assert(queue.size() == 2);
	queue.clear();
	assert(queue.size() == 0);

	std::cout << "Functional test passed." << std::endl;
}

int main() {
	std::cout << "Starting LoggerQueue tests..." << std::endl;

	try {
		functional_test();
		stress_test();
		performance_test();

		std::cout << "All tests passed successfully!" << std::endl;
		return 0;
	} catch (const std::exception& e) {
		std::cerr << "Test failed: " << e.what() << std::endl;
		return 1;
	}
}