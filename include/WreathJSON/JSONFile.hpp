#ifndef WREATH_JSON_FILE_HEADER
#define WREATH_JSON_FILE_HEADER

#include "JSONToken.hpp"

#include <filesystem>
#include <fstream>
#include <string>

struct JSONFile{
private:
    JSONToken root;

    typedef std::string::iterator Strit;
    static std::pair<ObjectType, Strit> ParseObject(Strit lhs, Strit rhs);
    static std::pair<ArrayType, Strit> ParseArray(Strit lhs, Strit rhs);
    static std::pair<StringType, Strit> ParseString(Strit lhs, Strit rhs);
    static std::pair<BooleanType, Strit> ParseBoolean(Strit lhs, Strit rhs);
    static std::pair<NumberType, Strit> ParseNumber(Strit lhs, Strit rhs);
    static std::pair<JSONToken, Strit> ParseValue(Strit lhs, Strit rhs);

public:
    JSONFile();
    JSONFile(const ObjectType&);

    JSONToken operator[](const std::string& name) const;
    JSONToken& operator[](const std::string& name);
    JSONToken at(const std::string& name) const;
    JSONToken& at(const std::string& name);
    bool contains(const std::string& name) const;

    std::string ToString() const;
    void ToFile(std::string path) const;
    
    //TODO: Support UTF-8 encoded strings. Currently only supports ASCII
    //TODO: Replace with more efficient method of reading file
    static JSONFile FromString(std::string);
    static JSONFile FromFile(const std::filesystem::path&);
    static JSONFile FromFile(std::ifstream&);
};

#endif
