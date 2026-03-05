#include "Json.h"
#include <algorithm>
#include <cctype>

static std::string escape(const std::string& s) {
    std::string res = "\"";
    for (char c : s) {
        if (c == '"') res += "\\\"";
        else if (c == '\\') res += "\\\\";
        else res += c;
    }
    res += "\"";
    return res;
}

std::string JsonValue::serialize(int indent) const {
    std::string space(indent, ' ');
    switch (type) {
        case JsonType::Null: return "null";
        case JsonType::Bool: return bool_val ? "true" : "false";
        case JsonType::Number: {
            std::ostringstream ss;
            ss << num_val;
            return ss.str();
        }
        case JsonType::String: return escape(str_val);
        case JsonType::Array: {
            std::string res = "[";
            for (size_t i = 0; i < array_val.size(); ++i) {
                res += array_val[i].serialize();
                if (i < array_val.size() - 1) res += ", ";
            }
            res += "]";
            return res;
        }
        case JsonType::Object: {
            std::string res = "{";
            size_t i = 0;
            for (auto const& pair : object_val) {
                res += escape(pair.first) + ": " + pair.second.serialize();
                if (++i < object_val.size()) res += ", ";
            }
            res += "}";
            return res;
        }
    }
    return "null";
}

class JsonParser {
    std::string s;
    size_t pos;

    void skip_ws() {
        while (pos < s.size() && std::isspace(s[pos])) pos++;
    }

public:
    JsonParser(const std::string& str) : s(str), pos(0) {}

    JsonValue parse() {
        skip_ws();
        if (pos >= s.size()) return JsonValue();
        char c = s[pos];
        if (c == '{') return parse_object();
        if (c == '[') return parse_array();
        if (c == '"') return parse_string();
        if (c == 't' || c == 'f') return parse_bool();
        if (c == 'n') return parse_null();
        if (std::isdigit(c) || c == '-') return parse_number();
        return JsonValue();
    }

    JsonValue parse_object() {
        JsonValue v = JsonValue::object();
        pos++; // '{'
        while (true) {
            skip_ws();
            if (pos < s.size() && s[pos] == '}') { pos++; break; }
            JsonValue key_v = parse_string();
            std::string key = key_v.get_string();
            skip_ws();
            if (pos < s.size() && s[pos] == ':') pos++;
            v[key] = parse();
            skip_ws();
            if (pos < s.size() && s[pos] == ',') pos++;
            else if (pos < s.size() && s[pos] == '}') { pos++; break; }
            else break;
        }
        return v;
    }

    JsonValue parse_array() {
        JsonValue v = JsonValue::array();
        pos++; // '['
        while (true) {
            skip_ws();
            if (pos < s.size() && s[pos] == ']') { pos++; break; }
            v.push_back(parse());
            skip_ws();
            if (pos < s.size() && s[pos] == ',') pos++;
            else if (pos < s.size() && s[pos] == ']') { pos++; break; }
            else break;
        }
        return v;
    }

    JsonValue parse_string() {
        pos++; // '"'
        std::string res;
        while (pos < s.size() && s[pos] != '"') {
            if (s[pos] == '\\') pos++; // Skip escape
            res += s[pos++];
        }
        if (pos < s.size()) pos++; // '"'
        return JsonValue(res);
    }

    JsonValue parse_bool() {
        if (s.substr(pos, 4) == "true") { pos += 4; return JsonValue(true); }
        if (s.substr(pos, 5) == "false") { pos += 5; return JsonValue(false); }
        return JsonValue();
    }

    JsonValue parse_null() {
        if (s.substr(pos, 4) == "null") { pos += 4; return JsonValue(); }
        return JsonValue();
    }

    JsonValue parse_number() {
        size_t start = pos;
        if (s[pos] == '-') pos++;
        while (pos < s.size() && (std::isdigit(s[pos]) || s[pos] == '.')) pos++;
        return JsonValue(std::stod(s.substr(start, pos - start)));
    }
};

JsonValue JsonValue::parse(const std::string& s) {
    JsonParser p(s);
    return p.parse();
}
