#include <bits/stdc++.h>
#include "game.cpp"

using namespace std;

vector<pair<pair<int,int>,pair<int,int>>> directions;

void updateDirections()
{
    directions.push_back(make_pair(make_pair(5,29),make_pair(4,0)));
    for(int i=28;i>20;i--)
    {
        if(i%5 == 0)
        {
            directions.push_back(make_pair(make_pair(4,i-5),make_pair(3,i-10)));
            continue;
        }
        directions.push_back(make_pair(make_pair(5,i),make_pair(4,i-5)));

        //vertical left half
        if(i > 25)
            directions.push_back(make_pair(make_pair(5,i),make_pair(4,i-6)));
    }
    directions.push_back(make_pair(make_pair(5,29),make_pair(4,23)));
    directions.push_back(make_pair(make_pair(4,0),make_pair(3,0)));

    for(int i=1;i<10;i++)
    {
        if(i%5==0)
        {
            directions.push_back(make_pair(make_pair(4,i-1),make_pair(3,i-2)));
            continue;
        }
        directions.push_back(make_pair(make_pair(5,i),make_pair(4,i-1)));

        //vertical right half
        if(i < 5)
            directions.push_back(make_pair(make_pair(5,i),make_pair(4,i)));
    }
}


pair<int, Game*> maxval( pair<pair<int, Game*>, vector<pair<int, int> > > , int , int , int );


void do_remove(Game* x, pair<pair<int, int>, pair<int, int> > option, vector<Game*> &future_possibilities) {
    Node *curr = x->board.at(option.first.first).at(option.first.second);
    int i = 0;
    for (i = 0; i < 6; i++) {
        pair<int, int> p = curr->neighbours.at(i);
        if (p.first == -1 || p.second == -1) continue;
        int leng = x->removal_len(x->board.at(p.first).at(p.second), option.first.first, option.first.second, 2);
        pair<int, int> ind = curr->index;
        if (leng) {
            for (int j = 0; j <= leng - x->max_row; j++) {
                Game* toremove = x->copy_board();
                toremove->board.at(ind.first).at(ind.second)->data = 0;
                toremove->remove_now(toremove->board.at(p.first).at(p.second), ind.first, ind.second, 2);
                vector<pair<int, int>> possible_rings;
                if (toremove->player + 3 == toremove->my_marker) possible_rings = toremove->my_rings();
                else possible_rings = toremove->opponents_rings();
                for (pair<int, int> r : possible_rings) {
                    if (r.first == -1 || r.second == -1) continue;
                    Game* readytoremove = toremove->copy_board();
                    readytoremove->remove_ring(r.first, r.second);
                    future_possibilities.push_back(readytoremove);
                }
                int next_index = -1;
                for (int k = 0; k < 6; k++) {
                    if (toremove->board.at(p.first).at(p.second)->neighbours.at(k) == ind) next_index = k;
                }
                next_index = (next_index + 3) % 6;
                tie(ind.first, ind.second) = p;
                tie(p.first, p.second) = toremove->board.at(p.first).at(p.second)->neighbours.at(next_index);
            }
            break;
        }

    }
    if (i == 6) {
        future_possibilities.push_back(x);
    }


};

vector<Game*> remove_rows(vector<pair<pair<int, int>, pair<int, int> > > removal, Game* game) {
    vector<Game*> possibilities;
    do {
        vector<Game*> current_possibilities;
        current_possibilities.push_back(game);
        for (pair<pair<int, int>, pair<int, int>> option: removal) {
            vector<Game *> future_possibilities;
            for (Game *x: current_possibilities) {
                do_remove(x, option, future_possibilities);
            }
            current_possibilities.resize(0);
            for (Game *x:future_possibilities) {
                current_possibilities.push_back(x);
            }
            future_possibilities.resize(0);
        }
        for(Game* u: current_possibilities){
            possibilities.push_back(u);
        }
    } while(next_permutation(removal.begin(), removal.end()));
    return possibilities;
};

vector<pair<pair<pair<int, Game*>, vector<pair<int, int> > >, string> > get_successors(vector<Game*> newboard) {
    vector<pair<pair<pair<int, Game*>, vector<pair<int, int>>>, string>> successors;
    for (Game* gam: newboard) {
        vector<pair<int, int>> rings ;
        if(gam->player==gam->my_marker-3) rings = gam->my_rings();
        else rings = gam->opponents_rings();
        for (pair<int, int> ring: rings) {
            if (ring.first != -1 || ring.second != -1) {
                vector<pair<int, int>> paths = gam->possible_paths(ring.first, ring.second);
                for (pair<int, int> path: paths) {
                    Game* temp = gam->copy_board();
                    temp->select_ring(ring.first, ring.second);
                    vector<pair<int, int>> changed;
                    temp->move_ring(path.first, path.second, changed);
                    vector<pair<pair<int, int>, pair<int, int>>> removal = temp->check5(changed, temp->player + 3);
                    string curr_move = "S " + to_string(ring.first) + " " + to_string(ring.second) + " M " + to_string(path.first) + " " + to_string(path.second) + " ";
                    if (!removal.empty()) {
                        vector<Game*> current_possibilities = remove_rows(removal, temp);
                        for (Game* g: current_possibilities) {
                            g->player = (g->player + 1) % 2;
                            int heur = g->heuristic();
                            pair<int, Game*> nextstate = make_pair(heur, g);
                            pair<pair<int, Game*>, vector<pair<int, int>>> c = make_pair(nextstate, changed);
                            auto d = make_pair(c, curr_move);
                            successors.push_back(d);
                        }
                    }
                    else {
                        int heuristic = temp->heuristic();
                        temp->player = (temp->player + 1) % 2;
                        pair<int, Game*> curr = make_pair(heuristic, temp);
                        pair<pair<int, Game*>, vector<pair<int, int>>> c = make_pair(curr, changed);
                        auto d = make_pair(c, curr_move);
                        successors.push_back(d);
                    }
                }
            }
        }
    }

    return successors;
};


