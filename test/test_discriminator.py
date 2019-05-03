from unittest import TestCase, main
import wisardpkg as wp
import json

class DiscriminatorTestCase(TestCase):

    X = None
    size = None

    def setUp(self):
        self.X = wp.DataSet()
        data = [
            [1,1,1,0,0,0,0,0,0],
            [1,1,1,1,0,0,0,0,0],
            [0,0,0,0,1,1,1,1,1],
            [0,0,0,0,0,1,1,1,1]
        ]
        self.size = len(data[0])
        for x in data:
            self.X.add(x)

    def test_build(self):
        try:
            d = wp.Discriminator(3,self.size)
            self.assertIsInstance(d,wp.Discriminator)
        except TypeError:
            self.fail("discriminator instantiation failed!")

    def test_train(self):
        try:
            d = wp.Discriminator(3,self.size)
            d.train(self.X)
        except RuntimeError and TypeError:
            self.fail("train test fail!")

    def test_classify(self):
        try:
            d = wp.Discriminator(3,self.size)
            d.train(self.X)
            d.classify(self.X[0])
        except RuntimeError and TypeError:
            self.fail("classify test fail!")

    def test_json(self):
        try:
            d = wp.Discriminator(3,self.size)
            d.train(self.X)
            out = d.json()

            import sys
            if sys.version_info[0] < 3:
                self.assertIsInstance(out,unicode)
            else:
                self.assertIsInstance(out,str)

            out2 = json.loads(out)
            self.assertIsInstance(out2,dict)
            d2 = wp.Discriminator(out)
            self.assertSequenceEqual(d.classify(self.X[0]),d2.classify(self.X[0]))
        except RuntimeError and TypeError:
            self.fail("json test fail!")

    def test_mapping(self):
        try:
            mapping = [[0, 8], [1, 8], [2, 7], [3, 6], [4, 5]]
            d = wp.Discriminator(2, self.size, mapping=mapping)
            d.train(self.X)
            out = d.classify(self.X[0])
            self.assertEqual(len(out),len(mapping))
            self.assertSequenceEqual(json.loads(d.json())["mapping"], mapping)
        except RuntimeError and TypeError:
            self.fail("mapping test fail!")

if __name__ == '__main__':
    main(verbosity=2)