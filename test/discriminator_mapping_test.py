from wisardpkg import Discriminator
import json

print("\n")
print("### Discriminator::MAPPING ###")

X = [
    [1, 1, 1, 0, 0, 0, 0, 0, 0],
    [1, 1, 1, 1, 0, 0, 0, 0, 0],
    [0, 0, 0, 0, 1, 1, 1, 1, 1],
    [0, 0, 0, 0, 0, 1, 1, 1, 1]
]

mapping = [[0, 8], [1, 8], [2, 7], [3, 6], [4, 5]]

print("Creating Discriminator with mapping...", end="\r")
addressSize = 2
d = Discriminator(addressSize, len(X[0]), mapping=mapping)
print("[OK] Creating Discriminator with mapping")

print("Training...", end="\r")
d.train(X)
print("[OK] Training")

print("Classifying...", end="\r")
out = d.classify(X[0])
print("[{}] Classifying".format("OK" if len(out) == len(mapping) else "ERROR"))

print("[{}] Mapping comparison".format("OK" if json.loads(
    d.json())["mapping"] == mapping else "ERROR"))

print("### DONE Discriminator ###")
