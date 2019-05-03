from unittest import TestCase, main
import wisardpkg as wp

class ThermometerTestCase(TestCase):

    def test_build(self):
        try:
            sizes = [20, 100]
            maximum = [2.0, 3.0]
            therm = wp.Thermometer(sizes, maximum)
            self.assertIsInstance(therm, wp.Thermometer)
        except RuntimeError and TypeError:
            self.fail("thermometer instantiation fail!")

    def test_transform(self):
        try:
            therm = wp.Thermometer([4, 2, 2], [1.0, 1.0, 1.0])

            expected = [1, 1, 1, 0, 1, 0, 1, 1]
            data = [0.5, 0.499999, 1.0]

            out = therm.transform(data)
            expectedOut = wp.BinInput(expected)
            a = [out[i] for i in range(len(out))]
            b = [expectedOut[i] for i in range(len(expectedOut))]
            self.assertSequenceEqual(a, b)
                
        except RuntimeError and TypeError:
            self.fail("thresthermometerholding transform fail!")

if __name__ == '__main__':
    main(verbosity=2)