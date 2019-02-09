
cc=c++
flags=-Wall -O3 -shared -std=c++11 -fPIC `python3 -m pybind11 --includes`
flags2=-Wall -O3 -shared -std=c++11 -fPIC `python -m pybind11 --includes`
bin=../test/wisardpkg
exec_py=`python3-config --extension-suffix`
exec_py2=`python-config --extension-suffix`
code=wisard_bind.cc

all:
	rm -f $(bin)*.so
	$(cc) $(flags) -o $(bin)$(exec_py) $(code)

test-build:
	rm -f $(bin)*.so
	$(cc) $(flags2) -o $(bin)$(exec_py2) $(code)
