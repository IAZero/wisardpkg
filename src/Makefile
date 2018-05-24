
cc=c++
flags=-Wall -O3 -shared -std=c++11 -fPIC `python3 -m pybind11 --includes`
bin=../test/wisardpkg
exec_py=`python3-config --extension-suffix`
code=wisard_bind.cc
all:
	$(cc) $(flags) -o $(bin)$(exec_py) $(code)
