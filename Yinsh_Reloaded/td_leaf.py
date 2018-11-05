
def read_files():
    features1 = []
    reward1 = []
    features2 = []
    reward2 = []
    weights = []

    with open("weights.txt", r) as f:
    content = f.readlines()
    weights = [float(x) for x in content[1].split()]

    with open("logs1.txt", r) as f:
        for line in f:
            if(line!=""):
                temp = line.split()
                features1.append([float(x) for x in temp[:-1]])
                reward1.append(int(temp[-1]))

    with open("logs2.txt", r) as f:
        for line in f:
            if(line!=""):
                temp = line.split()
                features2.append([float(x) for x in temp[:-1]])
                reward2.append(int(temp[-1]))

    return [features1, features2, reward1, reward2, weights]


def td_leaf(f, r, w, lemda, alpha):
    j = []
    m = len(w)
    N = len(r)
    for i in range(len(f)):
        l = len(f[i])
        j.append(sum([w[j]*f[i][j] for j in range(l)]))
    j[-1] = r
    d = [j[i+1]-j[i] for i in range(N-1)]
    w_temp = []
    for i in range(m):
        d = 0
        for t in range(N-1):
            d += (f[t][i] * sum([(lamda ** (j-t)) * d[t] for j in range(t, N)]))
        w_temp[i] = w[i]

    return w_temp




if __name__ == "__main__":
    f1, f2, r1, r2, w = read_files();
    crit_points = []
    cond = 0

