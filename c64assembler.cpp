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

struct Mnemonics_ : qi::symbols<char, Mnemonic> {
  Mnemonics_() {
    // we need to add symbols dynamically
    for (const auto& elem : {LDA, STA}) {
      add(elem, elem);
    }
  }
} Mnemonics;

// Custom skipper for comments in assembly language
// see http://stackoverflow.com/a/8534840 about customer skippers
template <typename Iterator>
struct CommentSkipper : qi::grammar<Iterator> {
  CommentSkipper() : CommentSkipper::base_type{skip} {
    // TODO reactivate skipping comments
    skip = ascii::space | (qi::char_(COMMENT_CHAR) >> +(qi::char_ - '\n'));
    skip.name("skip");
    qi::debug(skip);
  }
  qi::rule<Iterator> skip;
};

// // Grammar for parsing C64 assembly language
// template <typename Iterator, typename Skipper = CommentSkipper<Iterator>>
// struct AsmGrammar : public qi::grammar<Iterator, string(), Skipper> {
//   AsmGrammar() : AsmGrammar::base_type{line} {
//     line = +qi::char_ - qi::eol;
//     line.name("line");
//     qi::debug(line);
//   }
//   qi::rule<Iterator, string(), Skipper> line;
// };

void print_mnemo(const char* mnemo) {
  cout << "DETECTED mnemo: " << mnemo << endl;
}

void print_line() { cout << "DETECTED line" << endl; }
void print_lines() { cout << "DETECTED lines" << endl; }

void print_addr_spec(unsigned int addr_spec) {
  cout << "DETECTED addr_spec: " << addr_spec << endl;
}

// // Fragment for testing symbol table
// template <typename Iterator, typename Skipper = CommentSkipper<Iterator>>
// struct MnemonicGrammarFragment : public qi::grammar<Iterator, Skipper> {
//   MnemonicGrammarFragment() : MnemonicGrammarFragment::base_type{mnemo} {
//     mnemo = Mnemonics[&print_mnemo];
//     mnemo.name("mnemo");
//     qi::debug(mnemo);
//   }
//   qi::rule<Iterator, Skipper> mnemo;
// };

// Fragment for parsing line
template <typename Iterator, typename Skipper = CommentSkipper<Iterator>>
struct AsmGrammar : public qi::grammar<Iterator, Skipper> {
  AsmGrammar() : AsmGrammar::base_type{lines} {
    mnemo = Mnemonics[&print_mnemo];
    mnemo.name("mnemo");
    qi::debug(mnemo);

    addr_spec = qi::char_('*') >> qi::char_('=') >>
                qi::lexeme[qi::char_('$') >> qi::hex[&print_addr_spec]];
    addr_spec.name("addr_spec");
    qi::debug(addr_spec);

    instr_arg_absolute = qi::char_('$') >> qi::hex;
    instr_arg_absolute.name("instr_arg_absolute");
    qi::debug(instr_arg_absolute);

    instr_arg_immediate = qi::char_('#') >> qi::char_('$') >> qi::hex;
    instr_arg_immediate.name("instr_arg_immediate");
    qi::debug(instr_arg_immediate);

    instr_arg = instr_arg_absolute | instr_arg_immediate;

    instr = mnemo >> instr_arg;
    instr.name("instr");
    qi::debug(instr);

    line = instr | addr_spec;
    line.name("line");
    qi::debug(line);

    lines = ((line[&print_line] % qi::eol) >> *qi::char_('\n'))[&print_lines];
    lines.name("lines");
    qi::debug(lines);
  }
  qi::rule<Iterator, Skipper> mnemo;
  qi::rule<Iterator, Skipper> addr_spec;

  // absolute addressing mode, e.g. LDA $d000
  qi::rule<Iterator, Skipper> instr_arg_absolute;

  // immediate addressing mode, e.g. LDA #$d0
  // TODO enforce two digits or max 0xff
  qi::rule<Iterator, Skipper> instr_arg_immediate;
  qi::rule<Iterator, Skipper> instr_arg;

  qi::rule<Iterator, Skipper> instr;
  qi::rule<Iterator, Skipper> line;
  qi::rule<Iterator, Skipper> lines;
};

int main() {
  ifstream ifs{"asm_example_1.asm"};
  string prog_str{istreambuf_iterator<char>(ifs), istreambuf_iterator<char>()};

  cout << "Prog: " << prog_str << endl;
  cout << "-----------------" << endl;

  // string prog_fragment = "LDA #$a0\nSTA $e000\nLDA $ff\n";
  string prog_fragment = "\n\n* = $c000\nSTA $a000\nSTA $e000\nLDA $ff\n";
  auto it = prog_fragment.begin();
  auto end = prog_fragment.end();
  string result_str;

  typedef string::iterator iterator_t;
  typedef AsmGrammar<iterator_t> grammar;
  typedef CommentSkipper<iterator_t> skipper;

  grammar g;
  skipper sk;

  while (it != end) {
    bool match = qi::phrase_parse(it, end, g, sk, qi::skip_flag::postskip);
    cout << "match? " << boolalpha << match << endl;
    cout << "Remaining (unparsed) text:" << endl;
    if (it != end) {
      std::cout << std::string{it, end} << std::endl;
    }
  }
}