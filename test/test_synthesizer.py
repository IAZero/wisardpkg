from unittest import TestCase
import wisardpkg as wp

class SynthesizerTestCase(TestCase):

    mi = {
        "cold":[2,2,2,2,1,0,0,0,0],
        "hot":[0,0,0,0,1,2,2,2,2]
    }

    def test_build(self):
        try:
            syn = wp.Synthesizer(self.mi["cold"])
            self.assertIsInstance(syn,wp.Synthesizer)
        except RuntimeError and TypeError:
            self.fail("Synthesizer instantiation failed!")

    def test_make(self):
        try:
            syn = wp.Synthesizer(self.mi["hot"])
            data = syn.make()
            with self.subTest(type="size"):
                self.assertEqual(len(data),len(self.mi["hot"]))
            for i,d in enumerate(data):
                with self.subTest(type="bin", i=i):
                    self.assertIn(d,[0,1])

        except RuntimeError and TypeError:
            self.fail("Synthesizer make test failed!")
