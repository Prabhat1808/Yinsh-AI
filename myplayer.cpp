#include <bits/stdc++.h>
#include "game1.cpp"
// #include "utilities.cpp"

using namespace std;

pair<pair<int, Game*>, string> maxval( pair<pair<int, Game*>, vector<pair<int, int> > > , int , int , int );

void do_remove(pair<Game*, string> x, pair<pair<int, int>, pair<int, int> > option, vector<pair<Game*, string>> &future_possibilities)
 {
    vector<pair<int, int>> indices = x.first->util->between_points(option.first, option.second);
    int leng = 0;
    vector<pair<pair<int, int>, pair<int, int> > > curr_options;
    int begin;
    for(int i=0; i<indices.size(); i++)
    {
        if(x.first->board.at(indices.at(i).first).at(indices.at(i).second)->data==x.first->player+3)
        {
            leng++;
        }
        else leng=0;
        if(leng==1) begin = i;
        if(leng==5)
        {
            curr_options.push_back(make_pair(indices.at(begin), indices.at(i)));
            begin++;
            leng--;
        }
    }
    if(curr_options.size())
    {
        for(pair<pair<int, int>, pair<int, int>> opt: curr_options)
        {
            Game* toremove = x.first->copy_board();
            toremove->remove_start(opt.first.first, opt.first.second);
            toremove->remove_end(opt.second.first, opt.second.second);
            vector<pair<int, int>> possible_rings;
            if (toremove->player + 3 == toremove->my_marker) possible_rings = toremove->ring_self;
            else possible_rings = toremove->ring_opponent;
            for (pair<int, int> r : possible_rings)
             {
                if (r.first == -1 || r.second == -1) continue;
                Game* readytoremove = toremove->copy_board();
                readytoremove->remove_ring(r.first, r.second);
                string move;
                if(x.second.length()) move = x.second + " RS " + to_string(opt.first.first) + " " + to_string(opt.first.second) + " RE " + to_string(opt.second.first) + " " + to_string(opt.second.second) + " X " + to_string(r.first) + " " + to_string(r.second);
                else move = "RS " + to_string(opt.first.first) + " " + to_string(opt.first.second) + " RE " + to_string(opt.second.first) + " " + to_string(opt.second.second) + " X " + to_string(r.first) + " " + to_string(r.second);
                pair<Game*, string> out = make_pair(readytoremove, move);
                future_possibilities.push_back(out);
            }
        }
    }
    else
    {
        future_possibilities.push_back(x);
    }
}

    // Node_game *curr = x.first->board.at(option.first.first).at(option.first.second);
    // int i = 0;
    // for (i = 0; i < 6; i++) {
    //     pair<int, int> p = curr->neighbours.at(i);
    //     if (p.first == -1 || p.second == -1) continue;
    //     int leng = x.first->removal_len(x.first->board.at(p.first).at(p.second), option.first.first, option.first.second, 2);
    //     pair<int, int> ind = curr->index;
    //     if (leng) {
    //         for (int j = 0; j <= leng - x.first->max_row; j++) {
    //             Game* toremove = x.first->copy_board();
    //             toremove->board.at(ind.first).at(ind.second)->data = 0;
    //             pair<int, int> ending = toremove->remove_now(toremove->board.at(p.first).at(p.second), ind.first, ind.second, 2);
    //             vector<pair<int, int>> possible_rings;
    //             if (toremove->player + 3 == toremove->my_marker) possible_rings = toremove->my_rings();
    //             else possible_rings = toremove->opponents_rings();
    //             for (pair<int, int> r : possible_rings) {
    //                 if (r.first == -1 || r.second == -1) continue;
    //                 Game* readytoremove = toremove->copy_board();
    //                 readytoremove->remove_ring(r.first, r.second);
    //                 string move;
    //                 if(x.second.length()) move = x.second + " RS " + to_string(ind.first) + " " + to_string(ind.second) + " RE " + to_string(ending.first) + " " + to_string(ending.second) + " X " + to_string(r.first) + " " + to_string(r.second);
    //                 else move = "RS " + to_string(ind.first) + " " + to_string(ind.second) + " RE " + to_string(ending.first) + " " + to_string(ending.second) + " X " + to_string(r.first) + " " + to_string(r.second);
    //                 pair<Game*, string> out = make_pair(readytoremove, move);
    //                 future_possibilities.push_back(out);
    //             }
    //             int next_index = -1;
    //             for (int k = 0; k < 6; k++) {
    //                 if (toremove->board.at(p.first).at(p.second)->neighbours.at(k) == ind) next_index = k;
    //             }
    //             next_index = (next_index + 3) % 6;
    //             tie(ind.first, ind.second) = p;
    //             tie(p.first, p.second) = toremove->board.at(p.first).at(p.second)->neighbours.at(next_index);
    //         }
    //         break;
    //     }

    // }
    // if (i == 6) {
    //     future_possibilities.push_back(x);
    // }




