from wisardpkg import DataSet

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

print("### DataSet ###")

print("## supervised ##")
supervised = DataSet()
for i,x in enumerate(X):
    supervised.append(x,y[i])

if supervised.isSupervised():
    print("supervised")
else:
    print("Error in supervised")



print("## semi-supervised ##")
semisupervised = DataSet()
for i,x in enumerate(X):
    if i%2 == 0:
        semisupervised.append(x,y[i])
    else:
        semisupervised.append(x)

if semisupervised.isSemiSupervised():
    print("semi-supervised")
else:
    print("Error in semi-supervised")


print("## unsupervised ##")
unsupervised = DataSet()
for x in X:
    unsupervised.append(x)

if unsupervised.isUnsupervised():
    print("unsupervised")
else:
    print("Error in unsupervised")


print("### DONE DataSet ###")
