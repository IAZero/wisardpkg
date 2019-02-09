from unittest import TestCase
import wisardpkg as wp
import json

class WisardTestCase(TestCase):
    X = [
        [1,1,1,0,0,0,0,0,0],
        [1,1,1,1,0,0,1,0,0],
        [1,1,1,0,0,0,1,0,0],
        [1,1,1,1,0,0,0,0,0],
        [1,0,1,0,1,0,1,0,1],
        [0,0,0,0,1,1,1,1,1],
        [0,0,1,0,0,1,1,1,1],
        [0,0,1,0,1,1,1,1,1],
        [0,0,0,0,0,1,1,1,1]
    ]

    y = [
        "cold",
        "cold",
        "cold",
        "cold",
        "cold",
        "hot",
        "hot",
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

            import sys
            if sys.version_info[0] < 3:
                self.assertIsInstance(ojsonout,unicode)
            else:
                self.assertIsInstance(ojsonout,str)

            jsonout = json.loads(ojsonout)
            self.assertIsInstance(jsonout,dict)
            wsd2 = wp.Wisard(ojsonout)
            self.assertSequenceEqual(wsd.classify(self.X),wsd2.classify(self.X))
        except RuntimeError and TypeError:
            self.fail("json test fail")

    def test_sizeof(self):
        try:
            wsd = wp.Wisard(20)
            X = []
            y = []
            from random import randint as ri
            for i in range(300):
                l = [ ri(0,1) if j < 51 else 0 for j in range(100) ]
                X.append(l)
                y.append("cold")

            for i in range(300):
                l = [ ri(0,1) if j > 50 else 0 for j in range(100) ]
                X.append(l)
                y.append("hot")
            wsd.train(X,y)

            self.assertLess(wsd.getsizeof(),len(wsd.json()))
        except RuntimeError and TypeError:
            self.fail("sizeof test fail")

    def test_mental_image(self):
        try:
            wsd = wp.Wisard(3)
            X = [
                [1,1,1,1,1,0,0,0,0],
                [1,1,1,1,0,0,0,0,0],
                [0,0,0,0,1,1,1,1,1],
                [0,0,0,0,0,1,1,1,1]
            ]
            y = ["cold","cold","hot","hot"]
            wsd.train(X,y)
            m = [
                [2,2,2,2,1,0,0,0,0],
                [0,0,0,0,1,2,2,2,2]
            ]
            im = wsd.getMentalImages()
            self.assertSequenceEqual(m[0],im["cold"])
            self.assertSequenceEqual(m[1],im["hot"])
        except RuntimeError and TypeError:
            self.fail("mental image test fail")
