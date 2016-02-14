#include <boost/spirit/include/qi.hpp>
#include <string>
#include <iostream>

using std::string;

// template <typename Iterator, typename Skipper>
//    struct AsmGrammar : qi::grammar <

const std::string sample_prog_1{
    "; This program rapidly updates the colors\n"
    ";\n"
    "of the screen and the border.\n"
    "\n"
    "*= $c000     ;  starting address of the program\n"
    "\n"
    "SCREEN = $d021 BORDER = $d020\n"
    "\n"
    "start inc SCREEN  ; increase screen colour\n"
    "inc BORDER        ; increase border colour"
    "jmp start         ; repeat\n"};

int main() { return 0; }