#include <iostream>
#include <string>
#include <iterator>
#include <algorithm>

// g++ -std=c++14 -Wall -o odd odd.cpp
// cat file | odd 
// - print lines with odd line numbers
// cat file | odd -s
// - print lines with odd line numbers defining second line as first
//   line

using namespace std;

int main(int argc, char **argv) {
    int count = 0;
    bool skip = false;
    if(argc > 1) {
        string flag { argv[1] };
        skip = ((flag == "-s") ? true : false);
    }

    copy_if(istream_iterator<string>(cin), 
            istream_iterator<string>(), 
            ostream_iterator<string>(cout,"\n"),
            [&](const string &s) -> bool { 
                if(skip) {
                    skip = false;
                    return false;
                }
                ++count; 
                return ((count % 2) != 0); 
            });
    return(0);
}

