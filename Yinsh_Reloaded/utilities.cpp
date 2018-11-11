#include <bits/stdc++.h>
using namespace std;

class Node{
public:
    int data;
    pair<int, int> index;
    vector<pair<int, int>> neighbours;
    vector<pair<int,int>> axis_mapping;
    Node(){
        this->data = 0;
        this->index.first = -1;
        this->index.second = -1;
        for(int i=0;i<3;i++)
            this->axis_mapping.push_back(make_pair(0,0));

        for(int i=0 ;i<6; i++){
            pair<int, int> temp;
            temp.first = -1;
            temp.second = -1;
            this->neighbours.push_back(temp);
        }
    }
};

class Utility{
public:

	vector<vector<pair<int, int>>> elems_on_diagonal1;
	vector<vector<pair<int, int>>> elems_on_diagonal2;
	vector<vector<pair<int, int>>> elems_on_vertical;
	int n;
	vector<vector<Node*>> board;
  pair<vector<int>,vector<int>> consecutive_weights;
    vector<vector<vector<vector<int>>>> distances;
	Utility(int n=5){
		vector<pair<pair<int,int>,pair<int,int>>> diagonal1;
		vector<pair<pair<int,int>,pair<int,int>>> diagonal2;
		vector<pair<pair<int,int>,pair<int,int>>> vertical;
		vector<Node*> tem;
		this->n = n;
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

        initialize_consecutive_weights(this->consecutive_weights);
        update_direction_vectors(diagonal1, diagonal2, vertical);

        elems_on_diagonal1 = elements_on_axis(diagonal1);
        elems_on_diagonal2 = elements_on_axis(diagonal2);
        elems_on_vertical = elements_on_axis(vertical);
        update_board(elems_on_diagonal1,0);
        update_board(elems_on_diagonal2,1);
        update_board(elems_on_vertical,2);
        for(int i =0;i<6;i++)
        {
        	Node* curr = board.at(n).at(n*i);
        	for(int j = 0;j<3;j++)
        		curr->axis_mapping.at(j) = make_pair(-1,-1);
        }
        initialize_distances();
        // update_distances(elems_on_diagonal1,elems_on_diagonal2,elems_on_vertical);
	}

  pair<int,int> common_on_axis(vector<pair<int,int>> axis1, vector<pair<int,int>> axis2)
  {
        pair<int,int> common = make_pair(-1,-1);

        for(auto u : axis1)
        {
            for(auto v : axis2)
            {
                if(u == v)
                {
                    common = u;
                }
            }
        }
        return common;
  }

