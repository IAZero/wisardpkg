from wisardpkg import KernelCanvas
from random import random

print("\n\n")
print("### Kernel Canvas  ###")
numberOfKernels = 10
dimension = 2
bitsByKernel = 4
kc = KernelCanvas(numberOfKernels, dimension, bitsByKernel)

sequenceData = []
for i in range(100):
    point = [0.0,0.0]
    point[0] = 10*random()
    point[1] = 10*random()
    sequenceData.append(point)

out = kc.transform(sequenceData)
print("binary output:",out)
print("### DONE Kernel Canvas  ###")
