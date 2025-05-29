/**
 * @file cc_logger.h
 * @brief Defines the CCLogger class, a flexible and high-performance logger with asynchronous and synchronous flushing capabilities.
 */

#pragma once

#include "format/logger_format.h"
#include "tools/class_helper.h"
#include <condition_variable>
#include <memory>
#include <mutex>
#include <thread>

class LoggerFormatFactory;
class AbstractIO;
class LoggerQueue;

/**
 * @brief CCLogger is a high-performance logger supporting both asynchronous and synchronous flushing.
 *
 * It uses a separate worker thread to handle log message writing to avoid blocking the main thread.
 * The log output format can be dynamically changed at runtime.
 */
class CCLogger {
public:
	DISABLE_COPY_MOVE(CCLogger);
	CCLogger() = delete;

	/**
	 * @brief Constructs the logger with a specified output interface.
	 * @param io A pointer to an AbstractIO implementation for actual output (e.g., file, console).
	 */
	explicit CCLogger(AbstractIO* io);

	/**
	 * @brief Destructor. Ensures that the worker thread stops and resources are properly released.
	 */
	~CCLogger();

	/**
	 * @brief Pushes a raw log message into the queue for asynchronous writing.
	 * @param raw The log message to enqueue.
	 */
	void push_message(const std::string& raw);

	/**
	 * @brief Asynchronously requests to flush the current log buffer.
	 *
	 * It triggers the worker thread to flush the output stream, but does not block the caller.
	 */
	void flush();

	/**
	 * @brief Synchronously flushes all buffered log messages.
	 *
	 * It ensures that all queued log messages are written and flushed to the output before returning.
	 */
	void sync_flush();

	/**
	 * @brief Gets the current logger format factory.
	 * @return A pointer to the current LoggerFormatFactory.
	 */
	LoggerFormatFactory* format_factory() const { return formater.get(); }

	/**
	 * @brief Sets a new logger format factory.
	 *
	 * Allows dynamic switching of log message formatting strategy.
	 * @param fmtFactory A pointer to the new LoggerFormatFactory.
	 */
	inline void set_formattor(LoggerFormatFactory* fmtFactory) { formater.reset(fmtFactory); }

private:
	/**
	 * @brief The main logging loop for the worker thread.
	 *
	 * It waits for new log messages or flush requests, then processes them.
	 */
	void logging_issue();

	std::shared_ptr<LoggerFormatFactory> formater {}; ///< Formatter for log messages.
	std::shared_ptr<AbstractIO> io; ///< Output interface.
	std::shared_ptr<LoggerQueue> queue; ///< Queue holding log messages.
	std::condition_variable notifier; ///< Notifier for new log messages or flush requests.
	std::condition_variable flush_cv; ///< Notifier for flush completion in synchronous flush.
	std::mutex locker; ///< Mutex to protect queue and flags.
	std::mutex flush_locker; ///< Mutex to protect flush waiting logic.
	std::thread worker; ///< Worker thread for asynchronous logging.
	std::atomic<bool> stopFlag; ///< Flag to stop the worker thread.
	std::atomic<bool> flushRequest; ///< Flag indicating a flush request.
	std::atomic<bool> flushFinish { true }; ///< Flag indicating flush completion for sync flush.
};
