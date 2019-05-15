
SRC_DIR = src
TEST_DIR = test
EXT = so
PIP = pip3
PYTHON = python3
TEST_EXEC = test.o
TEST_SRC = test.cc

compile:
	make -C $(SRC_DIR)

clean:
	rm $(TEST_DIR)/*.$(EXT)

install:
	$(PIP) install .

uninstall:
	$(PIP) uninstall wisardpkg

include:
	python generate_include.py

cpptest: include
	g++ $(TEST_SRC) -o $(TEST_EXEC)
	./$(TEST_EXEC)
	rm $(TEST_EXEC)

compile2:
	make -C $(SRC_DIR) test-build
	
unittest: compile2
	python test/testset.py
