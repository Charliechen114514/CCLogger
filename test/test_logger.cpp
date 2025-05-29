#include "IO/fileio.h"
#include "core/logger_tools.h"
#include "logger/logger.h"
#include <algorithm>
#include <cassert>
#include <chrono>
#include <iostream>
#include <ostream>
#include <string>
#include <thread>
#include <unordered_map>
#include <vector>
class MockTools : public AbsLoggerTools {
public:
	std::string current_time() override { return "MOCK_TIME"; }
	std::string thread_id() override { return "MOCK_THREAD"; }

	const std::string& toString(LogLevel level) override {
		static const std::unordered_map<LogLevel, std::string> levelMap = {
			{ LogLevel::TRACE, "TRACE" }, { LogLevel::DEBUG, "DEBUG" }, { LogLevel::INFO, "INFO" }, { LogLevel::WARN, "WARN" }, { LogLevel::ERROR, "ERROR" }, { LogLevel::FATAL, "FATAL" }, { LogLevel::OFF, "OFF" }
		};
		return levelMap.at(level);
	}

	LogLevel fromString(const std::string& str) override {
		static const std::unordered_map<std::string, LogLevel> levelMap = {
			{ "trace", LogLevel::TRACE }, { "debug", LogLevel::DEBUG }, { "info", LogLevel::INFO }, { "warn", LogLevel::WARN }, { "error", LogLevel::ERROR }, { "fatal", LogLevel::FATAL }, { "off", LogLevel::OFF }
		};
		std::string lower = str;
		std::transform(lower.begin(), lower.end(), lower.begin(),
		               [](char c) { return std::tolower(c); });
		return levelMap.at(lower);
	}
};

void interface_test() {
	std::cout << "==== 接口测试 ====" << std::endl;
	auto tools = new MockTools;
	auto io = new FileIO("interface_test_log.txt");

	CCLogger logger(io);
	logger.push_message("First log");
	logger.push_message("Second log");
	logger.push_message("Third log");

	std::cout << "接口测试：已写入 3 条日志。\n\n";

	logger.sync_flush();
	std::this_thread::sleep_for(std::chrono::milliseconds(1000));
	std::ifstream ifs("interface_test_log.txt");
	std::string line;
	int lineCount = 0;
	constexpr int count = 3;
	while (std::getline(ifs, line))
		++lineCount;
	std::cout << lineCount << std::endl;
	assert(lineCount >= count && "日志行数校验失败！");
	std::cout << "日志完整性测试：文件中有 " << lineCount << " 条，期望 >= " << count << "\n\n";
}

void stress_test() {
	std::cout << "==== 压力测试（高并发） ====" << std::endl;
	auto tools = new MockTools;
	auto io = new FileIO("stress_test_log.txt");

	CCLogger logger(io);

	constexpr int threadCount = 10;
	constexpr int logsPerThread = 100000;
	std::vector<std::thread> threads;

	auto start = std::chrono::high_resolution_clock::now();

	for (int i = 0; i < threadCount; ++i) {
		threads.emplace_back([&logger, i]() {
			for (int j = 0; j < logsPerThread; ++j) {
				logger.push_message("Thread " + std::to_string(i) + " log " + std::to_string(j));
			}
		});
	}

	for (auto& th : threads)
		th.join();

	auto end = std::chrono::high_resolution_clock::now();
	auto dur = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

	std::cout << "并发写入 " << (threadCount * logsPerThread) << " 条日志，耗时 "
	          << dur.count() << " ms\n";
	std::cout << "写入速率：" << (threadCount * logsPerThread * 1000.0 / dur.count()) << " 条/秒\n\n";
}

void edge_case_test() {
	std::cout << "==== 边界条件测试 ====" << std::endl;
	auto tools = new MockTools;
	auto io = new FileIO("edge_case_log.txt");

	CCLogger logger(io);
	logger.push_message(""); // 空字符串
	logger.push_message(std::string(1000000, 'A')); // 超长日志
	logger.push_message("Normal log");

	std::cout << "边界测试：写入空日志、超长日志、正常日志。\n\n";
}

void correctness_test() {
	std::cout << "==== 正确性测试 ====" << std::endl;
	auto tools = new MockTools;
	auto io = new FileIO("correctness_test_log.txt");
	constexpr int count = 100;
	CCLogger logger(io);

	for (int i = 0; i < count; ++i) {
		logger.push_message("Line " + std::to_string(i));
	}
	logger.sync_flush();
	std::this_thread::sleep_for(std::chrono::seconds(5));
	std::ifstream ifs("correctness_test_log.txt");
	std::string line;
	int lineCount = 0;
	while (std::getline(ifs, line))
		++lineCount;
	std::cout << lineCount << std::endl;
	assert(lineCount >= count && "日志行数校验失败！");
	std::cout << "日志完整性测试：文件中有 " << lineCount << " 条，期望 >= " << count << "\n\n";
}

int main() {
	interface_test();
	edge_case_test();
	correctness_test();
	stress_test();

	std::cout << "\n==== 所有测试完成！ ====\n";
	return 0;
}