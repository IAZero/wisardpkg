from unittest import TestCase, main
import wisardpkg as wp
import json

class WisardTestCase(TestCase):
    y = None
    X = None
    size = None

    def setUp(self):
        self.X = wp.DataSet()
        data = [
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
        self.y = [
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
        self.size = len(data[0])
        for i,x in enumerate(data):
            self.X.add(x,self.y[i])

    def test_build(self):
        try:
            wsd = wp.Wisard(3)
            self.assertIsInstance(wsd,wp.Wisard)
        except TypeError:
            self.fail("Wisard instantiation failed!")

    def test_train(self):
        try:
            wsd = wp.Wisard(3)
            wsd.train(self.X)
        except RuntimeError and TypeError:
            self.fail("train test fail")

    def test_classify(self):
        try:
            wsd = wp.Wisard(3)
            wsd.train(self.X)
            out = wsd.classify(self.X)
            self.assertSequenceEqual(self.y,out)
        except RuntimeError and TypeError:
            self.fail("classify test fail")

    def test_json(self):
        try:
            wsd = wp.Wisard(3)
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
                wsd2 = wp.Wisard(ojsonout)
                self.assertSequenceEqual(wsd.classify(self.X),wsd2.classify(self.X))
        except RuntimeError and TypeError:
            self.fail("json test fail")

    def test_sizeof(self):
        try:
            wsd = wp.Wisard(20)
            X = wp.DataSet()
            from random import randint as ri
            for i in range(300):
                l = [ ri(0,1) if j < 51 else 0 for j in range(100) ]
                X.add(l,"cold")

            for i in range(300):
                l = [ ri(0,1) if j > 50 else 0 for j in range(100) ]
                X.add(l,"hot")
            wsd.train(X)

            self.assertLess(wsd.getsizeof(),len(wsd.json()))
        except RuntimeError and TypeError:
            self.fail("sizeof test fail")

    def test_mental_image(self):
        try:
            wsd = wp.Wisard(3)
            data = [
                [1,1,1,1,1,0,0,0,0],
                [1,1,1,1,0,0,0,0,0],
                [0,0,0,0,1,1,1,1,1],
                [0,0,0,0,0,1,1,1,1]
            ]
            X = wp.DataSet()
            y = ["cold","cold","hot","hot"]
            for i,x in enumerate(data):
                X.add(x,y[i])
            wsd.train(X)
            m = {
                "cold":[2,2,2,2,1,0,0,0,0],
                "hot":[0,0,0,0,1,2,2,2,2]
            }
            im = wsd.getMentalImages()
            for key in m:
                with self.subTest(key=key):
                    self.assertSequenceEqual(m[key],im[key])
        except RuntimeError and TypeError:
            self.fail("mental image test fail")

    def test_mapping(self):
        try:
            mapping = {"cold": [[0, 8], [1, 8], [2, 7], [3, 6], [4, 5]],
                       "hot": [[8, 7], [6, 5], [4, 3], [2, 1], [0, 3]]}
            wsd = wp.Wisard(2,mapping=mapping)
            data = [
                [1,1,1,1,1,0,0,0,0],
                [1,1,1,1,0,0,0,0,0],
                [0,0,0,0,1,1,1,1,1],
                [0,0,0,0,0,1,1,1,1]
            ]
            y = ["cold","cold","hot","hot"]
            X = wp.DataSet()
            for i,x in enumerate(data):
                X.add(x,y[i])
            wsd.train(X)
            str_json = wsd.json()
            import json
            dict_json = json.loads(str_json)
            for label in mapping:
                 with self.subTest(label=label):
                     self.assertSequenceEqual(dict_json["classes"][label]["mapping"],mapping[label])

        except RuntimeError and TypeError:
            self.fail("mapping test fail")

if __name__ == '__main__':
    main(verbosity=2)
