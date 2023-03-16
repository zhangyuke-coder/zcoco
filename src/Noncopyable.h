#pragma once

namespace zcoco {
class Noncopyable {
public:
	Noncopyable(const Noncopyable& rhs) = delete;
	Noncopyable& operator=(const Noncopyable& rhs) = delete;

protected:
	Noncopyable() = default;
	~Noncopyable() = default;


};
}