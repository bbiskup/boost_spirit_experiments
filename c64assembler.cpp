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

// Custom skipper for comments
// see http://stackoverflow.com/a/8534840
// template<typename Iterator>
// struct CommentSkiper : qi

int main() {
  ifstream ifs{"asm_example_1.asm"};
  string prog_str{istreambuf_iterator<char>(ifs), istreambuf_iterator<char>()};

  cout << "Prog: " << prog_str << endl;
  cout << "-----------------" << endl;

  // auto it = prog_str.begin();

  qi::rule<std::string::iterator, string(), ascii::space_type> comment =
      qi::char_(COMMENT_CHAR) >> +qi::char_ >> *qi::eol;
  ;
  string tmp = "; hier";
  auto it = tmp.begin();
  auto end = tmp.end();
  string comment_str;

  qi::debug(comment);

  bool match = qi::phrase_parse(it, end, comment, ascii::space, comment_str);
  cout << "match? " << boolalpha << match << endl;

  // output remaining text
  if (it != end) {
    std::cout << std::string{it, end} << std::endl;
  }
}