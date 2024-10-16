#include "wreath/json/file.hpp"

#include <fstream>

namespace wreath{
namespace json{

File::File() : root(object_t()){}
File::File(const object_t& ri) : root(ri){}

Token File::operator[](const std::string& name) const{return root.object()[name];}
Token& File::operator[](const std::string& name){return root.object()[name];}

Token File::at(const std::string& name) const{return root.object().at(name);}
Token& File::at(const std::string& name){return root.object().at(name);}

bool File::contains(const std::string& name) const{return root.object().contains(name);}

std::string File::to_string() const{return root.to_string();}
void File::to_file(const std::filesystem::path& path) const{
    std::ofstream file(path, std::ios::out | std::ios::trunc);
    if (!file.is_open()) throw std::runtime_error("Error: Could not open JSON file for writing");
    file << to_string();
}

File File::from_string(std::string json){
    json.erase(std::remove_if(json.begin(), json.end(), [](char c){
        static bool inString = false;
        if (c == '\"') inString = !inString;
        return !inString && std::isspace(c);
    }), json.end());
    return parse_object(json.begin(), json.end()).first;
}
File File::from_file(const std::filesystem::path& path){
    std::ifstream file(path, std::ios::out);
    if (!file.is_open()) throw std::runtime_error("Error: Could not open JSON file for reading");
    return from_string({std::istreambuf_iterator<char>{file}, {}});
}

std::pair<object_t, File::Strit> File::parse_object(File::Strit lhs, File::Strit rhs){
    if (*lhs != '{') return {{}, lhs};
    else lhs++;
    object_t out;
    bool nextObject = false;
    while (*lhs != '}'){
        if (nextObject && *lhs != ',') throw std::runtime_error("Error: No comma before second object (Object)");
        auto name = parse_string(lhs+nextObject, rhs);
        lhs = name.second;
        if (*lhs != ':') throw std::runtime_error("Error: No colon after field name");
        auto value = parse_value(lhs+1, rhs);
        out[name.first] = value.first;
        lhs = value.second;
        nextObject = true;
    }
    return {out, lhs+1};
}
std::pair<array_t, File::Strit> File::parse_array(File::Strit lhs, File::Strit rhs){
    if (*lhs != '[') return {{}, lhs};
    else lhs++;
    array_t out;
    bool nextObject = false;
    while (*lhs != ']'){
        if (nextObject && *lhs != ',') throw std::runtime_error("Error: No comma before second object (Array)");
        auto value = parse_value(lhs+nextObject, rhs);
        out.push_back(value.first);
        lhs = value.second;
        nextObject = true;
    }
    return {out, lhs+1};
}
std::pair<string_t, File::Strit> File::parse_string(File::Strit lhs, File::Strit rhs){
    if (*lhs != '\"') return {"", lhs};
    auto end = std::find(lhs+1, rhs, '\"');
    if (end != rhs) return {{lhs+1, end}, end+1};
    throw std::runtime_error("Error: No end quote");
}
std::pair<boolean_t, File::Strit> File::parse_boolean(File::Strit lhs, File::Strit rhs){
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
std::pair<number_t, File::Strit> File::parse_number(File::Strit lhs, File::Strit rhs){
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
std::pair<Token, File::Strit> File::parse_value(File::Strit lhs, File::Strit rhs){
    if (*lhs == '\"'){
        auto stringValue = parse_string(lhs, rhs);
        return {stringValue.first, stringValue.second};
    }
    else if (*lhs == '-' || std::isdigit(*lhs)){
        auto numberValue = parse_number(lhs, rhs);
        return {numberValue.first, numberValue.second};
    }
    else if (*lhs == '{'){
        auto objectValue = parse_object(lhs, rhs);
        return {objectValue.first, objectValue.second};
    }
    else if (*lhs == '['){
        auto arrayValue = parse_array(lhs, rhs);
        return {arrayValue.first, arrayValue.second};
    }
    else if (*lhs == 't' || *lhs == 'f'){
        auto booleanValue = parse_boolean(lhs, rhs);
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

}
}
