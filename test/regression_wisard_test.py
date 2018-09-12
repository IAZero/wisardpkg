from wisardpkg import RegressionWisard


print("\n\n")
print("### RegressionWiSARD ###")
print("### Input ###")

X = [
    [1,1,1,0,0,0,0,0,0],
    [1,1,1,1,0,0,0,0,0],
    [0,0,0,0,1,1,1,1,1],
    [0,0,0,0,0,1,1,1,1]
]

y = [
    0.2,
    0.3,
    0.7,
    0.8
]
for i,d in enumerate(X):
    print(y[i],d)


print("\n")

addressSize = 2 # tamanho do endereçamento das rams
wsd = RegressionWisard(addressSize, useQuadraticPrecision=True, minZero=0, minOne=0)

print("training...")
wsd.train(X,y)

print("predicting...")
out=wsd.predict(X)

print("out:")
for i,d in enumerate(X):
    print(out[i],d)

#print("\nwisard: ",wsd.json())

print("### DONE RegressionWiSARD ###")
