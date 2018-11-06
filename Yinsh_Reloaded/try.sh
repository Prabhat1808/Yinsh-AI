xterm -title "server" -e python2 server.py 10000 -n 5 -NC 2 -TL 150 -LOG server.log &
xterm -title "client1" -e python2 client.py 0.0.0.0 10000 run.sh -mode GUI &
xterm -title "client2" -e python2 client.py 0.0.0.0 10000 run.sh
