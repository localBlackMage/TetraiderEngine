#pragma once

#ifndef SORTING_H
#define SORTING_H

#include <vector>

#include <iostream>
using namespace std;

namespace Sorting {
	template <typename Element, typename CompareFunc>
	void InsertionSort(std::vector<Element>& _array, CompareFunc _compareFunc) {
		int length = _array.size();
		int j;
		Element temp;

		for (int i = 0; i < length; ++i) {
			j = i;

			while (j > 0 && _compareFunc(_array[j], _array[j - 1])) {
				temp = _array[j];
				_array[j] = _array[j - 1];
				_array[j - 1] = temp;
				j--;
			}
		}
	}

}

#endif