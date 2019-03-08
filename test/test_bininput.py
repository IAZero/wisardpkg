from unittest import TestCase
import wisardpkg as wp

class BinInputTestCase(TestCase):

    v = [1,1,1,0,0,0,1,1,1]
    v_data = "B+OA"

    def test_build(self):
        try:
            bin = wp.BinInput(10)
            self.assertIsInstance(bin,wp.BinInput)
        except RuntimeError and TypeError:
            self.fail("BinInput instantiation failed!")

    def test_build_with_vector(self):
        try:
            bin = wp.BinInput(self.v)
            self.assertIsInstance(bin,wp.BinInput)
        except RuntimeError and TypeError:
            self.fail("BinInput instantiation failed!")

    def test_get(self):
        try:
            bin = wp.BinInput(self.v)
            self.assertSequenceEqual(self.v,[bin.get(i) for i in range(bin.size())])
        except RuntimeError and TypeError:
            self.fail("BinInput get test failed!")

    def test_set(self):
        try:
            size = 10
            bin = wp.BinInput(size)
            for i in range(bin.size()):
                bin.set(i,1)
            self.assertSequenceEqual([1]*size,[bin[i] for i in range(bin.size())])
        except RuntimeError and TypeError:
            self.fail("BinInput set test failed!")

    def test_data(self):
        try:
            bin = wp.BinInput(self.v)
            data = bin.data()
            self.assertEqual(data,self.v_data)
        except RuntimeError and TypeError:
            self.fail("BinInput data test failed!")

    def test_build_with_data(self):
        try:
            bin = wp.BinInput(self.v_data)
            self.assertSequenceEqual(self.v,[bin[i] for i in range(bin.size())])
        except RuntimeError and TypeError:
            self.fail("BinInput buil test with data failed!")
