#pragma once

// stored deleter for singleton
template <typename T>
class Singleton_Deleter
{
public:
	void operator()(const T * const instance);
};

template <typename T>
class Singleton
{
public:
	static T &GetInstance(void);

private:
	Singleton();
};

template <typename T>
void Singleton_Deleter<T>::operator()(const T * const instance)
{
	delete instance;
}

template <typename T>
T &Singleton<T>::GetInstance(void)
{
	static std::unique_ptr<T, Singleton_Deleter<T>> instance_ptr(new T());

	return *instance_ptr;
}

template <typename T>
Singleton<T>::Singleton() {}