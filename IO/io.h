#pragma once
#include <string>
struct AbstractIO {
	/**
	 * @brief the interface of the logger writing
	 *
	 * @param msg
	 */
	virtual void write_logger(const std::string& msg) = 0;
	/**
	 * @brief force the all write, this is expected to be sync!
	 *
	 */
	virtual void force_flush() = 0;
	virtual ~AbstractIO() = default;
};
