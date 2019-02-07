from unittest import TestCase
import wisardpkg as wp
import json

class DiscriminatorTestCase(TestCase):

    X = [
        [1,1,1,0,0,0,0,0,0],
        [1,1,1,1,0,0,0,0,0],
        [0,0,0,0,1,1,1,1,1],
        [0,0,0,0,0,1,1,1,1]
    ]

    def test_build(self):
        try:
            d = wp.Discriminator(3,len(self.X[0]))
            self.assertIsInstance(d,wp.Discriminator)
        except TypeError:
            self.fail("discriminator instantiation failed!")

    def test_train(self):
        try:
            d = wp.Discriminator(3,len(self.X[0]))
            d.train(self.X)
        except RuntimeError and TypeError:
            self.fail("train test fail!")

    def test_classify(self):
        try:
            d = wp.Discriminator(3,len(self.X[0]))
            d.train(self.X)
            d.classify(self.X[0])
        except RuntimeError and TypeError:
            self.fail("classify test fail!")

    def test_json(self):
        try:
            d = wp.Discriminator(3,len(self.X[0]))
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
