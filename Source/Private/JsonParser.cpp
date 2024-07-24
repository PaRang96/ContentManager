#include "JsonParser.h"

JsonParser::JsonParser()
{

}

JsonParser::~JsonParser()
{

}

bool JsonParser::ReadJsonFile(const std::string& filepath)
{
	std::ifstream file(filepath);
	if (!file.is_open())
	{
		std::cerr << "Could not Open Json File from " << filepath << "! \nPlease check the route" << std::endl;
		return false;
	}
	std::stringstream buffer;
	buffer << file.rdbuf();
	JsonContent = buffer.str();

	JsonDocument.Parse(JsonContent.c_str());

	if (JsonDocument.HasParseError())
	{
		std::cerr << "Error Parsing the JSON file!" << std::endl;
		return false;
	}
	return true;
}

bool JsonParser::GetStringField(std::string FieldName, std::string& Target)
{
	if (!JsonDocument.HasMember(FieldName.c_str()))
	{
		return false;
	}
	rapidjson::Value& Value = JsonDocument[FieldName.c_str()];
	if (!Value.IsString())
	{
		return false;
	}
	Target = Value.GetString();
	if (Target.length() == 0)
	{
		return false;
	}
	return true;
}

bool JsonParser::GetStringArrayField(std::string FieldName, std::vector<std::string>& Target)
{
	if (!JsonDocument.HasMember(FieldName.c_str()))
	{
		return false;
	}
	rapidjson::Value& Value = JsonDocument[FieldName.c_str()];
	if (!Value.IsArray())
	{
		return false;
	}
	auto candidate = Value.GetArray();
	for (unsigned int i = 0; i < candidate.Size(); i++)
	{
		if (!candidate[i].IsString())
		{
			return false;
		}
		Target.push_back(candidate[i].GetString());
	}
	return true;
}

bool JsonParser::GetFloatField(std::string FieldName, float& Target)
{
	if (!JsonDocument.HasMember(FieldName.c_str()))
	{
		return false;
	}
	rapidjson::Value& Value = JsonDocument[FieldName.c_str()];
	if (!Value.IsFloat())
	{
		return false;
	}
	Target = Value.GetFloat();
	return true;
}

bool JsonParser::GetFloatArrayField(std::string FieldName, std::vector<float>& Target)
{
	if (!JsonDocument.HasMember(FieldName.c_str()))
	{
		return false;
	}
	rapidjson::Value& Value = JsonDocument[FieldName.c_str()];
	if (!Value.IsArray())
	{
		return false;
	}
	auto candidate = Value.GetArray();
	for (unsigned int i = 0; i < candidate.Size(); i++)
	{
		if (!candidate[i].IsFloat())
		{
			return false;
		}
		Target.push_back(candidate[i].GetFloat());
	}
	return true;
}

bool JsonParser::GetIntField(std::string FieldName, int& Target)
{
	if (!JsonDocument.HasMember(FieldName.c_str()))
	{
		return false;
	}
	rapidjson::Value& Value = JsonDocument[FieldName.c_str()];
	if (!Value.IsInt())
	{
		return false;
	}
	Target = Value.GetInt();
	return true;
}

bool JsonParser::GetIntArrayField(std::string FieldName, std::vector<int>& Target)
{
	if (!JsonDocument.HasMember(FieldName.c_str()))
	{
		return false;
	}
	rapidjson::Value& Value = JsonDocument[FieldName.c_str()];
	if (!Value.IsArray())
	{
		return false;
	}
	auto candidate = Value.GetArray();
	for (unsigned int i = 0; i < candidate.Size(); i++)
	{
		if (!candidate[i].IsInt())
		{
			return false;
		}
		Target.push_back(candidate[i].GetInt());
	}
	return true;
}

bool JsonParser::GetObjectField(std::string FieldName, rapidjson::GenericObject<false, rapidjson::Value>& Target)
{
	if (!JsonDocument.HasMember(FieldName.c_str()))
	{
		return false;
	}
	rapidjson::Value& Value = JsonDocument[FieldName.c_str()];
	if (!Value.IsObject())
	{
		return false;
	}
	Target = Value.GetObject();
	return true;
}

bool JsonParser::GetObjectArrayField(std::string FieldName, std::vector<rapidjson::GenericObject<false, rapidjson::Value>>& Target)
{
	if (!JsonDocument.HasMember(FieldName.c_str()))
	{
		return false;
	}
	rapidjson::Value& Value = JsonDocument[FieldName.c_str()];
	if (!Value.IsArray())
	{
		return false;
	}
	auto candidate = Value.GetArray();
	for (unsigned int i = 0; i < candidate.Size(); i++)
	{
		if (!candidate[i].IsObject())
		{
			return false;
		}
		Target.push_back(candidate[i].GetObject());
	}
	return true;
}
