#pragma once

#ifndef MY_ARRAY_H
#define MY_ARRAY_H

template<typename T>
class MyArray {
public:
	MyArray() m_size(0) {}
	~MyArray() {}
	void Remove(const T&) {
		for (int i = 0; i < m_size; ++i) {
			if (m_array[i] == T) {
				m_array[i] = m_array[m_size - 1];
				m_array[m_size - 1] = nullptr;
				--m_size;
				break;
			}
		}
	}
	void PushBack(const T&) {
		m_array[m_size] = T;
		++m_size;
	}
	void Clear() {
		for (int i = 0; i < m_size; ++i) {
			m_array[i] = nullptr;
		}
		m_size = 0;
	}
	int Size() { return m_size; }
	T* operator [] (int i) { return m_array[i]; }
private:
	T* m_array[100];
	int m_size;
};


#endif