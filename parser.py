s = input()


moves = []
l = []
while True:
    line = input()
    if line:
        l.append(line)
    else:
        break
# l = s.split("\n")
for i in l:
	moves.append(i.split("\"")[-2])

m = []
for i in moves:
	m.append("game.execute_move(\"" + i + "\");")

for i in m:
	print(i)