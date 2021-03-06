#define pb push_back
#define mp make_pair
#define fi first
#define se second 
#define all(...) begin(__VA_ARGS__) , end(__VA_ARGS__)
#define boost {ios_base::sync_with_stdio(false); cin.tie(); cout.tie();}

#include <bits/stdc++.h>
#include "game.h"
using namespace std;
mt19937 rng(chrono::steady_clock::now().time_since_epoch().count());
#define random_shuffle(...) shuffle(__VA_ARGS__, rng)

typedef long long ll;
typedef unsigned long long ull;
typedef long double ld;
typedef vector <int> vi;
typedef pair<int,int> PII;
typedef pair<ll,ll> PLL;

//  game G;

bool game::good(int x, int y)
{
	if(x < 1 || x > N || y < 1 || y > N || br[x][y] != 0) return false;
	return true;
}
void game::__init__(bool Mode)
{
	for(int i=0;i<=N;i++) for(int j=0;j<=N;j++) br[i][j] = vis[i][j] = 0;
	while(mvs.size() > 0) mvs.pop();
	moves_made = 0;
	nrv = 1;
	turn = 0;
	
	// option to play with ai
	if(!Mode)
	{
		puts("Which color do you choose?");
		puts("0 - Red (First)");
		puts("1 - Blue (Second)");
		cin >> op_id;
	}
	
	//  option to play ai alone
	else op_id = 1;
	
	my_id = op_id ^ 1;
}
void game::show()
{
	fprintf(stderr, "      \033[94m");
	for(int i=1;i<=N;i++) fprintf(stderr, "(%d) ", i);
	fprintf(stderr, "\033[0m");
	fprintf(stderr, "\n");
	fprintf(stderr, "     ");
	for(int j=0;j<N;j++) fprintf(stderr, " / \\");
	fprintf(stderr, "\n");
	for(int i=1;i<=N;i++)
	{
		for(int j=0;j<i*2;j++) fprintf(stderr, " ");
		fprintf(stderr, "\033[91m(%c)\033[0m|", (char)(i+'A'-1));
		for(int j=1;j<=N;j++)
		{
			if(br[i][j] == 1) fprintf(stderr, " \033[91mR\033[0m |");
			else if(br[i][j] == 2) fprintf(stderr, " \033[94mB\033[0m |");
			else fprintf(stderr, " . |");
			
		}
		fprintf(stderr, "\n");
		fprintf(stderr, "   ");
		for(int j=0;j<i*2;j++) fprintf(stderr, " ");
		for(int j=0;j<N;j++) fprintf(stderr, " \\ /");
		if(i != N) fprintf(stderr, " \\");
		fprintf(stderr, "\n");
	}
	fprintf(stderr, "\n");
}

int game::make(int x, int y, bool who)
{
	
	//  CODES:
	//  0 - invalid
	//  1 - valid
	//  2 - I won
	//  3 - Opponent won
	
	int col = 1 + who;
	if(!good(x,y)) return 0; // wrong move;
	moves_made++;
	turn ^= 1;
	mvs.push(mp(x,y));
	br[x][y] = col;
	bitset <4> t;
	t[0] = t[1] = t[2] = t[3] = 0;
	vi X{-1, -1, 0, 0, 1, 1}, Y{0, 1, -1, 1, -1, 0};
	if(nrv >= 1000000000)
	{
		for(int i=1;i<=N;i++) for(int j=1;j<=N;j++) vis[i][j] = 0;
		nrv = 1;
	}
	queue <PII> q;
	q.push(mp(x, y));
	nrv++;
	vis[x][y] = nrv;
	while(!q.empty())
	{
		PII cur = q.front();
		q.pop();
		if(cur.fi == 1) t[0] = 1;
		if(cur.fi == N) t[2] = 1;
		if(cur.se == 1) t[1] = 1;
		if(cur.se == N) t[3] = 1;
		if(t[0]==1 && t[2]==1 && col == 2) return 3 - my_id;
		if(t[1]==1 && t[3]==1 && col == 1) return 2 + my_id;
		for(int op = 0; op < 6; op++)
		{
			int a = cur.fi + X[op], b = cur.se + Y[op];
			if(a < 1 || a > N || b < 1 || b > N || vis[a][b] == nrv || br[a][b] != col) continue;
			vis[a][b] = nrv;
			q.push(mp(a,b));
		}
	}
	return 1;
}
void game::undo_move()
{
	turn ^= 1;
	PII tmp = mvs.top();
	mvs.pop();
	br[tmp.fi][tmp.se] = 0;
	moves_made--;
}
void game::let_make_move()
{
	fprintf(stderr, "Your move\n");
	int m = 0;
	while(m == 0) 
	{
		string s;
		cin >> s;
		if(s.size() != 2 && s.size() != 3)
		{
			fprintf(stderr, "Invalid move\nvalid move e.g. A1");
			continue;
		}
		int x = s[0] - 'A'+1, y = s[1] - '0';
		if(s.size() == 3)
		{
			y = y * 10 + s[2] - '0';
		}
		m = make(x,y,turn);
		if(m > 1) declare_winner(turn);
		if(m == 0) fprintf(stderr, "Invalid move");
	}
}

