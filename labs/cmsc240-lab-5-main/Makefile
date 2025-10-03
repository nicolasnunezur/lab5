all:
	make clean
	make ai_utils
	make ai_parser
	
ai_parser: ai_parser.cpp
	g++ ai_utils.o ai_parser.cpp -o ai_parser

ai_utils: ai_utils.cpp ai_utils.h
	g++ -c ai_utils.cpp -o ai_utils.o

clean:
	rm -f ai_utils.o ai_parser