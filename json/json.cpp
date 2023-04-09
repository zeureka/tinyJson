#include "json.h"
#include "parser.h"
#include <iostream>
#include <map>
#include <ostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

using namespace iaee::json;
/************************* 构造函数 *************************/
Json::Json() : m_type(json_null) {}
Json::Json(const bool &value) {
  m_type = json_bool;
  m_value.m_bool = value;
}

Json::Json(const int &value) {
  m_type = json_int;
  m_value.m_int = value;
}

Json::Json(const double &value) {
  m_type = json_double;
  m_value.m_double = value;
}

Json::Json(const char *value) {
  m_type = json_string;
  m_value.m_string = new std::string(value);
}

Json::Json(const std::string &value) {
  m_type = json_string;
  m_value.m_string = new std::string(value);
}

Json::Json(const Type &type) : m_type(type) {
  switch (m_type) {
  case json_null:
    break;
  case json_bool:
    m_value.m_bool = false;
    break;
  case json_int:
    m_value.m_int = 0;
    break;
  case json_double:
    m_value.m_double = 0.0;
    break;
  case json_string:
    m_value.m_string = new std::string("");
    break;
  case json_array:
    m_value.m_array = new std::vector<Json>;
    break;
  case json_object:
    m_value.m_object = new std::map<std::string, Json>;
  default:
    break;
  }
}

// 拷贝构造函数
Json::Json(const Json &other) { copy(other); }

// 析构函数
Json::~Json() { clear(); }

/************************* 操作符重载 *************************/
// 赋值构造函数
Json &Json::operator=(Json other) {
  if (json_object != m_type && json_null != m_type) {
    throw std::logic_error("Assignment operation type mismatch!");
  }

  swap(*this, other);
  return *this;
}

Json &Json::operator=(bool value) {
  if (json_null == m_type) {
    m_type = json_bool;
  }

  if (json_bool != m_type) {
    throw std::logic_error("Assignment operation type mismatch!");
  }

  std::swap(m_value.m_bool, value);
  return *this;
}

Json &Json::operator=(int value) {
  if (json_null == m_type) {
    m_type = json_int;
  }

  if (json_int != m_type) {
    throw std::logic_error("Assignment operation type mismatch!");
  }

  std::swap(m_value.m_int, value);
  return *this;
}

Json &Json::operator=(double value) {
  if (json_null == m_type) {
    m_type = json_double;
  }

  if (json_double != m_type) {
    throw std::logic_error("Assignment operation type mismatch!");
  }

  std::swap(m_value.m_double, value);
  return *this;
}

Json &Json::operator=(std::string value) {
  if (json_null == m_type) {
    m_type = json_string;
    m_value.m_string = new std::string();
  }

  if (json_string != m_type) {
    throw std::logic_error("Assignment operation type mismatch!");
  }

  std::swap(*m_value.m_string, value);
  return *this;
}

Json &Json::operator=(char *value) {
  std::string name(value);
  *this = name;
  return *this;
}

Json::operator bool() {
  if (json_bool != m_type) {
    throw std::logic_error("type error, not bool value!");
  }

  return m_value.m_bool;
}

Json::operator int() {
  if (json_int != m_type) {
    throw std::logic_error("type error, not int value!");
  }

  return m_value.m_int;
}

Json::operator double() {
  if (json_double != m_type) {
    throw std::logic_error("type error, not double value!");
  }

  return m_value.m_double;
}

Json::operator std::string() {
  if (json_string != m_type) {
    throw std::logic_error("type error, not string value!");
  }

  return *m_value.m_string;
}

Json &Json::operator[](const int &index) {
  if (json_null == m_type) {
    m_type = json_array;
    m_value.m_array = new std::vector<Json>;
  }
  if (json_array != m_type) {
    throw std::logic_error(
        "Wrong object type calling overloaded function for []");
  }

  if (index < 0) {
    throw std::logic_error("array[] index less than zero!");
  }

  int size = (m_value.m_array)->size();

  if (index >= size) {
    for (int i = size; i <= index; ++i) {
      (m_value.m_array)->push_back(Json());
    }
  }

  return m_value.m_array->at(index);
}

Json &Json::operator[](const char *key) {
  std::string name(key);
  return (*(this))[name];
}

