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

    Utility* get_util(){
        return util;
    }
    int get_removed0(){
        return rings_removed0;
    }
    int get_removed1(){
        return rings_removed1;
    }
    int self_removed(){
      if(my_marker == 3) return rings_removed0;
      else return rings_removed1;
    }
    int opp_removed(){
      if(my_marker == 3) return rings_removed1;
      else return rings_removed0;
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
    vector<pair<int, int>> possible_paths(int i, int j,bool d1, bool d2, bool v)
    {
        Node_game curr = board.at(i).at(j);
        vector<pair<int, int>> possible;
        vector<pair<int,int>> axis_map = util->board.at(i).at(j)->axis_mapping;
        int cons = 0;

        //Diagonal1
        if(d1)
        {
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
        }
        //Diagonal2
        if(d2)
        {
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
        }
        // //Vertical
        if(v)
        {
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
        }

        // //Diagonal1
        if(d1)
        {
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
        }

        // //Diagonal2
        if(d2)
        {
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
        }

        //Vertical
        if(v)
        {
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

    vector<pair<pair<int,int>,pair<int,int>>> checkN(vector<pair<int, int>> changed, int player_marker, int N)
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
                    if(count >= N)
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
            if(count >= N)
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
                    if(count >= N)
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
            if(count >= N)
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
                    if(count >= N)
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
            if(count >= N)
                sequences.push_back(make_pair(st,en));
            axes_checked.insert("V"+to_string(vax));
            //Vertical
        }
        return sequences;
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
        int i= board.at(h).at(p).get_data();
        if(i==0) return ".";
        else return to_string(i);
    }

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

    pair<vector<int>,vector<int>> spatial_distribution()
    {
        vector<int> spat3(n+1);
        vector<int> spat4(n+1);

        for(int i = 0;i < n+1; i++)
        {
            for(int j = 0; j< 6*i; j++)
            {
                if(board.at(i).at(j).get_data() == 3)
                {
                    spat3.at(i) = spat3.at(i) + 1;
                }
                if(board.at(i).at(j).get_data() == 4)
                {
                    spat4.at(i) = spat4.at(i) + 1;
                }
            }
        }
        return make_pair(spat3,spat4);
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

    pair<vector<int>,vector<int>> find_consecutives(vector<vector<pair<int,int>>> directions)
    {
        int curr3 =0, curr4 =0;

        vector<int> counts3(20), counts4(20);
        int cn =0;
        for(int i = 1; i < directions.size() - 1; i++)
        {
            vector<pair<int,int>> axis = directions.at(i);
            for(auto w: axis)
            {
                int data = board.at(w.first).at(w.second).get_data();
                if(data == 3)
                {
                    curr3++;
                    if(curr4 > 1)
                        counts4.at(curr4 - 2) += 1;
                    curr4 = 0;
                }
                else if(data == 4)
                {

                    curr4++;
                    if(curr3 > 1)
                        counts3.at(curr3 - 2) += 1;
                    curr3 = 0;
                }
                else
                {
                    if(curr4 > 1)
                        counts4.at(curr4 - 2) += 1;
                    if(curr3 > 1)
                        counts3.at(curr3 - 2) += 1;
                    curr3 =0;
                    curr4 =0;
                }
            }

            if(curr3 > 1)
                counts3.at(curr3 - 2) +=1;
            if(curr4 > 1)
                counts4.at(curr4 - 2) += 1;

            curr3 = 0;
            curr4 = 0;

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
            for(auto w: axis)
            {

                int data = board.at(w.first).at(w.second).get_data();
                if(data == 3)
                {
                    if(curr3 == 0)
                        st3 = w;
                    en3 = w;
                    curr3++;
                    if(curr4 == max_row-1)
                        cons4.push_back(make_pair(st4,en4));
                    curr4 = 0;
                }
                else if(data == 4)
                {
                    if(curr4 == 0)
                        st4 = w;
                    en4 = w;
                    curr4++;
                    if(curr3 == max_row-1)
                        cons3.push_back(make_pair(st3,en3));
                    curr3 = 0;
                }
                else
                {
                    if(curr4 == max_row-1)
                        cons4.push_back(make_pair(st4,en4));
                    if(curr3 == max_row-1)
                    {
                        cons3.push_back(make_pair(st3,en3));
                    }
                    curr3 =0;
                    curr4 =0;
                }
            }

            if(curr4 == max_row-1)
                cons4.push_back(make_pair(st4,en4));
            if(curr3 == max_row-1)
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
            // cout << axis.at(i).first << " , " << axis.at(i).second << " --> " << dat << endl;
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
            if(dat == my_marker)
                break;
        }

        for(int i=axis_locations.second - 1;i >= 0; i--)
        {
            int dat = board.at(axis.at(i).first).at(axis.at(i).second).get_data();
            // cout << axis.at(i).first << " , " << axis.at(i).second << " --> " << dat << endl;
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
            if(dat == my_marker)
                break;
        }
        // cout << "FWD -> " << fwd << "\tBCD -> " << bcd << endl;
        if( (fwd == 1 && bcd == -1) || (fwd == -1 && bcd == 1) )
        {
            return true;
        }
        return false;
    }

    bool point_in_danger(pair<int,int> point, int opponent_marker, int my_marker, bool diag1, bool diag2, bool vert)
    {
        vector<pair<int,int>> axis_map = util->board.at(point.first).at(point.second)->axis_mapping;
        bool d1 = false,d2 = false, v = false;
        if(diag1)
          d1 = danger_on_axis(util->elems_on_diagonal1,axis_map.at(0),opponent_marker,my_marker);
        if(d1)
        {
            return true;
        }
        // cout << "ALONG D1 : " << d1 <<endl;
        if(diag2)
          d2 = danger_on_axis(util->elems_on_diagonal2,axis_map.at(1),opponent_marker,my_marker);
        if(d2)
        {
            return true;
        }
        // cout << "ALONG D2 : " << d2 <<endl;
        if(vert)
          v = danger_on_axis(util->elems_on_vertical,axis_map.at(2),opponent_marker,my_marker);
        // cout << "ALONG v : " << v <<endl;
        // return (d1 || d2 || v);
        return v;
    }

    bool streak_breakable(vector<pair<int,int>> points, int opponent_marker, int my_marker)
    {
        bool breakable = false;
        for(auto w: points)
        {
            breakable =  breakable || point_in_danger(w, opponent_marker, my_marker,true,true,true);
            if(breakable)
            {
                return breakable;
            }
        }
        return breakable;
    }

    pair<int,int> place_ring_heuristic()
    {
        vector<pair<int,int>> rings = rings_opponent1;
        if(rings.size()==0){
            return make_pair(0,0);
        }
        int r = 0, dirn = 0, max = 0;
        int ring = 0;
        int rn = 0;
        for(auto point:rings)
        {
            vector<pair<int,int>> ndr = total_moves(point);
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

    bool redeemable_streak(pair<int,int> st,pair<int,int> en, int player_marker)
    {
        vector<pair<int,int>> point1 = util->board.at(st.first).at(st.second)->axis_mapping;
        vector<pair<int,int>> point2 = util->board.at(en.first).at(en.second)->axis_mapping;

        int direction = -1;
      	for(int i =0;i<3;i++)
      	{
      		if(point1.at(i).first == point2.at(i).first)
      		{
      			direction = i;
      			break;
      		}
      	}

        int axis = point1.at(direction).first;
      	int low = min(point1.at(direction).second , point2.at(direction).second);
      	int high = max(point1.at(direction).second , point2.at(direction).second);

        bool d1=true,d2=true,v=true;

        vector<pair<int,int>> axis_vals;
        if(direction == 0){
    			axis_vals = util->elems_on_diagonal1.at(axis);
          d1=false;
        }
    		if(direction == 1){
    			axis_vals = util->elems_on_diagonal2.at(axis);
          d2=false;
        }
    		if(direction == 2){
    			axis_vals = util->elems_on_vertical.at(axis);
          v=false;
        }

        vector<pair<int,int>> checkpoints;
        if(low == 0 && high != axis_vals.size()-1)
            checkpoints.push_back(axis_vals.at(high+1));
        else if(low ==0 && high == axis_vals.size()-1)
            return false;
        else if(low != 0 && high == axis_vals.size()-1)
            checkpoints.push_back(axis_vals.at(low-1));
        else
        {
            checkpoints.push_back(axis_vals.at(low-1));
            checkpoints.push_back(axis_vals.at(high+1));
        }

        for(auto bp: checkpoints)
        {
            if(board.at(bp.first).at(bp.second).get_data() == player_marker)
            {
                vector<pair<int, int>> paths = possible_paths(bp.first, bp.second, d1, d2, v);
                if(paths.size() != 0)
                    return true;
            }
            if(board.at(bp.first).at(bp.second).get_data() == (player_marker%2)+3)//i.e. opponent marker
            {
                if(point_in_danger(bp, player_marker, player_marker%2 + 1,d1,d2,v))
                    return true;
            }
        }
        return false;
    }

    vector<pair<int,int>> analyse_streaks()//combines redeemable and breakable streaks
    {
        pair<vector<pair<pair<int,int>,pair<int,int>>>,vector<pair<pair<int,int>,pair<int,int>>>> d1 = find_consecutive_locations(util->elems_on_diagonal1);
        pair<vector<pair<pair<int,int>,pair<int,int>>>,vector<pair<pair<int,int>,pair<int,int>>>> d2 = find_consecutive_locations(util->elems_on_diagonal2);
        pair<vector<pair<pair<int,int>,pair<int,int>>>,vector<pair<pair<int,int>,pair<int,int>>>> v = find_consecutive_locations(util->elems_on_vertical);
        //have to consider difference in breakability, i.e. along axis and cross axis
        int c3 = 0, c4 = 0;
        int r3 = 0, r4 = 0;
        int nbc3 = 0, nbc4 = 0;
        //first values are for point 3, i.e opponent marker = 2
        bool nb, c;
        for(auto w:d1.first)
        {
            nb = true;
            c = false;
            if(streak_breakable(util->between_points(w.first,w.second),2,1))
            {
                c3++;
                nb = false;
            }
            if(redeemable_streak(w.first,w.second,1))
            {
                r3++;
                c = true;
            }
            if(nb && c)
            {
                nbc3++;
            }
        }
        for(auto w:d2.first)
        {
          nb = true;
          c = false;
            if(streak_breakable(util->between_points(w.first,w.second),2,1))
            {
                c3++;
                nb = false;
            }
            if(redeemable_streak(w.first,w.second,1))
            {
                r3++;
                c = true;
            }
            if(nb && c)
            {
                nbc3++;
            }
        }
        for(auto w:v.first)
        {
          nb = true;
          c = false;
            if(streak_breakable(util->between_points(w.first,w.second),2,1))
            {
                c3++;
                nb = false;
            }
            if(redeemable_streak(w.first,w.second,1))
            {
                r3++;
                c = true;
            }
            if(nb && c)
            {
                nbc3++;
            }
        }
        //---------now for marker 4
        for(auto w:d1.second)
        {
          nb = true;
          c = false;
            if(streak_breakable(util->between_points(w.first,w.second),1,2))
            {
                c4++;
                nb = false;
            }
            if(redeemable_streak(w.first,w.second,1))
            {
                r4++;
                c = true;
            }
            if(nb && c)
            {
                nbc4++;
            }
        }
        for(auto w:d2.second)
        {
          nb = true;
          c = false;
            if(streak_breakable(util->between_points(w.first,w.second),1,2))
            {
                c4++;
                nb = false;
            }
            if(redeemable_streak(w.first,w.second,1))
            {
                r4++;
                c = true;
            }
            if(nb && c)
            {
                nbc4++;
            }
        }
        for(auto w:v.second)
        {
          nb = true;
          c = false;
            if(streak_breakable(util->between_points(w.first,w.second),1,2))
            {
                c4++;
                nb = false;
            }
            if(redeemable_streak(w.first,w.second,1))
            {
                r4++;
                c = true;
            }
            if(nb && c)
            {
                nbc4++;
            }
        }

        vector<pair<int,int>> streak_info;
        streak_info.push_back(make_pair(c3,c4));
        streak_info.push_back(make_pair(r3,r4));
        streak_info.push_back(make_pair(nbc3,nbc4));
        return streak_info;
    }

    void density()
    {
        
    }

    pair<vector<int>,vector<int>> get_features()
    {
        vector<int> feat3, feat4;
        pair<vector<int>,vector<int>> features;

        pair<vector<int>,vector<int>> d1 = find_consecutives(util->elems_on_diagonal1);
        pair<vector<int>,vector<int>> d2 = find_consecutives(util->elems_on_diagonal2);
        pair<vector<int>,vector<int>> v = find_consecutives(util->elems_on_vertical);

        vector<pair<int,int>> streaks = analyse_streaks();
        pair<int,int> critical = critical_points();
        pair<vector<int>,vector<int>> ring_mobilities = ring_freedom();
        pair<vector<int>,vector<int>> spatial = spatial_distribution();

        for(int i = 0 ; i < 5; i++)
        {
            feat3.push_back(d1.first.at(i)+d2.first.at(i)+v.first.at(i));
            feat4.push_back(d1.second.at(i)+d2.second.at(i)+v.second.at(i));
        }

        // feat3.push_back(streaks.first.first);  Replaced by non breakable
        feat3.push_back(feat3.at(max_row-3) - streaks.at(0).first);
        feat4.push_back(streaks.at(0).second);
        // feat3.push_back(streaks.second.first);
        feat3.push_back(feat4.at(max_row-3) - streaks.at(1).first);
        feat4.push_back(streaks.at(1).second);

        feat3.push_back(streaks.at(2).first);
        feat4.push_back(streaks.at(2).second);

        feat3.push_back(critical.first);
        feat4.push_back(critical.second);

        vector<int> binary_removal0(4);
        vector<int> binary_removal1(4);
        binary_removal0.at(min(rings_removed0,3)) = 1;
        binary_removal1.at(min(rings_removed1,3)) = 1;
        // feat3.push_back(rings_removed0);
        // feat4.push_back(rings_removed1);

        for(int i = 0; i < 4; i++)
        {
            feat3.push_back(binary_removal0.at(i));
            feat4.push_back(binary_removal1.at(i));
        }

        // else{
        //  feat3.push_back(rings_removed0);
        //   feat4.push_back(rings_removed1);
        // }

        // Adding spatial distribution
        for(int i = 1; i < n+1; i++)
        {
            feat3.push_back(spatial.first.at(i));
            feat4.push_back(spatial.second.at(i));
        }

        if(my_marker == 3)
        {
            for (int i=0; i<feat4.size(); i++) feat4.at(i)*=-1;
            features = make_pair(feat3,feat4);
        }
        else
        {
            for (int i=0; i<feat3.size(); i++) feat3.at(i)*=-1;
            features = make_pair(feat4,feat3);
        }
        //
        int sum = 0;
        for(auto r:ring_mobilities.first)
        {
            sum += r;
            // features.first.push_back(r);
        }
        features.first.push_back(sum);
        sum = 0;
        for(auto r:ring_mobilities.second)
        {
            sum += r;
            // features.second.push_back(r*-1);
        }
        features.second.push_back(sum*-1);

        return features;
    }

    double heuristic(vector<double> weights){
      auto v = get_features();
      double out = 0;
      for (int i=0; i<v.first.size(); i++){
        out += weights.at(i)*v.first.at(i);
      }
     for (int i=0; i<v.first.size(); i++){
        out += weights.at(i+v.first.size())*v.second.at(i);
      }
      return out;
    }

    // int heuristic(vector<double> weights){
    //     int out = 0;
    //     pair<vector<int>,vector<int>> d1 = find_consecutives(util->elems_on_diagonal1);
    //     pair<vector<int>,vector<int>> d2 = find_consecutives(util->elems_on_diagonal2);
    //     pair<vector<int>,vector<int>> v = find_consecutives(util->elems_on_vertical);
    //     int sum3=0;
    //     int sum4=0;
    //     pair<vector<int>,vector<int>> ring_mobilities = ring_freedom();
    //     pair<pair<int,int>,pair<int,int>> streaks = analyse_streaks();
    //     pair<int,int> critical = critical_points();

    //     int c3,c4,r3,r4;
    //     c3 = streaks.first.first;
    //     c4 = streaks.first.second;
    //     r3 = streaks.second.first;
    //     r4 = streaks.second.second;

    //     for(int i =0 ; i < 9; i++)
    //     {
    //         sum3 += util->consecutive_weights.first.at(i)*(d1.first.at(i) + d2.first.at(i) + v.first.at(i));
    //         sum4 += util->consecutive_weights.second.at(i)*(d1.second.at(i) + d2.second.at(i) + v.second.at(i));
    //     }

    //     if(my_marker==3){
    //         out += sum3 - sum4;
    //         out += 100*r3 + 50*c4 - 50*c3 - 100*r4;
    //         out += 100*critical.first - 100*critical.second;
    //         out = out + (int) (4*pow(6, rings_removed0+1)) - (int)(4*pow(5, rings_removed1+1));
    //     }
    //     if(my_marker==4){
    //         out += sum4 - sum3;
    //         out += 100*r4 + 50*c3 - 50*c4 - 100*r3;
    //         out += -100*critical.first + 100*critical.second;
    //         out = out - (int) (4*pow(5, rings_removed0+1)) + (int)(4*pow(6, rings_removed1+1));
    //     }

    //     out += accumulate(ring_mobilities.first.begin(),ring_mobilities.first.end(),0) - accumulate(ring_mobilities.second.begin(),ring_mobilities.second.end(),0);
    //     return out;
    // }

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

    vector<pair<int,int>> intermittent_on_axis(vector<vector<pair<int,int>>> directions, int player_marker, bool d1, bool d2, bool v)
    {
        //boolean for the direction which is given as input should be false
        int count_d1 = 0, count_d2 = 0;
        int buffer = 0;
        int ind1 = -1, ind2 = -1;
        vector<pair<int,int>> buffer_points;

        for(auto axis: directions)
        {
            int i = 0;
            vector<pair<int,int>> tmp;
            for(auto point:axis)
            {
                i++;
                int val = board.at(point.first).at(point.second).get_data();
                if(val == player_marker+2)
                {
                    if(buffer == 0)
                        count_d1++;
                    if(buffer == 1)
                        count_d2++;
                }
                else
                {
                    if(buffer == 0 && count_d1 > 0)
                    {
                        buffer++;
                        ind1 = i;
                        continue;
                    }
                    if(buffer == 1)
                    {
                        if(count_d2 > 0)
                        {
                            buffer++;
                            ind2 = i;
                        }
                        else
                        {
                            buffer = 0;
                            ind1 = -1;
                            count_d1 = 0;
                        }
                    }
                    if(buffer == 2)
                    {
                        if(count_d1 + count_d2 >= max_row - 1)
                        {
                          if(ind1 > 0)
                            tmp.push_back(axis.at(ind1-1));
                            // cout << "storing in buffer (" << axis.at(ind1-1).first << "," << axis.at(ind1-1).second << ")" << endl;
                        }
                        buffer--;
                        ind1 = ind2;
                        ind2 = -1;
                        count_d1 = count_d2;
                        count_d2 = 0;
                    }
                }
            }
            if(count_d1 + count_d2 >= max_row - 1)
            {
              if(ind1 > 0)
                tmp.push_back(axis.at(ind1-1));
                // cout << "storing in buffer (" << axis.at(ind1-1).first << "," << axis.at(ind1-1).second << ")" << endl;
            }

            for(auto bp: tmp)
            {
                if(board.at(bp.first).at(bp.second).get_data() == player_marker)
                {
                    vector<pair<int, int>> paths = possible_paths(bp.first, bp.second, d1, d2, v);
                    if(paths.size() != 0)
                    {
                        buffer_points.push_back(bp);
                    }
                }
                if(board.at(bp.first).at(bp.second).get_data() == (player_marker%2)+3)//i.e. opponent marker
                {
                    if(point_in_danger(bp, player_marker, player_marker%2 + 1,d1,d2,v))
                    {
                        buffer_points.push_back(bp);
                    }
                }
            }

            count_d1 = 0, count_d2 = 0;
            buffer = 0;
            ind1 = -1, ind2 = -1;
        }
        return buffer_points;
    }

    pair<int,int> critical_points()//1st index is for player 3. second is for player 2
    {
        int sum3,sum4;

        sum3 += (intermittent_on_axis(util->elems_on_diagonal1,1,false,true,true)).size();
        sum3 += (intermittent_on_axis(util->elems_on_diagonal2,1,true,false,true)).size();
        sum3 += (intermittent_on_axis(util->elems_on_vertical,1,true,true,false)).size();

        sum4 += (intermittent_on_axis(util->elems_on_diagonal1,2,false,true,true)).size();
        sum4 += (intermittent_on_axis(util->elems_on_diagonal2,2,true,false,true)).size();
        sum4 += (intermittent_on_axis(util->elems_on_vertical,2,true,true,false)).size();

        return make_pair(sum3,sum4);
    }

    pair<vector<int>,vector<int>> ring_freedom() //first element of pair is my moves available. second is opponent's moves available
    {
        vector<int> self_moves;
        vector<int> opponent_moves;

        int i = 0;
        for(auto w:ring_self)
        {
            if (w.first == -1 || w.second == -1)
                self_moves.push_back(0);
            else
                self_moves.push_back((possible_paths(w.first, w.second,true, true, true)).size());
        }
        i = 0;
        for(auto w:ring_opponent)
        {
            if (w.first == -1 || w.second == -1)
                opponent_moves.push_back(0);
            else
                opponent_moves.push_back((possible_paths(w.first, w.second,true, true, true)).size());
        }
        return make_pair(self_moves,opponent_moves);
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
//
// int main()
// {
//     Utility* util = new Utility(6);
//     Game game = Game(0, 3, util, 6, 6, 6);
//     vector<pair<int,int>> ringos;
//     game.board[6][29].data = 4;
//     game.board[6][28].data = 4;
//     game.board[6][27].data = 3;
//     game.board[6][26].data = 2;
//     game.board[6][25].data = 4;
//
//     game.board[6][31].data = 4;
//     game.board[5][25].data = 4;
//     game.board[5][24].data = 2;
//     game.board[5][23].data = 4;
//     game.board[5][22].data = 4;
//     game.board[5][21].data = 4;
//
//     game.board[6][32].data = 4;
//     game.board[5][26].data = 3;
//     game.board[4][20].data = 4;
//     game.board[4][19].data = 4;
//     game.board[4][18].data = 4;
//     game.board[4][17].data = 4;
//
//     game.board[6][33].data = 4;
//     game.board[5][27].data = 4;
//     game.board[4][21].data = 4;
//     game.board[3][15].data = 4;
//     game.board[3][14].data = 3;
//     game.board[3][13].data = 4;
//
//     game.board[6][34].data = 4;
//     game.board[5][28].data = 4;
//     game.board[4][22].data = 4;
//     game.board[3][16].data = 4;
//     game.board[2][10].data = 0;
//     game.board[2][9].data = 4;
//
//     game.board[6][35].data = 4;
//     game.board[5][29].data = 4;
//     game.board[4][13].data = 4;
//     game.board[3][17].data = 4;
//     game.board[2][11].data = 4;
//     game.board[1][5].data = 2;
//
//     game.board[6][1].data = 4;
//     game.board[5][1].data = 4;
//     game.board[4][1].data = 4;
//     game.board[3][1].data = 3;
//     game.board[2][1].data = 4;
//     game.board[1][1].data = 4;
//     game.board[1][2].data = 3;
//     game.board[2][5].data = 3;
//     game.board[3][8].data = 4;
//     game.board[4][11].data = 4;
//     game.board[5][13].data = 4;
//
//     cout << game.redeemable_streak(make_pair(6,35),make_pair(2,11),2) << endl;
//
//     //////////////////////////
//     // game.board[3][12].data = 4;
//     // game.board[4][15].data = 3;
//     // game.board[5][18].data = 3;
//     // game.board[6][21].data = 4;
//     // game.board[6][25].data = 4;
//     // game.board[6][31].data = 4;
//     // game.board[5][25].data = 4;
//     // game.board[5][24].data = 3;
//     // game.board[5][23].data = 4;
//     // game.board[5][22].data = 4;
//     // game.board[5][21].data = 4;
//     // game.board[6][32].data = 4;
//     // game.board[5][26].data = 3;
//     // game.board[4][20].data = 4;
//     // game.board[4][19].data = 4;
//     // game.board[4][18].data = 4;
//     // game.board[4][17].data = 4;
//     // game.board[6][33].data = 4;
//     // game.board[5][27].data = 4;
//     // game.board[4][21].data = 4;
//     // game.board[3][15].data = 4;
//     // game.board[3][14].data = 3;
//     // game.board[3][13].data = 4;
//
//     // vector<pair<int,int>> p = game.intermittent_on_axis(util->elems_on_vertical,2,true,true,false);
//     // for(auto w: p)
//     // {
//     //     cout << w.first << "," << w.second << endl;
//     // }
//
//     // game.execute_move("P 3 9");
//     // game.execute_move("P 4 12");
//     // ringos.push_back(make_pair(3,9));Please let me know of the availability of positions.Please let me know of the availability of positions.
//     // ringos.push_back(make_pair(4,12));
//     // pair<int,int> ind = game.place_ring_heuristic(ringos);
//     // cout << ind.first << " , " << ind.second << endl;
// }
