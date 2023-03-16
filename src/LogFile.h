#pragma once

#include "Noncopyable.h"
#include <string>
namespace zcoco {
class LogFile : public Noncopyable {
public:
	LogFile(std::string basename);
	~LogFile();

	void persist(const char* data, size_t length);
	void flush();

private:
	std::string getFileName();
	static const int kMaxFileSize = 1024 * 1024 * 1024;

	std::string basename_;
	int bytes_writed_;
	FILE* fp_;
};
}