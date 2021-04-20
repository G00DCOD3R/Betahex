#define pb push_back
#define mp make_pair
#define fi first
#define se second 
#define all(...) begin(__VA_ARGS__) , end(__VA_ARGS__)
#define boost {ios_base::sync_with_stdio(false); cin.tie(); cout.tie();}

#include <bits/stdc++.h>
using namespace std;

typedef long long ll;
typedef unsigned long long ull;
typedef long double ld;
typedef vector <int> vi;
typedef pair<int,int> PII;
typedef pair<ll,ll> PLL;

class game
{
	public:
		static const int N = 11;
		int br[N+1][N+1];
		int vis[N+1][N+1], nrv = 1, moves_made = 0;
		stack <PII> mvs;
		bool my_id, op_id, turn=0;
		bool good(int x, int y);
		void __init__();
		void show();
		void declare_winner(bool who);
		int make(int x, int y, bool who);
		void undo_move();
		void let_make_move();
};

//  extern game G;

//  class player
//  {
	//  private: 
		//  vector <vector <pair<PII, int>>> v;
		//  unordered_map <int,int> term_nodes;
		//  int n, r[2][1000069];
		//  int moves_ahead;
		//  int random(int a, int b);
		
	//  public: 
		//  // decision_making
		//  int keep_close_policy;
		//  ld UCT_LOG_MULT;
		//  int MOVE_IGNORE;
		//  int ROLL_OUT_NR;
		
		//  // functions
		//  void __init__();
		//  PII getr_child(int & m);
		//  PII search_move(ld tm_for_move);
		//  int rollout();
		//  void get_path(int node, vi & path);
		//  void run(int nroll);
		//  pair<PII, int> uct_sel(int node);
//  };
