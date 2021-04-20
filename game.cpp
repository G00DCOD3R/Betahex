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
void game::__init__()
{
	for(int i=0;i<=N;i++) for(int j=0;j<=N;j++) br[i][j] = vis[i][j] = 0;
	while(mvs.size() > 0) mvs.pop();
	moves_made = 0;
	nrv = 1;
	turn = 0;
	
	//  // option to play with ai
	//  puts("Which color do you choose?");
	//  puts("0 - Red (First)");
	//  puts("1 - Blue (Second)");
	//  cin >> op_id;
	
	//  option to play ai alone (comment if uncommented above)
	op_id = 1;
	
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
	// swapping players here is intentional!
	char winner = 'B';
	if(who) winner = 'R';
	cerr << "Player " << winner << " has won the game!\n";
	show();
	exit(0);
}


//  int player::random(int a, int b)
//  {
	//  assert(a <= b);
	// // return a + rand()%(b-a+1);
	//  return uniform_int_distribution<int>(a,b)(rng1);
//  }

//  void player::__init__()
//  {
	//  // default 
	//  keep_close_policy = 91;
	//  UCT_LOG_MULT = 3.14;
	//  MOVE_IGNORE = 91;
	//  ROLL_OUT_NR = 310;
	
	// // cin >> keep_close_policy >> MOVE_IGNORE >> ROLL_OUT_NR >> UCT_LOG_MULT;
//  }

//  PII player::search_move(ld tm_for_move)
//  {
	//  v.clear();
	//  n = 0;
	//  moves_ahead = 0;
	//  term_nodes.clear();
	//  v.pb(vector<pair<PII,int>>(0));
	//  r[0][0] = r[1][0] = 0;
	//  clock_t ST = clock();
	//  for(int i=0;;i++)
	//  {
		//  clock_t TM = clock();
		//  if((ld)(TM - ST) / CLOCKS_PER_SEC > tm_for_move)
		//  {
			//  cerr << "snowcap size is " << i+1 << "\n";
			//  cerr << "moves ahead = " << moves_ahead << "\n";
			//  break;
		//  }
		// // cerr << (ld)(TM - ST) / CLOCKS_PER_SEC << "\n";
		//  run(ROLL_OUT_NR);
	//  }
	// // show();
	//  ld best = -1e9 - 69;
	//  PII w = mp(-1, -1);
	//  for(auto e: v[0])
	//  {
		//  ld cur = (ld)r[0][e.se] / r[1][e.se];
		//  if(cur > best)
		//  {
			//  w = e.fi;
			//  best = cur;
		//  }
	//  }
	//  cerr << "Current estimated win percentage: " << best * 100 << "%\n";
	//  return w;
	
//  }

//  void player::run(int nroll)
//  {
	//  vi path;
	//  get_path(0, path);
	//  int leaf = path.back();
	//  int reward = 0;
	//  for(int i=0;i<nroll;i++)
	//  {
		//  if(term_nodes.find(leaf) == term_nodes.end()) reward += rollout();
		//  else reward += (term_nodes[leaf] == 2 ? 1 : 0);
	//  }
	//  reverse(all(path));
	//  int cnt = path.size()-1;
	//  for(auto e: path)
	//  {
		//  r[1][e] += nroll;
		//  if(G.turn == G.my_id) r[0][e] += nroll - reward;
		//  else r[0][e] += reward;
		//  if(cnt > 0) G.undo_move();
		//  cnt--;
	//  }
//  }

//  PII player::getr_child(int & m)
//  {
	//  int keep_close = random(0, 100);
	//  if(G.mvs.empty()) keep_close = 0;
	//  else
	//  {
		//  PII last = G.mvs.top();
		//  if(last.fi == 1 || last.fi == G.N || last.se == 1 || last.se == G.N) keep_close = 0;
	//  }
	
	//  if(keep_close > keep_close_policy) // keep_close strategy
	//  {
		//  PII last = G.mvs.top();
		//  // vi X{-1, -1, 0, 0, 1, 1, -1, -2, -1, 1, 2, 1}, Y{0, 1, -1, 1, -1, 0, -1, 1, 2, -2, -1, 1};
		//  for(int reps = 0; reps < 13; reps++)
		//  {
			//  int nx = random(-2, 2), ny = 3 - abs(nx);
			//  if(random(0,1) == 0) ny = -ny;
			//  PII w = mp(last.fi + nx, last.se + ny);
			//  m = G.make(w.fi, w.se, G.turn);
			//  // if(m > 1) declare_winner(my_id);
			//  if(m >= 1) return w;
		//  }
	//  }
	//  while(true) 
	//  {
		//  PII w = mp(random(1, G.N), random(1, G.N));
		//  m = G.make(w.fi, w.se, G.turn);
		// //if(m > 1) declare_winner(my_id);
		//  if(m >= 1) return w;
	//  }
	
//  }
//  int player::rollout()
//  {
	//  int dur = 0;
	//  int res = 0;
	//  while(res <= 1)
	//  {
		//  getr_child(res);
		//  dur++;
	//  }
	//  for(int i=0;i<dur;i++) G.undo_move();
	//  if(res == 2) return 1;
	//  else return 0;
	
//  }
//  pair<PII, int> player::uct_sel(int node)
//  {
	//  pair<PII, int> choice;
	//  ld ans_choice = -1;
	//  for(auto e: v[node])
	//  {
		//  ld cur = (ld)r[0][e.se] / r[1][e.se] + sqrt(log(r[1][node])*UCT_LOG_MULT/r[1][e.se]);
		//  if(ans_choice < cur) 
		//  {
			//  ans_choice = cur;
			//  choice = e;
		//  }
	//  }
	//  return choice;
//  }
//  void player::get_path(int node, vi & path)
//  {
	//  path.pb(node);
	//  if(term_nodes.find(node) != term_nodes.end()) return;
	//  int possible = G.N * G.N - G.moves_made;
	//  if(v[node].size() * 100 < possible * MOVE_IGNORE)
	//  {
		//  int tmp;
		//  PII child = getr_child(tmp);
		//  n++;
		//  r[0][n] = r[1][n] = 0;
		//  v[node].pb(mp(child, n));
		//  v.pb(vector<pair<PII,int>>(0));
		//  if(tmp > 1) term_nodes[n] = tmp;
		//  path.pb(n);
		//  moves_ahead = max(moves_ahead, (int)path.size());
		//  return;
	//  }
	//  pair <PII, int> best = uct_sel(node);
	//  G.make(best.fi.fi, best.fi.se, G.turn);
	//  get_path(best.se, path);
//  }
