from wisardpkg import Discriminator

print("\n\n")
print("### Discriminator ###")
print("### Input ###")

X = [
    [1,1,1,0,0,0,0,0,0],
    [1,1,1,1,0,0,0,0,0],
    [0,0,0,0,1,1,1,1,1],
    [0,0,0,0,0,1,1,1,1]
]

for d in X:
    print(d)

print("\n")

addressSize = 2 # tamanho do endereçamento das rams
d = Discriminator(addressSize, len(X[0]))

print("training...")
d.train(X)

print("classifing...")
out=d.classify(X[0])
print(out)

print(d.json())

print("### DONE Discriminator ###")
