#include <iostream>
#include <string>
#include <iterator>
#include <algorithm>
#include <regex>

// g++ -std=c++14 -Wall -o strip-comments strip-comments.cpp
// cat file | strip-comments 

using namespace std;

class line {
    string input;
    public:
        friend istream& operator>>(istream &is, line &Line) {
            return getline(is, Line.input);
        }

        operator string() const { return(input); }
};

int main(int argc, char **argv) {
    regex pattern {R"(^\s*?[#%].*)"};
    copy_if(istream_iterator<line>(cin), 
            istream_iterator<line>(), 
            ostream_iterator<string>(cout,"\n"),
            [&](const string &s) -> bool { 
                smatch match;
                return(!regex_match(s, match, pattern));
            });
    return(0);
}

