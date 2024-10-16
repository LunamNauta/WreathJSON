# wreath-json

"wreath-json" is an attempt to add JSON parsing capabilities to C++. Currently, only basic support for reading JSON is supported.\
The API is too convoluted for easy use, so the next major addition will be a simplification to the API

# Functionality (Everything is in the "wreath" namespace)

## Tokens (Everything is in the "wreath::json" namespace)

Each JSON object is split into tokens. These tokens are:\
```enum class token_t{Number, String, Boolean, Array, Object, Null}```\
A ```Token``` Is effectively a ```union``` of potential tokens.\
The underlying types for the tokens are subject to change, but currently are equivalent to:
```
typedef long double number_t;
typedef std::string string_t;
typedef bool boolean_t;
typedef std::vector<Token> array_t;
typedef std::unordered_map<string_t, Token> object_t;
```
Null is a special in that it doesn't have a value.\
Its existence means that no member of the union has been set

### Tokens Functions

Constructors for each token type (as well as a default constructor, which maps to ```null```).
```
Token();
Token(const number_t&);
Token(const string_t&);
Token(const boolean_t&);
Token(const array_t&);
Token(const object_t&);
Token(const std::nullptr_t&);
```
Special getter functions for the token type and whether it's null.
```
enum token_t type() const;
bool is_null() const;
```
Getters for each token type.
```
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
```
Function that converts the token to an std::string (the token itself will be valid JSON).
```
std::string to_string() const;
```

## Files (Everything is in the "wreath::json" namespace)

A ```File``` is just a wrapper around a root ```Token```.\
Necessarily, this root will always be of type ```object_t```.

### Files Functions
Default constructor and one to take in an ```object_t```.
```
File();
File(const object_t&);
```
Accessors for the fields of the root ```object_t```.\
These will return ```Token```s.\
Each token must be cast to the appropriate type by calling the getter function for that type before the underlying data is accessed.
```
Token operator[](const std::string& name) const;
Token& operator[](const std::string& name);

Token at(const std::string& name) const;
Token& at(const std::string& name);

bool contains(const std::string& name) const;
```
Functions for turning the ```File``` to a string (and writing that string to a file).
```
std::string to_string() const;
void to_file(const std::filesystem::path&) const;
```
Functions for reading the ```File``` from a string or file.
```
static File from_string(std::string);
static File from_file(const std::filesystem::path&);
```
