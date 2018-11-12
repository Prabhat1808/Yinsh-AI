xterm -title "server" -e python2 server.py 10000 -n 6 -s 6 -NC 2 -TL 150 -LOG server.log &
xterm -title "client1" -e python2 client.py 0.0.0.0 10000 run2.sh -mode GUI &
sleep 3
python2 client.py 0.0.0.0 10000 run.sh
