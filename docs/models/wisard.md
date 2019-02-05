# Wisard
## constructors:
### type 1
```python
addressSize=3

wsd = Wisard(
   addressSize,              # required
   bleachingActivated=True,  # optional
   ignoreZero=False,         # optional
   completeAddressing=True,  # optional
   verbose=False,            # optional
   indexes=[],               # optional
   base=2,                   # optional
   confidence=1,             # optional

   ## types of return of classify
   returnActivationDegree=False, # optional
   returnConfidence=False,       # optional
   returnClassesDegrees=False    # optional
)
```
The default value for the optional parameters are showing in the example above.
- **addressSize: [int]** is the size of addressing RAM.
- **bleachingActivated: [boolean]** this enable or disable the bleaching process on the classification.
- **ignoreZero: [boolean]** this enable or disable the wisard to read the address zero in each RAM.
- **completeAddressing: [boolean]** this enable or disable the automatic complete of addressing in the case when the size of entry is not divisible by address size, this create redundant information, but address all indexes of entry.
- **verbose: [boolean]** enable or disable prints of the progress of train() and classify()
- **indexes: [list(int)]** this is to use a specific addressing of entry to the discriminators, all discriminators wiil use this addressing.
- **base: [int]** this is the base of addressing, by default is binary addressing.
- **confidence: [int]** this is the difference between the number of active RAMs of the first and second discriminators classified. It is used in the classification process.

#### Types of return of classify:
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

### type 2
```python
jsonConfig = wsd.json()
wsd2 = Wisard(jsonConfig)
```
The wisard can be saved with json or jsonConfig methods and reloaded later with this value.

## methods:
### train
This method train with the data passed to it.
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

wsd.train(X, y)
```
### classify
This method classify the data passed to it, based on what it learn.
```python

# load input data, just zeros and ones  
X = [
      [1,1,1,0,0,0,0,0],
      [1,1,1,1,0,0,0,0],
      [0,0,0,0,1,1,1,1],
      [0,0,0,0,0,1,1,1]
    ]

# the output is a list of string, this represent the classes attributed to each input
out = wsd.classify(X)

for oneout in out:
    print(oneout)
```
- **X: [list(list(int))]** This is the list of inputs to be classified.

### getMentalImages
This one show the pattern learned by the model, it return a dictionary where the key is of type string and it is the class and value is the list of integer representing the learned pattern.
```python
patterns = wsd.getMentalImages()

for key in patterns:
    print(key, patterns[key])

```

### leaveOneOut
This will untrain an input trained before.
```python
x = [1,1,1,0,0,0,0,0]
wsd.leaveOneOut(x,"cold")
```

### leaveMoreOut
This will untrain a list of inputs trained before.
```python
X = [
   [1,1,1,0,0,0,0,0],
   [0,0,0,0,1,1,1,1],
]
y = [
   "cold",
   "hot"
]
wsd.leaveMoreOut(X,y)
```

### json
This return the configuration and ram values as JSON format converted to string.
```python
print("Wisard: ", wsd.json())
# or pass true as parameter to save ram data in files (this is useful for huge rams)
print("Wisard: ", wsd.json(True,"path/to/save/data"))
```

### jsonConfig
This do the same as json() method, but without ram values.
```python
print("Wisard: ", wsd.jsonConfig())
```
