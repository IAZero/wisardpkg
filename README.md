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

    # load input data, just zeros and ones  
    X = [
        [1,1,1,0,0,0,0,0],
        [1,1,1,1,0,0,0,0],
        [0,0,0,0,1,1,1,1],
        [0,0,0,0,0,1,1,1]
    ]

    # load label data, which must be strings
    y = [
        "cold",
        "cold",
        "hot",
        "hot"
    ]


    addressSize = 3 # position of addressing bit in memory
    ignoreZero = False # this value is optional, this make the rams ignore the address
    wsd = wp.Wisard(addressSize, ignoreZero)


    # by default this value is false because affect the performance
    # this make the WiSARD print the training and the classify progress
    wsd.verbose = True

    # train the input data
    wsd.train(X,y)

    # classify some data
    out=wsd.classify(X)

    # the output of classify is a list of string in the same sequence of the input
    for i,d in enumerate(X):
        print(out[i],d)

### ClusWiSARD:
ClusWiSARD with bleaching by default

    addressSize = 3 # number of bit of addressing in the ram.
    minScore = 0.1 # min score of training process
    threshold = 10 # limit of trainings by discriminator
    discriminatorLimit = 5 # limit of discriminators by clusters

    clus = ClusWisard(addressSize,minScore,threshold,discriminatorLimit)

    # by default this value is false because affect the performance
    # this make the ClusWiSARD print the training and the classify progress
    clus.verbose = True

    # train the input data
    clus.train(X,y)

    # or you can train the data with some labels
    # input with some labels, the keys must be integer and values must be string
    # the keys are indexes of the input array which the labels are associated.
    y2 = {
      1: "cold"
      3: "hot"
    }

    clus.train(X,y2)

    # classify some data
    out=clus.classify(X)

    # the output of classify is a list of string in the same sequence of the input
    for i,d in enumerate(X):
        print(out[i],d)