Json &Json::operator[](const std::string &key) {
  if (json_null == m_type) {
    m_type = json_object;
    m_value.m_object = new std::map<std::string, Json>;
  }

  if (json_object != m_type) {
    throw std::logic_error(
        "function Json::operator [const string &] requires object value");
  }

  return (*(m_value.m_object))[key];
}

bool Json::operator==(const Json &other) {
  if (m_type != other.m_type) {
    return false;
  }

  switch (m_type) {
  case json_null:
    return true;
  case json_bool:
    return m_value.m_bool & other.m_value.m_bool;
  case json_int:
    return m_value.m_int == other.m_value.m_int;
  case json_double:
    return m_value.m_double == other.m_value.m_double;
  case json_string:
    return *(m_value.m_string) == *(other.m_value.m_string);
  case json_array: {
    if (m_value.m_array->size() != other.m_value.m_array->size()) {
      return false;
    } else {
      auto it1 = m_value.m_array->begin();
      auto it2 = other.m_value.m_array->begin();
      for (; it1 != m_value.m_array->end(); ++it1, ++it2) {
        if (*it1 != *it2) {
          return false;
        }
      }
    }
  }
    return true;
  case json_object: {
    if (m_value.m_object->size() != other.m_value.m_object->size()) {
      return false;
    } else {
      auto it1 = m_value.m_object->begin();
      auto it2 = other.m_value.m_object->begin();
      for (; it1 != m_value.m_object->end(); ++it1, ++it2) {
        if (it1->first != it2->first || it1->second != it2->second) {
          return false;
        }
      }
    }
  }
    return true;
  default:
    break;
  }

  return false;
}

bool Json::operator==(const bool &value) {
  Json other(value);
  return *this == other;
}

bool Json::operator==(const int &value) {
  Json other(value);
  return *this == other;
}

bool Json::operator==(const double &value) {
  Json other(value);
  return *this == other;
}

bool Json::operator==(const char *value) {
  Json other(value);
  return *this == other;
}

bool Json::operator==(const std::string &value) {
  Json other(value);
  return *this == other;
}

/************************* public functions *************************/
void Json::append(const Json &other) {
  if (json_null == m_type) {
    m_type = json_array;
    m_value.m_array = new std::vector<Json>;
  }

  if (json_array != m_type) {
    throw std::logic_error("function Json::append requires array value");
  }

  (m_value.m_array)->push_back(other);
}

std::string Json::toString() const {
  std::stringstream os;

  switch (m_type) {
  case json_null:
    os << "null";
    break;
  case json_bool:
    if (m_value.m_bool) {
      os << "true";
    } else {
      os << "false";
    }
    break;
  case json_int:
    os << m_value.m_int;
    break;
  case json_double:
    os << m_value.m_double;
    break;
  case json_string:
    os << '\"' << *m_value.m_string << '\"';
    break;
  case json_array: {
    os << '[';
    for (auto it = (m_value.m_array)->begin(); it != (m_value.m_array)->end();
         ++it) {
      if (it != (m_value.m_array->begin())) {
        os << ',';
      }

      os << it->toString();
    }

    os << ']';
  } break;
  case json_object: {
    os << '{';

    for (auto it = (m_value.m_object)->begin(); it != (m_value.m_object)->end();
         ++it) {
      if (it != (m_value.m_object)->begin()) {
        os << ',';
      }
      os << '\"' << it->first << '\"' << ':' << it->second.toString();
    }

    os << '}';
    return os.str();
  } break;
  default:
    break;
  }

  return os.str();
}

bool Json::asBool() const {
  if (json_bool != m_type) {
    throw std::logic_error("type error, not bool value!");
  }

  return m_value.m_bool;
}

int Json::asInt() const {
  if (json_int != m_type) {
    throw std::logic_error("type error, not int value!");
  }

  return m_value.m_int;
}

double Json::asDouble() const {
  if (json_double != m_type) {
    throw std::logic_error("type error, not double value!");
  }

  return m_value.m_double;
}

std::string Json::asString() const {
  if (json_string != m_type) {
    throw std::logic_error("type error, not double value!");
  }

  return *(m_value.m_string);
}

bool Json::has(const int &index) {
  if (json_array != m_type) {
    return false;
  }

  if (index >= 0 && index < (m_value.m_array->size())) {
    return true;
  }

  return false;
}

