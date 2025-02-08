#ifndef PRINT_H
#define PRINT_H

#ifndef LOGLEVEL
#define LOGLEVEL (-1)
#endif

#include <print>

namespace rt
{
enum LogLevel {
	Default = -1,
	Err     = 0,
	Warning = 1,
	Notice  = 2,
	Info    = 3,
	Debug   = 4,
	Verbose = 5,
};

template<int log_level = LogLevel::Err, typename... Args>
void print(std::format_string<Args...> fmt, Args &&...args)
{
	if constexpr (LOGLEVEL >= log_level)
		std::print(fmt, std::forward<Args>(args)...);
}
}

#endif // PRINT_H