void game::declare_winner(bool who)
{
	char winner = 'R';
	if(who) winner = 'B';
	cerr << "Player " << winner << " has won the game!\n";
	show();
	exit(0);
}

string game::conv_move(PII where)
{
	string row;
	if(where.se < 10) row = (char)(where.se + '0');
	else 
	{
		int dig = where.se / 10;
		row = (char)(dig + '0');
		dig = where.se % 10;
		row = row + (char)(dig + '0');
	}
	row = (char)(where.fi + 'A'-1) + row;
	return row;
}

inline bool game::inboard(int x, int y)
{
	if(x < 1 || x > N || y < 1 || y > N) return false;
	return true;
}

int game::eval(bool who) 
{
	// returns second-smallest distance between two sides of the board
	//  who is variable describing player, 0 is Red, 1 is Blue
	//  basically evaluates board position
	
	int small_inf = 10000;
	int dis[N+1][N+1];
	vi X{-1, -1, 0, 0, 1, 1}, Y{0, 1, -1, 1, -1, 0};
	for(int i=1;i<=N;i++) for(int j=1;j<=N;j++) dis[i][j] = small_inf;
	deque <PII> q;
	
	if(!who) // Red
	{
		for(int i=1;i<=N;i++) 
		{
			if(br[i][1] == 2) continue;
			if(br[i][1] == 0) dis[i][1] = 1;
			else dis[i][1] = 0;
			if(dis[i][1] == 0) q.push_front(mp(i, 1));
			else q.push_back(mp(i, 1));
		}
	}
	else 
	{
		for(int i=1;i<=N;i++) 
		{
			if(br[1][i] == 1) continue;
			if(br[1][i] == 0) dis[1][i] = 1;
			else dis[1][i] = 0;
			if(dis[1][i] == 0) q.push_front(mp(1, i));
			else q.push_back(mp(1, i));
		}
	}
	
	int fin_ans = small_inf;
	
	while(!q.empty())
	{
		PII cur = q.front();
		q.pop_front();
		int bad = 2;
		if(who) bad = 1;
		
		for(int op = 0; op < 6; op++) 
		{
			int x = cur.fi + X[op], y = cur.se + Y[op];
			if(!inboard(x, y)) continue;
			if(br[x][y] == bad) continue;
			int cost = 0;
			if(br[x][y] == 0) cost = 1;
			if(dis[x][y] > dis[cur.fi][cur.se] + cost) 
			{
				dis[x][y] = dis[cur.fi][cur.se] + cost;
				if(cost == 0) q.push_front(mp(x, y));
				else q.push_back(mp(x, y));
			}
		}
		
	}
	
	if(who) // Blue
	{
		for(int i=1;i<=N;i++) 
		{
			int best1, best2;
			best1 = best2 = small_inf;
			for(int op = 0; op < 6; op++) 
			{
				int x = N + X[op], y = i + Y[op];
				if(!inboard(x, y)) continue;
				if(dis[x][y] < best1)
				{
					best2 = best1;
					best1 = dis[x][y];
				}
				else if(dis[x][y] < best2) best2 = dis[x][y];
			}
			if(br[N][i] == 0) best2++;
			fin_ans = min(fin_ans, best2);
		}
	}
	else
	{
		for(int i=1;i<=N;i++) 
		{
			int best1, best2;
			best1 = best2 = small_inf;
			for(int op = 0; op < 6; op++) 
			{
				int x = i + X[op], y = N + Y[op];
				if(!inboard(x, y)) continue;
				if(dis[x][y] < best1)
				{
					best2 = best1;
					best1 = dis[x][y];
				}
				else if(dis[x][y] < best2) best2 = dis[x][y];
			}
			if(br[i][N] == 0) best2++;
			fin_ans = min(fin_ans, best2);
		}
	}
	
	//  for(int i=1;i<=N;i++) 
	//  {
		//  for(int j=1;j<=N;j++) printf("%d ", dis[i][j]);
		//  puts("");
	//  }
	
	return fin_ans;
}
