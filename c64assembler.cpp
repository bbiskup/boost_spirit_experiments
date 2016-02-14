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

const char COMMENT_CHAR{';'};

typedef const char* Mnemonic;

struct Mnemonics_ : qi::symbols<char, Mnemonic> {
  Mnemonics_() {
    // we need to add symbols dynamically
    for (const auto& elem : {"lda", "sta", "inx"}) {
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

void print_mnemo(const char* mnemo) {
  cout << "DETECTED mnemo: " << mnemo << endl;
}

void print_line() { cout << "DETECTED line" << endl; }
void print_lines() { cout << "DETECTED lines" << endl; }

void print_addr_spec(unsigned int addr_spec) {
  cout << "DETECTED addr_spec: " << addr_spec << endl;
}

// Fragment for parsing line
template <typename Iterator, typename Skipper = CommentSkipper<Iterator>>
struct AsmGrammar : public qi::grammar<Iterator, Skipper> {
  AsmGrammar() : AsmGrammar::base_type{line} {
    mnemo = Mnemonics[&print_mnemo];
    mnemo.name("mnemo");
    qi::debug(mnemo);

    addr_spec = qi::char_('*') >> qi::char_('=') >>
                qi::lexeme[qi::char_('$') >> qi::hex[&print_addr_spec]];
    addr_spec.name("addr_spec");
    qi::debug(addr_spec);

    identifier = qi::repeat(1, 32)[qi::upper];
    identifier.name("identifier");
    qi::debug(identifier);
    hex_num = qi::lexeme[qi::char_('$') >> qi::hex];
    addr_arg = hex_num | identifier;

    instr_arg_implicit = qi::eps;

    instr_arg_absolute_x =
        addr_arg >> qi::char_(',') >> qi::no_case[qi::char_('x')];
    instr_arg_absolute_x.name("instr_arg_absolute_x");
    qi::debug(instr_arg_absolute_x);

    instr_arg_absolute_y =
        addr_arg >> qi::char_(',') >> qi::no_case[qi::char_('y')];
    instr_arg_absolute_y.name("instr_arg_absolute_y");
    qi::debug(instr_arg_absolute_y);

    instr_arg_absolute = addr_arg;
    instr_arg_absolute.name("instr_arg_absolute");
    qi::debug(instr_arg_absolute);

    instr_arg_immediate = qi::char_('#') >> addr_arg;
    instr_arg_immediate.name("instr_arg_immediate");
    qi::debug(instr_arg_immediate);

    // instr_arg_implicit must be checked last, otherwise only the mnemonic
    // of an instruction with argument will be parsed
    instr_arg = instr_arg_absolute_x | instr_arg_absolute_y |
                instr_arg_absolute | instr_arg_immediate | instr_arg_implicit;
    instr_arg.name("instr_arg");
    qi::debug(instr_arg);

    instr = mnemo >> instr_arg;
    instr.name("instr");
    qi::debug(instr);

    var_value = qi::lexeme[qi::char_('$') >> qi::hex];
    var_assignment = identifier >> qi::char_('=') >> var_value;
    var_assignment.name("var_assignment");
    debug(var_assignment);

    line = instr | addr_spec | var_assignment;
    line.name("line");
    qi::debug(line);
  }
  qi::rule<Iterator, Skipper> mnemo;
  qi::rule<Iterator, Skipper> addr_spec;

  qi::rule<Iterator, Skipper> hex_num;
  qi::rule<Iterator, Skipper> addr_arg;

  // e.g. LDA $d000,X
  qi::rule<Iterator, Skipper> instr_arg_absolute_x;

  // e.g. LDA $d000,Y
  qi::rule<Iterator, Skipper> instr_arg_absolute_y;

  // e.g. LDA $d000
  qi::rule<Iterator, Skipper> instr_arg_absolute;

  // e.g. LDA #$d0
  // TODO enforce two digits or max 0xff
  qi::rule<Iterator, Skipper> instr_arg_immediate;

  // e.g. INX
  qi::rule<Iterator, Skipper> instr_arg_implicit;

  qi::rule<Iterator, Skipper> instr_arg;

  qi::rule<Iterator, Skipper> instr;

  qi::rule<Iterator, Skipper> identifier;
  qi::rule<Iterator, Skipper> var_value;
  qi::rule<Iterator, Skipper> var_assignment;

  qi::rule<Iterator, Skipper> line;
  qi::rule<Iterator, Skipper> lines;
};

int main() {
  // TODO reactivate reading from file
  // ifstream ifs{"asm_example_1.asm"};
  // string prog_str{istreambuf_iterator<char>(ifs),
  // istreambuf_iterator<char>()};
  // cout << "Prog: " << prog_str << endl;
  // cout << "-----------------" << endl;

  // string prog_fragment = "LDA #$a0\nSTA $e000\nLDA $ff\n";
  // string prog_fragment =
  //     "\n\n* = $c000; comment 1\nSTA $a000; comment 2\nSTA $e000\nLDA $ff\n";
  string prog_fragment =
      //"* = $c000\n"
      //"ABC = $a000\n"
      "inx\n"
      "sta $e000\n"
      "sta ABC\n"
      "lda #$ff\n"
      //"lda $cfff, X\n"
      //"lda "
      //"$ce00, Y"
      ;
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
    if (!match) {
      cout << "!match" << endl;
      cout << "Remaining (unparsed) text:" << endl;
      if (it != end) {
        std::cout << std::string{it, end} << std::endl;
      }
      break;
    }
  }
}