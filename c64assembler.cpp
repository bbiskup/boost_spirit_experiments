#include <boost/spirit/include/qi.hpp>
#include <string>
#include <iostream>
#include <fstream>

using std::string;
using std::cout;
using std::endl;
using std::ifstream;
using std::istreambuf_iterator;

using namespace boost::spirit;

// template <typename Iterator, typename Skipper>
//    struct AsmGrammar : qi::grammar <

typedef const char* Mnemonic;
Mnemonic LDA = "LDA";
Mnemonic STA = "STA";

struct Mnemonics : qi::symbols<char, Mnemonic> {
  Mnemonics() {
    for (const auto& elem : {LDA, STA}) {
      add(elem, elem);
    }
  }
};

int main() {
  ifstream ifs{"asm_example_1.asm"};
  string prog_str{istreambuf_iterator<char>(ifs), istreambuf_iterator<char>()};

  cout << "Prog: " << prog_str << endl;
}