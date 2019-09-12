CXX = g++
CXXFLAGS = -Wall -Wextra -Werror -pedantic -pedantic-errors -O3

all:mapper

mapper: main.o mapper.o 
	$(CXX) -o mapper main.o mapper.o 

My_class.o: My_class.cpp 
	$(CXX) $(CXXFLAGS) -c mapper.cpp 

main.o: main.cpp 
	$(CXX) $(CXXFLAGS) -c main.cpp 

clean:
	rm mapper.o main.o 
	rm mapper
