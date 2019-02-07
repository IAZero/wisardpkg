from unittest import TestCase
import wisardpkg as wp
from random import random

class KernelCanvasTestCase(TestCase):

    def test_build(self):
        try:
            dimension = 2
            numberOfKernels = 10
            kc = wp.KernelCanvas(dimension, numberOfKernels, bitsByKernel=4, useDirection=True)
            self.assertIsInstance(kc,wp.KernelCanvas)
        except RuntimeError and TypeError:
            self.fail("kernel canvas instantiation fail!")

    def test_transform(self):
        try:
            kc = wp.KernelCanvas(2, 10, bitsByKernel=4, useDirection=False)

            sequenceData = []
            p = [10*random(),10*random()]
            for i in range(100):
                point = list(p)
                point[0] += i
                point[1] += i
                sequenceData.append(point)

            out = kc.transform(sequenceData)
            for i in range(len(out)):
                self.assertIn(out[i],[0,1])

        except RuntimeError and TypeError:
            self.fail("kernel canvas transform fail!")