vector<pair<Game*, string>> remove_rows(vector<pair<pair<int, int>, pair<int, int> > > removal, Game* game) {
    vector<pair<Game*, string>> possibilities;
    do {
        vector<pair<Game*, string>> current_possibilities;
        pair<Game*, string> init = make_pair(game, "");
        current_possibilities.push_back(init);
        for (pair<pair<int, int>, pair<int, int>> option: removal) {
            vector<pair<Game *, string>> future_possibilities;
            for (pair<Game*, string> x: current_possibilities) {
                do_remove(x, option, future_possibilities);
            }
            current_possibilities.resize(0);
            for (pair<Game *, string> x:future_possibilities) {
                current_possibilities.push_back(x);
            }
            future_possibilities.resize(0);
        }
        for(pair<Game*, string> u: current_possibilities){
            possibilities.push_back(u);
        }
    } while(next_permutation(removal.begin(), removal.end()));
    return possibilities;
};

vector<pair<pair<pair<int, Game*>, vector<pair<int, int> > >, string> > get_successors(vector<pair<Game*, string>> newboard) {
    vector<pair<pair<pair<int, Game*>, vector<pair<int, int>>>, string>> successors;
    for (pair<Game*, string> ga: newboard) {
        Game* gam = ga.first;
        vector<pair<int, int>> rings ;
        if(gam->player==gam->my_marker-3) rings = gam->ring_self;
        else rings = gam->ring_opponent;
        for (pair<int, int> ring: rings) {
            if (ring.first != -1 || ring.second != -1) {
                vector<pair<int, int>> paths = gam->possible_paths(ring.first, ring.second);
                for (pair<int, int> path: paths) {
                    Game* temp = gam->copy_board();
                    temp->select_ring(ring.first, ring.second);
                    vector<pair<int, int>> changed;
                    temp->move_ring(path.first, path.second, changed);
                    vector<pair<pair<int, int>, pair<int, int>>> removal = temp->check5(changed, temp->player + 3);
                    string curr_move = "S " + to_string(ring.first) + " " + to_string(ring.second) + " M " + to_string(path.first) + " " + to_string(path.second);
                    if (!removal.empty()) {
                        vector<pair<Game*, string>> current_possibilities = remove_rows(removal, temp);
                        for (pair<Game*, string> g: current_possibilities) {
                            g.first->player = (g.first->player + 1) % 2;
                            int heur = g.first->heuristic();
                            pair<int, Game*> nextstate = make_pair(heur, g.first);
                            pair<pair<int, Game*>, vector<pair<int, int>>> c = make_pair(nextstate, changed);
                            string move;
                            if(ga.second.length()) move = ga.second+ " " + curr_move + " " + g.second;
                            else move = curr_move + " " + g.second;
                            auto d = make_pair(c, move);
                            successors.push_back(d);
                        }
                    }
                    else {
                        int heuristic = temp->heuristic();
                        temp->player = (temp->player + 1) % 2;
                        pair<int, Game*> curr = make_pair(heuristic, temp);
                        pair<pair<int, Game*>, vector<pair<int, int>>> c = make_pair(curr, changed);
                        string mov;
                        if(ga.second.length()) mov = ga.second + " " + curr_move;
                        else mov = curr_move;

                        auto d = make_pair(c, mov);
                        successors.push_back(d);
                    }
                }
            }
        }
    }

    return successors;
};


