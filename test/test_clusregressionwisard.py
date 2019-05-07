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
            wsd = wp.ClusRegressionWisard(addressSize=3, minScore=0.5, threshold=10, limit=5)
            wsd.train(self.X)
        except RuntimeError and TypeError:
            self.fail("train test fail")

    def test_predict(self):
        try:
            wsd = wp.ClusRegressionWisard(addressSize=3, minScore=0.2, threshold=2, limit=10, steps=20)
            wsd.train(self.X)
            out = wsd.predict(self.X)
            for i in range(len(out)):
                self.assertAlmostEqual(self.y[i], out[i], places=2)

        except RuntimeError and TypeError:
            self.fail("predict test fail")
    
    def test_json(self):
        try:
            wsd = wp.ClusRegressionWisard(addressSize=3, minScore=0.2, threshold=2, limit=10, steps=20, mean=wp.ExponentialMean())
            wsd.train(self.X)
            ojsonout = wsd.json()

            import sys
            if sys.version_info[0] < 3:
                with self.subTest(type="instance_json_python2"):
                    self.assertIsInstance(ojsonout,unicode)
            else:
                with self.subTest(type="instance_json_python3"):
                    self.assertIsInstance(ojsonout,str)

            jsonout = json.loads(ojsonout)
            with self.subTest(type="instance_dict"):
                self.assertIsInstance(jsonout,dict)

            with self.subTest(type="sequence"):
                wsd2 = wp.ClusRegressionWisard(ojsonout)
                self.assertSequenceEqual(wsd.predict(self.X),wsd2.predict(self.X))
        except RuntimeError and TypeError:
            self.fail("json test fail")

if __name__ == '__main__':
    main(verbosity=2)
