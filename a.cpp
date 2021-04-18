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

int random(int a, int b)
{
	assert(a <= b);
	//  return a + rand()%(b-a+1);
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

class player
{
	private:
	vector <vector <pair<PII, int>>> v;
	unordered_map <int,int> term_nodes;
	int n, r[2][nax];
	public:
	PII search_move(ld tm_for_move)
	{
		v.clear();
		n = 0;
		term_nodes.clear();
		v.pb(vector<pair<PII,int>>(0));
		r[0][0] = r[1][0] = 0;
		clock_t ST = clock();
		for(int i=0;;i++)
		{
			clock_t TM = clock();
			if((ld)(TM - ST) / CLOCKS_PER_SEC > tm_for_move)
			{
				cout << "snowcap size is " << i+1 << "\n";
				break;
			}
			//  cerr << (ld)(TM - ST) / CLOCKS_PER_SEC << "\n";
			run(200);
		}
		//  show();
		ld best = -INF;
		PII w = mp(-1, -1);
		for(auto e: v[0])
		{
			ld cur = (ld)r[0][e.se] / r[1][e.se];
			if(cur > best)
			{
				w = e.fi;
				best = cur;
			}
		}
		cout << "Current estimated win percentage: " << best * 100 << "%\n";
		return w;
		
	}

	void run(int nroll)
	{
		vi path;
		get_path(0, path);
		int leaf = path.back();
		int reward = 0;
		for(int i=0;i<nroll;i++)
		{
			if(term_nodes.find(leaf) == term_nodes.end()) reward += rollout();
			else reward += (term_nodes[leaf] == 2 ? 1 : 0);
		}
		reverse(all(path));
		int cnt = path.size()-1;
		for(auto e: path)
		{
			r[1][e] += nroll;
			if(G.turn == G.my_id) r[0][e] += nroll - reward;
			else r[0][e] += reward;
			if(cnt > 0) G.undo_move();
			cnt--;
		}
	}

	PII getr_child(int & m)
	{
		int keep_close = random(0, 100);
		if(G.mvs.empty()) keep_close = 0;
		else
		{
			PII last = G.mvs.top();
			if(last.fi == 1 || last.fi == G.N || last.se == 1 || last.se == G.N) keep_close = 0;
		}
		
		if(keep_close > 80) // keep_close strategy
		{
			PII last = G.mvs.top();
			//  vi X{-1, -1, 0, 0, 1, 1, -1, -2, -1, 1, 2, 1}, Y{0, 1, -1, 1, -1, 0, -1, 1, 2, -2, -1, 1};
			for(int reps = 0; reps < 30; reps++)
			{
				int nx = random(-2, 2), ny = 3 - abs(nx);
				if(random(0,1) == 0) ny = -ny;
				PII w = mp(last.fi + nx, last.se + ny);
				m = G.make(w.fi, w.se, G.turn);
				//  if(m > 1) declare_winner(my_id);
				if(m >= 1) return w;
			}
		}
		while(true) 
		{
			PII w = mp(random(1, G.N), random(1, G.N));
			m = G.make(w.fi, w.se, G.turn);
			//  if(m > 1) declare_winner(my_id);
			if(m >= 1) return w;
		}
		
	}
	int rollout()
	{
		int dur = 0;
		int res = 0;
		while(res <= 1)
		{
			getr_child(res);
			dur++;
		}
		for(int i=0;i<dur;i++) G.undo_move();
		if(res == 2) return 1;
		else return 0;
		
	}
	pair<PII, int> uct_sel(int node)
	{
		vector <pair<ld, pair<PII,int>>> choice;
		for(auto e: v[node])
		{
			ld cur = (ld)r[0][e.se] / r[1][e.se] + sqrt(log(r[1][node])*2/r[1][e.se]);
			choice.pb(mp(cur, e));
		}
		sort(all(choice));
		return choice.back().se;
	}
	void get_path(int node, vi & path)
	{
		path.pb(node);
		if(term_nodes.find(node) != term_nodes.end()) return;
		int possible = G.N * G.N - G.moves_made;
		if(v[node].size() * 100 < possible * 93)
		{
			int tmp;
			PII child = getr_child(tmp);
			n++;
			r[0][n] = r[1][n] = 0;
			v[node].pb(mp(child, n));
			v.pb(vector<pair<PII,int>>(0));
			if(tmp > 1) term_nodes[n] = tmp;
			path.pb(n);
			return;
		}
		pair <PII, int> best = uct_sel(node);
		G.make(best.fi.fi, best.fi.se, G.turn);
		get_path(best.se, path);
	}
};

int main()
{
	bool turn = 0;
	player me;
	G.__init__();
	G.show();
	while(true)
	{
		turn = G.turn;
		if(G.turn == G.my_id)
		{
			int tmp=0;
			clock_t ST = clock();
			PII where = me.search_move(3.0);
			clock_t ED = clock();
			cout << "move took " << setprecision(5) << fixed << (ld)(ED - ST) / CLOCKS_PER_SEC << " seconds\n";
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
			if(tmp > 1) G.declare_winner(turn);
			if(tmp == 0)
			{
				puts("resign");
				G.declare_winner(turn ^ 1);
			}
		}
		else G.let_make_move();
		G.show();
	}
}
