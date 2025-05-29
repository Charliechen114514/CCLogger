/**
 * @file logger_format.h
 * @brief Defines the interface and implementations for customizable log message formatting.
 */

#pragma once

#include "core/logger_tools.h"
#include "tools/class_helper.h"
#include <memory>
#include <source_location>
#include <string>
#include <string_view>

/**
 * @brief Abstract interface for formatting log messages.
 *
 * Implementations of this interface define how log messages are formatted
 * before being output to an I/O device.
 */
struct LoggerFormatFactory {
	/**
	 * @brief Virtual destructor for proper cleanup in derived classes.
	 */
	virtual ~LoggerFormatFactory() = default;

	/**
	 * @brief Formats a log message.
	 *
	 * @param message The log message content.
	 * @param loc The source location of the log call, for additional context.
	 * @return The formatted log string.
	 */
	virtual std::string format(
	    const std::string_view message,
	    const std::source_location& loc = std::source_location::current())
	    = 0;
};

/**
 * @brief A simple formatter that adds a newline to the message.
 *
 * This is a minimal implementation of LoggerFormatFactory, primarily
 * for testing or fallback scenarios.
 */
struct DummyFormatFactory : public LoggerFormatFactory {
	/**
	 * @copydoc LoggerFormatFactory::format
	 */
	virtual std::string format(
	    const std::string_view message,
	    const std::source_location& loc = std::source_location::current()) override {
		return std::string(message.data()) + "\n";
	}
};

/**
 * @brief A configurable log formatter with rich features.
 *
 * Supports:
 * - Logging level.
 * - Timestamp inclusion.
 * - Thread ID inclusion.
 * - Source location information.
 */
struct DefLoggerFormatFactory : public LoggerFormatFactory {
private:
	LogLevel loglevel { LogLevel::INFO }; ///< Current log level.
	bool enable_time { true }; ///< Flag to include timestamp.
	bool enable_threadid { true }; ///< Flag to include thread ID.
	bool enable_srcLocation { true }; ///< Flag to include source location.
	std::shared_ptr<AbsLoggerTools> tools { new LoggerTools }; ///< Logger utilities for formatting.

public:
	/**
	 * @brief Getter and setter for loglevel.
	 */
	PROPERTY_GET_SET(loglevel);

	/**
	 * @brief Getter and setter for enable_time.
	 */
	PROPERTY_GET_SET(enable_time);

	/**
	 * @brief Getter and setter for enable_threadid.
	 */
	PROPERTY_GET_SET(enable_threadid);

	/**
	 * @brief Getter and setter for enable_srcLocation.
	 */
	PROPERTY_GET_SET(enable_srcLocation);

	/**
	 * @brief Getter and setter for tools.
	 */
	PROPERTY_GET_SET(tools);

	/**
	 * @brief Destructor.
	 */
	~DefLoggerFormatFactory() override = default;

	/**
	 * @copydoc LoggerFormatFactory::format
	 *
	 * This implementation dynamically constructs a rich log message
	 * based on the configuration flags.
	 */
	virtual std::string format(
	    const std::string_view message,
	    const std::source_location& loc = std::source_location::current()) override;
};
