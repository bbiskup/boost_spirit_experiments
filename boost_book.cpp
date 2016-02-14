#define BOOST_SPIRIT_USE_PHOENIX_V3
#include <boost/spirit/include/qi.hpp>
#include <boost/phoenix/phoenix.hpp>
#include <boost/variant.hpp>
#include <boost/fusion/include/adapt_struct.hpp>
#include <string>
#include <iostream>
#include <iterator>
#include <vector>
#include <algorithm>

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

void prog_attribute() {
  std::string s;
  std::getline(std::cin, s);
  auto it = s.begin();
  std::vector<int> m;
  bool match = qi::phrase_parse(it, s.end(), qi::int_ % ",", ascii::space, m);

  // for (auto x : m) {
  //   std::cout << "x: " << x << std::endl;
  // }

  std::ostream_iterator<int> out{std::cout, ";"};
  std::copy(m.begin(), m.end(), out);
  std::cout << std::endl;

  std::cout << std::boolalpha << match << std::endl;
  if (it != s.end()) {
    std::cout << std::string{it, s.end()} << std::endl;
  }
}

struct print : public boost::static_visitor<> {
  template <typename T>
  void operator()(T t) const {
    std::cout << "t: " << std::boolalpha << t << std::endl;
  }
};

void prog_visitor() {
  std::string s;
  std::getline(std::cin, s);
  auto it = s.begin();
  qi::rule<std::string::iterator, boost::variant<int, bool>(),
           ascii::space_type> value = qi::int_ | qi::bool_;
  qi::rule<std::string::iterator, std::vector<boost::variant<int, bool>>(),
           ascii::space_type> values = value % ",";
  std::vector<boost::variant<int, bool>> v;
  if (qi::phrase_parse(it, s.end(), values, ascii::space, v)) {
    for (const auto& elem : v) {
      boost::apply_visitor(print{}, elem);
    }
  }
}

// ------------------------------------------------- grammar

template <typename Iterator, typename Skipper>
struct my_grammar
    : qi::grammar<Iterator, std::vector<boost::variant<int, bool>>(), Skipper> {
  my_grammar() : my_grammar::base_type{values} {
    value = qi::int_ | qi::bool_;
    values = value % ",";
  }
  qi::rule<std::string::iterator, boost::variant<int, bool>(),
           ascii::space_type> value = qi::int_ | qi::bool_;
  qi::rule<std::string::iterator, std::vector<boost::variant<int, bool>>(),
           ascii::space_type> values = value % ",";
};

void prog_grammar() {
  std::string s;
  std::getline(std::cin, s);
  auto it = s.begin();
  my_grammar<std::string::iterator, ascii::space_type> g;
  std::vector<boost::variant<int, bool>> v;
  if (qi::phrase_parse(it, s.end(), g, ascii::space, v)) {
    for (const auto& elem : v) {
      boost::apply_visitor(print{}, elem);
    }
  }
}

// ------------------------------------------------- parse to struct

typedef boost::variant<int, bool> int_or_bool;

struct int_or_bool_values {
  int_or_bool first;
  std::vector<int_or_bool> others;
};

BOOST_FUSION_ADAPT_STRUCT(int_or_bool_values,
                          (int_or_bool, first)(std::vector<int_or_bool>,
                                               others))

template <typename Iterator, typename Skipper>
struct my_struct_grammar
    : qi::grammar<Iterator, int_or_bool_values(), Skipper> {
  my_struct_grammar() : my_struct_grammar::base_type{values} {
    value = qi::int_ | qi::bool_;
    values = value >> ',' >> value % ',';
  }

  qi::rule<Iterator, int_or_bool(), Skipper> value;
  qi::rule<Iterator, int_or_bool_values(), Skipper> values;
};

struct struct_print : boost::static_visitor<> {
  template <typename T>
  void operator()(T t) const {
    std::cout << "struct_print: " << std::boolalpha << t << std::endl;
  }
};

void prog_parse_to_struct() {
  std::string s;
  std::getline(std::cin, s);
  auto it = s.begin();
  my_struct_grammar<std::string::iterator, ascii::space_type> g;
  int_or_bool_values v;
  if (qi::phrase_parse(it, s.end(), g, ascii::space, v)) {
    struct_print p;
    boost::apply_visitor(p, v.first);
    for (const auto& elem : v.others) {
      boost::apply_visitor(p, elem);
    }
  }
}

int main() { prog_parse_to_struct(); }
