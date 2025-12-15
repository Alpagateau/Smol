#include "compiler.hpp"
#include <cctype>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

char Lexer::peek() { return pos + 1 < src.size() ? src[pos + 1] : '\0'; }

void Lexer::skipWhiteSpace() {
  while (pos < src.size() && isspace(src[pos]))
    pos++;
  if(src[pos] == '#')
  {
    pos++;
    while (pos < src.size() && !(src[pos] == '\n' || src[pos] == '#' ))
      pos++;
    pos++;
  }
  while (pos < src.size() && isspace(src[pos]))
    pos++;
}

lexem Lexer::identifier() {
  unsigned int start = pos;
  while (pos < src.size() && std::isalpha(src[pos]))
    pos++;
  std::string txt = src.substr(start, pos - start);

  if (txt == "when") {
    return {lexem_type::kw_when, txt};
  }
  return {lexem_type::identifier, txt};
}

lexem Lexer::number() {
  unsigned int start = pos;
  while (pos < src.size() && (std::isdigit(src[pos]) || src[pos] == '-' || src[pos] == '+'))
    pos++;
  return {lexem_type::integer, src.substr(start, pos - start)};
}

lexem Lexer::next() {
  skipWhiteSpace();
  if (pos >= src.size())
    return {lexem_type::end_of_file, ""};

  char c = src[pos];
  if (c == '!') {
    pos++;
    return {lexem_type::kw_bang, "!"};
  }
  if (c == ';') {
    pos++;
    return {lexem_type::kw_semicolon, ";"};
  }
  if (c == '=' && peek() == '>') {
    pos += 2;
    return {lexem_type::kw_arrow, "=>"};
  }

  if (isdigit(c) || c == '-' || c == '+') {
    return number();
  }

  if (isalpha(c)) {
    return identifier();
  }
  std::cout << "[ERROR] : Found character : " << c << " = " << (int)c << std::endl;
  throw std::runtime_error("Unexpected character");
}

bool Parser::match(lexem_type type) {
  if (current.type == type) {
    advance();
    return true;
  }
  return false;
}

lexem Parser::expect(lexem_type type) {
  if (current.type != type) {
    std::cout << "[ERROR] Unexpected : " << current.name << std::endl;
    throw std::runtime_error("Unexpected token");
  }
  lexem t = current;
  advance();
  return t;
}

rule_t Parser::parseRule() {
  rule_t rule = {};
  expect(lexem_type::kw_when);

  if (match(lexem_type::kw_bang)) {
    rule.condition.negated = true;
  }

  rule.condition.name = expect(lexem_type::identifier).name;

  if (match(lexem_type::kw_bang)) {
    rule.condition.rising_edge = true;
  }
  expect(lexem_type::kw_arrow);

  rule.command.name = expect(lexem_type::identifier).name;
  while (current.type == lexem_type::integer) {
    rule.command.args.push_back(std::stoi(current.name));
    advance();
  }
  return rule;
}

void compile_file(std::string path) {
  std::ifstream source_file(path);
  if (source_file.fail()) {
    std::cout << "[ERROR] Couldn't open file : " << path << std::endl;
    return;
  }

  std::cout << "reading file : " << path << std::endl;
  std::stringstream buffer;
  buffer << source_file.rdbuf();

  Lexer lexer(buffer.str());
  Parser parser(lexer);
  std::vector<rule_t> rules = {};
  try {
    while (true) {
      rule_t rule = parser.parseRule();
      std::cout << "Parsed rule: when "
                << (rule.condition.negated ? "not " : "") << rule.condition.name
                << (rule.condition.rising_edge ? " rising" : "") << " then "
                << rule.command.name << "\n";
      rules.push_back(rule);
    }
  } catch (...) {
    std::cout << "Done.\n";
  }
}
