
cc=c++
flags=-Wall -O3 -shared -std=c++11 -fPIC `python -m pybind11 --includes`
bin=../wisardpkg
exec_py=`python-config --extension-suffix`
code=wisard_bind.cc
all:
	$(cc) $(flags) -o $(bin)$(exec_py) $(code)
