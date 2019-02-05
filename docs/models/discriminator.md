# Discriminator
## constructors:
### type 1
```python
from wisardpkg import Discriminator

addressSize = 3
entrySize = 9

d = Discriminator(
   addressSize,            # required
   entrySize,              # required
   ignoreZero=False,       # optional
   completeAddressing=True,# optional
   base=2                  # optional
)
```
- **addressSize: [int]** is the size of addressing RAM.
- **entrySize: [int]** is the size of one entry of the data.
- **ignoreZero: [boolean]** this enable or disable the wisard to read the address zero in each RAM.
- **completeAddressing: [boolean]** this enable or disable the automatic complete of addressing in the case when the size of entry is not divisible by address size, this create redundant information, but address all indexes of entry.
- **base: [int]** this is the base of addressing, by default is binary addressing.

### type 2
```python
jsonConfig = d.json()
d2 = Discriminator(jsonConfig)
```
The Discriminator can be saved with json or jsonConfig methods and reloaded later with this value.

## methods:
### train
It train with one entry or a list of entries.
```python

X = [
      [1,1,1,0,0,0,0,0],
      [1,1,1,1,0,0,0,0],
      [0,0,0,0,1,1,1,1],
      [0,0,0,0,0,1,1,1]
    ]

# train with one entry
d.train(X[0])

# train with a list of entries
d.train(X)
```

### classify
Return the outputs of each RAM.
```python
x = [1,1,1,0,0,0,0,0]

ramsOutput = d.classify(x)

for ramoutput in ramsOutput:
    print("ram output: ", ramoutput)
```

### json
This return the configuration and ram values as JSON format converted to string.
```python
print("Discriminator: ", d.json())
# or pass true as parameter to save ram data in files (this is useful for huge rams)
print("Discriminator: ", d.json(True,"path/to/save/data"))
```

### jsonConfig
This do the same as json() method, but without ram values.
```python
print("Discriminator: ", d.jsonConfig())
```
