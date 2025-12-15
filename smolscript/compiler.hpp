#ifndef SMOL_COMPILER_HPP
#define SMOL_COMPILER_HPP

#include <string>
#include <vector>

//Lexer
enum class lexem_type
{
  kw_when,
  kw_arrow,
  kw_bang,
  kw_semicolon,
  identifier,
  integer,
  end_of_file
};

struct lexem 
{
  lexem_type type;
  std::string name;
};

class Lexer
{
public: 
  explicit Lexer(const std::string& input)
        : src(input), pos(0) {}
  lexem next();

private:
  std::string src;
  unsigned int pos;
  char peek();
  void skipWhiteSpace();
  lexem identifier();
  lexem number();
};

//Parser
struct condition_t
{
  std::string name;
  bool negated = false;
  bool rising_edge = false;
};

struct command_t
{
  std::string name;
  std::vector<int> args;
};

struct rule_t
{
  condition_t condition;
  command_t command;
};

class Parser
{
public:
  explicit Parser(Lexer& lexer)
        : lexer(lexer) {
        advance();
  }
  rule_t parseRule();
private:
  Lexer& lexer;
  lexem current;
  void advance()
  {
    current = lexer.next();
  }

  bool match(lexem_type type);
  lexem expect(lexem_type type);
};

//API
void compile_file(std::string path);

#endif
