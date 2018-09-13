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
    int my_marker;
    pair<int, int> beginning;
    pair<int, int> ending;
public:
    Game(int player, int my_marker, int n=5, int rings=5, int max_row=5, int l=3){
        this-> n = n;
        this-> rings = rings;
        this-> max_row = max_row;
        this -> l = l;
        vector<Node*> tem;
        Node* no = new Node();
        no->index.first = 0;
        no->index.second = 0;
        this->update_neighbours(no, 0, 0);
        tem.push_back(no);
        this->board.push_back(tem);
        for(int i=1; i<=n; i++){
            vector<Node*> temp;
            for(int j=0; j<i*6; j++){
                Node* nod = new Node();
                nod->index.first = i;
                nod->index.second = j;
                this->update_neighbours(nod, i, j);
                temp.push_back(nod);
                if(i==n && j%i==0) nod->data = -1;
            }
            this->board.push_back(temp);
        }
        this->my_marker = my_marker;
        this->state = 0;
        this->rings_removed0 = 0;
        this->rings_removed1 = 0;
        this->player = player;
        this->rings_placed = 0;
        this->beginning.first = -1;
        this->beginning.second = -1;
//        print_neighbours();
    }
    void update_board(int hexagon, int position, int data){
        board.at(hexagon).at(position)->data = data;
    }
    void update_state(int state1){
        this->state = state1;
    }
    void update_rings(int rings0, int rings1, int rings_placed){
        this->rings_removed0 = rings0;
        this->rings_removed1 = rings1;
        this->rings_placed = rings_placed;
    }
    Game copy_board(){
        Game copied(player, my_marker, n, rings, max_row, l);
        copied.update_state(this->state);
        copied.update_rings(this->rings_removed0, this->rings_removed1, this->rings_placed);
        for(int i=0; i<board.size(); i++){
            for (int j=0; j<board.at(i).size(); j++){
                copied.update_board(i, j, board.at(i).at(j)->data);
            }
        }
        return copied;
    }
    void print_data(){
        for (auto u:board){
            for(auto v:u){
                cout << "( " << v->index.first << ", " << v->index.second << " ):  ";
                cout << v->data <<endl;
            }
            cout << endl;
        }
    }
    void print_neighbours(){
        for (auto u:board){
            for(auto v:u){
                cout << "( " << v->index.first << ", " << v->index.second << " ):  ";
                for (auto w:v->neighbours){
                    cout << "( " << w.first << ", " << w.second << " )  ";
                }
                cout << endl;
            }
            cout << endl;
        }
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
                nod->neighbours.at(5).second = j + (j / i) -1;
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
        else if(i==n) {
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
        else{
            nod->neighbours.at(0).first = i;
            nod->neighbours.at(0).second = (j + 1) % (6 * i);

            nod->neighbours.at(1).first = i+1;
            nod->neighbours.at(1).second = j+j/i;

            nod->neighbours.at(2).first = i+1;
            nod->neighbours.at(2).second = (j+j/i+1)%(6*(i+1));

            nod->neighbours.at(3).first = i;
            nod->neighbours.at(3).second = (j - 1) % (6 * i);

            nod->neighbours.at(4).first = i-1;
            nod->neighbours.at(4).second = (j - j/i)%(6*(i-1));

            nod->neighbours.at(5).first = i-1;
            nod->neighbours.at(5).second = (j - j/i - 1)%(6*(i-1));
        }
    }

    vector<pair<int, int>> possible_paths(int i, int j)
    {
        Node* curr = board.at(i).at(j);
        vector<pair<int, int>> out;
        for(auto w:curr->neighbours)
        {
            int r = w.first;
            int c = w.second;
            if (r == -1 || c == -1)
                continue;

            int data = board.at(r).at(c)->data;

            if(data == -1 || data == 1 || data == 2);
            else
            {
                if(data == 0)
                {
                    pair<int, int> temp = make_pair(r, c);
                    out.push_back(temp);
//                    cout << "( " << r << ", " << c << " )  ";
                }
                check_line(r,c,i,j, out);
            }

        }
        return out;
    }

    void check_line(int i, int j, int prev_i, int prev_j, vector<pair<int, int>> &out)
    {
        int prev_neighbour = 0;
        if(i==-1 || j==-1)
            return;

        Node* curr = board.at(i).at(j);
        for(auto w:curr->neighbours)
        {
            int r = w.first;
            int c = w.second;
            if(r == prev_i && c == prev_j)
                break;
            prev_neighbour++;
        }

        int nxt_neighbour = (prev_neighbour+3)%6;
        int r_nxt = curr->neighbours.at(nxt_neighbour).first;
        int c_nxt = curr->neighbours.at(nxt_neighbour).second;

        if(r_nxt==-1 || c_nxt == -1)
            return;

        int data = board.at(r_nxt).at(c_nxt)->data;

        if(data == -1 || data == 1 || data == 2);

        else
        {
            if(data == 0)
            {
                pair<int, int> temp = make_pair(r_nxt, c_nxt);
//                out.at(index) = temp;
//                out.at(index).first = r_nxt;
//                out.at(index).second = c_nxt;
//                index++;
                out.push_back(temp);
//                cout << "( " << r_nxt << ", " << c_nxt << " )  ";
                if (curr->data == 3 || curr->data == 4)
                    return;
            }
            check_line(r_nxt,c_nxt,i,j, out);
        }
    }

    bool place_ring(int hexagon, int position){
        if(state!=0) return false;
        if(board.at(hexagon).at(position)->data!=0) return false;
        if(state!=0) return false;
        board.at(hexagon).at(position) -> data = player+1;
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
            if(nod->neighbours.at(k)==pre){
                ind = k;
                break;
            }
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
            if(nod->neighbours.at(k)==pre){
                ind = k;
                break;
            }
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
    bool place_it(Node* nod, int i, int j, vector<pair<int, int>> &changed){
        if(nod->index == ending && nod->data==0){
            nod->data = player+1;
            return true;
        }
        else if(nod->index == ending) return false;
        else if(nod->data>2){
            if(nod->data==3) nod->data = 4;
            else if(nod->data==4) nod->data = 3;
            changed.push_back(nod->index);
        }
        int ind = -1;
        pair<int, int> pre = make_pair(i, j);
        for(int k=0; k<6; k++){
            if(nod->neighbours.at(k)==pre){
                ind = k;
                break;
            }
        }
        if(ind==-1) return false;
        ind = (ind+3)%6;
        pair<int, int> p = nod->neighbours.at(ind);
        return place_it(board.at(p.first).at(p.second), nod->index.first, nod->index.second, changed);
//        if(change){
//            if(nod->data==3) nod->data = 4;
//            else if(nod->data==4) nod->data = 3;
//            else return false;
//            auto p = nod->neighbours.at(ind);
//            return place_it(board.at(p.first).at(p.second), nod->index.first, nod->index.second, change);
//        }
//        else if(nod->data>2){
//            if(nod->data==3) nod->data = 4;
//            else if(nod->data==4) nod->data = 3;
//            else return false;
//            auto p = nod->neighbours.at(ind);
//            return place_it(board.at(p.first).at(p.second), nod->index.first, nod->index.second, true);
//        }
//        else if(nod->data==0){
//            auto p = nod->neighbours.at(ind);
//            return place_it(board.at(p.first).at(p.second), nod->index.first, nod->index.second, false);
//        }
//        else return false;
    }
    bool move_ring(int hexagon, int position, vector<pair<int, int>> changed){
//        if(board.at(hexagon).at(position)->data!=0){
//            board.at(beginning.first).at(beginning.second)->data-=2;
//            return false;
//        }
        ending.first = hexagon;
        ending.second = position;
//        bool valid = false;
//        vector<pair<int, int>> changed;
        pair<int, int> temp = make_pair(beginning.first, beginning.second);
        changed.push_back(temp);
        for(int i=0; i<6; i++){
            auto p = board.at(beginning.first).at(beginning.second)->neighbours.at(i);
            if(p.first==-1 || p.second==-1) continue;
            if(move_validity(board.at(p.first).at(p.second), beginning.first, beginning.second)){
//                valid = move_possible(board.at(p.first).at(p.second), beginning.first, beginning.second, false);
                place_it(board.at(p.first).at(p.second), beginning.first, beginning.second, changed);
                break;
            }
        }
        return true;
//        if (valid) return true;
//        board.at(beginning.first).at(beginning.second)->data-=2;
//        return false;
    }
    bool remove_start(int hexagon, int position){
        if(board.at(hexagon).at(position)->data!=player+3) return false;
        beginning.first = hexagon;
        beginning.second = position;
        return true;
    }
    bool remove_validity(Node* nod, int i, int j, int x){
        if(nod->data!=player+3) return false;
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
            nod->data = 0;
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
    int execute_move1(vector<string> moves, vector<pair<int, int>> changed){
        // vector<string> moves = split_string(s);
//        if(moves.size()>3) return execute_sequence(moves);
//        else player = (player+1)%2;;
        string mt = moves.at(0);
        int hexagon = stoi(moves.at(1));
        int position = stoi(moves.at(2));
        bool success = true;

        if(mt=="P") success = success && place_ring(hexagon, position);
        else if(mt=="S") success = success && select_ring(hexagon, position);
        else if(mt=="M") {
            success = success && move_ring(hexagon, position, changed);
        }
        else if(mt=="RS"){
            success = success && remove_start(hexagon, position);
        }
        else if(mt=="RE") success = success && remove_end(hexagon, position);
        else if(mt=="X") success = success && remove_ring(hexagon, position);
        else return 0;

        bool won = check_won();

        if(!success) return 0;
        if(won) return 2;
        return 1;

    }
    vector<pair<int, int>> execute_move(string s){
        vector<string> moves;
        string buff = "";
        for(auto i:s){
            if(i == ' '){
                moves.push_back(buff);
                buff = "";
            }
            else {
                buff+=i;
            }
        }
        if(!buff.empty()) moves.push_back(buff);
        return (execute_sequence(moves));
    }
    vector<pair<int, int>> execute_sequence(vector<string> moves){
        vector<string> temp(3);
        vector<pair<int, int>> changed;
        for (int i=0; i<moves.size(); i++){
            if(i%3==2){
                temp.at(2) = moves.at(i);
                int move_success = execute_move1(temp, changed);
//                if(move_success == 0) return 0;
//                else if(move_success==2) return 2;
            }
            else{
                temp.at(i%3) = moves.at(i);
            }
        }
        player = (player+1)%2;
        return changed;
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
        return true;
    }
    int heuristic(){
        int out = 0;
        for(vector<Node*> u: board){
            for(Node* v : u){
                if(v->data == my_marker) out++;
                else if(v->data == my_marker-2) out-=6;
                else if(v->data<3 && v->data>0) out+=6;
                else if(v->data>2) out--;
            }
        }
        return out;
    }
};

int main(){
    Game game = Game(0, 3);
//    game.execute_move("P 0 0");
//    game.execute_move("P 5 26");
//    game.execute_move("P 4 13");
//    game.execute_move("P 5 19");
//    game.execute_move("P 3 1");
//    game.execute_move("P 1 2");
//    game.execute_move("P 4 5");
//    game.execute_move("P 5 21");
//    game.execute_move("P 3 5");
//    game.execute_move("P 1 4");
//    game.execute_move("S 3 5 M 2 5");
//    game.execute_move("S 5 21 M 3 13");
//    game.execute_move("S 2 5 M 2 4");
//    game.execute_move("S 5 19 M 4 15");
//    game.execute_move("S 0 0 M 2 2");
//    game.execute_move("S 4 15 M 4 14");
//    game.print_data();
////    game.execute_move("S 3 13 M 2 9");
//    game.print_data();
////    game.execute_move("S 2 2 M 4 22");
////    game.execute_move("S 4 14 M 5 17");
//    game.execute_move("S 4 5 M 4 4");
//    game.execute_move("S 5 26 M 1 1");
//    game.execute_move("S 2 3 M 3 2");
//    game.execute_move("S 1 4 M 2 7");
//    game.execute_move("S 3 1 M 3 14");
//    game.execute_move("S 2 9 M 1 5");
//    game.execute_move("S 4 13 M 3 8");
//    game.execute_move("S 1 1 M 4 21");
//    game.execute_move("S 5 17 M 3 9");
//    game.execute_move("S 3 8 M 5 8");
//    game.execute_move("S 4 21 M 1 0");
//    game.execute_move("S 4 22 M 2 8");
//    game.execute_move("S 1 5 M 4 17");
//    game.execute_move("S 2 8 M 3 12");
//    game.execute_move("S 1 2 M 2 1");
//    game.print_data();
////    game.execute_move("S 3 2 M 3 7 RS 2 8 RE 2 2 X 3 7");
////    game.execute_move("S 2 1 M 3 4");
////    game.execute_move("S 1 3 M 0 0");
////    game.execute_move("S 1 0 M 1 1");
////    game.execute_move("S 3 12 M 1 4");
////    game.execute_move("S 3 4 M 2 2");
////    game.execute_move("S 5 8 M 5 9");
////    game.execute_move("S 1 1 M 4 11");
////    game.execute_move("S 0 0 M 2 6");
////    game.execute_move("S 4 17 M 4 16");
////    game.execute_move("S 2 6 M 2 0");
////    game.execute_move("S 2 7 M 3 11");
////    game.execute_move("S 2 0 M 2 11");
////    game.execute_move("S 3 11 M 2 10");
////    game.execute_move("S 2 11 M 3 16");
////    game.execute_move("S 2 2 M 3 7");
////    game.execute_move("S 4 4 M 5 6");
////    game.execute_move("S 3 16 M 3 17");
//    game.execute_move("S 2 8 M 2 7");
//    game.execute_move("S 1 4 M 1 3");
//    game.print_data();
//    game.execute_move("S 2 7 M 1 5 RS 2 8 RE 2 2 X 4 4");
    game.execute_move("P 0 0");
    game.execute_move("P 1 0");
    game.execute_move("P 1 1");
    game.execute_move("P 1 2");
    game.execute_move("P 1 3");
    game.execute_move("P 1 4");
    game.execute_move("P 1 5");
    game.execute_move("P 2 0");
    game.execute_move("P 2 1");
    game.execute_move("P 2 2");
    game.execute_move("S 1 5 M 2 11");
    game.execute_move("S 1 0 M 2 9");
    game.execute_move("S 2 1 M 3 1");
    game.execute_move("S 2 0 M 3 0");
    game.execute_move("S 2 11 M 3 16");
    game.execute_move("S 2 2 M 3 17");
    game.execute_move("S 3 1 M 5 3");
    game.execute_move("S 3 0 M 3 2");
    game.execute_move("S 5 3 M 2 10");
    game.execute_move("S 3 2 M 4 23");
    game.execute_move("S 2 10 M 4 2");
//    game.print_data();
    game.execute_move("S 2 9 M 5 24 RS 2 9 RE 3 2 X 4 23");
//    game.print_data();
    game.execute_move("S 1 1 M 4 1");
    game.execute_move("S 1 4 M 3 14");
//    game.execute_move("S 4 1 M 4 1");
    game.execute_move("S 4 1 M 5 1");
    Game copy = game.copy_board();

//
//    game.possible_paths(5, 1);
//    cout << endl;
//    vector<pair<int, int>> out = game.possible_paths(4, 2);
//    for (auto u:out){
//        cout << u.first << " " << u.second << endl;
//    }
//    cout << endl;
//    game.possible_paths(3, 17);
//    cout << endl;
//    game.possible_paths(3, 16);
//    cout << endl;
//    game.possible_paths(5, 24);
//    cout << endl;
//    game.possible_paths(3, 14);
//    cout << endl;
//    game.possible_paths(0, 0);
//    cout << endl;
//    game.possible_paths(1, 2);
//    cout << endl;
//    game.possible_paths(1, 3);
//    cout << endl;
//    cout << endl;

//    game.print_data();
//    int a = 0;
    game.print_data();
    copy.print_data();
    cout << game.heuristic() << endl;
    cout << copy.heuristic() << endl;
    int a = 0;

}