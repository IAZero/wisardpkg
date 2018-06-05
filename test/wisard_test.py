from wisardpkg import Wisard

print("\n\n")
print("### WiSARD ###")
print("### Input ###")

X = [
    [1,1,1,0,0,0,0,0,0],
    [1,1,1,1,0,0,0,0,0],
    [0,0,0,0,1,1,1,1,1],
    [0,0,0,0,0,1,1,1,1]
]

y = [
    "cold",
    "cold",
    "hot",
    "hot"
]
for i,d in enumerate(X):
    print(y[i],d)


print("\n")

addressSize = 2 # tamanho do endereçamento das rams
wsd = Wisard(addressSize, bleachingActivated=True)

print("training...")
wsd.train(X,y)

print("classifing...")
out=wsd.classify(X, returnActivationDegree=True, returnConfidence=True)

print("out:")
for i,d in enumerate(X):
    print(out[i],d)

print("### DONE WiSARD ###")
