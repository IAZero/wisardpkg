# Synthesizer
## constructor:
```python
wsd = Wisard(3)
# do the training...
mentalImages = wsd.getMentalImages() 

s = Synthesizer(mentalImages["hot"])
```
- **mentalImage: [list[int]]** The constructor receive only a mental image.

## methods:

### make
This will return a synthetic data based on mentalImage.
```python
syntheticData = s.make()
```
The return type will be a list of integers.
