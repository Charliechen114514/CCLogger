#include "logger.h"
#include "IO/io.h"
#include "cached_queue/logger_queue.h"
#include "format/logger_format.h"
#include <memory>
#include <thread>

CCLogger::CCLogger(AbstractIO* io) {
	this->formater = std::make_shared<DummyFormatFactory>();
	this->io = std::shared_ptr<AbstractIO>(io);
	this->queue = std::make_shared<LoggerQueue>();
	worker = std::thread([this]() { this->logging_issue(); });
}

CCLogger::~CCLogger() {
	stopFlag.store(true);
	notifier.notify_one();
	if (worker.joinable())
		worker.join();
}

void CCLogger::push_message(const std::string& raw) {
	queue->enqueue(raw);
	notifier.notify_one();
}

void CCLogger::flush() {
	{
		std::lock_guard<std::mutex> lock(locker);
		flushRequest = true;
	}
	notifier.notify_one();
}

void CCLogger::sync_flush() {
	{
		std::lock_guard<std::mutex> lock(locker);
		flushRequest = true;
		flushFinish = false;
	}
	notifier.notify_one();

	std::unique_lock<std::mutex> lock(flush_locker);
	flush_cv.wait(lock, [this]() { return flushFinish.load(); });
}

void CCLogger::logging_issue() {
	while (1) {
		std::unique_lock<std::mutex> lock(locker);
		notifier.wait(lock, [this]() { return stopFlag.load() || !queue->empty() || flushRequest; });

		if (stopFlag.load() && queue->empty()) {
			break;
		}

		auto write_sessions = queue->current_left();
		queue->clear();
		lock.unlock();

		for (const auto& each : write_sessions) {
			io->write_logger(formater->format(each));
		}

		lock.lock();
		if (flushRequest) {
			io->force_flush();
			flushRequest = false;

			if (!flushFinish) {
				lock.unlock();
				{
					std::lock_guard<std::mutex> f_lock(flush_locker);
					flushFinish = true;
				}
				flush_cv.notify_one();
				lock.lock();
			}
		}
	}
}
