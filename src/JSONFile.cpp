#include "WreathJSON/JSONFile.hpp"

JSONFile::JSONFile() : root(ObjectType()){}
JSONFile::JSONFile(const ObjectType& ri) : root(ri){}

JSONToken JSONFile::operator[](const std::string& name) const{return root.Object()[name];}
JSONToken& JSONFile::operator[](const std::string& name){return root.Object()[name];}
JSONToken JSONFile::at(const std::string& name) const{return root.Object().at(name);}
JSONToken& JSONFile::at(const std::string& name){return root.Object().at(name);}
bool JSONFile::contains(const std::string& name) const{return root.Object().contains(name);}

std::string JSONFile::ToString() const{return root.ToString();}
void JSONFile::ToFile(std::string path) const{
    std::ofstream file(path, std::ios::out | std::ios::trunc);
    if (!file.is_open()) throw std::runtime_error("Error: Could not open JSON file for writing");
    file << ToString();
}
    
//TODO: Support UTF-8 encoded strings. Currently only supports ASCII
//TODO: Replace with more efficient method of reading file
JSONFile JSONFile::FromString(std::string json){
    json.erase(std::remove_if(json.begin(), json.end(), [](char c){
        static bool inString = false;
        if (c == '\"') inString = !inString;
        return !inString && std::isspace(c);
    }), json.end());
    return ParseObject(json.begin(), json.end()).first;
}
JSONFile JSONFile::FromFile(const std::filesystem::path& path){
    std::ifstream file(path, std::ios::out);
    if (!file.is_open()) throw std::runtime_error("Error: Could not open JSON file for reading");
    return FromString({std::istreambuf_iterator<char>{file}, {}});
}
JSONFile JSONFile::FromFile(std::ifstream& file){
    if (!file.is_open()) throw std::runtime_error("Error: JSON file is not open for reading");
    return FromString({std::istreambuf_iterator<char>{file}, {}});
}

std::pair<ObjectType, JSONFile::Strit> JSONFile::ParseObject(JSONFile::Strit lhs, JSONFile::Strit rhs){
    if (*lhs != '{') return {{}, lhs};
    else lhs++;
    ObjectType out;
    bool nextObject = false;
    while (*lhs != '}'){
        if (nextObject && *lhs != ',') throw std::runtime_error("Error: No comma before second object (Object)");
        auto name = ParseString(lhs+nextObject, rhs);
        lhs = name.second;
        if (*lhs != ':') throw std::runtime_error("Error: No colon after field name");
        auto value = ParseValue(lhs+1, rhs);
        out[name.first] = value.first;
        lhs = value.second;
        nextObject = true;
    }
    return {out, lhs+1};
}
std::pair<ArrayType, JSONFile::Strit> JSONFile::ParseArray(JSONFile::Strit lhs, JSONFile::Strit rhs){
    if (*lhs != '[') return {{}, lhs};
    else lhs++;
    ArrayType out;
    bool nextObject = false;
    while (*lhs != ']'){
        if (nextObject && *lhs != ',') throw std::runtime_error("Error: No comma before second object (Array)");
        auto value = ParseValue(lhs+nextObject, rhs);
        out.push_back(value.first);
        lhs = value.second;
        nextObject = true;
    }
    return {out, lhs+1};
}
std::pair<StringType, JSONFile::Strit> JSONFile::ParseString(JSONFile::Strit lhs, JSONFile::Strit rhs){
    if (*lhs != '\"') return {"", lhs};
    auto end = std::find(lhs+1, rhs, '\"');
    if (end != rhs) return {{lhs+1, end}, end+1};
    throw std::runtime_error("Error: No end quote");
}
std::pair<BooleanType, JSONFile::Strit> JSONFile::ParseBoolean(JSONFile::Strit lhs, JSONFile::Strit rhs){
    if (*lhs == 't'){
        if (lhs+4 >= rhs) throw std::invalid_argument("Error: Not enough space for 'true'");
        auto booleanValue = std::string(lhs, lhs+4);
        if (booleanValue != "true") throw std::runtime_error("Error: Expected boolean 'true'");
        return {true, lhs+4};
    }
    if (*lhs == 'f'){
        if (lhs+5 >= rhs) throw std::invalid_argument("Error: Not enough space for 'false'");
        auto booleanValue = std::string(lhs, lhs+5);
        if (booleanValue != "false") throw std::runtime_error("Error: Expected boolean 'false'");
        return {false, lhs+5};
    }
    return {false, lhs};
}
std::pair<NumberType, JSONFile::Strit> JSONFile::ParseNumber(JSONFile::Strit lhs, JSONFile::Strit rhs){
    std::string tmp = "";
    if (*lhs == '-') tmp += *(lhs++);
    if (*lhs == '0') tmp += '0';
    else if (std::isdigit(*lhs)){
        tmp += *lhs;
        while (std::isdigit(*(++lhs))) tmp += *lhs;
    }
    if (*lhs == '.'){
        tmp += *lhs;
        while (std::isdigit(*(++lhs))) tmp += *lhs;
    }
    if (*lhs == 'e' || *lhs == 'E'){
        tmp += *lhs;
        if (*(++lhs) == '-' || *lhs == '+') tmp += *(lhs++);
        if (std::isdigit(*lhs)){
            tmp += *lhs;
            while (std::isdigit(*(++lhs))) tmp += *lhs;
        } else throw std::runtime_error("Error: No digit after exponent");
    }
    return {std::stold(tmp), lhs};
}
std::pair<JSONToken, JSONFile::Strit> JSONFile::ParseValue(JSONFile::Strit lhs, JSONFile::Strit rhs){
    if (*lhs == '\"'){
        auto stringValue = ParseString(lhs, rhs);
        return {stringValue.first, stringValue.second};
    }
    else if (*lhs == '-' || std::isdigit(*lhs)){
        auto numberValue = ParseNumber(lhs, rhs);
        return {numberValue.first, numberValue.second};
    }
    else if (*lhs == '{'){
        auto objectValue = ParseObject(lhs, rhs);
        return {objectValue.first, objectValue.second};
    }
    else if (*lhs == '['){
        auto arrayValue = ParseArray(lhs, rhs);
        return {arrayValue.first, arrayValue.second};
    }
    else if (*lhs == 't' || *lhs == 'f'){
        auto booleanValue = ParseBoolean(lhs, rhs);
        return {booleanValue.first, booleanValue.second};
    }
    else if (*lhs == 'n'){
        if (lhs+4 >= rhs) throw std::invalid_argument("Error: Not enough space for 'null'");
        auto booleanValue = std::string(lhs, lhs+4);
        if (booleanValue != "null") throw std::runtime_error("Error: Expected 'null'");
        return {nullptr, lhs+4};
    }
    throw std::runtime_error("Error: Expected value");
}
