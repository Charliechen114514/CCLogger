#pragma once

#include <algorithm>
#include <cstdint>
#include <string>
#include <unordered_map>

/**
 * @enum LogLevel
 * @brief Represents the severity level of a log message.
 *
 * The levels are ordered from lowest to highest severity:
 * TRACE, DEBUG, INFO, WARN, ERROR, FATAL, and OFF.
 */
enum class LogLevel : uint8_t {
	TRACE, ///< Detailed information for debugging.
	DEBUG, ///< General debugging information.
	INFO, ///< Standard operational messages.
	WARN, ///< Indications of potential issues.
	ERROR, ///< Errors that need attention.
	FATAL, ///< Critical errors causing premature termination.
	OFF ///< Logging is completely disabled.
};

/**
 * @brief Returns the numeric weight of a LogLevel.
 *
 * @param level The log level to convert.
 * @return The underlying numeric representation of the log level.
 */
constexpr std::underlying_type_t<LogLevel> Weight(LogLevel level) {
	return static_cast<uint8_t>(level);
}

/**
 * @brief Abstract interface for logger utilities.
 *
 * This interface defines the conversion between log levels and strings,
 * as well as utility functions for timestamp and thread ID.
 */
struct AbsLoggerTools {
	/**
	 * @brief Converts a LogLevel to its string representation.
	 *
	 * @param level The log level to convert.
	 * @return The corresponding string representation.
	 */
	virtual constexpr const std::string& toString(LogLevel level) = 0;

	/**
	 * @brief Parses a string to determine the corresponding LogLevel.
	 *
	 * @param str The string representation of the log level.
	 * @return The parsed LogLevel.
	 */
	virtual constexpr LogLevel fromString(const std::string& str) = 0;

	/**
	 * @brief Gets the current system time as a string.
	 *
	 * @return The current time in string format.
	 */
	virtual std::string current_time() = 0;

	/**
	 * @brief Gets the current thread ID as a string.
	 *
	 * @return The thread ID in string format.
	 */
	virtual std::string thread_id() = 0;
};

/**
 * @brief Default implementation of AbsLoggerTools.
 *
 * Provides standard conversions and utilities for loggers.
 */
struct LoggerTools : public AbsLoggerTools {
	/**
	 * @copydoc AbsLoggerTools::toString
	 */
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

	/**
	 * @copydoc AbsLoggerTools::fromString
	 */
	constexpr LogLevel fromString(const std::string& str) override {
		static const std::unordered_map<std::string, LogLevel> kLevelMap = {
			{ "trace", LogLevel::TRACE }, { "debug", LogLevel::DEBUG }, { "info", LogLevel::INFO }, { "warn", LogLevel::WARN }, { "error", LogLevel::ERROR }, { "fatal", LogLevel::FATAL }, { "off", LogLevel::OFF }
		};

		std::string lower;
		std::transform(str.begin(), str.end(), std::back_inserter(lower),
		               [](char c) { return std::tolower(c); });

		return kLevelMap.at(lower);
	}

	/**
	 * @brief Gets the current system time as a string.
	 *
	 * @return The current time in a formatted string.
	 */
	std::string current_time() override;

	/**
	 * @brief Gets the current thread ID as a string.
	 *
	 * @return The thread ID as a string.
	 */
	std::string thread_id() override;
};
