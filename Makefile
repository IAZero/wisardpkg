
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

geninclude:
	python generate_include.py

cpptest:
	g++ $(TEST_SRC) -o $(TEST_EXEC)
	./$(TEST_EXEC)
	rm $(TEST_EXEC)

unittest:
	$(PYTHON) $(TEST_DIR)/unittest.py
