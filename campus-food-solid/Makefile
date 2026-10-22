CXX=g++
FLAGS=-std=c++17 -Wall -Wextra -Iinclude -Iextensions
all: campus_food extension_demo run_tests
campus_food: src/main.cpp
	$(CXX) $(FLAGS) $< -o $@
extension_demo: examples/extension_demo.cpp
	$(CXX) $(FLAGS) $< -o $@
run_tests: tests/test_main.cpp
	$(CXX) $(FLAGS) $< -o $@
test: run_tests
	./run_tests
clean:
	rm -f campus_food extension_demo run_tests
