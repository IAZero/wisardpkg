from unittest import TestCase
import wisardpkg as wp

class ThresholdingTestCase(TestCase):

    def test_build(self):
        try:
            tValue = 10
            thresh = wp.Thresholding(tValue)
            self.assertIsInstance(thresh,wp.Thresholding)
        except RuntimeError and TypeError:
            self.fail("thresholding instantiation fail!")

    def test_transform(self):
        try:
            thresh = wp.Thresholding(10)

            expected = [1, 1, 1, 0, 0, 0, 0]
            data = [999999999999, 10.000001, 10, 9, 9.99999999, -1, 0]

            out = thresh.transform(data)
            expectedOut = wp.BinInput(expected)
            a = [out[i] for i in range(out.size())]
            b = [expectedOut[i] for i in range(expectedOut.size())]
            self.assertSequenceEqual(a, b)
        except RuntimeError and TypeError:
            self.fail("thresholding transform fail!")