  int calculate_distance(pair<int,int> point1, pair<int,int> point2)
  {
        vector<pair<int,int>> axes1 = board.at(point1.first).at(point1.second)->axis_mapping;
        vector<pair<int,int>> axes2 = board.at(point2.first).at(point2.second)->axis_mapping;
        //axis.at.first = axis in that direction
        //axis.at.second = position in that axis
        int d1_1 = axes1.at(0).first, d2_1 = axes1.at(1).first, v_1 = axes1.at(2).first;
        int d1_2 = axes2.at(0).first, d2_2 = axes2.at(1).first, v_2 = axes2.at(2).first;

        if(d1_1 == d1_2 || d2_1 == d2_2 || v_1 == v_2)
        {
            return ((d1_1 == d1_2)*abs(axes1.at(0).second - axes2.at(0).second)  +  (d2_1 == d2_2)*abs(axes1.at(1).second - axes2.at(1).second)  +  (v_1 == v_2)*abs(axes1.at(2).second - axes2.at(2).second));
        }
        else
        {
            //taking d1 of first point and d2 and v of other points
            int d1_d2 = -1, d1_v = -1;
            vector<pair<int,int>> d1_p1 = elems_on_diagonal1.at(d1_1);
            vector<pair<int,int>> d2_p2 = elems_on_diagonal2.at(d2_2);
            vector<pair<int,int>> v_p2 = elems_on_vertical.at(v_2);

            pair<int,int> common;
            vector<pair<int,int>> axes_com;
            common = common_on_axis(d1_p1,d2_p2);
            axes_com = board.at(common.first).at(common.second)->axis_mapping;
            if(common.first != -1 && common.second != -1)
            {
                d1_d2 = abs(axes1.at(0).second - axes_com.at(0).second) + abs(axes2.at(1).second - axes_com.at(1).second);
            }
            common = common_on_axis(d1_p1,v_p2);
            axes_com = board.at(common.first).at(common.second)->axis_mapping;
            if(common.first != -1 && common.second != -1)
            {
                d1_v = abs(axes1.at(0).second - axes_com.at(0).second) + abs(axes2.at(2).second - axes_com.at(2).second);
            }

            //taking d2 of first point and d1 and v of other points
            int d2_d1 = -1, d2_v = -1;
            vector<pair<int,int>> d2_p1 = elems_on_diagonal2.at(d2_1);
            vector<pair<int,int>> d1_p2 = elems_on_diagonal1.at(d1_2);
            // vector<pair<int,int>> v_p2 = elems_on_vertical.at(v_2);

            // pair<int,int> common;
            // vector<pair<int,int>> axes_com;
            common = common_on_axis(d2_p1,d1_p2);
            axes_com = board.at(common.first).at(common.second)->axis_mapping;
            if(common.first != -1 && common.second != -1)
            {
                d2_d1 = abs(axes1.at(1).second - axes_com.at(1).second) + abs(axes2.at(0).second - axes_com.at(0).second);
            }
            common = common_on_axis(d2_p1,v_p2);
            axes_com = board.at(common.first).at(common.second)->axis_mapping;
            if(common.first != -1 && common.second != -1)
            {
                d2_v = abs(axes1.at(1).second - axes_com.at(1).second) + abs(axes2.at(2).second - axes_com.at(2).second);
            }

            //taking v of first point and d2 and d1 of other points
            int v_d1 = -1, v_d2 = -1;
            vector<pair<int,int>> v_p1 = elems_on_vertical.at(v_1);
            // vector<pair<int,int>> d1_p2 = elems_on_diagonal2.at(d1_2);
            // vector<pair<int,int>> d2_p2 = elems_on_vertical.at(d2_2);

            // pair<int,int> common;
            // vector<pair<int,int>> axes_com;
            common = common_on_axis(v_p1,d1_p2);
            axes_com = board.at(common.first).at(common.second)->axis_mapping;
            if(common.first != -1 && common.second != -1)
            {
                v_d1 = abs(axes1.at(2).second - axes_com.at(2).second) + abs(axes2.at(0).second - axes_com.at(0).second);
            }
            common = common_on_axis(v_p1,d2_p2);
            axes_com = board.at(common.first).at(common.second)->axis_mapping;
            if(common.first != -1 && common.second != -1)
            {
                v_d2 = abs(axes1.at(2).second - axes_com.at(2).second) + abs(axes2.at(1).second - axes_com.at(1).second);
            }
            return min({d1_d2,d1_v,d2_d1,d2_v,v_d1,v_d2});
        }

  }
  void initialize_consecutive_weights(pair<vector<int>,vector<int>> &consecutive_weights)
  {
      vector<int> first,second;
      for(int i=0;i<9;i++)
      {
          if(i<4)
          {
              first.push_back((int) pow(2,i+1));
              second.push_back((int) pow(2,i+1));
          }
          else
          {
              first.push_back((int) pow(2,i-3) + 16);
              second.push_back((int) pow(2,i-3) + 16);
          }
      }
      consecutive_weights.first = first;
      consecutive_weights.second = second;
  }


  vector<vector<int>> get_empty(){
    vector<vector<int>> out;
    vector<int> tem;
    out.push_back(tem);

    for(int i=1; i<=n; i++){
        vector<int> curr(i*6);
        out.push_back(curr);
    }
    return out;

  }

