#include <bits/stdc++.h>
#include "Game.cpp"

using namespace std;

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
                    future_possibilities.push_back(toremove);
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
    vector<Game*> current_possibilities;
    current_possibilities.push_back(game);
    for (pair<pair<int, int>, pair<int, int>> option: removal) {
        vector<Game*> future_possibilities;
        for (Game* x: current_possibilities) {
            do_remove(x, option, future_possibilities);
        }
        current_possibilities.resize(0);
        for (Game* x:future_possibilities) {
            current_possibilities.push_back(x);
        }
        future_possibilities.resize(0);
    }
    return current_possibilities;
};

vector<pair<pair<int, Game*>, vector<pair<int, int> > > > get_successors(vector<Game*> newboard, vector<pair<int, int>> rings) {
    vector<pair<pair<int, Game*>, vector<pair<int, int>>>> successors;
    for (Game* gam: newboard) {
        for (pair<int, int> ring: rings) {
            if (ring.first != -1 || ring.second != -1) {
                vector<pair<int, int>> paths = gam->possible_paths(ring.first, ring.second);
                for (pair<int, int> path: paths) {
                    Game* temp = gam->copy_board();
                    temp->select_ring(ring.first, ring.second);
                    vector<pair<int, int>> changed;
                    temp->move_ring(path.first, path.second, changed);
                    vector<pair<pair<int, int>, pair<int, int>>> removal = temp->check5(changed, temp->player + 3);
                    if (!removal.empty()) {
                        vector<Game*> current_possibilities = remove_rows(removal, temp);
//                        vector<Game*> current_possibilities;
                        for (Game* g: current_possibilities) {
                            g->player = (g->player + 1) % 2;
                            int heur = g->heuristic();
                            pair<int, Game*> nextstate = make_pair(heur, g);
                            pair<pair<int, Game*>, vector<pair<int, int>>> c = make_pair(nextstate, changed);
                            successors.push_back(c);
                        }
                    }
                    else {
                        int heuristic = temp->heuristic();
                        temp->player = (temp->player + 1) % 2;
                        pair<int, Game*> curr = make_pair(heuristic, temp);
                        pair<pair<int, Game*>, vector<pair<int, int>>> c = make_pair(curr, changed);
                        successors.push_back(c);
                    }
                }
            }
        }
    }

    return successors;
};


pair<int, Game*> minval(pair<pair<int, Game*>, vector<pair<int, int> > > mygame, int alpha, int beta, int h) {
    if (h == 0) return mygame.first;

    vector<pair<int, int>> rings = mygame.first.second->opponents_rings();
    vector<pair<pair<int, int>, pair<int, int>>> before_removal = mygame.first.second->check5(mygame.second, mygame.first.second->player + 3);
    vector<Game*> newboard;
    if (!before_removal.empty()) {
//        newboard = remove_rows(before_removal, mygame.first.second);
    }
    else newboard.push_back(mygame.first.second);

    vector<pair<pair<int, Game*>, vector<pair<int, int> > > > successors = get_successors(newboard, rings);
    sort(successors.begin(), successors.end(), greater<>());
    for (pair<pair<int, Game*>, vector<pair<int, int>>> u: successors) {
        pair<int, Game*> child = maxval(u, alpha, beta, h - 1);
        beta = min(beta, child.first);
        if (alpha >= beta) return child;
    }
    return successors.at(successors.size() - 1).first;
};

pair<int, Game*> maxval(pair<pair<int, Game*>, vector<pair<int, int> > > mygame, int alpha, int beta, int h) {
    if (h == 0) return mygame.first;
//    vector<pair<int, Game>> successors;
    vector<pair<int, int>> rings = mygame.first.second->my_rings();
    vector<pair<pair<int, int>, pair<int, int>>> before_removal = mygame.first.second->check5(mygame.second, mygame.first.second->player + 3);
    vector<Game*> newboard;
    if (!before_removal.empty()) {
        newboard = remove_rows(before_removal, mygame.first.second);
    } else newboard.push_back(mygame.first.second);
    vector<pair<pair<int, Game*>, vector<pair<int, int>>>> successors = get_successors(newboard, rings);
    sort(successors.begin(), successors.end());
    for (pair<pair<int, Game*>, vector<pair<int, int>>> u: successors) {
        pair<int, Game*> child = minval(u, alpha, beta, h - 1);
        alpha = max(alpha, child.first);
        if (alpha >= beta) return child;
    }
    return successors.at(successors.size() - 1).first;
};

int main()
{
    Game* game = new Game(0, 4);

}
