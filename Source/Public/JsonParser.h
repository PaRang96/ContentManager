#pragma once

#include <rapidjson/document.h>
#include <rapidjson/writer.h>
#include <rapidjson/stringbuffer.h>

#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

class JsonParser
{
private:
	std::string filepath = "Config.json";
	std::string JsonContent = "";
	rapidjson::Document JsonDocument;

protected:

public:
	inline std::string GetFilePath() { return filepath; };

	JsonParser();
	virtual ~JsonParser();
	bool ReadJsonFile(const std::string& filepath);

	/// <summary>
	/// Get string value from config json
	/// </summary>
	/// <param name="FieldName"> Wanted field name </param>
	/// <param name="Target"> Target string to put the value in </param>
	/// <returns> Returns false if there's no field name in json
	/// Also return false when field has zero-length string </returns>
	bool GetStringField(std::string FieldName, std::string& Target);

	/// <summary>
	/// 
	/// </summary>
	/// <param name="FieldName"></param>
	/// <param name="Target"></param>
	/// <returns></returns>
	bool GetStringArrayField(std::string FieldName, std::vector<std::string>& Target);

	/// <summary>
	/// Get float value from config json
	/// </summary>
	/// <param name="FieldName"> Wanted float name </param>
	/// <param name="Target"> Target float to put the value in </param>
	/// <returns> Returns false if there's no field name in json </returns>
	bool GetFloatField(std::string FieldName, float& Target);

	/// <summary>
	/// Get float array value from config json
	/// </summary>
	/// <param name="FieldName"> Wanted float array name </param>
	/// <param name="Target"> Target float arry to put the value in </param>
	/// <returns> Returns false if there's no field name in json </returns>
	bool GetFloatArrayField(std::string FieldName, std::vector<float>& Target);

	/// <summary>
	/// Get int value from config json
	/// </summary>
	/// <param name="FieldName"> Wanted int name </param>
	/// <param name="Target"> Target int to put the value in </param>
	/// <returns> Returns false if there's no field name in json </returns>
	bool GetIntField(std::string FieldName, int& Target);

	/// <summary>
	/// Get int array value from config json
	/// </summary>
	/// <param name="FieldName"> Wanted int array name </param>
	/// <param name="Target"> Target int array to put the value in </param>
	/// <returns> Returns false if there's no field name in json </returns>
	bool GetIntArrayField(std::string FieldName, std::vector<int>& Target);

	/// <summary>
	/// Get object value from config json
	/// </summary>
	/// <param name="FieldName"> Wanted object name </param>
	/// <param name="Target"> Target object to put the value in </param>
	/// <returns> Returns false if there's no field name in json </returns>
	bool GetObjectField(std::string FieldName, rapidjson::GenericObject<false, rapidjson::Value>& Target);

	/// <summary>
	/// Get object array value from config json
	/// </summary>
	/// <param name="FieldName"> Wanted object array name </param>
	/// <param name="Target"> Target object array to put the value in </param>
	/// <returns> Returns false if there's no field name in json </returns>
	bool GetObjectArrayField(std::string FieldName, std::vector<rapidjson::GenericObject<false, rapidjson::Value>>& Target);
};