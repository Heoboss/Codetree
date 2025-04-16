#include <iostream>
#include <algorithm>
#include <vector>
#include <queue>
#include <tuple>
using namespace std;
#define X first
#define Y second
const int MXN = 50;
int dx[] = { -1,1,0,0 };
int dy[] = { 0,0,-1,1 };
int N, K;
char Smap[MXN + 2][MXN + 2];
int Emap[MXN + 2][MXN + 2];
int gunchae[MXN + 2][MXN + 2];
bool isgong[MXN][MXN];

queue<pair<int, int>> q;
vector<pair<int, int>> gong_hoobo;
vector<pair<int, int>> gong_real;
vector<pair<int, int>> gong_razer;
// T C M -> RGB // T: 민트(100) C: 초코(010) : M : 우유(001)
// R G B -> MCY // R :초코우유(011), G : 민트우유(101), B : 민트초코(110)
// W -> W // W : 민트초코우유(111)
void print_smap() {
	cout << "smap ------------------------------\n";
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			cout << Smap[i][j] << ' ';
		}
		cout << '\n';
	}
}

void print_emap() {
	cout << "emap ------------------------------\n";
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			cout << Emap[i][j] << ' ';
		}
		cout << '\n';
	}
}

void print_gunchae() {
	cout << "gunchae ------------------------------\n";
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			cout << gunchae[i][j] << ' ';
		}
		cout << '\n';
	}
}

void gen_energy() {
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			Emap[i][j]++;
		}
	}
}

int bfs_gunchae() {
	int cnt = 0;
	for (int i = 0; i < N; i++) {
		fill(gunchae[i], gunchae[i] + N, 0);
	}
	while (1) {
		bool flag = false;
		for (int i = 0; i < N; i++) {
			for (int j = 0; j < N; j++) {
				if (gunchae[i][j] != 0) continue;
				//print_gunchae();
				cnt++; flag = true;
				q.push({ i,j }); gunchae[i][j] = cnt;
				char val = Smap[i][j];
				while (!q.empty()) {
					auto cur = q.front(); q.pop();
					for (int dir = 0; dir < 4; dir++) {
						int nx = cur.X + dx[dir];
						int ny = cur.Y + dy[dir];
						if (nx < 0 || nx >= N || ny < 0 || ny >= N) continue;
						if (Smap[nx][ny] != val || gunchae[nx][ny] != 0) continue;
						gunchae[nx][ny] = cnt; q.push({ nx,ny });
					}
				}
			}
		}
		if (flag == false) break;
	}
	return cnt;
}

void bfs_gong(int gunchae_ccnt) {
	int cnt = 1;
	bool vis[MXN][MXN];
	for (int i = 0; i < N; i++) {
		fill(vis[i], vis[i] + N, 0);
	}
	while (cnt <= gunchae_ccnt) {
		queue<pair<int, int>> que; // 같은 균체들 좌표 담을 큐
		int mn = 0;
		for (int i = 0; i < N; i++) {
			for (int j = 0; j < N; j++) {
				if (gunchae[i][j] != cnt || vis[i][j]) continue; // 다른 균체이면 continue
				vis[i][j] = 1;
				que.push({ i,j });
				if (Emap[i][j] > mn) {
					mn = Emap[i][j];
					gong_hoobo.clear();
					gong_hoobo.push_back({ i,j });
				}
				else if (Emap[i][j] == mn) {
					gong_hoobo.push_back({ i,j });
				}
			}
		}
		sort(gong_hoobo.begin(), gong_hoobo.end());
		gong_real.push_back({ gong_hoobo[0].X, gong_hoobo[0].Y });
		int ii = gong_hoobo[0].X; int jj = gong_hoobo[0].Y;
		while (!que.empty()) {
			auto cur = que.front(); que.pop();
			if (cur.X == ii && cur.Y == jj) {
				continue;
			}
			Emap[ii][jj]++; Emap[cur.X][cur.Y]--;
		}
		cnt++;
	}
}

