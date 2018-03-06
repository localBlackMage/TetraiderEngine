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

//#include "External/json.hpp"
//#include "Math/MathLibs.h"
//#include <vector>
//#include <string>

using json = nlohmann::json;

typedef std::vector<Point> ControlPoints;

namespace JsonReader {
	typedef std::string String;

	std::vector<Point> ParsePoints(const json& j, const String&);
	std::vector<Point> ParsePoints(const json& j, const String&, const String&);

	float ParseFloat(const json& j, const String&);
	float ParseFloat(const json& j, const String&, const String&);
	float ParseFloat(const json& j, const String&, const String&, const String&);

	double ParseDouble(const json& j, const String&);
	double ParseDouble(const json& j, const String&, const String&);
	double ParseDouble(const json& j, const String&, const String&, const String&);

	int ParseInt(const json& j, const String&);
	int ParseInt(const json& j, const String&, const String&);
	int ParseInt(const json& j, const String&, const String&, const String&);

	String ParseString(const json& j, const String&);
	String ParseString(const json& j, const String&, const String&);
	String ParseString(const json& j, const String&, const String&, const String&);

	bool ParseBool(const json& j, const String&);
	bool ParseBool(const json& j, const String&, const String&);
	bool ParseBool(const json& j, const String&, const String&, const String&);

	std::vector<String> ParseStringList(const json& j, const String&);
	std::vector<String> ParseStringList(const json& j, const String&, const String&);
	std::vector<String> ParseStringList(const json& j, const String&, const String&, const String&);

	Vector2D ParseVector2D(const json& j, const String&);
	Vector2D ParseVector2D(const json& j, const String&, const String&);
	
	Vector3D ParseVector3D(const json& j, const String&);
	Vector3D ParseVector3D(const json& j, const String&, const String&);

	Vector3D ParseColor(const json& j, const String&);
	Vector3D ParseColor(const json& j, const String&, const String&);

	bool ValueExists(const json& j, const String&);
	bool ValueExists(const json& j, const String&, const String&);
	bool ValueExists(const json& j, const String&, const String&, const String&);
	
	json OpenJsonFile(const String& fileName);

	bool KeyIs(json::iterator it, const String& key);
}

#endif