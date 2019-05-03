from unittest import TestCase, main
import wisardpkg as wp
import json


class ClusWisardTestCase(TestCase):
    y = None
    y2 = None
    X = None
    X2 = None
    size = None

    def setUp(self):
        self.X = wp.DataSet()
        self.X2 = wp.DataSet()
        data = [
            [1,1,1,0,0,0,0,0,0],
            [1,1,1,1,0,0,0,0,0],
            [1,0,1,0,1,0,1,0,1],
            [0,0,0,0,1,1,1,1,1],
            [0,0,0,0,0,1,1,1,1]
        ]
        self.y = [
            "cold",
            "cold",
            "cold",
            "hot",
            "hot"
        ]
        self.y2 = {
            1:"cold",
            3:"hot"
        }
        self.size = len(data[0])
        for i,x in enumerate(data):
            self.X.add(x,self.y[i])
            if i in self.y2:
                self.X2.add(x,self.y2[i])
            else:
                self.X2.add(x)

    def test_build(self):
        try:
            addressSize = 3
            minScore = 0.1
            threshold = 10
            discriminatorLimit = 5
            clus = wp.ClusWisard(addressSize,minScore,threshold,discriminatorLimit)
            self.assertIsInstance(clus,wp.ClusWisard)
        except RuntimeError and TypeError:
            self.fail("cluswisard instantiation failed!")

    def test_train_supervised(self):
        try:
            clus = wp.ClusWisard(3,0.1,10,5)
            clus.train(self.X)
        except RuntimeError and TypeError:
            self.fail("supervised train test fail!")

    def test_train_unsupervised(self):
        try:
            clus = wp.ClusWisard(3,0.1,10,5)
            clus.trainUnsupervised(self.X)
        except RuntimeError and TypeError:
            self.fail("unsupervised train test fail!")

    def test_train_semisupervised(self):
        try:
            clus = wp.ClusWisard(3,0.1,10,5)
            clus.train(self.X2)
        except RuntimeError and TypeError:
            self.fail("semisupervised train test fail!")

    def test_classify_supervised(self):
        try:
            clus = wp.ClusWisard(3,0.1,10,5)
            clus.train(self.X)
            out = clus.classify(self.X)
            self.assertSequenceEqual(out,self.y)
        except RuntimeError and TypeError:
            self.fail("supervised classify test fail!")

    def test_classify_unsupervised(self):
        try:
            clus = wp.ClusWisard(3,0.1,10,5)
            clus.trainUnsupervised(self.X)
            out = clus.classifyUnsupervised(self.X)
        except RuntimeError and TypeError:
            self.fail("unsupervised classify test fail!")

    def test_classify_semisupervised(self):
        try:
            clus = wp.ClusWisard(3,0.1,10,5)
            clus.train(self.X2)
            out = clus.classify(self.X2)
            out2 = {k:out[k] for k in self.y2.keys()}
            self.assertSequenceEqual(out2,self.y2)
        except RuntimeError and TypeError:
            self.fail("semisupervised classify test fail!")

    def test_json_supervised(self):
        try:
            clus = wp.ClusWisard(3,0.1,10,5)
            clus.train(self.X)
            out = clus.json()
            import sys
            if sys.version_info[0] < 3:
                self.assertIsInstance(out,unicode)
            else:
                self.assertIsInstance(out,str)
            out2 = json.loads(out)
            clus2 = wp.ClusWisard(out)
            self.assertSequenceEqual(clus.classify(self.X),clus2.classify(self.X))
        except RuntimeError and TypeError:
            self.fail("supervised json test fail!")

    def test_json_unsupervised(self):
        try:
            clus = wp.ClusWisard(3,0.1,10,5)
            clus.trainUnsupervised(self.X)
            out = clus.json()
            import sys
            if sys.version_info[0] < 3:
                self.assertIsInstance(out,unicode)
            else:
                self.assertIsInstance(out,str)
            out2 = json.loads(out)
            self.assertIsInstance(out2,dict)
            clus2 = wp.ClusWisard(out)
            self.assertSequenceEqual(clus.classifyUnsupervised(self.X),clus2.classifyUnsupervised(self.X))
        except RuntimeError and TypeError:
            self.fail("unsupervised json test fail!")

if __name__ == '__main__':
    main(verbosity=2)