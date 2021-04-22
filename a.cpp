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
#include "player.h"
using namespace std;
//  mt19937 rng(chrono::steady_clock::now().time_since_epoch().count());
//  #define random_shuffle(...) shuffle(__VA_ARGS__, rng)

//  int random(int a, int b)
//  {
	//  assert(a <= b);
	//  // return a + rand()%(b-a+1);
	//  return uniform_int_distribution<int>(a,b)(rng);
//  }

typedef long long ll;
typedef unsigned long long ull;
typedef long double ld;
typedef vector <int> vi;
typedef pair<int,int> PII;
typedef pair<ll,ll> PLL;
constexpr ll nax = 1e6+6969, INF = 2e9+6969;

player me, they;
ld TIME_PER_MOVE = 30.0; // time that ai will take to choose a move (in seconds)
bool chosen_mode = 1; // 0 -> human vs ai, 1 -> ai with itself


int main()
{
	bool turn = 0;
	
	G.__init__(chosen_mode);
	//  they.G.__init__();
	
	me.__init__();
	they.__init__();
	// me plays red and they plays blue (if ai vs ai)
	
	G.show();
	while(true)
	{
		turn = G.turn;
		if(G.turn == G.my_id)
		{
			int tmp=0;
			clock_t ST = clock();
			PII where = me.search_move(TIME_PER_MOVE);
			clock_t ED = clock();
			cerr << "move took " << setprecision(5) << fixed << (ld)(ED - ST) / CLOCKS_PER_SEC << " seconds\n";
			string Move = G.conv_move(where);
			cout << Move << "\n";
			tmp = G.make(where.fi, where.se, turn);
			if(tmp > 1) G.declare_winner(turn);
			if(tmp == 0)
			{
				cerr << "resign\n";
				G.declare_winner(turn ^ 1);
			}
		}
		
		//  // option to play ai alone
		else if(chosen_mode)
		{
			int tmp=0;
			clock_t ST = clock();
			PII where = they.search_move(TIME_PER_MOVE);
			clock_t ED = clock();
			cerr << "move took " << setprecision(5) << fixed << (ld)(ED - ST) / CLOCKS_PER_SEC << " seconds\n";
			string Move = G.conv_move(where);
			cout << Move << "\n";
			tmp = G.make(where.fi, where.se, turn);
			if(tmp > 1) G.declare_winner(turn);
			if(tmp == 0)
			{
				cerr << "resign\n";
				G.declare_winner(turn ^ 1);
			}
		}
		
		else G.let_make_move(); // option to play with human 
		
		G.show();
	}
}