bool Json::has(const char *key) {
  std::string name(key);
  return has(name);
}

bool Json::has(const std::string &key) {
  if (json_object != m_type) {
    return false;
  }

  return (m_value.m_object)->find(key) != (m_value.m_object)->end();
}

void Json::remove(const int &index) {
  if (has(index)) {
    (m_value.m_array)->at(index).clear();
    m_value.m_array->erase(m_value.m_array->begin() + index);
  }
}

void Json::remove(const char *key) {
  std::string name(key);
  remove(name);
}

void Json::remove(const std::string &key) {
  if (has(key)) {
    (*(m_value.m_object))[key].clear();
    (m_value.m_object)->erase(key);
  }
}

int Json::size() const {
  switch (m_type) {
  case json_array:
    return m_value.m_array->size();
  case json_object:
    return m_value.m_object->size();
  default:
    break;
  }

  throw std::logic_error("function Json::size value type error");
}

bool Json::empty() const {
  switch (m_type) {
  case json_array:
    return m_value.m_array->empty();
  case json_object:
    return m_value.m_object->empty();
  default:
    break;
  }

  throw std::logic_error("function Json::empty value type error");
}

void Json::copy(const Json &other) {
  m_type = other.m_type;
  switch (m_type) {
  case json_null:
    break;
  case json_bool:
    m_value.m_bool = other.m_value.m_bool;
    break;
  case json_int:
    m_value.m_int = other.m_value.m_int;
    break;
  case json_double:
    m_value.m_double = other.m_value.m_double;
    break;
  case json_string:
    m_value.m_string = new std::string(*(other.m_value.m_string));
    break;
  case json_array:
    m_value.m_array = new std::vector<Json>(*(other.m_value.m_array));
    break;
  case json_object:
    m_value.m_object =
        new std::map<std::string, Json>(*(other.m_value.m_object));
  default:
    break;
  }
}

void Json::clear() {
  switch (m_type) {
  case json_bool:
    m_value.m_bool = false;
    break;
  case json_int:
    m_value.m_int = 0;
    break;
  case json_double:
    m_value.m_double = 0.0;
    break;
  case json_string:
    delete m_value.m_string;
    m_value.m_array = nullptr;
    break;
  case json_array: {
    for (auto it = (m_value.m_array)->begin(); it != (m_value.m_array)->end();
         ++it) {
      it->clear();
    }
    delete m_value.m_array;
    m_value.m_array = nullptr;
  } break;
  case json_object: {
    for (auto it = (m_value.m_object)->begin(); it != (m_value.m_object)->end();
         ++it) {
      (it->second).clear();
    }
    delete m_value.m_object;
    m_value.m_object = nullptr;
  } break;
  default:
    break;
  }

  m_type = json_null;
}

void Json::parser(const std::string &str) {
  Parser p;
  p.load(str);
  *this = p.parser();
}

void Json::swap(Json &self, Json &other) {
  using std::swap;

  swap(self.m_value, other.m_value);
  swap(self.m_type, other.m_type);
}

void Json::pformat() {
  std::string str = this->toString();

  int n = str.size();
  int leftBracketNum = 0;
  bool isArr = false;
  char preCh = ' ';
  for (int i = 0; i < n; ++i) {
    char ch = str[i];

    switch (ch) {
    case '[':
      isArr = true;
      std::cout << ch;
      break;
    case ']':
      isArr = false;
      std::cout << ch;
      break;
    case '{':
      if ('[' == preCh) {
        isArr = false;
      }

      std::cout << ch;
      std::cout << std::endl;
      leftBracketNum++;

      for (int i = 0; i < leftBracketNum; ++i) {
        std::cout << "  ";
      }

      break;
    case '}':
      std::cout << std::endl;
      leftBracketNum--;

      for (int i = 0; i < leftBracketNum; ++i) {
        std::cout << "  ";
      }

      std::cout << ch;
      break;
    case ',':
      std::cout << ch;

      if (!isArr) {
        std::cout << std::endl;
      }

      for (int i = 0; i < leftBracketNum; ++i) {
        std::cout << "  ";
      }

      break;
    case ':':
      std::cout << ch;
      std::cout << " ";
      break;
    default:
      std::cout << ch;
      break;
    }
    preCh = ch;
  }

  std::cout << std::endl;
}
