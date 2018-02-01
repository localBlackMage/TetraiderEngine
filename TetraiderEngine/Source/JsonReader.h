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

#include "External/json.hpp"
#include "Math/MathLibs.h"
#include <vector>
#include <string>

using json = nlohmann::json;

namespace JsonReader {
	typedef std::string String;

	float ParseFloat(const json j, String);
	float ParseFloat(const json j, String, String);
	float ParseFloat(const json j, String, String, String);

	double ParseDouble(const json j, String);
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
}

#endif