#include "wreath/json/token.hpp"

namespace wreath{
namespace json{

Token::Token() : tokenType(token_t::Null), data(){}
Token::Token(const number_t& ni) : tokenType(token_t::Number), data(ni){}
Token::Token(const string_t& si) : tokenType(token_t::String), data(si){}
Token::Token(const boolean_t& bi) : tokenType(token_t::Boolean), data(bi){}
Token::Token(const array_t& ai) : tokenType(token_t::Array), data(ai){}
Token::Token(const object_t& oi) : tokenType(token_t::Object), data(oi){}
Token::Token(const std::nullptr_t& ni) : tokenType(token_t::Null), data(false){}

enum token_t Token::type() const{return tokenType;}
bool Token::is_null() const{return tokenType == token_t::Null;}

number_t Token::number() const{return std::get<number_t>(data);}
string_t Token::string() const{return std::get<string_t>(data);}
boolean_t Token::boolean() const{return std::get<boolean_t>(data);}
array_t Token::array() const{return std::get<array_t>(data);}
object_t Token::object() const{return std::get<object_t>(data);}

number_t& Token::number(){return std::get<number_t>(data);}
string_t& Token::string(){return std::get<string_t>(data);}
boolean_t& Token::boolean(){return std::get<boolean_t>(data);}
array_t& Token::array(){return std::get<array_t>(data);}
object_t& Token::object(){return std::get<object_t>(data);}

std::string Token::to_string() const{
    if (tokenType == token_t::Number) return std::to_string(std::get<number_t>(data));
    if (tokenType == token_t::String) return "\"" + std::get<string_t>(data) + "\"";
    if (tokenType == token_t::Boolean) return std::get<boolean_t>(data) ? "true" : "false";
    if (tokenType == token_t::Array){
        std::string out = "[";
        const array_t& arr = std::get<array_t>(data);
        for (auto a = arr.begin(); a != arr.end(); a++){
            out += a->to_string();
            if (a != arr.end()-1) out += ",";
        }
        return out + "]";
    }
    if (tokenType == token_t::Object){
        std::string out = "{";
        const object_t& obj = std::get<object_t>(data);
        for (auto a = obj.begin(); a != obj.end(); a++){
            out += "\"" + a->first + "\":" + a->second.to_string();
            if (std::distance(a, obj.end()) != 1) out += ",";
        }
        return out + "}";
    }
    return "null";
}

}
}
