#define pb push_back
#define mp make_pair
#define fi first
#define se second 
#define all(...) begin(__VA_ARGS__) , end(__VA_ARGS__)
#define boost {ios_base::sync_with_stdio(false); cin.tie(); cout.tie();}

#include <bits/stdc++.h>
#include "game.h"
using namespace std;

typedef long long ll;
typedef unsigned long long ull;
typedef long double ld;
typedef vector <int> vi;
typedef pair<int,int> PII;
typedef pair<ll,ll> PLL;


bool game::good(int x, int y)
{
	if(x < 1 || x > N || y < 1 || y > N || br[x][y] != 0) return false;
	return true;
}
void game::__init__()
{
	for(int i=0;i<=N;i++) for(int j=0;j<=N;j++) br[i][j] = vis[i][j] = 0;
	puts("Which color do you choose?");
	puts("0 - Red (First)");
	puts("1 - Blue (Second)");
	cin >> op_id;
	my_id = op_id ^ 1;
}
void game::show()
{
	printf("      \033[94m");
	for(int i=1;i<=N;i++) printf("(%d) ", i);
	printf("\033[0m");
	puts("");
	printf("     ");
	for(int j=0;j<N;j++) printf(" / \\");
	puts("");
	for(int i=1;i<=N;i++)
	{
		for(int j=0;j<i*2;j++) printf(" ");
		printf("\033[91m(%c)\033[0m|", (char)(i+'A'-1));
		for(int j=1;j<=N;j++)
		{
			if(br[i][j] == 1) printf(" \033[91mR\033[0m |");
			else if(br[i][j] == 2) printf(" \033[94mB\033[0m |");
			else printf(" . |");
			
		}
		puts("");
		printf("   ");
		for(int j=0;j<i*2;j++) printf(" ");
		for(int j=0;j<N;j++) printf(" \\ /");
		if(i != N) printf(" \\");
		puts("");
	}
	puts("");
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
	printf("Your move\n");
	int m = 0;
	while(m == 0) 
	{
		string s;
		cin >> s;
		if(s.size() != 2 && s.size() != 3)
		{
			puts("Invalid move\nvalid move e.g. A1");
			continue;
		}
		int x = s[0] - 'A'+1, y = s[1] - '0';
		if(s.size() == 3)
		{
			y = y * 10 + s[2] - '0';
		}
		m = make(x,y,turn);
		if(m > 1) declare_winner(turn);
		if(m == 0) puts("Invalid move");
	}
}

void game::declare_winner(bool who)
{
	char winner = 'R';
	if(who) winner = 'B';
	cout << "Player " << winner << " has won the game!\n";
	show();
	exit(0);
}