# wisardpkg

## Description:
This is a project to make available the different models based on WiSARD,
with high performance, easy to use and to install and following a pattern of use.
These provided models are machine learning models,
with supervised, unsupervised and semi-supervised learning.

## to install:
```
pip install wisardpkg
```
Works to python2 and pyhton3.  
If you are on Linux and not in a virtual environment, you may need to run as superuser.


## to uninstall:
```
pip uninstall wisardpkg
```

## to import:
```
import wisardpkg as wp
```

## to use:
### WiSARD

WiSARD with bleaching by default:
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
You can find the complete documentation in the [wiki](https://github.com/aluiziolimafilho/wisardpkg/wiki).
