# WreathJSON

WreathJSON is an attempt to add JSON parsing capabilities to C++. Currently, only basic support for reading JSON is supported.\
The API is too convoluted for easy use, so the next major addition will be a simplification to the API

## Tokens

Each JSON object is split into tokens. These tokens are:\
```enum class TokenType{Number, String, Boolean, Array, Object, Null}```\
A ```JSONToken``` Is effectively a ```union``` of potential tokens.\
The underlying types for the tokens are subject to change, but currently are equivalent to:
```
typedef long double NumberType;
typedef std::string StringType;
typedef bool BooleanType;
typedef std::vector<JSONToken> ArrayType;
typedef std::unordered_map<StringType, JSONToken> ObjectType;
```
Null is a special in that it doesn't have a value.\
Its existence means that no member of the union has been set

### Tokens Functions

Constructors for each token type (as well as a default constructor, which maps to ```null```).
```
JSONToken();
JSONToken(const NumberType&);
JSONToken(const StringType&);
JSONToken(const BooleanType&);
JSONToken(const ArrayType&);
JSONToken(const ObjectType&);
JSONToken(const std::nullptr_t&);
```
Special getter functions for the token type and whether it's null.
```
enum TokenType Type() const;
bool IsNull() const;
```
Getters for each token type.
```
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
```
Function that converts the token to an std::string (the token itself will be valid JSON).
```
std::string ToString() const;
```

### Files

A ```JSONFile``` is just a wrapper around a root ```JSONToken```.\
Necessarily, this root will always be of type ```ObjectType```.

### Files Functions
Default constructor and one to take in an ObjectType.
```
JSONFile();
JSONFile(const ObjectType&);
```
Accessors for the fields of the root ObjectType.\
These will return ```JSONToken```s.\
Each token must be cast to the appropriate type by calling the getter function for that type before the underlying data is accessed.
```
JSONToken operator[](const std::string& name) const;
JSONToken& operator[](const std::string& name);
JSONToken at(const std::string& name) const;
JSONToken& at(const std::string& name);
bool contains(const std::string& name) const;
```
Functions for turning the ```JSONFile``` to a string (and writing that string to a file).
```
std::string ToString() const;
void ToFile(std::string path) const;
```
Functions for reading the ```JSONFile``` from a string or file.
```
static JSONFile FromString(std::string);
static JSONFile FromFile(const std::filesystem::path&);
static JSONFile FromFile(std::ifstream&);
```
