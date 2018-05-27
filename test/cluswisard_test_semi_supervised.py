from wisardpkg import ClusWisard

print("\n\n")
print("### ClusWiSARD SEMI SUPERVISED ###")
print("### Input ###")

X = [
    [1,1,1,0,0,0,0,0,0],
    [1,1,1,1,0,0,0,0,0],
    [0,0,0,0,1,1,1,1,1],
    [0,0,0,0,0,1,1,1,1]
]

y = {
    1:"cold",
    3:"hot"
}

for i,d in enumerate(X):
    if i in y:
        print(y[i],d)
    else:
        print("unknown",d)

print("\n")

addressSize = 3 # tamanho do endereçamento das rams
minScore = 0.1 # score mínimo do processo de treino
threshold = 10 # limite de treinos por discriminador
discriminatorLimit = 5 # limit de discriminadores por cluster
clus = ClusWisard(addressSize,minScore,threshold,discriminatorLimit)


print("training...")
clus.train(X,y)

print("classifing...")
out=clus.classify(X)

print("out:")
for i,d in enumerate(X):
    print(out[i],d)

print("### DONE ClusWiSARD SEMI SUPERVISED ###")
