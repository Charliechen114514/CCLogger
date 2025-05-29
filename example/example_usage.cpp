#include "IO/fileio.h"
#include "core/logger_tools.h"
#include "format/logger_format.h"
#include "logger/logger.h"
#include <chrono>
#include <iostream>
#include <thread>
#include <vector>
// 简单写入测试
void simple_usage_example() {
	DefLoggerFormatFactory* fmtFactory = new DefLoggerFormatFactory;
	fmtFactory->set_loglevel(LogLevel::INFO);
	fmtFactory->set_enable_time(true);
	fmtFactory->set_enable_threadid(true);
	fmtFactory->set_enable_srcLocation(true);
	std::cout << "========== Simple Usage Example ==========\n";

	auto tools = new LoggerTools();
	auto io = new FileIO("simple_usage_log.txt");
	CCLogger logger(io);
	logger.set_formattor(fmtFactory);

	// 简单写入几条日志
	logger.push_message("First log line");
	logger.push_message("Second log line");

	// 强制将缓冲区内容刷新到文件
	logger.sync_flush();

	std::cout << "Simple usage logs written to 'simple_usage_log.txt'.\n";
}

// 性能测试示例
void performance_test_example() {
	DefLoggerFormatFactory* fmtFactory = new DefLoggerFormatFactory;
	fmtFactory->set_loglevel(LogLevel::INFO);
	fmtFactory->set_enable_time(true);
	fmtFactory->set_enable_threadid(true);
	fmtFactory->set_enable_srcLocation(true);
	std::cout << "\n========== Performance Test Example ==========\n";

	auto tools = new LoggerTools();
	auto io = new FileIO("performance_test_log.txt");
	CCLogger logger(io);
	logger.set_formattor(fmtFactory);
	const int numThreads = 16; ///< 并发线程数
	const int logsPerThread = 20000; ///< 每个线程写入的日志数
	const int totalLogs = numThreads * logsPerThread;

	std::cout << "Starting performance test with " << numThreads << " threads, "
	          << logsPerThread << " logs per thread (" << totalLogs << " total logs)...\n";

	auto start = std::chrono::high_resolution_clock::now();

	// 多线程并发写入日志
	std::vector<std::thread> threads;
	for (int i = 0; i < numThreads; ++i) {
		threads.emplace_back([i, logsPerThread, &logger, &fmtFactory]() {
			for (int j = 0; j < logsPerThread; ++j) {
				logger.push_message("Thread " + std::to_string(i) + " log " + std::to_string(j));
			}
		});
	}

	for (auto& t : threads) {
		t.join();
	}

	// 统计耗时
	auto end = std::chrono::high_resolution_clock::now();
	auto durationMs = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();

	// 强制刷新缓冲区
	logger.sync_flush();

	std::cout << "Performance test complete! Time taken: " << durationMs << " ms.\n";
	std::cout << "Logs written to 'performance_test_log.txt'.\n";
}

int main() {
	// 简单使用示例
	simple_usage_example();

	// 性能测试示例
	performance_test_example();

	return 0;
}
