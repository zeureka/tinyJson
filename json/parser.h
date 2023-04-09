#pragma once
#include "json.h"
#include <string>

namespace iaee {
namespace json {
class Parser {
public:
  Parser();
  ~Parser() = default;
  void load(const std::string &str);
  Json parser();

private:
  // 跳过空格
  void skip_white_space();
  // 获取下一个索引的字符
  char get_next_token();
  // 解析不同类型的json对象
  Json parser_null();
  Json parser_bool();
  Json parser_number();
  std::string parser_string();
  Json parser_array();
  Json parser_object();

private:
  std::string m_str;
  int m_idx;
};
} // namespace json
} // namespace iaee
