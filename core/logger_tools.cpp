#include "logger_tools.h"
#include <chrono>
#include <format>
#include <thread>

std::string LoggerTools::current_time() {
	using namespace std::chrono;
	const auto now = system_clock::now();
	const auto secs = floor<seconds>(now);
	const auto ns = duration_cast<nanoseconds>(now - secs);
	return std::format("{:%Y-%m-%d %H:%M:%S}.{:09}", secs, ns.count());
}

std::string LoggerTools::thread_id() {
	return std::format("0x{:x}",
	                   std::hash<std::thread::id> {}(std::this_thread::get_id()));
}