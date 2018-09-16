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
	    //Diagonal 1
//	    cout << "Entered";
	    diagonal1.push_back(make_pair(make_pair(5,1),make_pair(5,2)));
	    diagonal1.push_back(make_pair(make_pair(5,29),make_pair(4,0)));
	    for(int i=28;i>20;i--)
	    {
	        if(i%5 == 0)
	        {
	            diagonal1.push_back(make_pair(make_pair(4,i-5),make_pair(3,i-10)));
	            continue;
	        }
	        diagonal1.push_back(make_pair(make_pair(5,i),make_pair(4,i-5)));
	    }
	    diagonal1.push_back(make_pair(make_pair(5,19),make_pair(5,18)));
	    //Diagonal 1

	    //Diagonal 2
	    diagonal2.push_back(make_pair(make_pair(5,29),make_pair(5,28)));
	    for(int i=1;i<10;i++)
	    {
	        if(i%5==0)
	        {
	            diagonal2.push_back(make_pair(make_pair(4,i-1),make_pair(3,i-2)));
	            continue;
	        }
	        diagonal2.push_back(make_pair(make_pair(5,i),make_pair(4,i-1)));
	    }
	    diagonal2.push_back(make_pair(make_pair(5,11),make_pair(5,12)));
	    //Diagonal 2

	    //Vertical
	    vertical.push_back(make_pair(make_pair(5,24),make_pair(5,23)));
	    for(int i = 26;i<30;i++)
	        vertical.push_back(make_pair(make_pair(5,i),make_pair(4,i-6)));

	    vertical.push_back(make_pair(make_pair(4,0),make_pair(3,0)));

	    for(int i=1;i<5;i++)
	        vertical.push_back(make_pair(make_pair(5,i),make_pair(4,i)));

	    vertical.push_back(make_pair(make_pair(5,6),make_pair(5,7)));
	    //Vertical
//	    cout << "Exited";
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
	Utility* use = new Utility();
//	cout << "works";
	 cout << "(" << 0 << " , " << 0 << ") -> " << "(" <<  "(" << use->board.at(0).at(0)->axis_mapping.at(0).first << "," << use->board.at(0).at(0)->axis_mapping.at(0).second << ")" << "," << "(" << use->board.at(0).at(0)->axis_mapping.at(1).first << "," << use->board.at(0).at(0)->axis_mapping.at(1).second << ")," << "(" << use->board.at(0).at(0)->axis_mapping.at(2).first << "," << use->board.at(0).at(0)->axis_mapping.at(2).second << "))\n";
     for(int x = 1;x<6;x++)
     {
         for(int y = 0; y<6*x;y++)
         {
             if(x==5 && y%5==0)
                 continue;
             vector<pair<int,int>> mappings = use->board.at(x).at(y)->axis_mapping;
             cout << "(" << x << " , " << y << ") -> " << "(" <<  "(" << mappings.at(0).first << "," << mappings.at(0).second << ")" << "," << "(" << mappings.at(1).first << "," << mappings.at(1).second << ")," << "(" << mappings.at(2).first << "," << mappings.at(2).second << "))\n";

         }
     }
}