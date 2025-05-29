/**
 * @file format_set.cpp
 * @author Charliechen114514 (chengh1922@mails.jlu.edu.cn)
 * @brief this file shows that you can set the format for logger to display
 * @version 0.1
 * @date 2025-05-29
 *
 * @copyright Copyright (c) 2025
 *
 */
#include "IO/stdio.h"
#include "format/logger_format.h"
#include "logger/logger.h"
#include <thread>
#include <vector>
int main() {
	// 初始化日志格式工厂
	DefLoggerFormatFactory* fmtFactory = new DefLoggerFormatFactory;
	fmtFactory->set_loglevel(LogLevel::INFO);
	fmtFactory->set_enable_time(true);
	fmtFactory->set_enable_threadid(true);
	fmtFactory->set_enable_srcLocation(true);

	auto tools = new LoggerTools();
	auto io = new ConsoleIO;

	CCLogger logger(io);
	logger.set_formattor(fmtFactory);

	const int numThreads = 10;
	const int logsPerThread = 50;

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

	logger.sync_flush();
	return 0;
}
