/* Start Header -------------------------------------------------------
Copyright (C) 2017 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name: JsonReader.h
Purpose: A set of functions for opening and reading a JSON file.
Language: C++
Platform: Windows 8.1 / 10.0.15063.0  or  GCC 7.1.0
Project: CS529_holdenprofit_
Author: Holden Profit, holden.profit, 42833
Creation date: 10/29/17
- End Header --------------------------------------------------------*/

#pragma once

#ifndef JSON_READER_H
#define JSON_READER_H

#include "External\json.hpp"
#include "Math\Vector2D.h"
#include "Math\Vector3D.h"
#include <vector>
#include <string>

using json = nlohmann::json;

namespace JsonReader {
	typedef std::string String;

	//template <typename ParseType, typename Indexor>
	//ParseType ParseValue(const json j, ParseType defaultValue, Indexor index)
	//{
	//	if (j.find(index) != j.end())
	//		return j[index];
	//	else
	//		return defaultValue;
	//}

	//template <typename ParseType, class ... Indexor>
	//ParseType ParseValue(const json j, ParseType defaultValue, Indexor ... indeces)
	//{
	//	return ParseValue(j, result, defaultValue, indeces...);
	//}

	template <typename ParseType, typename I1>
	ParseType ParseValue(const json j, ParseType defaultValue, I1 i1)
	{
		if (j.find(i1) != j.end()) return j[i1];
		return defaultValue;
	}


	template <typename ParseType, typename I1, typename I2, typename I3>
	ParseType ParseValue(const json j, ParseType defaultValue, I1 i1, I2 i2, I3 i3)
	{
		if (j.find(i1) != j.end()) {
			if (j[i1].find(i2) != j[i1].end()) {
				if (j[i1][i2].find(i3) != j[i1][i2].end())
					return j[i1][i2][i3];
			}
		}
		return defaultValue;
	}






	//template <typename T>
	//void ParseJsonValue(const json j, T&, T, String);

	//template <typename T>
	//void ParseJsonValue(const json j, T&, T, String, String);

	//template <typename T>
	//void ParseJsonValue(const json j, T&, T, String, String, String);

	float ParseFloat(const json j, String);
	float ParseFloat(const json j, String, String);
	float ParseFloat(const json j, String, String, String);

	double ParseDouble(const json j, String, String);
	double ParseDouble(const json j, String, String, String);

	int ParseInt(const json j, String);
	int ParseInt(const json j, String, String);
	int ParseInt(const json j, String, String, String);

	String ParseString(const json j, String);
	String ParseString(const json j, String, String);
	String ParseString(const json j, String, String, String);

	bool ParseBool(const json j, String);
	bool ParseBool(const json j, String, String);
	bool ParseBool(const json j, String, String, String);

	std::vector<String> ParseStringList(const json j, String);
	std::vector<String> ParseStringList(const json j, String, String);
	std::vector<String> ParseStringList(const json j, String, String, String);

	Vector2D ParseVector2D(const json j, String);
	Vector2D ParseVector2D(const json j, String, String);
	
	Vector3D ParseVector3D(const json j, String);
	Vector3D ParseVector3D(const json j, String, String);

	Vector3D ParseColor(const json j, String);
	Vector3D ParseColor(const json j, String, String);

	bool ValueExists(const json j, String);
	bool ValueExists(const json j, String, String);
	bool ValueExists(const json j, String, String, String);
	
	json OpenJsonFile(String fileName);

	bool KeyIs(json::iterator it, String key);
	

	//#include "JsonReader.cpp"
}

#endif