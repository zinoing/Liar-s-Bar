#pragma once

template <typename T>
class Singleton {

	static T instance;

	Singleton() = default;
	Singleton(const Singleton&) = delete;
	Singleton& operator=(const Singleton&) = delete;
	Singleton(Singleton&&) = delete;
	Singleton& operator=(Singleton&&) = delete;

public:
	static T& getInstance() {
		return instance;
	}
};

template <typename T>
T Singleton<T>::instance;
