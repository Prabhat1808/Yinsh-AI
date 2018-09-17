user_zip=$1
user_folder=${user_zip%.*}
roll_number=${user_zip%_*}

rm -rf $user_folder
unzip $user_zip
cd $user_folder 

# Get the latest git
git clone https://github.com/NikhilGupta1997/Yinsh-AI.git
# Remove files which can possibly  conflict with user-files
rm Yinsh-AI/Makefile Yinsh-AI/main.cpp Yinsh-AI/run.sh Yinsh-AI/compile.sh 
# Copy the files (Replaces the existing files!)
cp -R Yinsh-AI/* .

# Compile the user code
bash compile.sh

# Run the code
python server.py 10000 -n 5 -NC 2 -TL 120 -LOG server.log &
server_pid=$!
python client.py 0.0.0.0 10000 RandomPlayer.py > player1 &
client1_pid=$!
python client.py 0.0.0.0 10000 run.sh > player2 &
client2_pid=$!

# Wait for completion
wait $client1_pid
wait $client2_pid
wait $server_pid

score=`grep -o "Player 2 SCORE : [0-9.]*" server.log | awk -F" " '{print $5}'`
echo $roll_number"\t"$score >> ../results.tsv
