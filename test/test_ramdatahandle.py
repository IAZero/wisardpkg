from unittest import TestCase
import wisardpkg as wp

class RAMDataHandleTestCase(TestCase):

    ram_data = "AQAAAAAAAAABAAAAAwAAAAAAAAACAAAA"
    discriminator_data="AAAAAAAAAAACAAAA.AwAAAAAAAAACAAAA.\
AAAAAAAAAAACAAAA.AQAAAAAAAAACAAAA.\
AgAAAAAAAAABAAAAAwAAAAAAAAABAAAA"
    data = [{0:2},{3: 2},{0: 2},{1: 2},{2: 1, 3: 1}]

    def test_build_ram_data(self):
        try:
            bin = wp.RAMDataHandle(self.ram_data)
            self.assertIsInstance(bin,wp.RAMDataHandle)
        except RuntimeError and TypeError:
            self.fail("RAMDataHandle with ram data instantiation failed!")

    def test_build_discriminator_data(self):
        try:
            bin = wp.RAMDataHandle(self.discriminator_data)
            self.assertIsInstance(bin,wp.RAMDataHandle)
        except RuntimeError and TypeError:
            self.fail("RAMDataHandle with discriminator data instantiation failed!")

    def test_get(self):
        try:
            bin = wp.RAMDataHandle(self.discriminator_data)
            for i in range(len(self.data)):
                with self.subTest(i=i):
                    dict_ram = bin.get(i)
                    self.assertDictEqual(dict_ram,self.data[i])
            for j in self.data[-1]:
                with self.subTest(j=j):
                    self.assertEqual(bin.get(len(self.data)-1,j),self.data[-1][j])

        except RuntimeError and TypeError:
            self.fail("get test failed!")

    def test_set(self):
        try:
            bin = wp.RAMDataHandle("AAAAAAAAAAACAAAA")
            bin.set(0,0,10)
            self.assertDictEqual(bin.get(0),{0:10})
        except RuntimeError and TypeError:
            self.fail("set test failed!")

    def test_data(self):
        try:
            bin = wp.RAMDataHandle("AAAAAAAAAAACAAAA.AAAAAAAAAAACAAAA")
            bin.set(0,0,10)
            d2 = [{0:10},{0:2}]
            with self.subTest(type="all"):
                bin2 = wp.RAMDataHandle(bin.data())
                for i in range(2):
                    with self.subTest(i=i):
                        self.assertDictEqual(bin2.get(i),d2[i])
            with self.subTest(type="one"):
                bin2 = wp.RAMDataHandle(bin.data(0))
                self.assertDictEqual(bin2.get(0),d2[0])
        except RuntimeError and TypeError:
            self.fail("data test failed!")
