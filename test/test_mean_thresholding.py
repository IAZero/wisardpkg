from unittest import TestCase
import wisardpkg as wp

class MeanThresholdingTestCase(TestCase):

    def test_build(self):
        try:
            mthresh = wp.MeanThresholding()
            self.assertIsInstance(mthresh,wp.MeanThresholding)
        except RuntimeError and TypeError:
            self.fail("mean thresholding instantiation fail!")

    def test_transform(self):
        try:
            mthresh = wp.MeanThresholding()

            expected = [0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1]
            data = [-1, 0, 1, 2, 2.9999999999, 3, 3.0000000001, 4, 5, 6, 7]

            out = mthresh.transform(data)
            expectedOut = wp.BinInput(expected)
            a = [out[i] for i in range(out.size())]
            b = [expectedOut[i] for i in range(expectedOut.size())]
            self.assertSequenceEqual(a, b)
        except RuntimeError and TypeError:
            self.fail("mean thresholding transform fail!")
