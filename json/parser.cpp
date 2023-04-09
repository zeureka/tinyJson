#include "parser.h"
#include <cstdlib>
#include <expected>
#include <iostream>
#include <new>
#include <stdexcept>

using namespace iaee::json;

Parser::Parser() : m_str(""), m_idx(-1) {}
void Parser::load(const std::string &str) {
  m_str = str;
  m_idx = 0;
}

void Parser::skip_white_space() {
  while (' ' == m_str[m_idx] || '\n' == m_str[m_idx] || '\r' == m_str[m_idx] ||
         '\t' == m_str[m_idx]) {
    m_idx++;
  }
}

char Parser::get_next_token() {
  skip_white_space();
  if (m_str.size() <= m_idx) {
    throw std::logic_error("unexpected end of input!");
  }
  return m_str[m_idx++];
}

Json Parser::parser() {
  char ch = get_next_token();

  if ('n' == ch) {
    m_idx--;
    return parser_null();
  } else if ('t' == ch || 'f' == ch) {
    m_idx--;
    return parser_bool();
  } else if ('-' == ch || (ch >= '0' && ch <= '9')) {
    m_idx--;
    return parser_number();
  } else if ('"' == ch) {
    return Json(parser_string());
  } else if ('[' == ch) {
    return parser_array();
  } else if ('{' == ch) {
    return parser_object();
  } else {
    throw std::logic_error("unexpected char!");
  }
}

Json Parser::parser_null() {
  if (m_str.compare(m_idx, 4, "null") == 0) {
    m_idx += 4;
    return Json();
  }
  throw std::logic_error("parser null error!");
  return Json();
}

Json Parser::parser_bool() {
  if (m_str.compare(m_idx, 4, "true") == 0) {
    m_idx += 4;
    return Json(true);
  } else if (m_str.compare(m_idx, 5, "false") == 0) {
    m_idx += 5;
    return Json(false);
  }
  throw std::logic_error("parser bool error!");
}

Json Parser::parser_number() {
  int pos = m_idx;
  if ('-' == m_str[m_idx]) {
    m_idx++;
  }

  if ('0' == m_str[m_idx]) {
    m_idx++;
    if ('0' <= m_str[m_idx] && '9' >= m_str[m_idx]) {
      throw std::logic_error("leading 0s not permitted in numbers!");
    }
  } else if ('1' <= m_str[m_idx] && '9' >= m_str[m_idx]) {
    m_idx++;
    while ('0' <= m_str[m_idx] && '9' >= m_str[m_idx]) {
      m_idx++;
    }
  } else {
    throw std::logic_error("invalid character in number!");
  }

  if ('.' != m_str[m_idx]) {
    return Json(std::atoi(m_str.c_str() + pos));
  }

  if ('.' == m_str[m_idx]) {
    m_idx++;
    if ('0' >= m_str[m_idx] || '9' <= m_str[m_idx] || m_idx >= m_str.size()) {
      throw std::logic_error("at least one digit required in fractional part!");
    }
    while ('0' <= m_str[m_idx] && '9' >= m_str[m_idx]) {
      m_idx++;
    }
  }

  return Json(std::atof(m_str.c_str() + pos));
}

std::string Parser::parser_string() {
  std::string out{""};

  while (true) {
    if (m_idx >= m_str.size()) {
      throw std::logic_error("unexpected end of input in string!");
    }

    char ch = m_str[m_idx++];
    if ('"' == ch) {
      break;
    } else if ('\\' != ch) {
      out += ch;
    } else {
      throw std::logic_error("not support escaped characters in string!");
    }
  }

  return out;
}

Json Parser::parser_array() {
  Json arr(Json::json_array);
  char ch = get_next_token();

  if (']' == ch) {
    return arr;
  }

  m_idx--;
  while (true) {
    arr.append(parser());
    ch = get_next_token();
    if (']' == ch) {
      break;
    } else if (',' != ch) {
      throw std::logic_error("expected ',' in array!");
    }
  }

  return arr;
}

Json Parser::parser_object() {
  Json obj(Json::json_object);
  char ch = get_next_token();
  if ('}' != ch) {
    m_idx--;
    while (true) {
      ch = get_next_token();
      if ('"' != ch) {
        throw std::logic_error("expected '\"' in object!");
      }

      std::string key = parser_string();
      ch = get_next_token();

      if (':' != ch) {
        throw std::logic_error("expected ':' in object!");
      }

      obj[key] = parser();
      ch = get_next_token();

      if ('}' == ch) {
        break;
      } else if (',' != ch) {
        throw std::logic_error("expected ',' in object");
      }
    }
  }
  return obj;
}
