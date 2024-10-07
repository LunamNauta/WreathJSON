#ifndef WREATH_JSON_TOKEN_HEADER
#define WREATH_JSON_TOKEN_HEADER

#include <unordered_map>
#include <variant>
#include <vector>
#include <string>

struct JSONToken;
enum class TokenType{
    Number,
    String,
    Boolean,
    Array,
    Object,
    Null
};
typedef long double NumberType;
typedef std::string StringType;
typedef bool BooleanType;
typedef std::vector<JSONToken> ArrayType;
typedef std::unordered_map<StringType, JSONToken> ObjectType;
typedef std::variant<NumberType, StringType, BooleanType, ArrayType, ObjectType> DataType;

struct JSONToken{
private:
    TokenType type;
    DataType data;

public:
    JSONToken();
    JSONToken(const NumberType&);
    JSONToken(const StringType&);
    JSONToken(const BooleanType&);
    JSONToken(const ArrayType&);
    JSONToken(const ObjectType&);
    JSONToken(const std::nullptr_t&);

    TokenType Type() const;
    bool IsNull() const;

    NumberType Number() const;
    StringType String() const;
    BooleanType Boolean() const;
    ArrayType Array() const;
    ObjectType Object() const;

    NumberType& Number();
    StringType& String();
    BooleanType& Boolean();
    ArrayType& Array();
    ObjectType& Object();

    std::string ToString() const;
};

#endif
