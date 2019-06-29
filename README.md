# wisardpkg [![](https://img.shields.io/pypi/v/wisardpkg.svg)](https://pypi.org/project/wisardpkg/) [![](https://travis-ci.org/IAZero/wisardpkg.svg?branch=master)](https://travis-ci.org/IAZero/wisardpkg)

https://github.com/IAZero/wisardpkg

## Description:
This is a project to make available the different models based on WiSARD,
with high performance, easy to use and to install and following a pattern of use.
These provided models are machine learning models,
with supervised, unsupervised and semi-supervised learning.

## to install:
python:
```
pip install wisardpkg
```
Works to python2 and pyhton3.  
If you are on Linux and not in a virtual environment, you may need to run as superuser.

#### obs:
To install on windows platform you can use [anaconda](https://anaconda.org/) and do:
```
python -m pip install wisardpkg
```
c++:
copy the file wisardpkg.hpp inside your project 
```
include/wisardpkg.hpp
```

## to uninstall:
```
pip uninstall wisardpkg
```

## to import:
python:
```python
import wisardpkg as wp
```
c++:
```c++
# include "wisardpkg.hpp"

namespace wp = wisardpkg;
```

## to use:
### WiSARD

WiSARD with bleaching by default:

python:
```python
# load input data, just zeros and ones  
X = [
      [1,1,1,0,0,0,0,0],
      [1,1,1,1,0,0,0,0],
      [0,0,0,0,1,1,1,1],
      [0,0,0,0,0,1,1,1]
    ]

# load label data, which must be a string array
y = [
      "cold",
      "cold",
      "hot",
      "hot"
    ]


addressSize = 3     # number of addressing bits in the ram
ignoreZero  = False # optional; causes the rams to ignore the address 0

# False by default for performance reasons,
# when True, WiSARD prints the progress of train() and classify()
verbose = True

wsd = wp.Wisard(addressSize, ignoreZero=ignoreZero, verbose=verbose)



# train using the input data
wsd.train(X,y)

# classify some data
out = wsd.classify(X)

# the output of classify is a string list in the same sequence as the input
for i,d in enumerate(X):
    print(out[i],d)
```
c++:
```c++

vector<vector<int>> X(4);
X[0] = {1,1,1,0,0,0,0,0};
X[1] = {1,1,1,1,0,0,0,0};
X[2] = {0,0,0,0,1,1,1,1};
X[3] = {0,0,0,0,0,1,1,1};

vector<string> y = {
      "cold",
      "cold",
      "hot",
      "hot"
};


wp::Wisard w(3, {
      {"ignoreZero", false},
      {"verbose", true}
});

w.train(X,y);

vector<string> out = w.classify(X);

for(int i=0; i<4; i++){
      cout << "i: " << i << "; class: " << out[i] << endl;
}

```
### ClusWiSARD

ClusWiSARD with bleaching by default:
```python
addressSize        = 3    # number of addressing bits in the ram.
minScore           = 0.1  # min score of training process
threshold          = 10   # limit of training cycles by discriminator
discriminatorLimit = 5    # limit of discriminators by clusters

# False by default for performance reasons
# when enabled,e ClusWiSARD prints the progress of train() and classify()
verbose = True

clus = ClusWisard(addressSize, minScore, threshold, discriminatorLimit, verbose=True)



# train using the input data
clus.train(X,y)

# optionally you can train using arbitrary labels for the data
# input some labels in a dict,
# the keys must be integer indices indicating which input array the entry is associated to,
# the values are the labels which must be strings
y2 = {
  1: "cold",
  3: "hot"
}

clus.train(X,y2)

# classify some data
out = clus.classify(X)

# the output of classify is a string list in the same sequence as the input
for i,d in enumerate(X):
    print(out[i], d)
```

## Documentation:
You can find the complete documentation in the [page](https://iazero.github.io/wisardpkg/).

## Build on libraries:
[pybind11](https://github.com/pybind/pybind11)
[nlohmann/json](https://github.com/nlohmann/json)
