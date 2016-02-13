#include <boost/spirit/include/qi.hpp>
#include <string>
#include <iostream>

using namespace boost::spirit;

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

int main() { prog_float(); }
