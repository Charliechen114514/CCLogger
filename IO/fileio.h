/**
 * @file file_io.h
 * @brief Defines the FileIO class, which writes logs to a file and ensures flush using fsync.
 */

#pragma once

#include "io.h"
#include <fcntl.h> ///< open
#include <fstream>
#include <string>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h> ///< fsync

/**
 * @brief Implementation of AbstractIO that writes log messages to a file.
 *
 * This class uses std::ofstream for convenient C++ file writing,
 * but also maintains a native file descriptor to ensure data is physically
 * flushed to disk using fsync.
 */
class FileIO : public AbstractIO {
private:
	std::ofstream ofs; ///< Output file stream for high-level file writing.
	int fd { -1 }; ///< Native file descriptor to enable real fsync for guaranteed durability.

public:
	/**
	 * @brief Constructs a FileIO object with the given file path.
	 *
	 * Opens the file in append mode. Also obtains a native file descriptor for fsync.
	 *
	 * @param file_path The path to the log file.
	 */
	explicit FileIO(const std::string& file_path)
	    : ofs(file_path, std::ios_base::app) {
		fd = ::open(file_path.c_str(), O_WRONLY | O_APPEND);
	}

	/**
	 * @brief Writes a log message to the file.
	 *
	 * @param msg The log message to write.
	 */
	void write_logger(const std::string& msg) override {
		ofs << msg;
	}

	/**
	 * @brief Flushes the output stream and forces data to be written to disk.
	 *
	 * This method calls both std::ofstream::flush and ::fsync to ensure
	 * the log message is truly persisted.
	 */
	void force_flush() override {
		ofs.flush();
		if (fd != -1) {
			::fsync(fd);
		}
	}

	/**
	 * @brief Destructor.
	 *
	 * Closes the native file descriptor and the output file stream.
	 */
	~FileIO() {
		if (fd != -1) {
			::close(fd);
		}
		ofs.close();
	}
};
