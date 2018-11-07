import os
import csv
import numpy as np
import sys

def read_files():
    features1 = []
    reward1 = []
    features2 = []
    reward2 = []
    weights = []

    with open("weights.txt",'r') as f:
        content = f.readlines()
        weights = [float(x) for x in content[1].split()]

    with open("logs1.txt", 'r') as f:
        for line in f:
            if(line!=""):
                temp = line.split()
                features1.append([float(x) for x in temp[:-1]])
                reward1.append(int(temp[-1]))

    with open("logs2.txt", 'r') as f:
        for line in f:
            if(line!=""):
                temp = line.split()
                features2.append([float(x) for x in temp[:-1]])
                reward2.append(int(temp[-1]))

    if(reward1[-1]<6):
        reward1.append(-7)
        features1.append(features1[-1])
    # if(reward2[-1]<6):
    #     reward2.append(-7)
    #     features2.append(features1[-1])


    return [features1, features2, reward1, reward2, weights]


def td_leaf(f, r, w, lemda, alpha):
    j = []
    m = len(w)
    print("len w:",len(w))
    print("len f:",len(f[0]))

    N = len(f)
    for i in range(len(f)):
        l = len(f[i])
        j.append(sum([w[notj]*f[i][notj] for notj in range(l)]))

    for jval in j:
        print (j)
    print ("That's it folks!!")

    j[-1] = r
    d = [j[i+1]-j[i] for i in range(N-1)]
    w_temp = [w[x] for x in range(len(w))]
    for i in range(m):
        diff = 0
        for t in range(N-1):
            diff += (f[t][i] * sum([(lemda ** (j-t)) * d[t] for j in range(t, N-1)]))
        w_temp[i] = w[i] + alpha*diff

    return w_temp

def calc_abselen(maxi, n):
    return maxi/n;


if __name__ == "__main__":

    csv_file = sys.argv[1]

    for p in range(5):
        fl1 = "archives/logs1_" + str(p) + ".txt"
        fl2 = "archives/logs2_" + str(p) + ".txt"
        os.system("./try.sh")
        os.system("cp logs1.txt "+fl1)
        os.system("cp logs2.txt "+fl2)
        f1, f2, r1, r2, w = read_files();
        crit_points = []
        f = f1
        r = r1
        for i in range(len(r)):
            if(r[i]):
                crit_points.append(i)
        lemda = 0.2
        alpha = 0.0001
        for i in crit_points:
            w = td_leaf(f[:i+1], r[i], w, lemda, alpha)

        with open(csv_file,'a') as f:
            writer = csv.writer(f)
            writer.writerow(w)

        abselen = calc_abselen(1, p+2)
        with open("weights.txt", 'w') as f:
            f.write(str(abselen)+" ")
            f.write(str(len(w))+'\n')
            for x in w:
                f.write(str(x)+" ")
