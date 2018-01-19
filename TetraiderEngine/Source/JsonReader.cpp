#ifndef JSON_READER_CPP
#define JSON_READER_CPP

#include "JsonReader.h"
#include <fstream>
#include <iostream>

namespace JsonReader {
	//template <typename T>
	//void ParseJsonValue(const json j, T& result, T defaultValue, String first)
	//{
	//	if (j.find(first) != j.end())
	//		result = j[first];
	//	result = defaultValue;
	//}

	//template <typename T>
	//void ParseJsonValue(const json j, T& result, T defaultValue, String first, String second)
	//{
	//	if (j.find(first) != j.end()) {
	//		if (j[first].find(second) != j[first].end())
	//			result = j[first][second];
	//	}6
	//	result = defaultValue;
	//}

	//template <typename T>
	//void ParseJsonValue(const json j, T& result, T defaultValue, String first, String second, String third)
	//{
	//	if (j.find(first) != j.end()) {
	//		if (j[first].find(second) != j[first].end()) {
	//			if (j[first][second].find(third) != j[first][second].end())
	//				result = j[first][second][third];
	//		}
	//	}
	//	result = defaultValue;
	//}

	float ParseFloat(const json j, String first)
	{
		if (j.find(first) != j.end())
				return j[first];

		return 0.0f;
	}

	float ParseFloat(const json j, String first, String second)
	{
		if (j.find(first) != j.end()) {
			if (j[first].find(second) != j[first].end())
				return j[first][second];
		}
		return 0.0f;
	}

	float ParseFloat(const json j, String first, String second, String third)
	{
		if (j.find(first) != j.end()) {
			if (j[first].find(second) != j[first].end()) {
				if (j[first][second].find(third) != j[first][second].end())
					return j[first][second][third];
			}
		}
		return 0.0f;
	}

	double ParseDouble(const json j, String first, String second)
	{
		if (j.find(first) != j.end()) {
			if (j[first].find(second) != j[first].end())
				return j[first][second];
		}
		return 0.0;
	}

	double ParseDouble(const json j, String first, String second, String third)
	{
		if (j.find(first) != j.end()) {
			if (j[first].find(second) != j[first].end()) {
				if (j[first][second].find(third) != j[first][second].end())
					return j[first][second][third];
			}
		}
		return 0.0;
	}

	int ParseInt(const json j, String first)
	{
		if (j.find(first) != j.end())
			return j[first];
		return 0;
	}

	int ParseInt(const json j, String first, String second)
	{
		if (j.find(first) != j.end()) {
			if (j[first].find(second) != j[first].end())
				return j[first][second];
		}
		return 0;
	}

	int ParseInt(const json j, String first, String second, String third)
	{
		if (j.find(first) != j.end()) {
			if (j[first].find(second) != j[first].end()) {
				if (j[first][second].find(third) != j[first][second].end())
					return j[first][second][third];
			}
		}
		return 0;
	}

	String ParseString(const json j, String first)
	{
		if (j.find(first) != j.end())
			return j[first];
		return "";
	}

	String ParseString(const json j, String first, String second)
	{
		if (j.find(first) != j.end()) {
			if (j[first].find(second) != j[first].end())
				return j[first][second];
		}
		return "";
	}

	String ParseString(const json j, String first, String second, String third)
	{
		if (j.find(first) != j.end()) {
			if (j[first].find(second) != j[first].end()) {
				if (j[first][second].find(third) != j[first][second].end())
					return j[first][second][third];
			}
		}
		return "";
	}

	bool ParseBool(const json j, String first)
	{
		if (j.find(first) != j.end())
			return j[first];
		return false;
	}

	bool ParseBool(const json j, String first, String second)
	{
		if (j.find(first) != j.end()) {
			if (j[first].find(second) != j[first].end())
				return j[first][second];
		}
		return false;
	}

	bool ParseBool(const json j, String first, String second, String third)
	{
		if (j.find(first) != j.end()) {
			if (j[first].find(second) != j[first].end()) {
				if (j[first][second].find(third) != j[first][second].end())
					return j[first][second][third];
			}
		}
		return false;
	}

	std::vector<String> ParseStringList(const json j, String first)
	{
		if (j.find(first) != j.end())
			return j[first];
		return std::vector<String>();
	}

	std::vector<String> ParseStringList(const json j, String first, String second)
	{
		if (j.find(first) != j.end()) {
			if (j[first].find(second) != j[first].end())
				return j[first][second];
		}
		return std::vector<String>();
	}

	std::vector<String> ParseStringList(const json j, String first, String second, String third)
	{
		if (j.find(first) != j.end()) {
			if (j[first].find(second) != j[first].end()) {
				if (j[first][second].find(third) != j[first][second].end())
					return j[first][second][third];
			}
		}
		return std::vector<String>();
	}

	Vector2D ParseVector2D(const json j, String first, String second)
	{
		if (j.find(first) != j.end()) {
			if (j[first].find(second) != j[first].end()) {
				return Vector2D(
					ParseFloat(j, first, second, "x"),
					ParseFloat(j, first, second, "y"),
					ParseFloat(j, first, second, "w")
				);
			}
		}
		return Vector2D();
	}

	Vector3D ParseVector3D(const json j, String first, String second)
	{
		if (j.find(first) != j.end()) {
			if (j[first].find(second) != j[first].end()) {
				return Vector3D(
					ParseFloat(j, first, second, "x"),
					ParseFloat(j, first, second, "y"),
					ParseFloat(j, first, second, "z"),
					ParseFloat(j, first, second, "w")
				);
			}
		}
		return Vector3D();
	}

	Vector3D ParseColor(const json j, String first, String second)
	{
		if (j.find(first) != j.end()) {
			if (j[first].find(second) != j[first].end()) {
				return Vector3D(
					ValueExists(j, first, second) ? ParseFloat(j, first, second, "r") : 1.0f,
					ValueExists(j, first, second) ? ParseFloat(j, first, second, "g") : 1.0f,
					ValueExists(j, first, second) ? ParseFloat(j, first, second, "b") : 1.0f,
					ValueExists(j, first, second) ? ParseFloat(j, first, second, "a") : 1.0f
				);
			}
		}
		return Vector3D(1, 1, 1, 1);
	}

	bool ValueExists(const json j, String first, String second)
	{
		if (j.find(first) != j.end()) {
			return j[first].find(second) != j[first].end();
		}
		return false;
	}

	bool ValueExists(const json j, String first, String second, String third)
	{
		if (j.find(first) != j.end()) {
			if (j[first].find(second) != j[first].end()) {
				if (j[first][second].find(third) != j[first][second].end())
					return true;
			}
		}
		return false;
	}

	json OpenJsonFile(String fileName)
	{
		try {
			std::ifstream i(fileName);
			json j;
			i >> j;
			i.close();
			return j;
		}
		catch (json::parse_error &e) {
			std::cerr << e.what() << " in" + fileName << std::endl;
			return 0;
		}
	}

	bool KeyIs(json::iterator it, String key)
	{
		return it.key().compare(key) == 0;
	}

}

#endif