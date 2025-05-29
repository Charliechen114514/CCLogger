/**
 * @file console_io.h
 * @brief Defines the ConsoleIO class, which writes log messages to the console.
 */

#pragma once

#include "io.h"
#include <iostream>

/**
 * @brief Implementation of AbstractIO that writes log messages to the console.
 *
 * This class outputs log messages to standard output using std::cout.
 */
struct ConsoleIO : AbstractIO {
	/**
	 * @brief Writes a log message to the console.
	 *
	 * @param msg The log message to write.
	 */
	void write_logger(const std::string& msg) override {
		std::cout << msg;
	}

	/**
	 * @brief Flushes the console output stream.
	 *
	 * Ensures that all buffered data is output to the console.
	 */
	void force_flush() override {
		std::cout.flush();
	}

	/**
	 * @brief Default constructor.
	 */
	ConsoleIO() = default;

	/**
	 * @brief Destructor.
	 */
	~ConsoleIO() override = default;
};
