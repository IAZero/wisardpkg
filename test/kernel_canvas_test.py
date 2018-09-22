from wisardpkg import KernelCanvas
from random import random

print("\n\n")
print("### Kernel Canvas  ###")
dimension = 2
numberOfKernels = 10
bitsByKernel = 4
kc = KernelCanvas(dimension, numberOfKernels, bitsByKernel)

sequenceData = []
p = [10*random(),10*random()]
for i in range(100):
    point = list(p)
    point[0] += i
    point[1] += i
    sequenceData.append(point)

out = kc.transform(sequenceData)
print("binary output:",out)
print("### DONE Kernel Canvas  ###")