bool cmp(const tuple<int, int, int> a, const tuple<int, int,int> b) {
	if (get<0>(a) > get<0>(b)) {
		return true;
	}
	else if (get<0>(a) == get<0>(b)) {
		if (get<1>(a) < get<1>(b))
			return true;
		else if (get<1>(a) == get<1>(b)) {
			if (get<2>(a) < get<2>(b)) {
				return true;
			}
			else
				return false;
		}
		else return false;
	}
	else
		return false;
}

void razer_1() {
	vector<tuple<int, int, int>> color1, color2, color3;
	for (auto c : gong_real) {
		if (Smap[c.X][c.Y] == 'T' || Smap[c.X][c.Y] == 'C' || Smap[c.X][c.Y] == 'M')
			color1.push_back(make_tuple(Emap[c.X][c.Y], c.X, c.Y));
		else if (Smap[c.X][c.Y] == 'R' || Smap[c.X][c.Y] == 'G' || Smap[c.X][c.Y] == 'B')
			color2.push_back(make_tuple(Emap[c.X][c.Y], c.X, c.Y));
		else
			color3.push_back(make_tuple(Emap[c.X][c.Y], c.X, c.Y));
	}
	sort(color1.begin(), color1.end(), cmp);
	sort(color2.begin(), color2.end(), cmp);
	sort(color3.begin(), color3.end(), cmp);
	if (!color1.empty()) {
		for (auto cc : color1) {
			gong_razer.push_back({ get<1>(cc),get<2>(cc) });
			isgong[get<1>(cc)][get<2>(cc)] = 1; // 공격균임을 표시
		}
	}
	if (!color2.empty()) {
		for (auto cc : color2) {
			gong_razer.push_back({ get<1>(cc),get<2>(cc) });
			isgong[get<1>(cc)][get<2>(cc)] = 1; // 공격균임을 표시
		}
	}
	if (!color3.empty()) {
		for (auto cc : color3) {
			gong_razer.push_back({ get<1>(cc),get<2>(cc) });
			isgong[get<1>(cc)][get<2>(cc)] = 1; // 공격균임을 표시
		}
	}
}
// T C M -> RGB // T: 민트(100) C: 초코(010) : M : 우유(001)
// R G B -> MCY // R :초코우유(011), G : 민트우유(101), B : 민트초코(110)
// W -> W // W : 민트초코우유(111)
char change_color(const char a, const char b) {
	int aa = 0, bb = 0, ca1 = 0, cb1 = 0, ca2 = 0, cb2 = 0, ca3 = 0, cb3 = 0;
	int d1 = 0, d2 = 0, d3 = 0;
	int ret = 0;
	if (a == 'T')
		aa = 100;
	else if (a == 'C')
		aa = 10;
	else if (a == 'M')
		aa = 1;
	else if (a == 'R')
		aa = 11;
	else if (a == 'G')
		aa = 101;
	else if (a == 'B')
		aa = 110;
	else
		aa = 111;

	if (b == 'T')
		bb = 100;
	else if (b == 'C')
		bb = 10;
	else if (b == 'M')
		bb = 1;
	else if (b == 'R')
		bb = 11;
	else if (b == 'G')
		bb = 101;
	else if (b == 'B')
		bb = 110;
	else
		bb = 111;

	if (aa == 111 || bb == 111)
		return 'W';
	else { // 하나라도 111이 아니면 계산해봐야함
		ca1 = aa / 100;
		aa %= 100;
		cb1 = bb / 100;
		bb %= 100;
		ca2 = aa / 10;
		aa %= 10;
		cb2 = bb / 10;
		bb %= 10;
		ca3 = aa;
		cb3 = bb;
		if (ca1 == 0 && cb1 == 0) {
			d1 = 0;
		}
		else d1 = 1;

		if (ca2 == 0 && cb2 == 0) {
			d2 = 0;
		}
		else d2 = 1;

		if (ca3 == 0 && cb3 == 0) {
			d3 = 0;
		}
		else d3 = 1;

		ret = d1 * 100 + d2 * 10 + d3;
	}
	if (ret == 111)
		return 'W';
	else if (ret == 11)
		return 'R';
	else if (ret == 101)
		return 'G';
	else if (ret = 110)
		return 'B';
	else if (ret == 100)
		return 'T';
	else if (ret == 10)
		return 'C';
	else
		return 'M';
}

