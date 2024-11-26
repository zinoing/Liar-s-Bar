#pragma once

template <typename T>
class Singleton {
	static T* instance;

public:
	static T* getInstance() {
		if (instance == nullptr)
			instance = new T();

		return instance;
	}
};

template <typename T>
T* Singleton<T>::instance = nullptr;
