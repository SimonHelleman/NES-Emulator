#pragma once
#include <cstdint>
#include <functional>
#include <string>
#include <vector>

#ifdef DEBUG
	#define LOG_DEBUG(message) Logger::Get().LogDebug(message)
	#define LOG_DEBUG_FL(message) Logger::Get().LogDebug(message, __FILE__, __LINE__)
#else
	#define LOG_DEBUG(message) ;
	#define LOG_DEBUG_FL(message) ;
#endif


#define INFO(message) Logger::Get().LogInfo(message)
#define INFO_FL(message) Logger::Get()::LogInfo(message, __FILE__, __LINE__)
#define WARN(message) Logger::Get().LogWarning(message);
#define WARN_FL(message) Logger::Get().LogWarning(message, __FILE__, __LINE__)
#define ERROR(message) Logger::Get().LogError(message);
#define ERROR_FL(message) Logger::Get().LogError(message, __FILE__, __LINE__)

enum class LogLevel
{
	Debug = 1 << 0,
	Info = 1 << 1,
	Warning = 1 << 2,
	Error = 1 << 3,
	All = 1 << 0 | 1 << 1 | 1 << 2 | 1 << 3
};

constexpr inline LogLevel operator|(LogLevel a, LogLevel b)
{
	return static_cast<LogLevel>(static_cast<int>(a) | static_cast<int>(b));
}

constexpr inline int operator&(LogLevel a, LogLevel b)
{
	return static_cast<int>(a) & static_cast<int>(b);
}

std::string Int8HexString(uint8_t x);
std::string Int16HexString(uint16_t x);

class Logger
{
public:
	static Logger& Get()
	{
		static Logger logger;
		return logger;
	}

	void LogDebug(const std::string& message, const char* file = "", int line = -1);
	void LogInfo(const std::string& message, const char* file = "", int line = -1);
	void LogWarning(const std::string& message, const char* file = "", int line = -1);
	void LogError(const std::string& message, const char* file = "", int line = -1);

	void AddOutput(LogLevel sensitivity, std::function<void(std::string message, LogLevel level)> callback);

public:
	static std::function<void(std::string message, LogLevel level)> StandardOutput;
	static std::function<void(std::string message, LogLevel level)> StandardErrorOutput;

private:
	Logger() = default;
	void Log(LogLevel level, const std::string& message, const char* file, int line);
private:

	struct Output
	{
		Output(std::function<void(std::string, LogLevel)> callback, LogLevel sensitivity)
			: callback(callback), sensitivity(sensitivity)
		{
		}
		std::function<void(std::string, LogLevel)> callback;
		LogLevel sensitivity;
	};

	std::vector<Output> _output;
};