void razer_launch() {
	for (auto c : gong_razer) {
		if (isgong[c.X][c.Y] == 0) continue; // 방어균 됐다면 pass
		queue<pair<int, int>> qq;
		qq.push({ c.X,c.Y });
		int dir = Emap[c.X][c.Y]%4;
		int power = Emap[c.X][c.Y] - 1;
		Emap[c.X][c.Y] = 1;
		while (!qq.empty()) {
			if (power <= 0)break;
			auto cur = qq.front(); qq.pop();
			int nx = cur.X + dx[dir];
			int ny = cur.Y + dy[dir];
			if (nx < 0 || nx >= N || ny < 0 || ny >= N) continue;
			if (Smap[nx][ny] == Smap[c.X][c.Y]) { // 같은 색이면
				qq.push({ nx,ny });
				continue;
			}
			// 여기부턴 다른색인 경우
			if (power > Emap[nx][ny]) { // power가 방어균의 에너지보다 큰경우
				isgong[nx][ny] = 0; // 방어균으로 만듬
				Smap[nx][ny] = Smap[c.X][c.Y]; // 공격균의 색이됨
				power -= (Emap[nx][ny]+1); // 방어균의 에너지만큼 깎임
				Emap[nx][ny]++;
				qq.push({ nx,ny });
			}
			else { // 파워가 작거나 같을경우 
				isgong[nx][ny] = 0;
				Emap[nx][ny] += power;
				Smap[nx][ny] = change_color(Smap[nx][ny], Smap[c.X][c.Y]);
				power = 0;
			}
		}
		//print_smap();
		//print_emap();
	}
}

int main() {
	ios::sync_with_stdio(0);
	cin.tie(0);
	cin >> N >> K;
	for (int i = 0; i < N; i++) {
		string tmp;
		cin >> tmp;
		for (int j = 0; j < N; j++) {
			Smap[i][j] = tmp[j];
		}
	}
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			cin >> Emap[i][j];
		}
	}
	while (K--) {
		gen_energy(); // 1. 모든 균이 에너지를 1씩 얻음 
		int gunchae_cnt = bfs_gunchae(); // 균체의 갯수 반환
		//print_gunchae();
		bfs_gong(gunchae_cnt);
		//print_emap();
		razer_1(); // 레이저 발사 순서를 정하는 함수
		/*cout << "gong_Razer ------------------\n";
		for (auto c : gong_razer) {
			cout << c.X << ' ' << c.Y << '\n';
		}*/
		razer_launch();
		int tt = 0, cc = 0, mm = 0, bb = 0, gg = 0, rr = 0, ww = 0;
		for (int i = 0; i < N; i++) {
			for (int j = 0; j < N; j++) {
				if (Smap[i][j] == 'T')
					tt += Emap[i][j];
				else if (Smap[i][j] == 'C')
					cc += Emap[i][j];
				else if (Smap[i][j] == 'M')
					mm += Emap[i][j];
				else if (Smap[i][j] == 'R')
					rr += Emap[i][j];
				else if (Smap[i][j] == 'G')
					gg += Emap[i][j];
				else if (Smap[i][j] == 'B')
					bb += Emap[i][j];
				else
					ww += Emap[i][j];
			}
		}
		cout << ww << ' ' << bb << ' ' << gg << ' ' << rr << ' ' << mm << ' ' << cc << ' ' << tt << '\n';
		gong_razer.clear();
		gong_real.clear();
		gong_hoobo.clear();
	}
	return 0;
}