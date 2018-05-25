# wisardpkg

## to install:
```
pip install git+http://github.com/aluiziolimafilho/wisardpkg.git
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
    
wsd = wp.Wisard(addressSize, ignoreZero)


# False by default for performance reasons,
# when True, WiSARD prints the progress of train() and classify()
wsd.verbose = True

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

clus = ClusWisard(addressSize,minScore,threshold,discriminatorLimit)

# False by default for performance reasons
# when enabled,e ClusWiSARD prints the progress of train() and classify()
clus.verbose = True

# train using the input data
clus.train(X,y)

# optionally you can train using arbitrary labels for the data
# input some labels in a dict, 
# the keys must be integer indices indicating which input array the entry is associated to,
# the values are the labels which must be strings 
y2 = {
  1: "cold"
  3: "hot"
}

clus.train(X,y2)

# classify some data
out = clus.classify(X)

# the output of classify is a string list in the same sequence as the input
for i,d in enumerate(X):
    print(out[i],d)
```
