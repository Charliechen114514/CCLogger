#include "logger_queue.h"
#include <mutex>
#include <stdexcept>

void LoggerQueue::enqueue(const std::string& s) {
	std::lock_guard<std::mutex> locker(this->locker_mutex);
	queue.push_back(s);
}

void LoggerQueue::enqueue(std::string&& s) {
	std::lock_guard<std::mutex> locker(this->locker_mutex);
	queue.push_back(std::move(s));
}

std::string LoggerQueue::dequeue() {
	std::lock_guard<std::mutex> locker(this->locker_mutex);
	if (queue.empty()) {
		throw std::runtime_error("Dequeue empty!");
	}
	auto result = std::move(queue.front());
	queue.pop_front();
	return result;
}

std::vector<std::string> LoggerQueue::current_left() {
	std::lock_guard<std::mutex> locker(this->locker_mutex);
	return { queue.begin(), queue.end() };
}

void LoggerQueue::clear() {
	std::lock_guard<std::mutex> locker(this->locker_mutex);
	queue.clear();
}

size_t LoggerQueue::size() {
	std::lock_guard<std::mutex> locker(this->locker_mutex);
	return queue.size();
}

bool LoggerQueue::empty() {
	std::lock_guard<std::mutex> locker(this->locker_mutex);
	return queue.empty();
}