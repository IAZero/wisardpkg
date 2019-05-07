from unittest import TestCase, main
import wisardpkg as wp

class SimpleThermometerTestCase(TestCase):

    def test_build(self):
        try:
            therm = wp.SimpleThermometer(thermometerSize=2, maximum=4)
            self.assertIsInstance(therm, wp.SimpleThermometer)
        except RuntimeError and TypeError:
            self.fail("simple thermometer instantiation fail!")

    def test_transform(self):
        try:
            therm = wp.SimpleThermometer(thermometerSize=2)

            expected = [1, 1, 1, 0, 1, 1]
            data = [0.51, 0.5, 1.0]

            out = therm.transform(data)
            expectedOut = wp.BinInput(expected)
            a = [out[i] for i in range(len(out))]
            b = [expectedOut[i] for i in range(len(expectedOut))]
            self.assertSequenceEqual(a, b)
                
        except RuntimeError and TypeError:
            self.fail("simple thermometer transform fail!")

    def test_getSize(self):
        try:
            therm = wp.SimpleThermometer(thermometerSize=2)
            self.assertEqual(2, therm.getSize())
        except RuntimeError and TypeError:
            self.fail("simple thermometer getSize fail!")


class DynamicThermometerTestCase(TestCase):

    def test_build(self):
        try:
            therm = wp.DynamicThermometer(thermometerSizes=[2, 3, 4], maximum=[1.0, 2.0, 1.0])
            self.assertIsInstance(therm, wp.DynamicThermometer)
        except RuntimeError and TypeError:
            self.fail("dynamic thermometer instantiation fail!")

    def test_transform(self):
        try:
            therm = wp.DynamicThermometer(thermometerSizes=[2, 4, 2])

            expected = [1, 1, 1, 1, 0, 0, 1, 1]
            data = [0.51, 0.5, 1.0]

            out = therm.transform(data)
            expectedOut = wp.BinInput(expected)
            a = [out[i] for i in range(len(out))]
            b = [expectedOut[i] for i in range(len(expectedOut))]
            self.assertSequenceEqual(a, b)
                
        except RuntimeError and TypeError:
            self.fail("dynamic thermometer transform fail!")

    def test_getSize(self):
        try:
            therm = wp.DynamicThermometer(thermometerSizes=[2, 2, 4, 2])
            self.assertEqual(10, therm.getSize())
        except RuntimeError and TypeError:
            self.fail("dynamic thermometer getSize fail!")

if __name__ == '__main__':
    main(verbosity=2)