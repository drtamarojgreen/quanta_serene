#ifndef JSON_H
#define JSON_H

#include <string>
#include <vector>
#include <map>
#include <iostream>
#include <sstream>

enum class JsonType {
    Null,
    Bool,
    Number,
    String,
    Array,
    Object
};

class JsonValue {
public:
    JsonValue() : type(JsonType::Null) {}
    JsonValue(bool b) : type(JsonType::Bool), bool_val(b) {}
    JsonValue(double d) : type(JsonType::Number), num_val(d) {}
    JsonValue(int i) : type(JsonType::Number), num_val(static_cast<double>(i)) {}
    JsonValue(long long l) : type(JsonType::Number), num_val(static_cast<double>(l)) { std::cout << "long long const" << std::endl;}
    JsonValue(const std::string& s) : type(JsonType::String), str_val(s) {}
    JsonValue(const char* s) : type(JsonType::String), str_val(s) {}

    static JsonValue array() {
        JsonValue v;
        v.type = JsonType::Array;
        return v;
    }

    static JsonValue object() {
        JsonValue v;
        v.type = JsonType::Object;
        return v;
    }

    void push_back(const JsonValue& v) {
        if (type == JsonType::Array) array_val.push_back(v);
    }

    JsonValue& operator[](const std::string& key) {
        if (type != JsonType::Object) {
            type = JsonType::Object;
            object_val.clear();
        }
        return object_val[key];
    }

    const JsonValue& operator[](const std::string& key) const {
        if (type == JsonType::Object) {
            auto it = object_val.find(key);
            if (it != object_val.end()) return it->second;
        }
        static const JsonValue null_v;
        return null_v;
    }

    const JsonValue& operator[](size_t index) const {
        if (type == JsonType::Array && index < array_val.size()) return array_val[index];
        static JsonValue null_v;
        return null_v;
    }

    size_t size() const {
        if (type == JsonType::Array) return array_val.size();
        if (type == JsonType::Object) return object_val.size();
        return 0;
    }

    bool contains(const std::string& key) const {
        if (type == JsonType::Object) return object_val.find(key) != object_val.end();
        return false;
    }

    std::string get_string() const { return type == JsonType::String ? str_val : ""; }
    double get_number() const { return type == JsonType::Number ? num_val : 0.0; }
    int get_int() const { return static_cast<int>(get_number()); }
    bool get_bool() const { return type == JsonType::Bool ? bool_val : false; }
    const std::vector<JsonValue>& get_array() const { return array_val; }
    const std::map<std::string, JsonValue>& get_object() const { return object_val; }

    std::string serialize(int indent = 0) const;
    static JsonValue parse(const std::string& s);

    JsonType type;
    bool bool_val = false;
    double num_val = 0.0;
    std::string str_val;
    std::vector<JsonValue> array_val;
    std::map<std::string, JsonValue> object_val;
};

#endif // JSON_H
