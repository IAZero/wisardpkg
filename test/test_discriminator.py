from unittest import TestCase
import wisardpkg as wp

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
            self.fail("build test fail!")

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
        except RuntimeError and TypeError:
            self.fail("json test fail!")
