bison -d -o parser.cpp parser.y
flex -o tokens.cpp flex.l parser.hpp

g++ -o parser parser.cpp tokens.cpp semantics.cpp main.cpp -std=c++11
