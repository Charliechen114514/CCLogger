#pragma once
#include "tools/class_helper.h"
#include <cstddef>
#include <deque>
#include <mutex>
#include <string>
#include <vector>
class LoggerQueue {
public:
	DISABLE_COPY_MOVE(LoggerQueue);

	LoggerQueue() = default;
	~LoggerQueue() = default;
	/**
	 * @brief enqueue pushes a queue into the logger
	 *
	 * @param s the message waiting for enlogger
	 */
	void enqueue(const std::string& s);
	void enqueue(std::string&& s);
	/**
	 * @brief   dequeue pop the first message out,
	 *          expectedly, it should be flushed into the files
	 *
	 * @return std::string the message should be loggers
	 */
	std::string dequeue();
	/**
	 * @brief   heavy invoke, this interfaces will returns
	 *          the copy of the left
	 *
	 * @return std::vector<std::string>
	 */
	std::vector<std::string> current_left();

	/**
	 * @brief   heavy invoke, this shell clear the everything out and
	 *          this shell fetch back what is left now :)
	 */
	void clear();

	/**
	 * @brief fetch how many messages are left
	 *
	 * @return size_t the size
	 */
	size_t size();

	/**
	 * @brief check if the queue empty!
	 *
	 * @return true it's empty
	 * @return false it's not empty
	 */
	bool empty();

private:
	std::mutex locker_mutex;
	std::deque<std::string> queue;
};
