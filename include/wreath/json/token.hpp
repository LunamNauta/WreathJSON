#ifndef WREATH_JSON_TOKEN_HEADER
#define WREATH_JSON_TOKEN_HEADER

#include <unordered_map>
#include <variant>
#include <vector>
#include <string>

namespace wreath{
namespace json{

struct Token;
enum class token_t{
    Number,
    String,
    Boolean,
    Array,
    Object,
    Null
};
typedef long double number_t;
typedef std::string string_t;
typedef bool boolean_t;
typedef std::vector<Token> array_t;
typedef std::unordered_map<string_t, Token> object_t;
typedef std::variant<number_t, string_t, boolean_t, array_t, object_t> data_t;

struct Token{
private:
    enum token_t tokenType;
    data_t data;

public:
    Token();
    Token(const number_t&);
    Token(const string_t&);
    Token(const boolean_t&);
    Token(const array_t&);
    Token(const object_t&);
    Token(const std::nullptr_t&);

    enum token_t type() const;
    bool is_null() const;

    number_t number() const;
    string_t string() const;
    boolean_t boolean() const;
    array_t array() const;
    object_t object() const;

    number_t& number();
    string_t& string();
    boolean_t& boolean();
    array_t& array();
    object_t& object();

    std::string to_string() const;
};

}
}

#endif
