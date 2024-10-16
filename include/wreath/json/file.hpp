#ifndef WREATH_JSON_FILE_HEADER
#define WREATH_JSON_FILE_HEADER

#include "token.hpp"

#include <filesystem>
#include <string>

namespace wreath{
namespace json{

struct File{
private:
    Token root;

    typedef std::string::iterator Strit;
    static std::pair<object_t, Strit> parse_object(Strit lhs, Strit rhs);
    static std::pair<array_t, Strit> parse_array(Strit lhs, Strit rhs);
    static std::pair<string_t, Strit> parse_string(Strit lhs, Strit rhs);
    static std::pair<boolean_t, Strit> parse_boolean(Strit lhs, Strit rhs);
    static std::pair<number_t, Strit> parse_number(Strit lhs, Strit rhs);
    static std::pair<Token, Strit> parse_value(Strit lhs, Strit rhs);

public:
    File();
    File(const object_t&);

    Token operator[](const std::string& name) const;
    Token& operator[](const std::string& name);

    Token at(const std::string& name) const;
    Token& at(const std::string& name);

    bool contains(const std::string& name) const;

    std::string to_string() const;
    void to_file(const std::filesystem::path&) const;

    static File from_string(std::string);
    static File from_file(const std::filesystem::path&);
};

}
}

#endif
