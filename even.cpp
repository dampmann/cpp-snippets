#include <iostream>
#include <string>
#include <iterator>
#include <algorithm>

// g++ -std=c++14 -Wall -o even even.cpp
// cat file | even 
// - print lines with even line numbers
// cat file | even -s
// - print lines with even line numbers defining second line as first
//   line

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
    int count = 0;
    bool skip = false;
    if(argc > 1) {
        string flag { argv[1] };
        skip = ((flag == "-s") ? true : false);
    }

    copy_if(istream_iterator<line>(cin), 
            istream_iterator<line>(), 
            ostream_iterator<string>(cout,"\n"),
            [&](const string &s) -> bool { 
                if(skip) {
                    skip = false;
                    return false;
                }
                ++count; 
                return ((count % 2) == 0); 
            });
    return(0);
}

