#include <bits/stdc++.h>
using namespace std;

class Node{
public:
    int data;
    pair<int, int> index;
    vector<pair<int, int>> neighbours;
    Node(){
        this->data = 0;
        this->index.first = -1;
        this->index.second = -1;
        for(int i=0 ;i<6; i++){
            pair<int, int> temp;
            temp.first = -1;
            temp.second = -1;
            this->neighbours.push_back(temp);
        }
    }
};

class Game{
private:
    vector<vector<Node*>> board;
    int state;
    int player;
    int rings_removed0;
    int rings_removed1;
    int l;
    int max_row;
    int n;
    int rings_placed;
    int rings;
    int ring_selected;
    pair<int, int> beginning;
    pair<int, int> ending;
public:
    Game(int player, int n=5, int rings=5, int max_row=5, int l=3){
        this-> n = n;
        this-> rings = rings;
        this-> max_row = max_row;
        this -> l = l;
        vector<Node*> tem;
        Node* no = new Node();
        auto pa = make_pair(0, 0);
        no->index.first = 0;
        no->index.second = 0;
        this->update_neighbours(no, 0, 0);
        tem.push_back(no);
        this->board.push_back(tem);
        for(int i=1; i<=n; i++){
            vector<Node*> temp;
            for(int j=0; j<i*6; j++){
                Node* nod = new Node();
                auto pa = make_pair(i, j);
                nod->index.first = i;
                nod->index.second = j;
                this->update_neighbours(nod, i, j);
                temp.push_back(nod);
                if(i==n && j%i==0) nod->data = -1;
            }
            this->board.push_back(temp);
        }
        this->state = 0;
        this->rings_removed0 = 0;
        this->rings_removed1 = 0;
        this->player = player;
        this->rings_placed = 0;
        this->beginning.first = -1;
        this->beginning.second = -1;
    }
    void update_neighbours(Node* nod, int i, int j){
        if(i==0){
            for(int k=0; k<6; k++){
                nod->neighbours.at(k).first = 1;
                nod->neighbours.at(k).second = k;
            }
        }
        else if(j%i==0 && i<n){
            if(i<n-1) {
                nod->neighbours.at(0).first = i + 1;
                nod->neighbours.at(0).second = j + (j / i);
            }
            else{
                nod->neighbours.at(0).first = -1;
                nod->neighbours.at(0).second = -1;
            }

            nod->neighbours.at(1).first = (i + 1);
            nod->neighbours.at(1).second = (j + j / i + 1) % (6 * i + 6);

            nod->neighbours.at(2).first = i;
            nod->neighbours.at(2).second = (j+1)%(6*i);

            nod->neighbours.at(3).first = i-1;
            nod->neighbours.at(3).second = j-j/i;

            nod->neighbours.at(4).first = i;
            nod->neighbours.at(5).first = i+1;
            if(j==0){
                nod->neighbours.at(4).second = 6*i-1;
                nod->neighbours.at(5).second = 6*i+5;
            }
            else{
                nod->neighbours.at(4).second = j-1;
                nod->neighbours.at(5).second = j-1;
            }
        }
        else if(j%i==0 && i==n){
            nod->neighbours.at(0).first = -1;
            nod->neighbours.at(0).second = -1;

            nod->neighbours.at(1).first = -1;
            nod->neighbours.at(1).second = -1;

            nod->neighbours.at(2).first = -1;
            nod->neighbours.at(2).second = -1;

            nod->neighbours.at(3).first = -1;
            nod->neighbours.at(3).second = -1;

            nod->neighbours.at(4).first = -1;
            nod->neighbours.at(4).second = -1;

            nod->neighbours.at(5).first = -1;
            nod->neighbours.at(5).second = -1;
        }
        else {
            if(j%i<i-1) {
                nod->neighbours.at(0).first = i;
                nod->neighbours.at(0).second = (j + 1) % (6 * i);
            }
            else{
                nod->neighbours.at(0).first = -1;
                nod->neighbours.at(0).second = -1;
            }

            nod->neighbours.at(1).first = -1;
            nod->neighbours.at(1).second = -1;

            nod->neighbours.at(2).first = -1;
            nod->neighbours.at(2).second = -1;
            if(j%i>1) {
                nod->neighbours.at(3).first = i;
                nod->neighbours.at(3).second = (j - 1) % (6 * i);
            }
            else{
                nod->neighbours.at(3).first = -1;
                nod->neighbours.at(3).second = -1;
            }
            nod->neighbours.at(4).first = i-1;
            nod->neighbours.at(4).second = (j - j/i)%(6*(i-1));

            nod->neighbours.at(5).first = i-1;
            nod->neighbours.at(5).second = (j - j/i - 1)%(6*(i-1));
        }
    }
    bool place_ring(int hexagon, int position){
        if(state!=0) return false;
        if(board.at(hexagon).at(position)->data!=0) return false;
        if(state!=0) return false;
        board.at(hexagon).at(position) -> data = player;
        rings_placed++;
        if(rings_placed==2*rings) state = 1;
        return true;
    }
    bool select_ring(int hexagon, int position){
        if(board.at(hexagon).at(position)->data!=player+1) return false;
        ring_selected = board.at(hexagon).at(position)->data;
        board.at(hexagon).at(position) -> data += 2;
        beginning.first = hexagon;
        beginning.second = position;
        return true;
    }
    bool move_validity(Node* nod, int i, int j){
        if(nod->index == ending) return true;

        pair<int, int> pre = make_pair(i, j);
        int ind = -1;
        for(int k=0; k<6; k++){
            if(nod->neighbours.at(k)==pre) ind = k;
        }
        if(ind==-1) return false;
        auto p = nod->neighbours.at((ind+3)%6);
        if(p.first==-1 || p.second==-1) return false; 
        return move_validity(board.at(p.first).at(p.second), nod->index.first, nod->index.second);
    }
    bool move_possible(Node* nod, int i, int j, bool change){
        if(nod->index == ending && nod->data==0){
            return true;
        }
        else if(nod->index == ending) return false;
        int ind = -1;
        pair<int, int> pre = make_pair(i, j);
        for(int k=0; k<6; k++){
            if(nod->neighbours.at(k)==pre) ind = k;
        }
        if(ind==-1) return false;
        ind = (ind+3)%6;
        if(change){
            if(nod->data<3) return false;
            auto p = nod->neighbours.at(ind);
            return move_possible(board.at(p.first).at(p.second), nod->index.first, nod->index.second, change);
        }
        else if(nod->data>2){
            auto p = nod->neighbours.at(ind);
            return move_possible(board.at(p.first).at(p.second), nod->index.first, nod->index.second, true);
        }
        else if(nod->data==0){
            auto p = nod->neighbours.at(ind);
            return move_possible(board.at(p.first).at(p.second), nod->index.first, nod->index.second, false);
        }
        else return false;
    }
    bool place_it(Node* nod, int i, int j, bool change){
        if(nod->index == ending && nod->data==0){
            nod->data = player+1;
            return true;
        }
        else if(nod->index == ending) return false;
        int ind = -1;
        pair<int, int> pre = make_pair(i, j);
        for(int k=0; k<6; k++){
            if(nod->neighbours.at(k)==pre) ind = k;
        }
        if(ind==-1) return false;
        ind = (ind+3)%6;
        if(change){
            if(nod->data==3) nod->data = 4;
            else if(nod->data==4) nod->data = 3;
            else return false;
            auto p = nod->neighbours.at(ind);
            return place_it(board.at(p.first).at(p.second), nod->index.first, nod->index.second, change);
        }
        else if(nod->data>2){
            if(nod->data==3) nod->data = 4;
            else if(nod->data==4) nod->data = 3;
            else return false;
            auto p = nod->neighbours.at(ind);
            return place_it(board.at(p.first).at(p.second), nod->index.first, nod->index.second, true);
        }
        else if(nod->data==0){
            auto p = nod->neighbours.at(ind);
            return place_it(board.at(p.first).at(p.second), nod->index.first, nod->index.second, false);
        }
        else return false;
    }
    bool move_ring(int hexagon, int position){
        if(board.at(hexagon).at(position)->data!=0){
            board.at(beginning.first).at(beginning.second)->data-=2;
            return false;
        }
        ending.first = hexagon;
        ending.second = position;
        bool valid = false;
        for(int i=0; i<6; i++){
            auto p = board.at(beginning.first).at(beginning.second)->neighbours.at(i);
            if(move_validity(board.at(p.first).at(p.second), beginning.first, beginning.second)){
                valid = move_possible(board.at(p.first).at(p.second), beginning.first, beginning.second, false);
                if (valid) valid = place_it(board.at(p.first).at(p.second), beginning.first, beginning.second, false);
                break;
            }
        }
        if (valid) return true;
        board.at(beginning.first).at(beginning.second)->data-=2;
        return false;
    }
    bool remove_start(int hexagon, int position){
        if(board.at(hexagon).at(position)->data!=player+3) return false;
        beginning.first = hexagon;
        beginning.second = position;
        return true;
    }
    bool remove_validity(Node* nod, int i, int j, int x){
        if(nod->data!=player+2) return false;
        else if(nod->index==ending && x==max_row) return true;
        else if(x==max_row || nod->index==ending) return false;
        pair<int, int> pre = make_pair(i, j);
        int ind = -1;
        for(int k=0; k<6; k++){
            if(nod->neighbours.at(k)==pre) ind = k;
        }
        if(ind==-1) return false;
        auto p = nod->neighbours.at((ind+3)%6);
        if(p.first==-1 || p.second==-1) return false;
        return remove_validity(board.at(p.first).at(p.second), nod->index.first, nod->index.second, x+1);
    }
    void remove_now(Node* nod, int i, int j, int x){
        nod->data = 0;
        if(x!=max_row) {
            pair<int, int> pre = make_pair(i, j);
            int ind = -1;
            for(int k=0; k<6; k++){
                if(nod->neighbours.at(k)==pre) ind = k;
            }
            auto p = nod->neighbours.at((ind+3)%6);
            remove_now(board.at(p.first).at(p.second), nod->index.first, nod->index.second, x+1);
        }
    }
    bool remove_end(int hexagon, int position){
        ending.first = hexagon;
        ending.second = position;
        for(int i=0; i<6; i++){
            auto p = board.at(beginning.first).at(beginning.second)->neighbours.at(i);
            if(remove_validity(board.at(p.first).at(p.second), beginning.first, beginning.second, 2)){
                board.at(beginning.first).at(beginning.second)->data = 0;
                remove_now(board.at(p.first).at(p.second), beginning.first, beginning.second, 2);
                return true;
            }
        }
        return false;
    }
    bool remove_ring(int hexagon, int position){
        if(board.at(hexagon).at(position)->data!=player+1) return false;
        board.at(hexagon).at(position) -> data = 0;
        if(player==0) rings_removed0++;
        else rings_removed1++;
        return true;
    }
    int execute_move(vector<string> moves){
        // vector<string> moves = split_string(s);
        if(moves.size()>3) return execute_sequence(moves);
        else player = (player+1)%2;;
        string mt = moves.at(0);
        int hexagon = stoi(moves.at(1));
        int position = stoi(moves.at(2));
        bool success = true;

        if(mt=="P") success = success && place_ring(hexagon, position);
        else if(mt=="S") success = success && select_ring(hexagon, position);
        else if(mt=="M") success = success && move_ring(hexagon, position);
        else if(mt=="RS") success = success && remove_start(hexagon, position);
        else if(mt=="RE") success = success && remove_end(hexagon, position);
        else if(mt=="X") success = success && remove_ring(hexagon, position);
        else return 0;

        bool valid = check_move_validity();
        bool won = check_won();

        if(!valid) return 0;
        if(won) return 2;
        return 1;

    }
//    vector<string> split_string(string s){
//        vector<string> moves;
//        string buff = "";
//        for(auto i:s){
//            if(i==" "){
//                moves.push_back(buff);
//                burff = " ";
//            }
//            else {
//                buff+=i;
//            }
//        }
//        if(buff!="") moves.push_back(buff);
//        return moves;
//    }
    bool execute_sequence(vector<string> moves){
        vector<string> temp(3);
        for (int i=0; i<moves.size(); i++){
            if(i%3==2){
                temp.at(2) = moves.at(i);
                int move_success = execute_move(temp);
                if(move_success == 0) return 0;
                else if(move_success==2) return 2;
            }
            else{
                temp.at(i%3) = moves.at(i);
            }
        }
        player = (player+1)%2;
        return 1;
    }
    bool check_won(){
        return (rings_removed1==l || rings_removed0==l);
    }
    int get_score(){
        return 0;
    }
    int calculate_score(){
        return 0;
    }
    int get_current_player(){
        return player;
    }
    int check_player_state(){
        return state;
    }
    bool check_move_validity() {
        return false;
    }
};

int main(){
    Game game = Game(0);
    int a = 0;
}