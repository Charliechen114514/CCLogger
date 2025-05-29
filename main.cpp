#include "IO/fileio.h"
#include "format/logger_format.h"
#include "logger/logger.h"
#include <iostream>
#include <thread>
#include <vector>

// Quick Example!
int main() {
	auto io = new FileIO("example_log.txt");
	CCLogger logger(io);

	auto formatter = new DefLoggerFormatFactory();
	formatter->set_enable_time(true);
	formatter->set_enable_threadid(true);
	logger.set_formattor(formatter);

	const int numThreads = 4;
	const int logsPerThread = 1000;

	std::vector<std::thread> threads;
	for (int i = 0; i < numThreads; ++i) {
		threads.emplace_back([i, logsPerThread, &logger]() {
			for (int j = 0; j < logsPerThread; ++j) {
				logger.push_message("Thread " + std::to_string(i) + " - Message " + std::to_string(j));
			}
		});
	}

	for (auto& t : threads)
		t.join();
	/* force all write in a sync way */
	logger.sync_flush();

	std::cout << "Logging complete! Logs written to 'example_log.txt'.\n";
	return 0;
}