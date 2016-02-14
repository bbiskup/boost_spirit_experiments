#include <boost/spirit/include/qi.hpp>
#include <string>
#include <iostream>
#include <fstream>

using std::string;
using std::boolalpha;
using std::cout;
using std::endl;
using std::ifstream;
using std::istreambuf_iterator;

using namespace boost::spirit;

// template <typename Iterator, typename Skipper>
//    struct AsmGrammar : qi::grammar <

const char COMMENT_CHAR{';'};

typedef const char* Mnemonic;
Mnemonic LDA = "LDA";
Mnemonic STA = "STA";

struct Mnemonics : qi::symbols<char, Mnemonic> {
  Mnemonics() {
    // we need to add symbols dynamically
    for (const auto& elem : {LDA, STA}) {
      add(elem);  // TODO call with same elem twice, or is once equivalent?
    }
  }
};

// Custom skipper for comments in assembly language
// see http://stackoverflow.com/a/8534840 about customer skippers
template <typename Iterator>
struct CommentSkipper : qi::grammar<Iterator> {
  CommentSkipper() : CommentSkipper::base_type{skip} {
    skip = ascii::space | (qi::char_(COMMENT_CHAR) >> +qi::char_ >> *qi::eol);
    // qi::debug(skip);
  }
  qi::rule<Iterator> skip;
};

// Grammar for parsing C64 assembly language
template <typename Iterator, typename Skipper = CommentSkipper<Iterator>>
struct AsmGrammar : public qi::grammar<Iterator, string(), Skipper> {
  AsmGrammar() : AsmGrammar::base_type{line} { line = +qi::char_ - qi::eol; }
  qi::rule<Iterator, string(), Skipper> line;
};

int main() {
  ifstream ifs{"asm_example_1.asm"};
  string prog_str{istreambuf_iterator<char>(ifs), istreambuf_iterator<char>()};

  cout << "Prog: " << prog_str << endl;
  cout << "-----------------" << endl;

  // auto it = prog_str.begin();

  // qi::rule<std::string::iterator, string(), ascii::space_type> comment =
  //    qi::char_(COMMENT_CHAR) >> +qi::char_ >> *qi::eol;
  //;
  string prog_fragment = "abc; hier";
  auto it = prog_fragment.begin();
  auto end = prog_fragment.end();
  string result_str;

  typedef string::iterator iterator_t;
  typedef AsmGrammar<iterator_t> grammar;
  typedef CommentSkipper<iterator_t> skipper;

  grammar g;
  skipper sk;

  bool match = qi::phrase_parse(it, end, g, sk, result_str);
  cout << "match? " << boolalpha << match << endl;
  cout << "result_str " << result_str << endl;

  // output remaining text
  if (it != end) {
    std::cout << std::string{it, end} << std::endl;
  }
}