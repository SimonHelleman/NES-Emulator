#include <string.h>
#include <sstream>
#include <iostream>
#include <chrono>
#include "Logger.h"

std::string Int8HexString(uint8_t x)
{
	char tmp[4];
	snprintf(tmp, 4, "%02x", x);

	return std::string(tmp);
}

std::string Int16HexString(uint16_t x)
{
	char tmp[8];
	snprintf(tmp, 8, "%04x", x);

	return std::string(tmp);
}


std::function<void(std::string, LogLevel)> Logger::StandardErrorOutput = [](std::string message, LogLevel)
{
	std::cerr << message << '\n';
};


std::function<void(std::string, LogLevel)> Logger::StandardOutput = [](std::string message, LogLevel)
{
	std::cout << message << '\n';
};

void Logger::LogDebug(const std::string& message, const char* file, int line)
{
	Log(LogLevel::Debug, message, file, line);
}

void Logger::LogInfo(const std::string& message, const char* file, int line)
{
	Log(LogLevel::Info, message, file, line);
}

void Logger::LogWarning(const std::string& message, const char* file, int line)
{
	Log(LogLevel::Warning, message, file, line);
}

void Logger::LogError(const std::string& message, const char* file, int line)
{
	Log(LogLevel::Error, message, file, line);
}

void Logger::Log(LogLevel level, const std::string& message, const char* file, int line)
{
	for (size_t i = 0; i < _output.size(); ++i)
	{
		if (level & _output[i].sensitivity)
		{
			const auto now = std::chrono::system_clock::now();
			const std::time_t time = std::chrono::system_clock::to_time_t(now);

			char buf[256];
			// Who was responsible for ctime being terminated by \n??
			strncpy(buf, std::ctime(&time), 256);
			buf[strlen(buf) - 1] = '\0';
			
			std::stringstream ss;
			ss << '[' << buf << ']';
			switch(level)
			{
			case LogLevel::Debug:
				ss << "[DEBUG]";
				break;
			case LogLevel::Info:
				ss << "[INFO]";
				break;
			case LogLevel::Warning:
				ss << "[WARNING]";
				break;
			case LogLevel::Error:
				ss << "[ERROR]";
			}

			if (file[0] != '\0' && line > 0)
			{
				ss << '[' << file << ':' << line << ']';
			}
			else if (file[0] != '\0')
			{
				ss << '[' << file << ']';
			}

			ss << message;
			_output[i].callback(ss.str(), level);
		}
	}
}

void Logger::AddOutput(LogLevel sensitivity, std::function<void(std::string message, LogLevel level)> callback)
{
	_output.emplace_back(callback, sensitivity);
}