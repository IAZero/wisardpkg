# ClusWisard
## constructor:
```python
from wisardpkg import ClusWisard

addressSize = 3 
minScore = 0.1 
threshold = 10
discriminatorLimit = 5

clus = ClusWisard(
   addressSize,              # required
   minScore,                 # required
   threshold,                # required
   discriminatorLimit,       # required
   bleachingActivated=True,  # optional
   ignoreZero=False,         # optional
   completeAddressing=True,  # optional
   verbose=False,            # optional
   indexes=[],               # optional
   base=2,                   # optional

   ## types of return of classify
   returnActivationDegree=False, # optional
   returnConfidence=False,       # optional
   returnClassesDegrees=False    # optional
)
```
The default value for the optional parameters are showing in the example above.
- **addressSize: [int]** is the size of addressing RAM.
- **minScore: [float]** is the minimum score accept by cluswisard to choose to create a discriminator. Must be between 0 and 1.
- **threshold: [int]** is the limit of examples learned by a discriminator.
- **discriminatorLimit: [int] is the limit of discriminator by cluster in supervised and semi-supervised learning, but in unsupervised learning this is the limit of classes that can be created.
- **bleachingActivated: [boolean]** this enable or disable the bleaching process on the classification.
- **ignoreZero: [boolean]** this enable or disable the wisard to read the address zero in each RAM.
- **completeAddressing: [boolean]** this enable or disable the automatic complete of addressing in the case when the size of entry is not divisible by address size, this create redundant information, but address all indexes of entry.
- **verbose: [boolean]** enable or disable prints of the progress of train() and classify()
- **indexes: [list(int)]** this is to use a specific addressing of entry to the discriminators, all discriminators wiil use this addressing.
- **base: [int]** this is the base of addressing, by default is binary addressing.

### Types of return of classify:
By default the parameters 'returnActivationDegree', returnClassesDegrees and 'returnConfidence' are false, for performance reasons.
- **returnActivationDegree: [boolean]** when true, add the activation degree value to each output.
- **returnConfidence: [boolean]** when true, add the confidence value to each output.
- **returnClassesDegrees: [boolean]** when true, add the list of degrees of each class.
When 'returnActivationDegree' or 'returnConfidence' are true the classify will return a list of dictionaries in the following format:
```python
[
  {'class': 'cold', 'activationDegree': 1.0, 'confidence': 0.800000011920929},
  {'class': 'hot', 'activationDegree': 1.0, 'confidence': 1.0}
]
```
If only one of them is true, so only it will appear in the dictionary.
when all are false, so the classify method will return a list of strings.
This only work for classify method.

## methods:
### train
This method train with the data passed to it. In the case of labels, can be a list for supervised learning or a dictinary to semi-supervised learning.
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

# load label data, which must be a dictionary with key integer and value string
y2 = {
       1: "cold",
       2: "hot
     }

# to supervised learning
clus.train(X, y)

# to semi-supervised learning
clus.train(X, y2)

```
Keep in mind that when a dataset is passed to this method, first it will train with supervised piece and after it will train with unsupervised piece.
### trainUnsupervised
This method train with the data passed to it. This use the unsupervised learning.
```python

# load input data, just zeros and ones  
X = [
      [1,1,1,0,0,0,0,0],
      [1,1,1,1,0,0,0,0],
      [0,0,0,0,1,1,1,1],
      [0,0,0,0,0,1,1,1]
    ]

# to unsupervised learning
clus.trainUnsupervised(X)

```
### classify
This method classify the data passed to it, based on what it learn, for supervised and semi-supervised learning.
```python

# load input data, just zeros and ones  
X = [
      [1,1,1,0,0,0,0,0],
      [1,1,1,1,0,0,0,0],
      [0,0,0,0,1,1,1,1],
      [0,0,0,0,0,1,1,1]
    ]

# the output is a list of string, this represent the classes atributed to each input
out = clus.classify(X)

for oneout in out:
    print(oneout)
```

### classifyUnsupervised
This method classify the data passed to it, based on what it learn, for unsupervised learning.
```python

# load input data, just zeros and ones  
X = [
      [1,1,1,0,0,0,0,0],
      [1,1,1,1,0,0,0,0],
      [0,0,0,0,1,1,1,1],
      [0,0,0,0,0,1,1,1]
    ]

# the output is a list of string, this represent the classes atributed to each input
out = clus.classifyUnsupervised(X)

for oneout in out:
    print(oneout)
```

### getMentalImages
This one show the pattern learned by the model, it return a dictionary where the key is of type string and it is the class and value is the list of list of integers representing the learned pattern for each discriminator in the cluster.
```python
patterns = clus.getMentalImages()

for key in patterns:
    cluster = patterns[key]
    for index,discriminator in enumerate(cluster)
        print(key, index, discriminator)

```
