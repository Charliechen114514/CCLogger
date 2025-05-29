#include "core/logger_tools.h"
#include "format/logger_format.h"
#include <cassert>
#include <iostream>
#include <memory>

struct MockTools : public AbsLoggerTools {
	std::string current_time() override { return "MOCK_TIME"; }
	std::string thread_id() override { return "MOCK_THREAD_ID"; }
	constexpr const std::string& toString(LogLevel level) override {
		static const std::unordered_map<LogLevel, std::string> kLevelNames = {
			{ LogLevel::TRACE, "TRACE" },
			{ LogLevel::DEBUG, "DEBUG" },
			{ LogLevel::INFO, "INFO" },
			{ LogLevel::WARN, "WARN" },
			{ LogLevel::ERROR, "ERROR" },
			{ LogLevel::FATAL, "FATAL" },
			{ LogLevel::OFF, "OFF" }
		};
		return kLevelNames.at(level);
	}

	constexpr LogLevel fromString(const std::string& str) override {
		static const std::unordered_map<std::string, LogLevel> kLevelMap = {
			{ "trace", LogLevel::TRACE }, { "debug", LogLevel::DEBUG }, { "info", LogLevel::INFO }, { "warn", LogLevel::WARN }, { "error", LogLevel::ERROR }, { "fatal", LogLevel::FATAL }, { "off", LogLevel::OFF }
		};

		std::string lower;
		std::transform(str.begin(), str.end(), std::back_inserter(lower),
		               [](char c) { return std::tolower(c); });

		return kLevelMap.at(lower);
	}
};

int main() {
	DefLoggerFormatFactory factory;
	auto tools = std::make_shared<MockTools>();
	factory.set_tools(tools);
	factory.set_loglevel(LogLevel::INFO);

	// 设置开关
	factory.set_enable_time(true);
	factory.set_enable_threadid(true);
	factory.set_enable_srcLocation(true);

	// 使用format接口，传入一个测试消息
	const std::string msg = "Hello, test!";
	auto log = factory.format(msg);

	// 输出结果
	std::cout << log << std::endl;

	// 使用assert验证 MockTools 输出是否正确
	assert(log.find("MOCK_TIME") != std::string::npos);
	assert(log.find("MOCK_THREAD_ID") != std::string::npos);
	assert(log.find("INFO") != std::string::npos);
	assert(log.find("Hello, test!") != std::string::npos);

	// 关闭线程ID和时间戳再测一次
	factory.set_enable_time(false);
	factory.set_enable_threadid(false);
	log = factory.format(msg);

	// 不应包含 MOCK_TIME 和 MOCK_THREAD_ID
	assert(log.find("MOCK_TIME") == std::string::npos);
	assert(log.find("MOCK_THREAD_ID") == std::string::npos);

	// 应该还包含 INFO 和消息本身
	assert(log.find("INFO") != std::string::npos);
	assert(log.find("Hello, test!") != std::string::npos);

	std::cout << "All tests passed!" << std::endl;
	return 0;
}