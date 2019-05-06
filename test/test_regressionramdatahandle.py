from unittest import TestCase, main
import wisardpkg as wp

class RegressionRAMDataHandleTestCase(TestCase):

    all_data = "AgAAAAAAAAAAAAAAAAAAAAAAAAAAABBAAAAAAAAA8D8AAAAAAAAAAAAAAAAAAABALUMc6+I2Oj8AAAAAAAAAAAEAAAAAAAAAAAAAAAAAAEAAAAAAAAAAAAAAAAAAAPA/.AgAAAAAAAADNzMzMzMwAQM3MzMzMzABAzczMzMzMAEAAAAAAAAAAAJqZmZmZmbk/mpmZmZmZuT+amZmZmZm5PwEAAAAAAAAAmpmZmZmZ8T+amZmZmZnxP5qZmZmZmfE/"
    ram_data = "AgAAAAAAAAAAAAAAAAAAAAAAAAAAABBAAAAAAAAA8D8AAAAAAAAAAAAAAAAAAABALUMc6+I2Oj8AAAAAAAAAAAEAAAAAAAAAAAAAAAAAAEAAAAAAAAAAAAAAAAAAAPA/"
    data = [{0:[2.0, 0.0004, 0.0], 1:[2.0, 0.0, 1.0], 2:[0.0, 4.0, 1.0]}, {0:[0.1, 0.1, 0.1], 1:[1.1, 1.1, 1.1], 2:[2.1, 2.1, 2.1]}]

    def test_build_ram_data(self):
        try:
            bin = wp.RegressionRAMDataHandle(self.ram_data)
            self.assertIsInstance(bin,wp.RegressionRAMDataHandle)
        except RuntimeError and TypeError:
            self.fail("RegressionRAMDataHandle with ram data instantiation failed!")

    def test_build_all_data(self):
        try:
            bin = wp.RegressionRAMDataHandle(self.all_data)
            self.assertIsInstance(bin,wp.RegressionRAMDataHandle)
        except RuntimeError and TypeError:
            self.fail("RegressionRAMDataHandle with all data instantiation failed!")

    def test_build_raw_data(self):
        try:
            bin = wp.RegressionRAMDataHandle(self.data)
            self.assertIsInstance(bin,wp.RegressionRAMDataHandle)
        except RuntimeError and TypeError:
            self.fail("RegressionRAMDataHandle with raw data instantiation failed!")

    def test_set_and_get_ram(self):
        try:
            bin = wp.RegressionRAMDataHandle()
            for i in range(len(self.data)):
                with self.subTest(i=i):
                    bin.set(i, self.data[i])
                    out = bin.get(i)
                    self.assertSequenceEqual(out, self.data[i])
        except RuntimeError and TypeError:
            self.fail("set and get ram test failed!")

    def test_data(self):
        try:
            bin = wp.RegressionRAMDataHandle(self.data)
            with self.subTest(type='all'):
                bin2 = wp.RegressionRAMDataHandle(bin.data())
                self.assertDictEqual(self.data[0], bin2.get(0))
                self.assertDictEqual(self.data[1], bin2.get(1))

            for i in range(len(self.data)):
                with self.subTest(i=i):
                    self.assertDictEqual(self.data[i], bin2.get(i))
        except RuntimeError and TypeError:
            self.fail("data test failed!")

if __name__ == '__main__':
    main(verbosity=2)