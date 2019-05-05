from unittest import TestCase, main
import wisardpkg as wp
import json

class ClusRegressionWisardTestCase(TestCase):
    y = None
    X = None
    size = None

    def setUp(self):
        data = [
            [1,1,1,0,0,0,0,0,0],
            [1,1,1,1,0,0,0,0,0],
            [0,0,0,0,1,1,1,1,1],
            [0,0,0,0,0,1,1,1,1]
        ]
        self.y = [
            0.2,
            0.3,
            0.7,
            0.8
        ]
        self.size = len(data[0])
        self.X = wp.DataSet(data, self.y)

    def test_build(self):
        try:
            wsd = wp.ClusRegressionWisard(3, 0.5, 10, 5)
            self.assertIsInstance(wsd,wp.ClusRegressionWisard)
        except TypeError:
            self.fail("ClusRegressionWisard instantiation failed!")

    def test_train(self):
        try:
            wsd = wp.ClusRegressionWisard(3, 0.5, 10, 5)
            wsd.train(self.X)
        except RuntimeError and TypeError:
            self.fail("train test fail")

    def test_predict(self):
        try:
            wsd = wp.ClusRegressionWisard(3, 0.2, 2, 10, steps=20)
            wsd.train(self.X)
            out = wsd.predict(self.X)
            for i in range(len(out)):
                self.assertAlmostEqual(self.y[i], out[i], places=2)

        except RuntimeError and TypeError:
            self.fail("predict test fail")

if __name__ == '__main__':
    main(verbosity=2)
