#include <bits/stdc++.h>
#include "utilities.cpp"
using namespace std;

class Node_game{
public:
    int data;
    pair<int, int> index;
    Node_game(int index1, int index2){
        this->data = 0;
        this->index.first = index1;
        this->index.second = index2;

        for(int i=0 ;i<6; i++){
            pair<int, int> temp;
            temp.first = -1;
            temp.second = -1;
        }
    }
    ~Node_game(){
        // cout << "Deleting Node." << endl;
    }
    void update_data(int data) {
        this->data = data;
    }
    int get_data() {
        return data;
    }
};

class Game{
public:
    vector<vector<Node_game>> board;
    int player;
    int rings_removed0;
    int rings_removed1;
    int l;
    int max_row;
    int n;
    Utility* util;
    vector<pair<int, int>> ring_self;
    vector<pair<int, int>> ring_opponent;
    vector<pair<int, int>> rings_opponent1;
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
        vector<Node_game> tem;
        Node_game no = Node_game(0, 0);
        tem.push_back(no);
        this->board.push_back(tem);
        for(int i=1; i<=n; i++){
            vector<Node_game> temp;
            for(int j=0; j<i*6; j++){
                Node_game nod = Node_game(i, j);
                if(i==n && j%i==0) nod.update_data(-1);
                temp.push_back(nod);
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
//    ~Game(){
//        for (int i=1; i<=n; i++){
//            for(int j=0; j<i*6; j++){
//                delete board.at(i).at(j);
//            }
//            board.at(i).clear();
//        }
//        board.clear();
//        // delete board;
//    }
    Utility* get_util(){
        return util;
    }
    int get_removed0(){
        return rings_removed0;
    }
    int get_removed1(){
        return rings_removed1;
    }
    int get_data(int hexagon, int position){
        return board.at(hexagon).at(position).get_data();
    }
    void update_board(int hexagon, int position, int data){
        board.at(hexagon).at(position).update_data(data);
    }
    int get_n(){
        return n;
    }
    void update_rings(int rings0, int rings1, int rings_placed){
        this->rings_removed0 = rings0;
        this->rings_removed1 = rings1;
        this->rings_placed = rings_placed;
    }
    void update_player(int p){
        player = p;
    }
    void update_ring_opponent(int i, int x, int y){
        ring_opponent.at(i).first = x;
        ring_opponent.at(i).second = y;
    }
    void update_ring_self(int i, int x, int y){
        ring_self.at(i).first = x;
        ring_self.at(i).second = y;
    }
    int get_player(){
        return player;
    }
    int get_marker(){
        return my_marker;
    }
    vector<pair<int, int>> self_rings(){
        return ring_self;
    }
    vector<pair<int, int>> opponent_rings(){
        return ring_opponent;
    }
    Game copy_board(){
        Game copied = Game(player, my_marker, util, n, rings, max_row, l);
        copied.update_rings(this->rings_removed0, this->rings_removed1, this->rings_placed);
        for (int i=0; i<rings; i++){
            copied.update_ring_self(i, ring_self.at(i).first, ring_self.at(i).second);
            copied.update_ring_opponent(i, ring_opponent.at(i).first, ring_opponent.at(i).second);
        }
        for(int i=0; i<board.size(); i++){
            for (int j=0; j<board.at(i).size(); j++){
                copied.update_board(i, j, board.at(i).at(j).get_data());
            }
        }
        return copied;
    }

//#########################PLACEMENT###################################
    pair<int,int> moves_along_axis(vector<pair<int,int>> axis, int index)
    {
        int fwd = 0, bcd = 0;
        for(int i =index-1;i>=0;i--)
        {
            pair<int,int> ele = axis.at(i);
            int dat = board.at(ele.first).at(ele.second).get_data();
            if(dat == 1 || dat == 2)
                break;
            bcd++;
        }
        for(int i =index+1;i<axis.size();i++)
        {
            pair<int,int> ele = axis.at(i);
            int dat = board.at(ele.first).at(ele.second).get_data();
            if(dat == 1 || dat == 2)
                break;
            fwd++;
        }
        return make_pair(fwd,bcd); // first has forward, 2nd has backward
    }

    vector<pair<int,int>> total_moves(pair<int,int> point)
    {
        vector<pair<int,int>> outputs;
        vector<pair<int,int>> axis_map = util->board.at(point.first).at(point.second)->axis_mapping;
        outputs.push_back(moves_along_axis(util->elems_on_diagonal1.at(axis_map.at(0).first) , axis_map.at(0).second));
        outputs.push_back(moves_along_axis(util->elems_on_diagonal2.at(axis_map.at(1).first) , axis_map.at(1).second));
        outputs.push_back(moves_along_axis(util->elems_on_vertical.at(axis_map.at(2).first) , axis_map.at(2).second));
        return outputs;
    }

    pair<int,int> place_ring_heuristic()
    {
        vector<pair<int,int>> rings = rings_opponent1;
        if(rings.size()==0){

            // srand(time(NULL));
            // int h = rand() % n;
            // int p;
            // while(true){
            //     p = rand() % (h*6);
            //     if(h != 5)
            //         break;
            //     else
            //     {
            //         if(p % 5 != 0)
            //             break;
            //     }
            // }

            // return make_pair(h, p);
            return make_pair(0,0);
        }
        int r = 0, dirn = 0, max = 0;
        int ring = 0;
        int rn = 0;
        for(auto point:rings)
        {
            // cout << "Checking ring : " << point.first << " , " << point.second << endl;
            vector<pair<int,int>> ndr = total_moves(point);
            // for(auto w: ndr)
            // {
            //     cout << w.first << " , " << w.second << endl;
            // }
            // int r = 0, dirn = 0, max = 0; // 1 means forward, -1 means backward
            for(int i = 0;i< 3; i++)
            {
                pair<int,int> tmp = ndr.at(i);
                if (tmp.first > max)
                {
                    // cout << "Max : -> " << max << " and Current -> " << tmp.first << endl;
                    max = tmp.first;
                    r = i;
                    dirn = 1;
                    ring = rn;
                }
                if(tmp.second > max)
                {
                    // cout << "Max : -> " << max << " and Current -> " << tmp.second << endl;
                    max = tmp.second;
                    r = i;
                    dirn = -1;
                    ring = rn;
                }
            }
            rn++;
        }

        pair<int,int> point = rings.at(ring);
        vector<pair<int,int>> axis_map = util->board.at(point.first).at(point.second)->axis_mapping;
        pair<int,int> index;
        if(r == 0)
            index = util->elems_on_diagonal1.at(axis_map.at(0).first).at(axis_map.at(0).second + dirn);
        if(r == 1)
            index = util->elems_on_diagonal2.at(axis_map.at(1).first).at(axis_map.at(1).second + dirn);
        if(r==2)
            index = util->elems_on_vertical.at(axis_map.at(2).first).at(axis_map.at(2).second + dirn);
        return index;
    }

//#########################PLACEMENT###################################
    // int removal_len(Node_game* nod, int i, int j, int x){

    vector<pair<int, int>> possible_paths(int i, int j)
    {
        Node_game curr = board.at(i).at(j);
        vector<pair<int, int>> possible;
        vector<pair<int,int>> axis_map = util->board.at(i).at(j)->axis_mapping;


        //Diagonal1
        int cons = 0;
        for(int itr = axis_map.at(0).second - 1;itr >= 0 ; itr--)
        {
            pair<int,int> element = util->elems_on_diagonal1.at(axis_map.at(0).first).at(itr);
            int data = board.at(element.first).at(element.second).get_data();
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
            int data = board.at(element.first).at(element.second).get_data();
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
            int data = board.at(element.first).at(element.second).get_data();
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
            int data = board.at(element.first).at(element.second).get_data();
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
            int data = board.at(element.first).at(element.second).get_data();
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
            int data = board.at(element.first).at(element.second).get_data();
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

                int data = board.at(w.first).at(w.second).get_data();
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

                int data = board.at(w.first).at(w.second).get_data();
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

                int data = board.at(w.first).at(w.second).get_data();
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

    pair<vector<int>,vector<int>> find_consecutives(vector<vector<pair<int,int>>> directions)
    {
        int curr3 =0, curr4 =0;
//        int buff3 = 0, buff4 = 0;
//        int prev3 = 0, prev4 = 0;
        vector<int> counts3(9), counts4(9);
        int cn =0;
        for(auto axis: directions)
        {
//            cout << "Entering AXIS : " << cn++ << endl;
            for(auto w: axis)
            {
                int data = board.at(w.first).at(w.second).get_data();
                if(data == 3)
                {
//                    if(buff3 == 1)
//                        curr3++;
//                    buff3 = 2;
//                    buff4 = max(0,buff4-1);
                    curr3++;
                    if(curr4 > 1)
                        counts4.at(curr4 - 2) += 1;
                    curr4 = 0;
                }
                else if(data == 4)
                {
//                    if(buff4 == 1)
//                        curr4++;
//                    buff4 = 2;
//                    buff3 = max(0,buff3-1);
                    curr4++;
                    if(curr3 > 1)
                        counts3.at(curr3 - 2) += 1;
                    curr3 = 0;
                }
                else
                {
//                    buff4 = max(0,buff4-1);
//                    buff3 = max(0,buff3-1);
                    if(curr4 > 1)
                        counts4.at(curr4 - 2) += 1;
                    if(curr3 > 1)
                        counts3.at(curr3 - 2) += 1;
                    curr3 =0;
                    curr4 =0;
                }
            }
//            if(buff3 == 1)
//                curr3 += 1;
//            if(buff4 == 1);
//            curr4 += 1;

            if(curr3 > 1)
                counts3.at(curr3 - 2) +=1;
            if(curr4 > 1)
                counts4.at(curr4 - 2) += 1;
//            buff3 = 0;
//            buff4 = 0;
            curr3 = 0;
            curr4 = 0;
//             cout << "AXIS " << cn++ << "\n";
//             for(int i =0;i<6;i++)
//            {
//                 cout << i << " --> " << "( marker 3: " << counts3.at(i) << ") and marker 4: " << counts4.at(i) << ")\n";
//                 counts3.at(i) = 0;
//                 counts4.at(i) = 0;
//            }
        }
        return make_pair(counts3,counts4);
    }

    pair<vector<pair<pair<int,int>,pair<int,int>>>,vector<pair<pair<int,int>,pair<int,int>>>> find_consecutive_locations(vector<vector<pair<int,int>>> directions)
    {
        int curr3 =0, curr4 =0;
        vector<pair<pair<int,int>,pair<int,int>>> cons3, cons4;
        pair<int,int> st3,en3,st4,en4;
        int cn =0;
        for(auto axis: directions)
        {
            // cout << "AXIS : " << cn++ << endl;
            for(auto w: axis)
            {
                
                 int data = board.at(w.first).at(w.second).get_data();
                 // cout << w.first << " , " << w.second << " --> " << data << endl;
                if(data == 3)
                {
                    if(curr3 == 0)
                        st3 = w;
                    en3 = w;
                    curr3++;
                    if(curr4 == 4)
                        cons4.push_back(make_pair(st4,en4));
                    curr4 = 0;
                }
                else if(data == 4)
                {
                    if(curr4 == 0)
                        st4 = w;
                    en4 = w;
                    curr4++;
                    if(curr3 == 4)
                        cons3.push_back(make_pair(st3,en3));
                    curr3 = 0;
                }
                else
                {
                    if(curr4 == 4)
                        cons4.push_back(make_pair(st4,en4));
                    if(curr3 == 4)
                    {
                        cons3.push_back(make_pair(st3,en3));
                        // cout << "updating vector 3" << endl;
                    }
                    curr3 =0;
                    curr4 =0;
                }
                // cout << curr3 << " and " << curr4 << endl;
            }

            if(curr4 == 4)
                cons4.push_back(make_pair(st4,en4));
            if(curr3 == 4)
                cons3.push_back(make_pair(st3,en3));
            curr3 = 0;
            curr4 = 0;
        }
        return make_pair(cons3,cons4);
    }

    bool danger_on_axis(vector<vector<pair<int,int>>> directions, pair<int,int> axis_locations, int opponent_marker, int my_marker)
    {
        int fwd = 0;
        int bcd = 0;
        vector<pair<int,int>> axis = directions.at(axis_locations.first);
        for(int i=axis_locations.second + 1;i < axis.size(); i++)
        {
            int dat = board.at(axis.at(i).first).at(axis.at(i).second).get_data();
            cout << axis.at(i).first << " , " << axis.at(i).second << " --> " << dat << endl;
            if(dat == 0)
            {
                fwd = -1;
                break;
            }
            // if(dat == 1 || dat == 2)
            if(dat == opponent_marker)
            {
                fwd = 1;
                break;
            }
            if(dat = my_marker)
                break;
        }

        for(int i=axis_locations.second - 1;i >= 0; i--)
        {
            int dat = board.at(axis.at(i).first).at(axis.at(i).second).get_data();
            cout << axis.at(i).first << " , " << axis.at(i).second << " --> " << dat << endl;
            if(dat == 0)
            {
                bcd = -1;
                break;
            }
            // if(dat == 1 || dat == 2)
            if(dat == opponent_marker)
            {
                bcd = 1;
                break;
            }
            if(dat = my_marker)
                break;
        }
        cout << "FWD -> " << fwd << "\tBCD -> " << bcd << endl;
        if( (fwd == 1 && bcd == -1) || (fwd == -1 && bcd == 1) )
            return true;
        return false;
    }

    bool point_in_danger(pair<int,int> point, int opponent_marker, int my_marker)
    {
        vector<pair<int,int>> axis_map = util->board.at(point.first).at(point.second)->axis_mapping;
        bool d1 = danger_on_axis(util->elems_on_diagonal1,axis_map.at(0),opponent_marker,my_marker);
        cout << "ALONG D1 : " << d1 <<endl;
        bool d2 = danger_on_axis(util->elems_on_diagonal2,axis_map.at(1),opponent_marker,my_marker);
        cout << "ALONG D2 : " << d2 <<endl;
        bool v = danger_on_axis(util->elems_on_vertical,axis_map.at(2),opponent_marker,my_marker);
        cout << "ALONG v : " << v <<endl;
        return (d1 || d2 || v);
    }

    bool streak_breakable(vector<pair<int,int>> points, int opponent_marker, int my_marker)
    {
        bool breakable = false;
        for(auto w: points)
            breakable =  breakable || point_in_danger(w, opponent_marker, my_marker);
        return breakable;
    }

    bool place_ring(int hexagon, int position){
        if(board.at(hexagon).at(position).get_data()!=0) return false;
        board.at(hexagon).at(position).update_data(player+1);
        if(player==my_marker-3) {
            ring_self.at(rings_placed / 2).first = hexagon;
            ring_self.at(rings_placed/2).second = position;
        }
        else{
            ring_opponent.at(rings_placed/2).first = hexagon;
            ring_opponent.at(rings_placed/2).second = position;
            rings_opponent1.push_back(make_pair(hexagon, position));
        }
        rings_placed++;
        return true;
    }

    bool select_ring(int hexagon, int position){
        if(board.at(hexagon).at(position).get_data()!=player+1) return false;
        ring_selected = board.at(hexagon).at(position).get_data();
        board.at(hexagon).at(position).update_data(board.at(hexagon).at(position).get_data()+2);
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
            if(board.at(p.first).at(p.second).get_data()==3){
                board.at(p.first).at(p.second).update_data(4);
                changed.push_back(p);
            }
            else if(board.at(p.first).at(p.second).get_data() == 4){
                board.at(p.first).at(p.second).update_data(3);
                changed.push_back(p);
            }
        }
        changed.push_back(beginning);
        board.at(hexagon).at(position).update_data(ring_selected);
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
        if(board.at(hexagon).at(position).get_data()!=player+3) return false;
        beginning.first = hexagon;
        beginning.second = position;
        return true;
    }

    bool remove_end(int hexagon, int position){
        ending.first = hexagon;
        ending.second = position;
        vector<pair<int, int>> pointstochange = util->between_points(beginning, ending);
        for (pair<int, int> u: pointstochange){
            board.at(u.first).at(u.second).update_data(0);
        }
        return true;
    }

    bool remove_ring(int hexagon, int position){
        if(board.at(hexagon).at(position).get_data()!=player+1) return false;
        board.at(hexagon).at(position).update_data(0);
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



    string get_position(int p, int h){
//        if(p<0) p+=(6*h);
        int i= board.at(h).at(p).get_data();
        if(i==0) return ".";
        else return to_string(i);
    }

    // int heuristic(){
    //     int out = 0;
    //     for(vector<Node_game> u: board){
    //         for(Node_game v : u){
    //             if(v.get_data() == my_marker) out++;
    //             else if(v.get_data() == my_marker-2) out-=18;
    //             else if(v.get_data()<3 && v.get_data()>0) out+=18;
    //             else if(v.get_data()>2) out--;
    //         }
    //     }
    //     return out;
    // }

    int heuristic(){
        int out = 0;
        pair<vector<int>,vector<int>> d1 = find_consecutives(util->elems_on_diagonal1);
        pair<vector<int>,vector<int>> d2 = find_consecutives(util->elems_on_diagonal2);
        pair<vector<int>,vector<int>> v = find_consecutives(util->elems_on_vertical);
        if(my_marker==3){
            // cerr << d1.first.size();
            out = out + (2*d1.first.at(0)) - (2*d1.second.at(0));
            out = out + (4*d1.first.at(1)) - (4*d1.second.at(1));
            out = out + (8*d1.first.at(2)) - (8*d1.second.at(2));
            out = out + (16*d1.first.at(3)) - (16*d1.second.at(3));
            out = out + (18*d1.first.at(4)) - (18*d1.second.at(4));
            out = out + (20*d1.first.at(5)) - (20*d1.second.at(5));
            out = out + (24*d1.first.at(6)) - (24*d1.second.at(6));
            out = out + (32*d1.first.at(7)) - (32*d1.second.at(7));
            out = out + (48*d1.first.at(8)) - (48*d1.second.at(8));

            out = out + (2*d2.first.at(0)) - (2*d2.second.at(0));
            out = out + (4*d2.first.at(1)) - (4*d2.second.at(1));
            out = out + (8*d2.first.at(2)) - (8*d2.second.at(2));
            out = out + (16*d2.first.at(3)) - (16*d2.second.at(3));
            out = out + (18*d2.first.at(4)) - (18*d2.second.at(4));
            out = out + (20*d2.first.at(5)) - (20*d2.second.at(5));
            out = out + (24*d2.first.at(6)) - (24*d2.second.at(6));
            out = out + (32*d2.first.at(7)) - (32*d2.second.at(7));
            out = out + (48*d2.first.at(8)) - (48*d2.second.at(8));

            out = out + (2*v.first.at(0)) - (2*v.second.at(0));
            out = out + (4*v.first.at(1)) - (4*v.second.at(1));
            out = out + (8*v.first.at(2)) - (8*v.second.at(2));
            out = out + (16*v.first.at(3)) - (16*v.second.at(3));
            out = out + (18*v.first.at(4)) - (18*v.second.at(4));
            out = out + (20*v.first.at(5)) - (20*v.second.at(5));
            out = out + (24*v.first.at(6)) - (24*v.second.at(6));
            out = out + (32*v.first.at(7)) - (32*v.second.at(7));
            out = out + (48*v.first.at(8)) - (48*v.second.at(8));

            out = out + (int) (4*pow(6, rings_removed0+1)) - (int)(4*pow(5, rings_removed1+1));


        }
        if(my_marker==4){
            // cerr << d1.first.size();
            out = out - (2*d1.first.at(0)) + (2*d1.second.at(0));
            out = out - (4*d1.first.at(1)) + (4*d1.second.at(1));
            out = out - (8*d1.first.at(2)) + (8*d1.second.at(2));
            out = out - (16*d1.first.at(3)) + (16*d1.second.at(3));
             out = out - (18*d1.first.at(4)) + (18*d1.second.at(4));
            out = out - (20*d1.first.at(5)) + (20*d1.second.at(5));
            out = out - (24*d1.first.at(6)) + (24*d1.second.at(6));
            out = out - (32*d1.first.at(7)) + (32*d1.second.at(7));
            out = out - (48*d1.first.at(8)) + (48*d1.second.at(8));

            out = out - (2*d2.first.at(0)) + (2*d2.second.at(0));
            out = out - (4*d2.first.at(1)) + (4*d2.second.at(1));
            out = out - (8*d2.first.at(2)) + (8*d2.second.at(2));
            out = out - (16*d2.first.at(3)) + (16*d2.second.at(3));
             out = out - (18*d2.first.at(4)) + (18*d2.second.at(4));
            out = out - (20*d2.first.at(5)) + (20*d2.second.at(5));
            out = out - (24*d2.first.at(6)) + (24*d2.second.at(6));
            out = out - (32*d2.first.at(7)) + (32*d2.second.at(7));
            out = out - (48*d2.first.at(8)) + (48*d2.second.at(8));

            out = out - (2*v.first.at(0)) + (2*v.second.at(0));
            out = out - (4*v.first.at(1)) + (4*v.second.at(1));
            out = out - (8*v.first.at(2)) + (8*v.second.at(2));
            out = out - (16*v.first.at(3)) + (16*v.second.at(3));
            out = out - (18*v.first.at(4)) + (18*v.second.at(4));
            out = out - (20*v.first.at(5)) + (20*v.second.at(5));
            out = out - (24*v.first.at(6)) + (24*v.second.at(6));
            out = out - (32*v.first.at(7)) + (32*v.second.at(7));
            out = out - (48*v.first.at(8)) + (48*v.second.at(8));
          
            out = out - (int) (4*pow(5, rings_removed0+1)) + (int)(4*pow(6, rings_removed1+1));

        }


        return out;
    }

    int heuristic_consecutives()
    {
        int out = 0;
        pair<vector<int>,vector<int>> d1 = find_consecutives(util->elems_on_diagonal1);
        pair<vector<int>,vector<int>> d2 = find_consecutives(util->elems_on_diagonal2);
        pair<vector<int>,vector<int>> v = find_consecutives(util->elems_on_vertical);
        return out;
        //ADD YOUR SUMMATIONS HERE
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
//     cout << sizeof(game) << endl;
//     delete game;
//     cout << sizeof(game) << endl;
// }

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
// int main()
// {
//     Utility* util = new Utility(5);
//     Game game = Game(0, 4, util);
//     vector<pair<int,int>> ringos;
//     game.execute_move("P 3 9");
//     game.execute_move("P 4 12");
//     ringos.push_back(make_pair(3,9));
//     ringos.push_back(make_pair(4,12));
//     pair<int,int> ind = game.place_ring_heuristic(ringos);
//     cout << ind.first << " , " << ind.second << endl; 
// }