from wisardpkg import Wisard
import json


def classify_test(wsd, X, y):
    equalOut = 0
    print("Classifying...", end="\r")
    yPred = wsd.classify(X)

    for i in range(len(y)):
        equalOut += 1 if y[i] == yPred[i] else 0
        print("Classifying({}/{})".format(equalOut, len(y)), end="\r")
    print("[{}] Classifying({}/{})".format("OK" if equalOut ==
                                           len(y) else "ERROR", equalOut, len(y)))


def mapping_test(mapping, jsonFile):
    for label in mapping.keys():
        print("[{}] {} Mapping comparison".format("OK" if json.loads(jsonFile)[
              "classes"][label]["mapping"] == mapping[label] else "ERROR", label))


def training_test(wsd, X, y):
    print("Training...", end="\r")
    wsd.train(X, y)
    print("[OK] Training")


print("\n")
print("### WiSARD::MAPPING ###")

X = [
    [1, 1, 1, 0, 0, 0, 0, 0, 0],
    [1, 1, 1, 1, 0, 0, 0, 0, 0],
    [1, 0, 1, 0, 1, 0, 1, 0, 1],
    [0, 0, 0, 0, 1, 1, 1, 1, 1],
    [0, 0, 0, 0, 0, 1, 1, 1, 1]
]

y = [
    "cold",
    "cold",
    "cold",
    "hot",
    "hot"
]

mapping = {"cold": [[0, 8], [1, 8], [2, 7], [3, 6], [4, 5]],
           "hot": [[8, 7], [6, 5], [4, 3], [2, 1], [0, 3]]}

print("Creating WiSARD with mapping...", end="\r")
addressSize = 2
original = Wisard(addressSize, mapping=mapping)
print("[OK] Creating WiSARD with mapping")

training_test(original, X, y)

classify_test(original, X, y)

originalJson = original.json()

mapping_test(mapping, originalJson)

print("Creating WiSARD clone with mapping...", end="\r")
clone = Wisard(originalJson)
print("[OK] Creating WiSARD clone with mapping")

cloneJson = clone.json()

mapping_test(mapping, cloneJson)

classify_test(clone, X, y)

print("### DONE WiSARD::MAPPING ###")
