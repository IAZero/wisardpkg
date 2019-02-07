from unittest import TestCase
import wisardpkg as wp
import json

class WisardTestCase(TestCase):
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
            wsd = wp.Wisard(3)
            self.assertIsInstance(wsd,wp.Wisard)
        except TypeError:
            self.fail("Wisard instantiation failed!")

    def test_train(self):
        try:
            wsd = wp.Wisard(3)
            wsd.train(self.X,self.y)
        except RuntimeError and TypeError:
            self.fail("train test fail")

    def test_classify(self):
        try:
            wsd = wp.Wisard(3)
            wsd.train(self.X,self.y)
            out = wsd.classify(self.X)
            self.assertSequenceEqual(self.y,out)
        except RuntimeError and TypeError:
            self.fail("classify test fail")

    def test_json(self):
        try:
            wsd = wp.Wisard(3)
            wsd.train(self.X,self.y)
            ojsonout = wsd.json()
            self.assertIsInstance(ojsonout,str)
            jsonout = json.loads(ojsonout)
            self.assertIsInstance(jsonout,dict)
            wsd2 = wp.Wisard(ojsonout)
            self.assertSequenceEqual(wsd.classify(self.X),wsd2.classify(self.X))
        except RuntimeError and TypeError:
            self.fail("json test fail")
