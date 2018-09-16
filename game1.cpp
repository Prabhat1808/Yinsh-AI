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
                }
                else
                {
                    if(count >= 5)
                    {
                        en = w;
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
            axes_checked.insert("D1"+to_string(d1ax));

            count = 0;
            obtained = false;
            for(auto w: util->elems_on_diagonal2.at(axis_map.at(1).first))
            {
                if(axes_checked.count("D2"+to_string(d2ax)) != 0)
                    break;

                int data = board.at(w.first).at(w.second)->data;
                if(data == player_marker)
                {
                    if(count == 0)
                        st = w;
                    count++;
                }
                else
                {
                    if(count >= 5)
                    {
                        en = w;
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
            axes_checked.insert("D2"+to_string(d2ax));

            count = 0;
            obtained = false;
            for(auto w: util->elems_on_vertical.at(axis_map.at(2).first))
            {
                if(axes_checked.count("V"+to_string(vax)) != 0)
                    break;

                int data = board.at(w.first).at(w.second)->data;
                if(data == player_marker)
                {
                    if(count == 0)
                        st = w;
                    count++;
                }
                else
                {
                    if(count >= 5)
                    {
                        en = w;
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
            axes_checked.insert("V"+to_string(vax));
        }
        return sequences;
    }

};

int main()
{
    Utility* util = new Utility();
    Game* game = new Game(0,4,util);
    vector<pair<int, int>> po = game->possible_paths(3,4);
    for(auto w: po)
        cout << w.first << " , " << w.second << endl;
}