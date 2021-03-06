#define pb push_back
#define mp make_pair
#define fi first
#define se second 
#define all(...) begin(__VA_ARGS__) , end(__VA_ARGS__)
#define boost {ios_base::sync_with_stdio(false); cin.tie(); cout.tie();}

#include <bits/stdc++.h>
#ifndef gameh
#define gameh 1
#include "game.h"
#endif
using namespace std;

typedef long long ll;
typedef unsigned long long ull;
typedef long double ld;
typedef vector <int> vi;
typedef pair<int,int> PII;
typedef pair<ll,ll> PLL;

extern game G;

class player
{
	private: 
		vector <vector <pair<PII, int>>> v;
		unordered_map <int,int> term_nodes;
		int n, r[2][1000069];
		set <PII> inv_moves[10000069];
		int moves_ahead;
		int random(int a, int b);
		ld beval[1000069];
		
	public: 
		// decision_making
		int keep_close_policy;
		ld UCT_LOG_MULT;
		int MOVE_IGNORE;
		int ROLL_OUT_NR;
		ld UCT_EVAL_MULT;
		
		// functions
		void __init__();
		PII getr_child(int & m);
		PII search_move(ld tm_for_move);
		int rollout();
		void get_path(int node, vi & path);
		void run(int nroll);
		pair<PII, int> uct_sel(int node);
};
