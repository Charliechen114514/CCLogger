#include "logger_format.h"
#include <sstream>
#include <string>
#include <string_view>

std::string DefLoggerFormatFactory::format(
    const std::string_view message,
    const std::source_location& loc) {

	const std::string_view filename = [&] {
		const std::string_view full = loc.file_name();
		const auto pos = full.find_last_of("/\\");
		return (pos == full.npos) ? full : full.substr(pos + 1);
	}();

	std::ostringstream oss;

	if (enable_time) {
		oss << "[" << tools->current_time() << "] ";
	}
	if (enable_threadid) {
		oss << "[th:" << tools->thread_id() << "] ";
	}
	oss << "[" << tools->toString(loglevel) << "] ";

	if (enable_srcLocation) {
		oss << "[" << filename << ":" << loc.line() << "] "
		    << "[" << loc.function_name() << "] ";
	}

	oss << ": " << message << "\n";

	return oss.str();
}
