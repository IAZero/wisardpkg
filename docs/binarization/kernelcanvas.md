# KernelCanvas
## constructor:
```python
numberOfKernels = 10
dimension = 2

kc = KernelCanvas(
    dimension,               # required
    numberOfKernels,         # required
    bitsBykernel = 3,        # optional
    activationDegree = 0.07, # optional
    useDirection = False     # optional
)
```
- **dimension: [int]** this is the dimension of your input.
- **numberOfKernels: [int]** this the number of kernel you wish to create.
- **bitsByKernel: [int]** this is the number of bits by kernel in the output data.
- **activationDegree: [float]** this is the proportion of how many kernels will be activated.
- **useDirection: [boolean]** this activate the use of direction of the data.  

## methods:
# transform
This method receive a list of points in double precision with dimension passed in the creation of KernelCanvas object.
 
```python
from random import random

kc = KernelCanvas(10,2,3)

sequenceData = []
for i in range(100):
    point = [0.0,0.0]
    point[0] = 10*random()
    point[1] = 10*random()
    sequenceData.append(point)

out = kc.transform(sequenceData)

```
It will return the list of new input mapped from kernel. It will just zeros and ones of size numberOfKernels * bitsBykernel
