#define pb push_back
#define mp make_pair
#define fi first
#define se second 
#define all(...) begin(__VA_ARGS__) , end(__VA_ARGS__)
#define boost {ios_base::sync_with_stdio(false); cin.tie(); cout.tie();}

#include <bits/stdc++.h>
#include "game.h"
#include "player.h"
using namespace std;
mt19937 rng(chrono::steady_clock::now().time_since_epoch().count());
#define random_shuffle(...) shuffle(__VA_ARGS__, rng)

int random(int a, int b)
{
	assert(a <= b);
	// //  return a + rand()%(b-a+1);
	return uniform_int_distribution<int>(a,b)(rng);
}

typedef long long ll;
typedef unsigned long long ull;
typedef long double ld;
typedef vector <int> vi;
typedef pair<int,int> PII;
typedef pair<ll,ll> PLL;
constexpr ll nax = 1e6+6969, INF = 2e9+6969;

game G;

struct strat
{
	int a,b,c; // 80 93 200
	ld d; // 2.0
	void show()
	{
		cout << a << " " << b << " " << c << " " << d << "\n";
	}
	void def()
	{
		a = 81; b = 91; c = 310;
		d = 3.14;
	}
	void insert(int A, int B, int C, ld D)
	{
		a = A; b = B; c = C; d = D;
	}
};
strat gen_similar(strat cur)
{
	strat w;
	w.a = cur.a + random(max(-cur.a + 1, -20), min(100 - cur.a, 20));
	w.b = cur.b + random(max(-cur.b + 1, -20), min(100 - cur.b, 20));
	w.c = cur.c + random(max(-cur.c + 1, -100), 100);
	ld tmp = cur.d * 100;
	w.d = cur.d + ((ld)random(max(-(int)tmp, -100), 100) / 100);
	return w;
}
strat new_random()
{
	strat w;
	w.a = random(30, 99);
	w.b = random(90, 99);
	w.c = random(80, 1000);
	w.d = (ld)random(1, 900) / 100;
	return w;
}
void __init__(strat cur, player & who)
{
	who.keep_close_policy = cur.a;
	who.UCT_LOG_MULT = cur.d;
	who.MOVE_IGNORE = cur.b;
	who.ROLL_OUT_NR = cur.c;
}

player me, they;
ld TIME_PER_MOVE = 15.0; // time that ai will take to choose a move

bool one_game(strat one, strat two)
{
	bool turn = 0;
	
	G.__init__();
	
	__init__(one, me);
	__init__(two, they);
	// me plays red and they plays blue
	
	//  G.show();
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
			string row;
			if(where.se < 10) row = (char)(where.se + '0');
			else 
			{
				int dig = where.se / 10;
				row = (char)(dig + '0');
				dig = where.se % 10;
				row = row + (char)(dig + '0');
			}
			cout << (char)(where.fi+'A'-1) << row << " \n";
			tmp = G.make(where.fi, where.se, turn);
			if(tmp > 1)
			{
				return turn;
				G.declare_winner(turn);
			}
			if(tmp == 0)
			{
				return turn ^ 1;
				cerr << "resign\n";
				G.declare_winner(turn ^ 1);
			}
		}
		
		//  option to play ai alone
		else
		{
			int tmp=0;
			clock_t ST = clock();
			PII where = they.search_move(TIME_PER_MOVE);
			clock_t ED = clock();
			cerr << "move took " << setprecision(5) << fixed << (ld)(ED - ST) / CLOCKS_PER_SEC << " seconds\n";
			string row;
			if(where.se < 10) row = (char)(where.se + '0');
			else 
			{
				int dig = where.se / 10;
				row = (char)(dig + '0');
				dig = where.se % 10;
				row = row + (char)(dig + '0');
			}
			cout << (char)(where.fi+'A'-1) << row << " \n";
			tmp = G.make(where.fi, where.se, turn);
			if(tmp > 1)
			{
				return turn;
				G.declare_winner(turn);
			}
			if(tmp == 0)
			{
				return turn ^ 1;
				cerr << "resign\n";
				G.declare_winner(turn ^ 1);
			}
		}
		
		//  else G.let_make_move(); // option to play with human 
		
		G.show();
	}
	//  G.show();
}

strat mn,other;

int main()
{
	//  mn.def();
	//  for(int rep = 0;; rep++)
	//  {
		//  if(rep % 5 != 0) other = gen_similar(mn);
		//  else other = new_random();
		//  mn.show();
		//  other.show();
		//  bool who = one_game(mn, other);
		//  if(who) mn = other;
		//  mn.show();
		
	//  }
	//  cout << "current winner: \n";
	//  mn.show();
	mn.insert(98, 90, 308, 0.46);
	other.insert(81, 91, 280, 3.19);
	mn.show();
	other.show();
	
	one_game(mn, other);
	
}


// some basic strats: 
	//  72 93 135 1.38
	//  81 91 280 3.19
	//  66 94 77 2.34
	//  98 90 308 0.46
	