pair<int, Game*> minval(pair<pair<int, Game*>, vector<pair<int, int> > > mygame, int alpha, int beta, int h) {


    vector<pair<pair<int, int>, pair<int, int>>> before_removal = mygame.first.second->check5(mygame.second, mygame.first.second->player + 3);
    vector<Game*> newboard;
    if (!before_removal.empty()) {
        newboard = remove_rows(before_removal, mygame.first.second);
    }
    else newboard.push_back(mygame.first.second);

    vector<pair<pair<pair<int, Game*>, vector<pair<int, int> > >, string > > successors = get_successors(newboard);
    if(h==1) {
        auto out = *std::min_element(successors.begin(), successors.end());
//        cout << "h: " << 1 << "  " << out.second << endl;
        return out.first.first;
    }
    sort(successors.begin(), successors.end());
//    cout << "####" <<endl;
    for (auto u: successors) {
//        cout << "h: " << h << "  " << u.second << endl;
        pair<int, Game*> child = maxval(u.first, alpha, beta, h - 1);
        beta = min(beta, child.first);
        if (alpha >= beta) {
//            cout << "##" << endl;
            return child;
        }
    }
    return successors.at(successors.size() - 1).first.first;
};

pair<int, Game*> maxval(pair<pair<int, Game*>, vector<pair<int, int> > > mygame, int alpha, int beta, int h) {

    vector<pair<pair<int, int>, pair<int, int>>> before_removal = mygame.first.second->check5(mygame.second, mygame.first.second->player + 3);
    vector<Game*> newboard;
    if (!before_removal.empty()) {
        newboard = remove_rows(before_removal, mygame.first.second);
        for(auto u: newboard) u->print_board();
    } else newboard.push_back(mygame.first.second);
    vector<pair<pair<pair<int, Game*>, vector<pair<int, int>>>, string>> successors = get_successors(newboard);
    if(h==1) {

        pair<pair<pair<int, Game *>, vector<pair<int, int>>>, string> & out = *std::max_element(successors.begin(), successors.end());
//        cout << "h: " << 1 << "  " << out.second << endl;
        return out.first.first;
    }
        sort(successors.begin(), successors.end(), greater<>());
//    cout << "####" << endl;
    for (pair<pair<pair<int, Game*>, vector<pair<int, int>>>, string> u: successors) {
//        cout << "h: " << h << "  " << u.second << endl;
        pair<int, Game*> child = minval(u.first, alpha, beta, h - 1);
        alpha = max(alpha, child.first);
        if (alpha >= beta) {
//            cout << "##" << endl;
            return child;
        }
    }

    return successors.at(0).first.first;
};

int main()
{
    updateDirections();
    // for(auto w:directions)
    //     cout << "(" << w.first.first << "," << w.first.second << ") -> " << "(" << w.second.first << "," << w.second.second << ")" << endl;
    // cout << directions.size();

    Game* game = new Game(0, 4);
    game->execute_move("P 0 0");
    game->execute_move("P 1 1");
    game->execute_move("P 4 0");
    game->execute_move("P 2 2");
    game->execute_move("P 5 1");
    game->execute_move("P 1 4");
    game->execute_move("P 5 29");
    game->execute_move("P 2 8");
    game->execute_move("P 3 0");
    game->execute_move("P 2 10");
    game->execute_move("S 0 0 M 1 2");
    game->execute_move("S 1 1 M 2 3");
    game->execute_move("S 1 2 M 2 4");
    game->execute_move("S 2 2 M 3 4");
    game->execute_move("S 2 4 M 3 5");
    game->execute_move("S 1 4 M 1 3");
    game->execute_move("S 3 5 M 4 6");
    game->execute_move("S 2 8 M 2 7");
    game->execute_move("S 4 0 M 4 1");
    game->execute_move("S 2 10 M 1 5");


    vector<pair<int, int> > changed = game->execute_move("S 3 0 M 3 1");
    game->print_board();
    int heur = game->heuristic();
    pair<int, Game*> mygame = make_pair(heur, game);
    pair<pair<int, Game*>, vector<pair<int, int>>> inp = make_pair(mygame, changed);
    clock_t tStart = clock();
    pair<int, Game*> max = minval(inp, INT_MIN, INT_MAX, 2);
    printf("Time taken: %.2fs\n", (double)(clock() - tStart)/CLOCKS_PER_SEC);

    cout << "Out Heuristic: " << max.first;
    max.second->print_board();


}

