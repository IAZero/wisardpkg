
SRC_DIR = src
TEST_DIR = test
EXT = so
PIP = pip3
PYTHON = python3

compile:
	make -C $(SRC_DIR)

clean:
	rm $(TEST_DIR)/*.$(EXT)

install:
	$(PIP) install .

uninstall:
	$(PIP) uninstall wisardpkg

unittest:
	$(PYTHON) $(TEST_DIR)/unittest.py
