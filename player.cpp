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
mt19937 rng1(chrono::steady_clock::now().time_since_epoch().count());
#define random_shuffle(...) shuffle(__VA_ARGS__, rng)

typedef long long ll;
typedef unsigned long long ull;
typedef long double ld;
typedef vector <int> vi;
typedef pair<int,int> PII;
typedef pair<ll,ll> PLL;

game G;

int player::random(int a, int b)
{
	assert(a <= b);
	//  return a + rand()%(b-a+1);
	return uniform_int_distribution<int>(a,b)(rng1);
}

void player::__init__()
{
	// default 
	keep_close_policy = 100;
	UCT_LOG_MULT = 1.14;
	MOVE_IGNORE = 100;
	ROLL_OUT_NR = 400;
	
	//  cin >> keep_close_policy >> MOVE_IGNORE >> ROLL_OUT_NR >> UCT_LOG_MULT;
}

PII player::search_move(ld tm_for_move)
{
	v.clear();
	n = 0;
	moves_ahead = 0;
	inv_moves[0].clear();
	term_nodes.clear();
	v.pb(vector<pair<PII,int>>(0));
	r[0][0] = r[1][0] = 0;
	clock_t ST = clock();
	for(int i=0;;i++)
	{
		clock_t TM = clock();
		if((ld)(TM - ST) / CLOCKS_PER_SEC > tm_for_move)
		{
			cerr << "snowcap size is " << i+1 << "\n";
			cerr << "moves ahead = " << moves_ahead << "\n";
			break;
		}
		//  cerr << (ld)(TM - ST) / CLOCKS_PER_SEC << "\n";
		run(ROLL_OUT_NR);
	}
	//  show();
	ld best = -1e9 - 69;
	PII w = mp(-1, -1);
	vector <pair<ld, PII>> possibilities;
	for(auto e: v[0])
	{
		ld cur = (ld)r[0][e.se] / r[1][e.se];
		possibilities.pb(mp(cur, e.fi));
		if(cur > best)
		{
			w = e.fi;
			best = cur;
		}
	}
	sort(all(possibilities));
	reverse(all(possibilities));
	cerr << "Current estimated win percentage: " << setprecision(2) << fixed << best * 100 << "%\n";
	cerr << "Other good moves: ";
	for(int i=1;i<min((int)possibilities.size(), 11);i++)
	{
		PII Move = possibilities[i].se;
		cerr << G.conv_move(Move)<< "=[" << setprecision(2) << fixed << possibilities[i].fi * 100 << "%]; ";
	}
	cerr << "\nWorst moves: ";
	for(int i=possibilities.size()-1;i > max(0, (int)possibilities.size() - 5);i--)
	{
		PII Move = possibilities[i].se;
		cerr << G.conv_move(Move)<< "=[" << setprecision(2) << fixed << possibilities[i].fi * 100 << "%]; ";
	}
	cerr << "\n";
	return w;
	
}

void player::run(int nroll)
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

PII player::getr_child(int & m)
{
	int keep_close = random(0, 100);
	if(G.mvs.empty()) keep_close = 0;
	else
	{
		PII last = G.mvs.top();
		if(last.fi == 1 || last.fi == G.N || last.se == 1 || last.se == G.N) keep_close = 0;
	}
	
	if(keep_close > keep_close_policy) // keep_close strategy
	{
		PII last = G.mvs.top();
		//  vi X{-1, -1, 0, 0, 1, 1, -1, -2, -1, 1, 2, 1}, Y{0, 1, -1, 1, -1, 0, -1, 1, 2, -2, -1, 1};
		for(int reps = 0; reps < 13; reps++)
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
int player::rollout()
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
pair<PII, int> player::uct_sel(int node)
{
	pair<PII, int> choice;
	ld ans_choice = -1;
	for(auto e: v[node])
	{
		ld cur = (ld)r[0][e.se] / r[1][e.se] + sqrt(log(r[1][node])*UCT_LOG_MULT/r[1][e.se]);
		if(ans_choice < cur) 
		{
			ans_choice = cur;
			choice = e;
		}
	}
	return choice;
}
void player::get_path(int node, vi & path)
{
	path.pb(node);
	if(term_nodes.find(node) != term_nodes.end()) return;
	int possible = G.N * G.N - G.moves_made;
	if(v[node].size() * 100 < possible * MOVE_IGNORE)
	{
		int tmp;
		PII child = getr_child(tmp);
		while(inv_moves[node].find(child) != inv_moves[node].end())
		{
			G.undo_move();
			child = getr_child(tmp);
		}
		n++;
		inv_moves[n].clear();
		inv_moves[node].insert(child);
		r[0][n] = r[1][n] = 0;
		v[node].pb(mp(child, n));
		v.pb(vector<pair<PII,int>>(0));
		if(tmp > 1) term_nodes[n] = tmp;
		path.pb(n);
		moves_ahead = max(moves_ahead, (int)path.size());
		return;
	}
	pair <PII, int> best = uct_sel(node);
	G.make(best.fi.fi, best.fi.se, G.turn);
	get_path(best.se, path);
}