  void initialize_distances(){

    vector<vector<vector<int>>> tem;
    auto s = get_empty();
    tem.push_back(s);
    distances.push_back(tem);

    for(int i=1; i<=n; i++){
        vector<vector<vector<int>>> temp;
        for(int j=0; j<i*6; j++){
            auto s = get_empty();
            temp.push_back(s);
        }
        distances.push_back(temp);
    }

    for (int i=0; i<distances.size(); i++){
        auto curr = distances.at(i);
        for (int j=0; j<curr.size(); j++){
            auto ins = curr.at(j);
            for (int k=0; k<ins.size(); k++){
                auto inins = ins.at(k);
                for(int l=0; l<inins.size(); l++){
                    distances[i][j][k][l] = calculate_distance(make_pair(i, j), make_pair(k, l));
                }
            }
        }
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
    void update_direction_vectors(vector<pair<pair<int,int>,pair<int,int>>> &diagonal1, vector<pair<pair<int,int>,pair<int,int>>> &diagonal2, vector<pair<pair<int,int>,pair<int,int>>> &vertical)
	{
	    diagonal1.push_back(make_pair(make_pair(n,1),make_pair(n,2)));
	    diagonal1.push_back(make_pair(make_pair(n,6*n-1),make_pair(n-1,0)));
	    for(int i=6*n-2;i>4*n;i--)
	    {
	        if(i%n == 0)
	        {
	            diagonal1.push_back(make_pair(make_pair(n-1,i-5),make_pair(n-2,i-10)));
	            continue;
	        }
	        diagonal1.push_back(make_pair(make_pair(n,i),make_pair(n-1,i-5)));
	    }
	    diagonal1.push_back(make_pair(make_pair(n,4*n-1),make_pair(n,4*n-2)));

	    diagonal2.push_back(make_pair(make_pair(n,6*n-1),make_pair(n,6*n-2)));
	    for(int i=1;i<2*n;i++)
	    {
	        if(i%n==0)
	        {
	            diagonal2.push_back(make_pair(make_pair(n-1,i-1),make_pair(n-2,i-2)));
	            continue;
	        }
	        diagonal2.push_back(make_pair(make_pair(n,i),make_pair(n-1,i-1)));
	    }
	    diagonal2.push_back(make_pair(make_pair(n,2*n+1),make_pair(n,2*n+2)));

	    vertical.push_back(make_pair(make_pair(n,5*n-1),make_pair(n,5*n-2)));
	    for(int i = 5*n+1;i<6*n;i++)
	        vertical.push_back(make_pair(make_pair(n,i),make_pair(n-1,i-6)));

	    vertical.push_back(make_pair(make_pair(n-1,0),make_pair(n-2,0)));

	    for(int i=1;i<n;i++)
	        vertical.push_back(make_pair(make_pair(n,i),make_pair(n-1,i)));

	    vertical.push_back(make_pair(make_pair(n,n+1),make_pair(n,n+2)));

	}

	vector<vector<pair<int,int>>> elements_on_axis(vector<pair<pair<int,int>,pair<int,int>>> directions)
    {
        vector<vector<pair<int,int>>> on_axes;
        for(auto w: directions)
        {
            vector<pair<int,int>> in_axis;
            in_axis.push_back(w.first);
            in_axis.push_back(w.second);

            int r1 = w.first.first, c1 = w.first.second, r2 = w.second.first, c2 = w.second.second;
            while(true)
            {
                //generating next node along the line
                Node* curr = board.at(r2).at(c2);
                int prev_n = 0;
                for(auto n: curr->neighbours)
                {
                    int rt = n.first;
                    int ct = n.second;
                    if(rt == r1 && ct == c1)
                        break;
                    prev_n++;
                }
                int nxt_n = (prev_n+3)%6;
                int r_nxt = curr->neighbours.at(nxt_n).first;
                int c_nxt = curr->neighbours.at(nxt_n).second;
                r1 = r2;
                c1 = c2;
                r2 = r_nxt;
                c2 = c_nxt;

                // cout << prev_n << "\t";

                if(r2 == -1 || c2 == -1)
                    break;
                in_axis.push_back(make_pair(r2,c2));
            }
            on_axes.push_back(in_axis);
        }
        return on_axes;
    }
    void update_board(vector<vector<pair<int, int>>> &axis, int dirn)
    {
    	int i = 0;
	    for(auto w:axis)
	    {
	        int j = 0;
	        // cout << "AXIS" << i << ":\t";
	        for(auto u:w)
	        {
	            // cout << "(" << u.first << "," << u.second << ")" << " -> ";
	            board.at(u.first).at(u.second)->axis_mapping.at(dirn).first = i;
	            board.at(u.first).at(u.second)->axis_mapping.at(dirn).second = j++;
	        }
	        i++;
	        // cout << endl;
	    }
    }

    vector<pair<int,int>> between_points(pair<int,int> pt1, pair<int,int> pt2)
    {
    	vector<pair<int,int>> point1 = board.at(pt1.first).at(pt1.second)->axis_mapping;
    	vector<pair<int,int>> point2 = board.at(pt2.first).at(pt2.second)->axis_mapping;

    	vector<pair<int,int>> between;
    	int direction = -1;
    	for(int i =0;i<3;i++)
    	{
    		if(point1.at(i).first == point2.at(i).first)
    		{
    			direction = i;
    			break;
    		}
    	}
    	if (direction == -1)
    		return between;

    	int axis = point1.at(direction).first;
    	int st = min(point1.at(direction).second , point2.at(direction).second);
    	int en = max(point1.at(direction).second , point2.at(direction).second);

    	for(int i = st; i <= en ; i++)
    	{
    		if(direction == 0)
    			between.push_back(elems_on_diagonal1.at(axis).at(i));
    		if(direction == 1)
    			between.push_back(elems_on_diagonal2.at(axis).at(i));
    		if(direction == 2)
    			between.push_back(elems_on_vertical.at(axis).at(i));
    	}
    	return between;
    }
};

int main()
{
	  // Utility* use = new Utility();
    // cout << "Weights 3:" << endl;
    // for(auto w:use->consecutive_weights.first)
    // {
    //     cout << w << "\t";
    // }
    // cout << endl;
    // cout << "Weights 4:" << endl;
    // for(auto w:use->consecutive_weights.second)
    // {
    //     cout << w << "\t";
    // }
    // cout << endl;
    // cout << "n=5" << endl;
    // cout << "vertical :" << endl;
    // for (auto u: use->elems_on_vertical){
    //     for (auto v: u){
    //         cout << v.first << " " << v.second << " ";
    //     }
    //     cout << endl;
    // }
    // cout << endl;
    // cout << "diagonal1 :" << endl;
    // for (auto u: use->elems_on_diagonal1){
    //     for (auto v: u){
    //         cout << v.first << " " << v.second << " ";
    //     }
    //     cout << endl;
    // }
    // cout << endl;
    // cout << "diagonal2 :" << endl;
    // for (auto u: use->elems_on_diagonal2){
    //     for (auto v: u){
    //         cout << v.first << " " << v.second << " ";
    //     }
    //     cout << endl;
    // }
    // cout << endl;

    Utility* use = new Utility(5);
  //   cout << "n=6" << endl;
  //   cout << "vertical :" << endl;
  //   for (auto u: use->elems_on_vertical){
  //       for (auto v: u){
  //           cout << v.first << " " << v.second << " ";
  //       }
  //       cout << endl;
  //   }
  //   cout << endl;
  //   cout << "diagonal1 :" << endl;
  //   for (auto u: use->elems_on_diagonal1){
  //       for (auto v: u){
  //           cout << v.first << " " << v.second << " ";
  //       }
  //       cout << endl;
  //   }
  //   cout << endl;
  //   cout << "diagonal2 :" << endl;
  //   for (auto u: use->elems_on_diagonal2){
  //       for (auto v: u){
  //           cout << v.first << " " << v.second << " ";
  //       }
  //       cout << endl;
  //   }
  //   cout << endl;
  //
	// cout << "works";

}
