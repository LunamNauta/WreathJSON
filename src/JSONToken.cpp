#include "WreathJSON/JSONToken.hpp"

JSONToken::JSONToken() : type(TokenType::Null), data(){}
JSONToken::JSONToken(const NumberType& ni) : type(TokenType::Number), data(ni){}
JSONToken::JSONToken(const StringType& si) : type(TokenType::String), data(si){}
JSONToken::JSONToken(const BooleanType& bi) : type(TokenType::Boolean), data(bi){}
JSONToken::JSONToken(const ArrayType& ai) : type(TokenType::Array), data(ai){}
JSONToken::JSONToken(const ObjectType& oi) : type(TokenType::Object), data(oi){}
JSONToken::JSONToken(const std::nullptr_t& ni) : type(TokenType::Null), data(false){}

TokenType JSONToken::Type() const{return type;}
bool JSONToken::IsNull() const{return type == TokenType::Null;}

NumberType JSONToken::Number() const{return std::get<NumberType>(data);}
StringType JSONToken::String() const{return std::get<StringType>(data);}
BooleanType JSONToken::Boolean() const{return std::get<BooleanType>(data);}
ArrayType JSONToken::Array() const{return std::get<ArrayType>(data);}
ObjectType JSONToken::Object() const{return std::get<ObjectType>(data);}

NumberType& JSONToken::Number(){return std::get<NumberType>(data);}
StringType& JSONToken::String(){return std::get<StringType>(data);}
BooleanType& JSONToken::Boolean(){return std::get<BooleanType>(data);}
ArrayType& JSONToken::Array(){return std::get<ArrayType>(data);}
ObjectType& JSONToken::Object(){return std::get<ObjectType>(data);}

std::string JSONToken::ToString() const{
    if (type == TokenType::Number) return std::to_string(std::get<NumberType>(data));
    if (type == TokenType::String) return "\"" + std::get<StringType>(data) + "\"";
    if (type == TokenType::Boolean) return std::get<BooleanType>(data) ? "true" : "false";
    if (type == TokenType::Array){
        std::string out = "[";
        const ArrayType& arr = std::get<ArrayType>(data);
        for (auto a = arr.begin(); a != arr.end(); a++){
            out += a->ToString();
            if (a != arr.end()-1) out += ",";
        }
        return out + "]";
    }
    if (type == TokenType::Object){
        std::string out = "{";
        const ObjectType& obj = std::get<ObjectType>(data);
        for (auto a = obj.begin(); a != obj.end(); a++){
            out += "\"" + a->first + "\":" + a->second.ToString();
            if (std::distance(a, obj.end()) != 1) out += ",";
        }
        return out + "}";
    }
    return "null";
}
