#define BOOST_SPIRIT_USE_PHOENIX_V3
#include <boost/spirit/include/qi.hpp>
#include <boost/phoenix/phoenix.hpp>
#include <string>
#include <iostream>

using namespace boost::spirit;
using boost::phoenix::ref;

void prog_lexeme() {
  std::string s;
  std::getline(std::cin, s);
  auto it = s.begin();
  bool match = qi::phrase_parse(
      it, s.end(), qi::lexeme[ascii::digit >> ascii::digit], ascii::space);
  std::cout << std::boolalpha << match << std::endl;
  if (it != s.end()) {
    std::cout << std::string{it, s.end()} << std::endl;
  }
}

void prog_plus() {
  std::string s;
  std::getline(std::cin, s);
  auto it = s.begin();
  bool match = qi::phrase_parse(it, s.end(), +ascii::digit, ascii::space);
  std::cout << std::boolalpha << match << std::endl;
  if (it != s.end()) {
    std::cout << std::string{it, s.end()} << std::endl;
  }
}

void prog_int() {
  std::string s;
  std::getline(std::cin, s);
  auto it = s.begin();
  bool match = qi::phrase_parse(it, s.end(), +qi::int_, ascii::space);
  std::cout << std::boolalpha << match << std::endl;
  if (it != s.end()) {
    std::cout << std::string{it, s.end()} << std::endl;
  }
}

void prog_float() {
  std::string s;
  std::getline(std::cin, s);
  auto it = s.begin();
  bool match = qi::phrase_parse(it, s.end(), +qi::float_, ascii::space);
  std::cout << std::boolalpha << match << std::endl;
  if (it != s.end()) {
    std::cout << std::string{it, s.end()} << std::endl;
  }
}

void prog_experiment() {
  std::string s;
  std::getline(std::cin, s);
  auto it = s.begin();
  auto opcode = qi::lexeme[qi::char_ >> qi::char_ >> qi::char_];
  bool match = qi::phrase_parse(it, s.end(), +opcode, ascii::space);
  std::cout << std::boolalpha << match << std::endl;
  if (it != s.end()) {
    std::cout << std::string{it, s.end()} << std::endl;
  }
}

void prog_lambda_action() {
  std::string s;
  std::getline(std::cin, s);
  auto it = s.begin();
  bool match = qi::phrase_parse(
      it, s.end(),
      +qi::int_[([](int m) { std::cout << "m: " << m << std::endl; })],
      ascii::space);
  std::cout << std::boolalpha << match << std::endl;
  if (it != s.end()) {
    std::cout << std::string{it, s.end()} << std::endl;
  }
}

void prog_phoenix_action() {
  std::string s;
  std::getline(std::cin, s);
  auto it = s.begin();
  int m;
  bool match =
      qi::phrase_parse(it, s.end(), +qi::int_[ref(m) = qi::_1], ascii::space);
  std::cout << "m: " << m << std::endl;
  std::cout << std::boolalpha << match << std::endl;
  if (it != s.end()) {
    std::cout << std::string{it, s.end()} << std::endl;
  }
}

int main() { prog_phoenix_action(); }
