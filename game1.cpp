#include <bits/stdc++.h>
#include "utilities.cpp"
using namespace std;

class Node_game{
public:
    int data;
    pair<int, int> index;
    Node_game(){
        this->data = 0;
        this->index.first = -1;
        this->index.second = -1;

        for(int i=0 ;i<6; i++){
            pair<int, int> temp;
            temp.first = -1;
            temp.second = -1;
        }
    }
};

class Game{
public:
    vector<vector<Node_game*>> board;
    int player;
    int rings_removed0;
    int rings_removed1;
    int l;
    int max_row;
    int n;
    Utility* util;
    vector<pair<int, int>> ring_self;
    vector<pair<int, int>> ring_opponent;
    int rings_placed;
    int rings;
    int ring_selected;
    int my_marker;
    pair<int, int> beginning;
    pair<int, int> ending;
    Game(int player, int my_marker,  Utility* util, int n=5, int rings=5, int max_row=5, int l=3){
        this-> n = n;
        this-> rings = rings;
        this-> max_row = max_row;
        this -> l = l;
        this->ring_self.resize(rings);
        this->ring_opponent.resize(rings);
        this->util = util;
        vector<Node_game*> tem;
        Node_game* no = new Node_game();
        no->index.first = 0;
        no->index.second = 0;
        tem.push_back(no);
        this->board.push_back(tem);
        for(int i=1; i<=n; i++){
            vector<Node_game*> temp;
            for(int j=0; j<i*6; j++){
                Node_game* nod = new Node_game();
                nod->index.first = i;
                nod->index.second = j;
                temp.push_back(nod);
                if(i==n && j%i==0) nod->data = -1;
            }
            this->board.push_back(temp);
        }
        this->my_marker = my_marker;
        this->rings_removed0 = 0;
        this->rings_removed1 = 0;
        this->player = player;
        this->rings_placed = 0;
        this->beginning.first = -1;
        this->beginning.second = -1;
    }
    void update_board(int hexagon, int position, int data){
        board.at(hexagon).at(position)->data = data;
    }

    void update_rings(int rings0, int rings1, int rings_placed){
        this->rings_removed0 = rings0;
        this->rings_removed1 = rings1;
        this->rings_placed = rings_placed;
    }
    Game* copy_board(){
        Game* copied = new Game(player, my_marker, util, n, rings, max_row, l);
        copied->update_rings(this->rings_removed0, this->rings_removed1, this->rings_placed);
        for (int i=0; i<5; i++){
            copied->ring_self.at(i).first = ring_self.at(i).first;
            copied->ring_self.at(i).second = ring_self.at(i).second;
            copied->ring_opponent.at(i).first = ring_opponent.at(i).first;
            copied->ring_opponent.at(i).second = ring_opponent.at(i).second;      
        }
        for(int i=0; i<board.size(); i++){
            for (int j=0; j<board.at(i).size(); j++){
                copied->update_board(i, j, board.at(i).at(j)->data);
            }
        }
        return copied;
    }
    
};