pair<pair<int, Game*>, string> minval(pair<pair<int, Game*>, vector<pair<int, int> > > mygame, int alpha, int beta, int h) {


    vector<pair<pair<int, int>, pair<int, int>>> before_removal = mygame.first.second->check5(mygame.second, mygame.first.second->player + 3);
    vector<pair<Game*, string>> newboard;
    if (!before_removal.empty()) {
        newboard = remove_rows(before_removal, mygame.first.second);
    }
    else{
        pair<Game*, string> curr_game = make_pair(mygame.first.second, "");
        newboard.push_back(curr_game);
    }

    vector<pair<pair<pair<int, Game*>, vector<pair<int, int> > >, string > > successors = get_successors(newboard);
    if(h==1) {
        auto out = *std::min_element(successors.begin(), successors.end());
        pair<pair<int, Game*>, string> output = make_pair(out.first.first, out.second);
        return output;
    }
    sort(successors.begin(), successors.end());
    for (auto u: successors) {
        pair<pair<int, Game*>, string> child = maxval(u.first, alpha, beta, h - 1);
        beta = min(beta, child.first.first);
        if (alpha >= beta) {
            return child;
        }
    }
    int l = successors.size()-1;
    pair<pair<int, Game*>, string> output = make_pair(successors.at(l).first.first, successors.at(l).second);
    return output;
};

pair<pair<int, Game*>, string> maxval(pair<pair<int, Game*>, vector<pair<int, int> > > mygame, int alpha, int beta, int h) {

    vector<pair<pair<int, int>, pair<int, int>>> before_removal = mygame.first.second->check5(mygame.second, mygame.first.second->player + 3);
    vector<pair<Game*, string>> newboard;
    if (!before_removal.empty()) {
        newboard = remove_rows(before_removal, mygame.first.second);
    }
    else {
        pair<Game*, string> curr_game = make_pair(mygame.first.second, "");
        newboard.push_back(curr_game);
    }
    vector<pair<pair<pair<int, Game*>, vector<pair<int, int>>>, string>> successors = get_successors(newboard);
    if(h==1) {

        auto out = *std::max_element(successors.begin(), successors.end());
        pair<pair<int, Game*>, string> output = make_pair(out.first.first, out.second);
        return output;
    }
    sort(successors.begin(), successors.end(), greater<>());
    for (pair<pair<pair<int, Game*>, vector<pair<int, int>>>, string> u: successors) {
        pair<pair<int, Game*>, string> child = minval(u.first, alpha, beta, h - 1);
        alpha = max(alpha, child.first.first);
        if (alpha >= beta) {
            return child;
        }
    }

    pair<pair<int, Game*>, string> output = make_pair(successors.at(0).first.first, successors.at(0).second);
    return output;
};

int main(int argc, char** argv)
{

    int player_id, board_size, time_limit;
    string move;
    string s;

    getline(cin, s);
    string buff = "";
    vector<int> starting;
    for(auto i:s){
        if(i == ' '){
            starting.push_back(stoi(buff));
            buff = "";
        }
        else {
            buff+=i;
        }
    }
    if(!buff.empty()) starting.push_back(stoi(buff));

    // cin >> player_id >> board_size >> time_limit;
    Utility* util = new Utility(5);
    Game* game = new Game(0, starting.at(0)+2, util, starting.at(1));


    
   	game->execute_move("P 0 0");
    game->execute_move("P 1 0");
    game->execute_move("P 1 1");
    game->execute_move("P 1 2");
    game->execute_move("P 1 3");
    game->execute_move("P 1 4");
    game->execute_move("P 1 5");
    game->execute_move("P 2 0");
    game->execute_move("P 2 1");
    game->execute_move("P 2 2");
    vector<pair<int, int>> changed;
    if(player_id==2){
        getline(cin, move);
        // cout << "Move taken: " << move <<  endl;
        changed = game->execute_move(move);
    }
    while(true){
        pair<int, Game*> inp = make_pair(game->heuristic(), game);
        pair<pair<int, Game*>, vector<pair<int, int>>> taken = make_pair(inp, changed);
        pair<pair<int, Game*>, string> mymove = maxval(taken, INT_MIN, INT_MAX, 2);
        // cerr << endl << endl;
        cerr << mymove.second << endl;
        game->execute_move(mymove.second);

        // if(game->check_won()) break;
        getline(cin, move);
        // cout << "Move taken: " << move <<  endl;
        vector<pair<int, int>> new_changes = game->execute_move(move);
        if(game->check_won()) break;
        changed.resize(0);
        for(pair<int, int> u: new_changes){
            changed.push_back(u);
        }
    }

}