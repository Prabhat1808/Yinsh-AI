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


    // int removal_len(Node_game* nod, int i, int j, int x){

    vector<pair<int, int>> possible_paths(int i, int j)
    {
        Node_game* curr = board.at(i).at(j);
        vector<pair<int, int>> possible;
        vector<pair<int,int>> axis_map = util->board.at(i).at(j)->axis_mapping;


        //Diagonal1
        int cons = 0;
        for(int itr = axis_map.at(0).second - 1;itr >= 0 ; itr--)
        {
            pair<int,int> element = util->elems_on_diagonal1.at(axis_map.at(0).first).at(itr);
            int data = board.at(element.first).at(element.second)->data;
            if(data == 1 || data == 2)
                break;
            if(data == 3 || data == 4)
            {
                cons++;
                continue;
            }
            if(data == 0)
            {
                if(cons > 0)
                {
                    possible.push_back(element);
                    break;
                }
                else
                    possible.push_back(element);
            }
        }
        //Diagonal2
        cons = 0;
        for(int itr = axis_map.at(1).second - 1;itr >= 0 ; itr--)
        {
            pair<int,int> element = util->elems_on_diagonal2.at(axis_map.at(1).first).at(itr);
            int data = board.at(element.first).at(element.second)->data;
            if(data == 1 || data == 2)
                break;
            if(data == 3 || data == 4)
            {
                cons++;
                continue;
            }
            if(data == 0)
            {
                if(cons > 0)
                {
                    possible.push_back(element);
                    cons = 0;
                    break;
                }
                else
                    possible.push_back(element);
            }
        }
        // //Vertical
        cons = 0;
        for(int itr = axis_map.at(2).second - 1;itr >= 0 ; itr--)
        {
            pair<int,int> element = util->elems_on_vertical.at(axis_map.at(2).first).at(itr);
            int data = board.at(element.first).at(element.second)->data;
            if(data == 1 || data == 2)
                break;
            if(data == 3 || data == 4)
            {
                cons++;
                continue;
            }
            if(data == 0)
            {
                if(cons > 0)
                {
                    possible.push_back(element);
                    cons = 0;
                    break;
                }
                else
                    possible.push_back(element);
            }
        }

        // //Diagonal1
        cons = 0;
        for(int itr = axis_map.at(0).second + 1;itr < util->elems_on_diagonal1.at(axis_map.at(0).first).size() ; itr++)
        {
            pair<int,int> element = util->elems_on_diagonal1.at(axis_map.at(0).first).at(itr);
            int data = board.at(element.first).at(element.second)->data;
            if(data == 1 || data == 2)
                break;
            if(data == 3 || data == 4)
            {
                cons++;
                continue;
            }
            if(data == 0)
            {
                if(cons > 0)
                {
                    possible.push_back(element);
                    break;
                }
                else
                    possible.push_back(element);
            }
        }

        // //Diagonal2
        cons = 0;
        for(int itr = axis_map.at(1).second + 1;itr < util->elems_on_diagonal2.at(axis_map.at(1).first).size() ; itr++)
        {
            pair<int,int> element = util->elems_on_diagonal2.at(axis_map.at(1).first).at(itr);
            int data = board.at(element.first).at(element.second)->data;
            if(data == 1 || data == 2)
                break;
            if(data == 3 || data == 4)
            {
                cons++;
                continue;
            }
            if(data == 0)
            {
                if(cons > 0)
                {
                    possible.push_back(element);
                    break;
                }
                else
                    possible.push_back(element);
            }
        }

        //Vertical
        cons = 0;
        for(int itr = axis_map.at(2).second + 1;itr < util->elems_on_vertical.at(axis_map.at(2).first).size() ; itr++)
        {
            pair<int,int> element = util->elems_on_vertical.at(axis_map.at(2).first).at(itr);
            int data = board.at(element.first).at(element.second)->data;
            if(data == 1 || data == 2)
                break;
            if(data == 3 || data == 4)
            {
                cons++;
                continue;
            }
            if(data == 0)
            {
                if(cons > 0)
                {
                    possible.push_back(element);
                    break;
                }
                else
                    possible.push_back(element);
            }
        }

        return possible;
    }

    vector<pair<pair<int,int>,pair<int,int>>> check5(vector<pair<int, int>> changed, int player_marker)
    {
        vector<pair<pair<int,int>,pair<int,int>>> sequences;
        unordered_set<string> axes_checked;
        for(auto position:changed)
        {
            vector<pair<int,int>> axis_map = util->board.at(position.first).at(position.second)->axis_mapping;
            int d1ax = axis_map.at(0).first;
            int d2ax = axis_map.at(1).first;
            int vax = axis_map.at(2).first;

            pair<int,int> st;
            pair<int,int> en;
            int count = 0;
            bool obtained = false;

            //Diagonal1
            for(auto w: util->elems_on_diagonal1.at(axis_map.at(0).first))
            {
                if(axes_checked.count("D1"+to_string(d1ax)) != 0)
                    break;

                int data = board.at(w.first).at(w.second)->data;
                if(data == player_marker)
                {
                    if(count == 0)
                        st = w;
                    count++;
                    en = w;
                }
                else
                {
                    if(count >= 5)
                    {
                        obtained = true;
                    }
                    count = 0;
                }
                if(obtained)
                {
                    sequences.push_back(make_pair(st,en));
                    obtained = false;
                }
            }
            if(count >= 5)
                sequences.push_back(make_pair(st,en));
            axes_checked.insert("D1"+to_string(d1ax));
            //Diagonal1

            count = 0;
            obtained = false;
            //Diagonal2
            for(auto w: util->elems_on_diagonal2.at(axis_map.at(1).first))
            {
                if(axes_checked.count("D2"+to_string(d2ax)) != 0)
                    break;

                int data = board.at(w.first).at(w.second)->data;
                if(data == player_marker)
                {
                    if(count == 0)
                        st = w;
                    en = w;
                    count++;
                }
                else
                {
                    if(count >= 5)
                    {
                        obtained = true;
                    }
                    count = 0;
                }
                if(obtained)
                {
                    sequences.push_back(make_pair(st,en));
                    obtained = false;
                }
            }
            if(count >= 5)
                sequences.push_back(make_pair(st,en));
            axes_checked.insert("D2"+to_string(d2ax));
            //Diagonal2

            count = 0;
            obtained = false;
            //Vertical
            for(auto w: util->elems_on_vertical.at(axis_map.at(2).first))
            {
                if(axes_checked.count("V"+to_string(vax)) != 0)
                    break;

                int data = board.at(w.first).at(w.second)->data;
                if(data == player_marker)
                {
                    if(count == 0)
                        st = w;
                    en = w;
                    count++;
                }
                else
                {
                    if(count >= 5)
                    {
                        obtained = true;
                    }
                    count = 0;
                }
                if(obtained)
                {
                    sequences.push_back(make_pair(st,en));
                    obtained = false;
                }
            }
            if(count >= 5)
                sequences.push_back(make_pair(st,en));
            axes_checked.insert("V"+to_string(vax));
            //Vertical
        }
        return sequences;
    }

    void find_consecutives(vector<pair<pair<int,int>,pair<int,int>>> directions)
    {
        int curr3 =0, curr4 =0;
        for(auto axis: directions)
        {
            for(auto w: axis)
            {
                int data = board.at(w.first).at(w.second)->data;
                if(data == 3)
                {
                    curr3++;
                }
            }
        }
    }

    bool place_ring(int hexagon, int position){
        if(board.at(hexagon).at(position)->data!=0) return false;
        board.at(hexagon).at(position)->data = player+1;
        if(player==my_marker-3) {
            ring_self.at(rings_placed / 2).first = hexagon;
            ring_self.at(rings_placed/2).second = position;
        }
        else{
            ring_opponent.at(rings_placed/2).first = hexagon;
            ring_opponent.at(rings_placed/2).second = position;
        }
        rings_placed++;
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

    bool move_ring(int hexagon, int position, vector<pair<int, int>> &changed){
        ending.first = hexagon;
        ending.second = position;
        vector<pair<int, int>> pointstochange = util->between_points(beginning, ending);
        for (int i=1 ;i<pointstochange.size()-1; i++){
            pair<int,int> p = pointstochange.at(i);
            if(board.at(p.first).at(p.second)->data==3){
                board.at(p.first).at(p.second)->data = 4;
                changed.push_back(p);
            }
            else if(board.at(p.first).at(p.second)->data == 4){
                board.at(p.first).at(p.second)->data = 3;
                changed.push_back(p);
            }
        }
        board.at(hexagon).at(position)->data = ring_selected;
        for(int i=0; i<ring_self.size(); i++){
            if(my_marker-3==player && beginning==ring_self.at(i)) {
                ring_self.at(i).first = ending.first;
                ring_self.at(i).second = ending.second;
                break;
            }
            else if(my_marker-3!=player && beginning==ring_opponent.at(i)){
                ring_opponent.at(i).first = ending.first;
                ring_opponent.at(i).second = ending.second;
                break;
            }
        }
        return true;
    }

    bool remove_start(int hexagon, int position){
        if(board.at(hexagon).at(position)->data!=player+3) return false;
        beginning.first = hexagon;
        beginning.second = position;
        return true;
    }

    bool remove_end(int hexagon, int position){
        ending.first = hexagon;
        ending.second = position;
        vector<pair<int, int>> pointstochange = util->between_points(beginning, ending);
        for (pair<int, int> u: pointstochange){
            board.at(u.first).at(u.second)->data = 0;
        }
        return true;
    }

    bool remove_ring(int hexagon, int position){
        if(board.at(hexagon).at(position)->data!=player+1) return false;
        board.at(hexagon).at(position) -> data = 0;
        if(player==0) rings_removed0++;
        else rings_removed1++;
        pair<int, int> temp = make_pair(hexagon, position);
        for(int i=0; i<ring_self.size(); i++){
            if(my_marker-3==player && temp==ring_self.at(i)) {
                ring_self.at(i).first = -1;
                ring_self.at(i).second = -1;
                break;
            }
            else if(my_marker-3!=player && temp==ring_opponent.at(i)){
                ring_opponent.at(i).first = -1;
                ring_opponent.at(i).second = -1;
                break;
            }
        }
        return true;
    }

    int execute_move1(vector<string> moves, vector<pair<int, int>> &changed){
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



    int get_position(int p, int h){
//        if(p<0) p+=(6*h);
        return board.at(h).at(p)->data;
    }

    int heuristic(){
        int out = 0;
        for(vector<Node_game*> u: board){
            for(Node_game* v : u){
                if(v->data == my_marker) out++;
                else if(v->data == my_marker-2) out-=6;
                else if(v->data<3 && v->data>0) out+=6;
                else if(v->data>2) out--;
            }
        }
        return out;
    }


    void print_board(){

        cout << "   " << "     " << " " << "     " << " " << " " << endl;
        cout << " " << "     " << " " << "     " << get_position(29,5) << "     " << get_position(1,5) << endl;
        cout << "   " << "     " << " " << get_position(28,5) << "     " << get_position(0,4) << "     " << get_position(2,5) << endl;
        cout << " " << "     " << get_position(27,5) << "     " << get_position(23,4) << "     " << get_position(1,4) << "     " << get_position(3,5) << endl;
        cout << "   " << get_position(26,5) << "     " << get_position(22,4) << "     " << get_position(0,3) << "     " << get_position(2,4) << "     " << get_position(4,5) << endl;
        cout << " " << "     " << get_position(21,4) << "     " << get_position(17,3) << "     " << get_position(1,3) << "     " << get_position(3,4) << "     " << " " << endl;
        cout << "   " << get_position(20,4) << "     " << get_position(16,3) << "     " << get_position(0,2) << "     " << get_position(2,3) << "     " << get_position(4,4) << endl;
        cout << get_position(24,5) << "     " << get_position(15,3) << "     " << get_position(11,2) << "     " << get_position(1,2) << "     " << get_position(3,3) << "     " << get_position(6,5) << endl;
        cout << "   " << get_position(19,4) << "     " << get_position(10,2) << "     " << get_position(0,1) << "     " << get_position(2,2) << "     " << get_position(5,4) << endl;
        cout << get_position(23,5) << "     " << get_position(14,3) << "     " << get_position(5,1) << "     " << get_position(1,1) << "     " << get_position(4,3) << "     " << get_position(7,5) << endl;
        cout << "   " << get_position(18,4) << "     " << get_position(9,2) << "     " << get_position(0,0) << "     " << get_position(3,2) << "     " << get_position(6,4) << endl;
        cout << get_position(22,5) << "     " << get_position(13,3) << "     " << get_position(4,1) << "     " << get_position(2,1) << "     " << get_position(5,3) << "     " << get_position(8,5) << endl;
        cout << "   " << get_position(17,4) << "     " << get_position(8,2) << "     " << get_position(3,1) << "     " << get_position(4,2) << "     " << get_position(7,4) << endl;
        cout << get_position(21,5) << "     " << get_position(12,3) << "     " << get_position(7,2) << "     " << get_position(5,2) << "     " << get_position(6,3) << "     " << get_position(9,5) << endl;
        cout << "   " << get_position(16,4) << "     " << get_position(11,3) << "     " << get_position(6,2) << "     " << get_position(7,3) << "     " << get_position(8,4) << endl;
        cout << " " << "     " << get_position(15,4) << "     " << get_position(10,3) << "     " << get_position(8,3) << "     " << get_position(9,4) << "     " << " " << endl;
        cout << "   " << get_position(19,5) << "     " << get_position(14,4) << "     " << get_position(9,3) << "     " << get_position(10,4) << "     " << get_position(11,5) << endl;
        cout << " " << "     " << get_position(18,5) << "     " << get_position(13,4) << "     " << get_position(11,4) << "     " << get_position(12,5) << endl;
        cout << "   " << " " << "     " << get_position(17,5) << "     " << get_position(12,4) <<  "     " << get_position(13,5) <<endl;
        cout << " " << "     " << " " << "     " << get_position(16,5) << "     " << get_position(14,5) << endl;
        cout << "   " << "     " << " " << "     " << " " << " " << endl;
    }

};

// int main(){
//     Utility* util = new Utility(5);
//     Game* game = new Game(0, 4, util);
//     // game->place_ring(0, 0);
//     // game->place_ring(4, 2);
//     // vector<pair<int, int>> changed;
//     // game->select_ring(4, 2);
//     // game->move_ring(5, 6, changed);
//     // game->print_board();
//     // for(auto u: changed){
//     //     cout << u.first << " " << u.second << endl;
//     // }
//     // game->select_ring(5, 6);
//     // game->move_ring(4, 1, changed);
//     // game->print_board();
//     // for(auto u: changed){
//     //     cout << u.first << " " << u.second << endl;
//     // }
//         //TEST MOVES 1
//     game->execute_move("P 0 0");
// //     game->execute_move("P 1 0");
//     game->execute_move("P 1 1");
//     game->execute_move("P 4 0");
//     game->execute_move("P 2 2");
//     game->execute_move("P 5 1");
//     game->execute_move("P 1 4");
//     game->execute_move("P 5 29");
//     game->execute_move("P 2 8");
//     game->execute_move("P 3 0");
//     game->execute_move("P 2 10");
//     game->execute_move("S 0 0 M 1 2");
//     game->execute_move("S 1 1 M 2 3");
//     game->execute_move("S 1 2 M 2 4");
//     game->execute_move("S 2 2 M 3 4");
//     game->execute_move("S 2 4 M 3 5");
//     game->execute_move("S 1 4 M 1 3");
//     game->execute_move("S 3 5 M 4 6");
//     game->execute_move("S 2 8 M 2 7");
//     game->execute_move("S 4 0 M 4 1");
//     game->execute_move("S 2 10 M 1 5");
//     game->print_board();
//     game->execute_move("S 3 0 M 3 1");
//     game->print_board();
//     vector<pair<int, int>> changed = game->execute_move("S 1 5 M 3 6");
//     for(auto w: changed)
//         cout << "(" << w.first << "," << w.second << ")" << endl;
//     // game->execute_move("S 1 5 M 3 6");
//     vector<pair<pair<int,int>,pair<int,int>>> consecutive = game->check5(changed,4);
//     for(auto w:consecutive)
//         cout << "(" << w.first.first << "," << w.first.second << ") -> " << "(" << w.second.first << "," << w.second.second << ")" << endl;
//     game->print_board();
//     // game->execute_move("S 2 9 M 5 24 RS 2 9 RE 3 2 X 4 23");
//     // game->print_board();
//     // game->print_data();
// }