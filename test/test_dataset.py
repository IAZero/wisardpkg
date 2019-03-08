from unittest import TestCase
import wisardpkg as wp

class DataSetTestCase(TestCase):

    X = [
        [1,1,1,0,0,0,0,0,0],
        [1,1,1,1,0,0,0,0,0],
        [1,0,1,0,1,0,1,0,1],
        [0,0,0,0,1,1,1,1,1],
        [0,0,0,0,0,1,1,1,1]
    ]

    y = [
        "cold",
        "cold",
        "cold",
        "hot",
        "hot"
    ]

    def test_build(self):
        try:
            ds = wp.DataSet()
            self.assertIsInstance(ds,wp.DataSet)
        except RuntimeError and TypeError:
            self.fail("DataSet instantiation failed!")


    def test_just_add_with_bin(self):
        try:
            ds = wp.DataSet()
            ds.add(wp.BinInput(self.X[0]))
            self.assertEqual(1,ds.size())
        except RuntimeError and TypeError:
            self.fail("DataSet just add with bin test failed!")

    def test_just_add_with_vector(self):
        try:
            ds = wp.DataSet()
            ds.add(self.X[0])
            self.assertEqual(1,ds.size())
        except RuntimeError and TypeError:
            self.fail("DataSet just add with vector test failed!")

    def test_just_add_with_bin_str(self):
        try:
            ds = wp.DataSet()
            ds.add(wp.BinInput(self.X[0]),self.y[0])
            self.assertEqual(1,ds.size())
        except RuntimeError and TypeError:
            self.fail("DataSet just add with bin str test failed!")

    def test_just_add_with_vector_str(self):
        try:
            ds = wp.DataSet()
            ds.add(self.X[0],self.y[0])
            self.assertEqual(1,ds.size())
        except RuntimeError and TypeError:
            self.fail("DataSet just add with vector str test failed!")

    def test_add_with_bin(self):
        try:
            ds = wp.DataSet()
            ds.add(wp.BinInput(self.X[0]))
            bin = ds.get(0)
            self.assertSequenceEqual(self.X[0],[bin[i] for i in range(bin.size())])
        except RuntimeError and TypeError:
            self.fail("DataSet add with bin test failed!")

    def test_add_with_vector(self):
        try:
            ds = wp.DataSet()
            ds.add(self.X[0])
            bin = ds.get(0)
            self.assertSequenceEqual(self.X[0],[bin[i] for i in range(bin.size())])
        except RuntimeError and TypeError:
            self.fail("DataSet add with vector test failed!")

    def test_add_with_bin_str(self):
        try:
            ds = wp.DataSet()
            ds.add(wp.BinInput(self.X[0]),self.y[0])
            bin = ds.get(0)
            self.assertSequenceEqual(self.X[0],[bin[i] for i in range(bin.size())])
        except RuntimeError and TypeError:
            self.fail("DataSet add with bin str test failed!")

    def test_add_with_vector_str(self):
        try:
            ds = wp.DataSet()
            ds.add(self.X[0],self.y[0])
            bin = ds.get(0)
            self.assertSequenceEqual(self.X[0],[bin[i] for i in range(bin.size())])
        except RuntimeError and TypeError:
            self.fail("DataSet add with vector str test failed!")

    def test_getlabel_with_bin(self):
        try:
            ds = wp.DataSet()
            ds.add(wp.BinInput(self.X[0]),self.y[0])
            label = ds.getLabel(0)
            self.assertEqual(label,self.y[0])
        except RuntimeError and TypeError:
            self.fail("DataSet getlabel bin test failed!")


    def test_getlabel_with_vector(self):
        try:
            ds = wp.DataSet()
            ds.add(self.X[0],self.y[0])
            label = ds.getLabel(0)
            self.assertEqual(label,self.y[0])
        except RuntimeError and TypeError:
            self.fail("DataSet getlabel vector test failed!")

    def test_size(self):
        try:
            ds = wp.DataSet()
            for x in self.X:
                ds.add(x)
            self.assertEqual(len(self.X),ds.size())
        except RuntimeError and TypeError:
            self.fail("DataSet size test failed!")

    def test_save(self):
        try:
            filename = "test-dataset845"
            ds = wp.DataSet()
            for i,x in enumerate(self.X):
                ds.add(x,self.y[i])
            ds.save(filename)
            import os
            out = filename+wp.dataset_sufix
            self.assertTrue(os.path.exists(out))
            if os.path.exists(out):
                os.remove(out)
        except RuntimeError and TypeError:
            self.fail("DataSet save test failed!")

    def test_load(self):
        try:
            filename = "test-dataset983"
            ds = wp.DataSet()
            for i,x in enumerate(self.X):
                ds.add(x,self.y[i])
            ds.save(filename)
            import os
            out = filename+wp.dataset_sufix
            ds2 = wp.DataSet(out)
            if os.path.exists(out):
                os.remove(out)
            for i,x in enumerate(self.X):
                with self.subTest(i=i,type="bin"):
                    bin = ds2.get(i)
                    self.assertSequenceEqual(x,[bin[j] for j in range(bin.size())])
                with self.subTest(i=i,type="label"):
                    label = ds2.getLabel(i)
                    self.assertEqual(label,self.y[i])
        except RuntimeError and TypeError:
            self.fail("DataSet load test failed!")
