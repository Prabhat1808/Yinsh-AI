#include <bits/stdc++.h>
#include "Game.cpp"

using namespace std;

pair<int, Game> minval(pair<int, Game> mygame, int alpha, int beta, int h){
    if(h==0) return mygame;
    vector<pair<int, Game>> successors;
    vector<pair<int, int>> rings = mygame.second.opponents_rings();
    for(pair<int, int> ring: rings){
        if (ring.first!=-1 || ring.second!=-1){
            vector<pair<int, int>> paths = mygame.second.possible_paths(ring.first, ring.second);
            for(pair<int, int> path: paths) {
                Game temp = mygame.second.copy_board();
                temp.select_ring(ring.first, ring.second);
                vector<pair<int, int>> changed;
                temp.move_ring(path.first, path.second, changed);
//                vector<pair<pair<int, int>, pair<int, int>>> removal = temp.check5(changed);
                int heuristic = temp.heuristic();
                pair<int, Game> curr = make_pair(heuristic, temp);
                successors.push_back(curr);
            }
        }
    }

    sort(successors.begin(), successors.end(), greater<>());
    for (pair<int, Game> u: successors){
        pair<int, Game> child = minval(u, alpha, beta, h-1);
        beta = max(alpha, child.first);
        if(alpha>=beta) return child;
    }
    return successors.at(successors.size()-1);
}

pair<int, Game> maxval( pair<int, Game> mygame, int alpha, int beta, int h){
    if(h==0) return mygame;
    vector<pair<int, Game>> successors;
    vector<pair<int, int>> rings = mygame.second.my_rings();
    for(pair<int, int> ring: rings){
        if (ring.first!=-1 || ring.second!=-1){
            vector<pair<int, int>> paths = mygame.second.possible_paths(ring.first, ring.second);
            for(pair<int, int> path: paths) {
                Game temp = mygame.second.copy_board();
                temp.select_ring(ring.first, ring.second);
                vector<pair<int, int>> changed;
                temp.move_ring(path.first, path.second, changed);
//                vector<pair<pair<int, int>, pair<int, int>>> removal = temp.check5(changed);
                int heuristic = temp.heuristic();
                pair<int, Game> curr = make_pair(heuristic, temp);
                successors.push_back(curr);
            }
        }
    }

    sort(successors.begin(), successors.end());
    for (pair<int, Game> u: successors){
        pair<int, Game> child = minval(u, alpha, beta, h-1);
        alpha = max(alpha, child.first);
        if(alpha>=beta) return child;
    }
    return successors.at(successors.size()-1);
}

int main(){
    Game mygame(0, 3);


}