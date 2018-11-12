#include <bits/stdc++.h>
#include <ctime>
#include "game1.cpp"
// #include "utilities.cpp"

using namespace std;

pair<pair<double, Game>, string> maxval( int, pair<pair<double, Game>, vector<pair<int, int> > > , double , double , int, vector<double>, double);

void do_remove(int k, pair<Game, string> x, pair<pair<int, int>, pair<int, int> > option, vector<pair<Game, string>> &future_possibilities)
{
    vector<pair<int, int>> indices = x.first.get_util()->between_points(option.first, option.second);
    int leng = 0;
    vector<pair<pair<int, int>, pair<int, int> > > curr_options;
    int begin=0;
    for(int i=0; i<indices.size(); i++)
    {
        if(x.first.get_data(indices.at(i).first, indices.at(i).second)==x.first.get_player()+3)
        {
            leng++;
        }
        else leng=0;
        if(leng==1) begin = i;
        if(leng==k)
        {
            curr_options.emplace_back(indices.at(begin), indices.at(i));
            begin++;
            leng--;
        }
    }
    if(!curr_options.empty())
    {
        for(pair<pair<int, int>, pair<int, int>> opt: curr_options)
        {
            Game toremove = x.first.copy_board();
            toremove.remove_start(opt.first.first, opt.first.second);
            toremove.remove_end(opt.second.first, opt.second.second);
            vector<pair<int, int>> possible_rings;
            if (toremove.get_player() + 3 == toremove.get_marker()) possible_rings = toremove.self_rings();
            else possible_rings = toremove.opponent_rings();
            for (pair<int, int> r : possible_rings)
            {
                if (r.first == -1 || r.second == -1) continue;
                Game readytoremove = toremove.copy_board();
                readytoremove.remove_ring(r.first, r.second);
                string move;
                if(x.second.length()){
                    move = x.second + " RS " + to_string(opt.first.first) + " " + to_string(opt.first.second) + " RE " + to_string(opt.second.first) + " " + to_string(opt.second.second) + " X " + to_string(r.first) + " " + to_string(r.second);
                }
                else move = "RS " + to_string(opt.first.first) + " " + to_string(opt.first.second) + " RE " + to_string(opt.second.first) + " " + to_string(opt.second.second) + " X " + to_string(r.first) + " " + to_string(r.second);
                pair<Game, string> out = make_pair(readytoremove, move);
                future_possibilities.push_back(out);
            }
            // ###############
//            delete toremove;
            // ################
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




vector<pair<Game, string>> remove_rows(int k, vector<pair<pair<int, int>, pair<int, int> > > removal, Game game) {
    vector<pair<Game, string>> possibilities;
    do {
        vector<pair<Game, string>> current_possibilities;
        pair<Game, string> init = make_pair(game, "");
        current_possibilities.push_back(init);
        for (pair<pair<int, int>, pair<int, int>> option: removal) {
            vector<pair<Game, string>> future_possibilities;
            for (pair<Game, string> x: current_possibilities) {
                if(x.first.get_removed1()>2 || x.first.get_removed0()>2){
                    future_possibilities.push_back(x);
                    continue;
                }
                do_remove(k, x, option, future_possibilities);
            }
            current_possibilities.clear();
            for (pair<Game, string> x:future_possibilities) {
                current_possibilities.push_back(x);
            }
            future_possibilities.clear();
        }
        for(pair<Game, string> u: current_possibilities){
            possibilities.push_back(u);
        }
    } while(next_permutation(removal.begin(), removal.end()));
    return possibilities;
};

vector<pair<pair<pair<double, Game>, vector<pair<int, int> > >, string> > get_successors(int k, vector<pair<Game, string>> newboard, vector<double> weights) {
    vector<pair<pair<pair<double, Game>, vector<pair<int, int>>>, string>> successors;
    for (pair<Game, string> ga: newboard) {
        Game gam = ga.first;
        if(gam.get_removed0()>2 || gam.get_removed1()>2){
            pair<double, Game> fir = make_pair(gam.heuristic(weights), gam);
            vector<pair<int, int>> chan;
            pair<pair<double, Game>, vector<pair<int, int>>> fin = make_pair(fir, chan);
            successors.emplace_back(fin, ga.second);
            continue;
        }
        vector<pair<int, int>> rings ;
        if(gam.get_player()==gam.get_marker()-3) rings = gam.self_rings();
        else rings = gam.opponent_rings();
        for (pair<int, int> ring: rings) {
            if (ring.first != -1 || ring.second != -1) {
                vector<pair<int, int>> paths = gam.possible_paths(ring.first, ring.second, true, true, true);
                for (pair<int, int> path: paths) {
                    Game temp = gam.copy_board();
                    temp.select_ring(ring.first, ring.second);
                    vector<pair<int, int>> changed;
                    temp.move_ring(path.first, path.second, changed);
                    // vector<pair<pair<int, int>, pair<int, int>>> removal = temp.check5(changed, temp.player + 3);
                    vector<pair<pair<int, int>, pair<int, int>>> removal = temp.checkN(changed, temp.player + 3, k);
                    string curr_move = "S " + to_string(ring.first) + " " + to_string(ring.second) + " M " + to_string(path.first) + " " + to_string(path.second);
                    if (!removal.empty()) {
                        vector<pair<Game, string>> current_possibilities = remove_rows(k, removal, temp);
                        for (pair<Game, string> g: current_possibilities) {
                            g.first.player = (g.first.player + 1) % 2;
                            double heur = g.first.heuristic(weights);
                            pair<double, Game> nextstate = make_pair(heur, g.first);
                            pair<pair<double, Game>, vector<pair<int, int>>> c = make_pair(nextstate, changed);
                            string move;
                            if(ga.second.length()) move = ga.second+ " " + curr_move + " " + g.second;
                            else move = curr_move + " " + g.second;
                            auto d = make_pair(c, move);
                            successors.push_back(d);
                        }
                        // ################
//                        delete temp;
                        // ################
                    }
                    else {
                        double heuristic = temp.heuristic(weights);
                        temp.update_player((temp.get_player() + 1) % 2);
                        pair<double, Game> curr = make_pair(heuristic, temp);
                        pair<pair<double, Game>, vector<pair<int, int>>> c = make_pair(curr, changed);
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
bool operat(pair<pair<pair<double, Game>, vector<pair<int, int> > >, string > a, pair<pair<pair<int, Game>, vector<pair<int, int> > >, string > b){
    return a.first.first.first >= b.first.first.first;
}

pair<pair<double, Game>, string> minval(int k, pair<pair<double, Game>, vector<pair<int, int> > > mygame, double alpha, double beta, int h, vector<double> weights, double abselen) {


    // vector<pair<pair<int, int>, pair<int, int>>> before_removal = mygame.first.second.check5(mygame.second, mygame.first.second.get_player() + 3);
    vector<pair<pair<int, int>, pair<int, int>>> before_removal = mygame.first.second.checkN(mygame.second, mygame.first.second.get_player() + 3,k);
    vector<pair<Game, string>> newboard;
    if (!before_removal.empty()) {
        newboard = remove_rows(k, before_removal, mygame.first.second);

    }
    else{
        pair<Game, string> curr_game = make_pair(mygame.first.second, "");
        newboard.push_back(curr_game);
    }

    vector<pair<pair<pair<double, Game>, vector<pair<int, int> > >, string > > successors = get_successors(k, newboard, weights);
    vector<pair<double, int>> forcomp;
    for(int i=0; i<successors.size(); i++){
        pair<double, int> topush = make_pair(successors.at(i).first.first.first, i);
        forcomp.push_back(topush);
    }
    if(h==1) {

        auto out = *std::min_element(forcomp.begin(), forcomp.end());
        pair<pair<double, Game>, string> output = make_pair(successors.at(out.second).first.first, successors.at(out.second).second);
        return output;
    }
    sort(forcomp.begin(), forcomp.end());
    pair<pair<double, Game>, string> besti = make_pair(make_pair(INT_MAX, successors.at(0).first.first.second), "");

    for(pair<double, int> v: forcomp){
        auto u = successors.at(v.second);
        pair<pair<int, Game>, string> child = maxval(k, u.first, alpha, beta, h - 1, weights, abselen);
        if(besti.first.first>=child.first.first){
            besti.first.first = child.first.first;
            besti.first.second = u.first.first.second;
            besti.second = u.second;
        }
        beta = min(beta, besti.first.first);
        if (alpha >= beta) {
            break;
        }
    }
    int num = (int)(drand48()*100);
    // cerr << "num: " << num << " ";
    // if((double) num / 100 > abselen) {
    //   // cerr << endl;
    //   return besti;
    // }
    // else {
    //   num = (int)(drand48() * successors.size());
    //   // cerr << num << endl;
    //   auto res = successors.at(num);
    //   pair<pair<double, Game>, string> out = {res.first.first, res.second};
    //   return out;
    // }
    return besti;
};

pair<pair<double, Game>, string> maxval(int k, pair<pair<double, Game>, vector<pair<int, int> > > mygame, double alpha, double beta, int h, vector<double> weights, double abselen) {

    // vector<pair<pair<int, int>, pair<int, int>>> before_removal = mygame.first.second.check5(mygame.second, mygame.first.second.player + 3);
    vector<pair<pair<int, int>, pair<int, int>>> before_removal = mygame.first.second.checkN(mygame.second, mygame.first.second.player + 3,k);
    vector<pair<Game, string>> newboard;
    if (!before_removal.empty()) {
        newboard = remove_rows(k, before_removal, mygame.first.second);
    }
    else {
        pair<Game, string> curr_game = make_pair(mygame.first.second, "");
        newboard.push_back(curr_game);
    }
    vector<pair<pair<pair<double, Game>, vector<pair<int, int>>>, string>> successors = get_successors(k, newboard, weights);
    vector<pair<double, int>> forcomp;
    for(int i=0; i<successors.size(); i++){
        pair<double, int> topush = make_pair(successors.at(i).first.first.first, i);
        forcomp.push_back(topush);
    }
    if(h==1) {

        auto out = *std::max_element(forcomp.begin(), forcomp.end());
        pair<pair<double, Game>, string> output = make_pair(successors.at(out.second).first.first, successors.at(out.second).second);
        return output;
    }
    sort(forcomp.begin(), forcomp.end());
    reverse(forcomp.begin(), forcomp.end());
    pair<pair<double, Game>, string> besti = make_pair(make_pair(INT_MIN, successors.at(0).first.first.second), "");
    for(pair<double, int> v: forcomp){
        pair<pair<pair<double, Game>, vector<pair<int, int>>>, string> u =  successors.at(v.second);
        pair<pair<double, Game>, string> child = minval(k, u.first, alpha, beta, h - 1, weights, abselen);
        if(besti.first.first<=child.first.first){
            besti.first.first = child.first.first;
            besti.first.second = u.first.first.second;
            besti.second = u.second;
        }
        alpha = max(alpha, besti.first.first);
        if (alpha >= beta) {
            break;
        }
    }
    // int num = rand()%100;
    // if((double) num / 100 > abselen){
    //   cerr << endl;
    //   return besti;
    // }
    // else {
    //   num = rand() % successors.size();
    //   auto res = successors.at(num);
    //   pair<pair<double, Game>, string> out = {res.first.first, res.second};
    //   return out;
    // }
    return besti;
};

string random_place(Game game){
    while (true){
        // cout << game->n << endl;
        // srand(time(NULL));
        // for (int i=0; i<100; i++)
        //   int temp = rand();

        int h = (int)(drand48() * game.get_n());
        if(h==0) continue;
        int p = (int)(drand48() * h*6);
        if(game.get_data(h, p)==0){
            return "P " + to_string(h) + " " + to_string(p);
        }
    }
    return "";
}

string centre_place(Game game)
{
    if(game.get_data(0,0)==0)
        return "P " + to_string(0) +" "+to_string(0);
    for(int i =1;i<3;i++)
    {
        for(int j=0;j<i*6;j++)
        {
            if(game.get_data(i,j)==0)
                return "P " + to_string(i) + " " + to_string(j);
        }
    }
}


int main(int argc, char** argv)
{

   // int player_id, board_size, time_limit;
  random_device rd;
  srand(3241234213);
  srand48(time(NULL));
  // for (int i=0; i<100; i++)
  //   int temp = rand();


   ifstream wfile;
   wfile.open("weights.txt");
   int num;
   double abselen;
   wfile >> abselen >> num;
   vector<double> weights;
   double tem;
   while(num--){
     wfile >> tem;
     cerr << tem <<endl;
     weights.push_back(tem);
   }

   string move;
   string s;
   // cerr << "Staring game" << endl;
   getline(cin, s);
   // cerr << "Input Taken: " << s << endl;
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
   Utility* util = new Utility(starting.at(1));
   Game game = Game(0, starting.at(0)+2, util, starting.at(1), starting.at(1), starting.at(3));
   cerr << "k: " << starting.at(3) << endl;
   // if(player_id==2){
   //     cin >> move;
   //     game->execute_move(move);

   // }


   // game->execute_move("P 0 0");
   // game->execute_move("P 1 0");
   // game->execute_move("P 1 1");
   // game->execute_move("P 1 2");
   // game->execute_move("P 1 3");
   // game->execute_move("P 1 4");
   // game->execute_move("P 1 5");
   // game->execute_move("P 2 0");
   // game->execute_move("P 2 1");
   // game->execute_move("P 2 2");
   int n = starting.at(1);
   vector<pair<int, int>> changed;
   if(starting.at(0)==2){
       for (int i=0; i<n; i++){
           getline(cin, move);
           // cout << "Move taken: " << move <<  endl;
           game.execute_move(move);
           // string mymove = random_place(game);
           string mymove = centre_place(game);
           // pair<int, int> mov = game.place_ring_heuristic();
           // string mymove = "P " + to_string(mov.first) + " " + to_string(mov.second);
           cerr << "Move " << mymove << endl;
           // cerr << mymove;
           cout << mymove << "\n";
           game.execute_move(mymove);
       }
   }
   else{
       for (int i=0; i<n; i++){
           // string mymove = random_place(game);
           string mymove = centre_place(game);
           // cerr << mymove << endl;
           // pair<int, int> mov = game.place_ring_heuristic();
           // string mymove = "P " + to_string(mov.first) + " " + to_string(mov.second);

           cout << mymove << endl;
           game.execute_move(mymove);
           getline(cin, move);
           changed = game.execute_move(move);
       }
   }

   // cerr << endl;
   if(starting.at(0)==2){
       getline(cin, move);
       changed = game.execute_move(move);
   }
   int ply=3;
   int t=0;
   ofstream myfile;
   string filename = "logs_"+to_string(starting.at(0))+".txt";
   myfile.open(filename);

   while(true){

       int reward = 0;
       int prev_removed = game.self_removed();
       pair<double, Game> inp = make_pair(game.heuristic(weights), game);
       pair<pair<int, Game>, vector<pair<int, int>>> taken = make_pair(inp, changed);
       pair<pair<int, Game>, string> mymove = maxval(starting.at(3), taken, INT_MIN, INT_MAX, ply, weights, abselen);
       // cerr << endl << endl;
       cout << mymove.second << endl;
       game.execute_move(mymove.second);
       reward+=(game.self_removed() - prev_removed);
       t++;
       // cerr << ply << endl;
       if(t==15) ply=4;
        // if(t==15) ply = 5;
        // if(t==30) ply=6;
       if(game.check_won()){
         reward+=7;
         pair<vector<int>, vector<int>> features = game.get_features();
         for(auto u: features.first) {
           myfile << u << " ";
         }
         for(auto u: features.second) {
           myfile << u << " ";
         }

         myfile << reward << endl;
         break;
       }
       getline(cin, move);
       // cerr<< "Checking final" << endl;
       // cout << "Move taken: " << move <<  endl;
       prev_removed = game.opp_removed();
       changed = game.execute_move(move);
       reward-=(game.opp_removed() - prev_removed);
       if(game.check_won()){
         reward+=3;
         auto features = game.get_features();
         for(auto u: features.first) {
           myfile << u << " ";
         }
         for(auto u: features.second) {
           myfile << u << " ";
         }

         myfile << reward << endl;
         break;
       }
       auto features = game.get_features();
       for(auto u: features.first) {
         myfile << u << " ";
       }
        for(auto u: features.second) {
           myfile << u << " ";
         }

       myfile << reward << endl;
   }
   myfile.close();

}
