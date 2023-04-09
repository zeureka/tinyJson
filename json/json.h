#pragma once
#include <map>
#include <sstream>
#include <string>
#include <vector>
namespace iaee {

namespace json {

class Json {
public:       // 数据成员的类型定义
  enum Type { // json 值的类型
    json_null,
    json_bool,
    json_int,
    json_double,
    json_string,
    json_array,
    json_object
  };

  union Value { // json 的值
    bool m_bool;
    int m_int;
    double m_double;
    std::string *m_string;
    std::vector<Json> *m_array;
    std::map<std::string, Json> *m_object;
  };

public: // 构造函数
  Json();
  explicit Json(const bool &value);
  explicit Json(const int &value);
  explicit Json(const double &value);
  explicit Json(const char *value);
  explicit Json(const std::string &value);
  explicit Json(const Type &type);
  Json(const Json &other);
  ~Json();

  Json &operator=(Json other);
  Json &operator=(bool value);
  Json &operator=(int value);
  Json &operator=(double value);
  Json &operator=(std::string value);
  Json &operator=(char *value);

public: // 重载运算符
        // 可以定义一个变量直接等于json对象对应类型的值
  operator bool();
  operator int();
  operator double();
  operator std::string();

  Json &operator*() { return *this; }
  Json *operator->() { return this; }

  // 通过下标返回array/map中index/key对应的元素值
  Json &operator[](const int &index);
  Json &operator[](const char *key);
  Json &operator[](const std::string &key);

  bool operator==(const Json &other);
  bool operator==(const bool &value);
  bool operator==(const int &value);
  bool operator==(const double &value);
  bool operator==(const char *value);
  bool operator==(const std::string &value);

  bool operator!=(const bool &value) { return !((*this) == value); }
  bool operator!=(const int &value) { return !((*this) == value); }
  bool operator!=(const double &value) { return !((*this) == value); }
  bool operator!=(const char *value) { return !((*this) == value); }
  bool operator!=(const std::string &value) { return !((*this) == value); }
  bool operator!=(const Json &other) { return !((*this) == other); }

public: // 公有函数
  typedef std::map<std::string, Json>::iterator iterator;
  // 向array类型的this对象中添加Json object
  void append(const Json &other);

  // 将this对象转换成字符串并返回
  std::string toString() const;

  // 定义两个头尾迭代器（只能作用于object类型）
  iterator begin() { return m_value.m_object->begin(); }
  iterator end() { return m_value.m_object->end(); }

  // 判断json对象的类型
  bool isNull() const { return m_type == json_null; }
  bool isBool() const { return m_type == json_bool; }
  bool isInt() const { return m_type == json_int; }
  bool isDouble() const { return m_type == json_double; }
  bool isString() const { return m_type == json_string; }
  bool isArray() const { return m_type == json_array; }
  bool isObject() const { return m_type == json_object; }

  // 返回对象的值
  bool asBool() const;
  int asInt() const;
  double asDouble() const;
  std::string asString() const;

  // 判断array/map中是否有某个index/key
  bool has(const int &index);
  bool has(const char *key);
  bool has(const std::string &key);

  // 删除 index/key 对应的数据
  void remove(const int &index);
  void remove(const char *key);
  void remove(const std::string &key);

  // 获取array/map类型对象的元素个数
  int size() const;
  // 判断array/map类型对象是否为空
  bool empty() const;

  // 复制对象
  void copy(const Json &other);

  // 释放内存，delete
  void clear();

  // 解析json对象
  void parser(const std::string &str);

  // copy and swap
  void swap(Json &self, Json &other);

  // format print;
  void pformat();

private:         // 数据成员
  Type m_type;   // 类的类型
  Value m_value; // 类的值
};

}; // namespace json

}; // namespace iaee
