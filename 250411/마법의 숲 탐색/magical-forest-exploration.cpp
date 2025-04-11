#include <iostream>
#include <algorithm>
#include <vector>
#include <string>
#include <queue>
#include <stack>
#include <unordered_set>
#include <unordered_map>
#include <set>
#include <map>
using namespace std;
int R, C, K,ans;
#define X first
#define Y second
int dx[] = { -1,0,1,0 };
int dy[] = { 0,1,0,-1 }; // 북 동 남 서 
int MagicMap[75][75];
queue<pair<int, int>> GolRem; // 출발하는 열, 출구 방향 정보
queue<pair<int, int>> q; // bfs 진행할 큐

// 격자 맨 윗줄이 1행이므로 나중에 행값 ans에 더할 때 +1해서 더해줘야함
// 골렘 중심은 (R + 2, C) 에서 시작
// 격자 크기는 (R + 2, C + 2)로 상정
// 골렘이 이동 완료했는데 행이 1 이하라면 숲을 clear 해야함
bool move_to_nam(int xx, int yy) { // (x,y)는 중심좌표
	if (xx + 2 > R + 1) return false; // 골렘이 벽 바깥이면
	if (MagicMap[xx + 2][yy] >= 1 || MagicMap[xx + 1][yy - 1] >= 1 || MagicMap[xx + 1][yy + 1] >= 1) return false;
	return true;
}

bool move_to_seo(int xx, int yy) {
	if (yy - 2 < 1 || xx + 2 > R+1) return false;
	if (MagicMap[xx][yy - 2] >= 1 || MagicMap[xx - 1][yy - 1] >= 1 || MagicMap[xx + 1][yy - 1] >= 1) return false;
	if (MagicMap[xx + 2][yy - 1] >= 1 || MagicMap[xx + 2][yy - 2] >= 1)return false;
	return true;
}

bool move_to_dong(int xx, int yy) {
	if (yy + 2 > C || xx + 2 > R + 1) return false;
	if (MagicMap[xx][yy + 2] >= 1 || MagicMap[xx - 1][yy + 1] >= 1 || MagicMap[xx + 1][yy + 1] >= 1) return false;
	if (MagicMap[xx + 2][yy + 1] >= 1 || MagicMap[xx + 2][yy + 2] >= 1)return false;
	return true;
}

void GolRem_to_map(int xx, int yy, int dd) {
	MagicMap[xx][yy] = 1;
	for (int dir = 0; dir < 4; dir++) {
		int nx = xx + dx[dir];
		int ny = yy + dy[dir];
		if (dir == dd) MagicMap[nx][ny] = 2;
		else MagicMap[nx][ny] = 1;
	}
}

void print_MagicMap() {
	cout << "-----------------------------------\n";
	for (int iii = 0; iii < R + 2; iii++) {
		for (int jjj = 0; jjj < C + 2; jjj++) {
			cout << MagicMap[iii][jjj] << ' ';
		}
		cout << '\n';
	}
}

int bfs(int xx, int yy, int dd) {
	int cnt = 0;
	int vis[75][75];
	for (int i = 0; i <=R + 2; i++) {
		fill(vis[i], vis[i] + C + 2, 0);
	}
	q.push({ xx,yy }); vis[xx][yy] = cnt+1;
	while (!q.empty()) {
		auto ccur = q.front(); q.pop(); cnt++;
		for (int dir = 0; dir < 4; dir++) {
			int nx = ccur.X + dx[dir];
			int ny = ccur.Y + dy[dir];
			// 골렘이 아니거나 방문했다면 pass
			if (MagicMap[nx][ny] == 0 || vis[nx][ny] != 0) continue;
			// 골렘이면서 아직 방문하지 않음
			if (MagicMap[nx][ny] != 2) {
				if (cnt > vis[ccur.X][ccur.Y]) q.push({nx,ny});
				vis[nx][ny] = cnt;
				continue; // 출구가 아니라면 골렘 색만 채우고 끝
			}
			// 방문도안했고 골렘이고 출구라면 큐에 push
			vis[nx][ny] = cnt; q.push({ nx,ny });
		}
	}
	for (int i = R + 1; i >= 2; i--) {
		for (int j = 1; j < C + 1; j++) {
			if (vis[i][j] != 0) return i-1;
		}
	}
}

int main() {
	ios::sync_with_stdio(0);
	cin.tie(0);
	cin >> R >> C >> K;
	for (int i = 0; i < K; i++) {
		int x, y;
		cin >> x >> y;
		GolRem.push({ x,y });
	}
	while (!GolRem.empty()) {
		auto cur = GolRem.front(); GolRem.pop();
		int x = 0; int y = cur.X; int d = cur.Y;
		while (1) {
			if (move_to_nam(x, y)) {
				x++;
			}
			else break;
		} // 남쪽으로 이동할 수 있을 만큼 (x,y)를 이동
		while (1) {
			if (move_to_seo(x, y)) {
				x++; y--;
				d = (d + 3) % 4;
			}
			else break;
		}
		while (1) {
			if (move_to_dong(x, y)) {
				x++; y++;
				d = (d + 1)%4;
			}
			else break;
		}
		// 골렘이 Map 안에 없다면 마법의 숲 clear
		if (x - 1 < 2) {
			for (int i = 0; i < 75; i++) {
				fill(MagicMap[i], MagicMap[i] + 75, 0);
			}
			continue;
		}
		
		GolRem_to_map(x,y,d); // 마법의 숲에 골렘 정착
		//print_MagicMap();
		ans += bfs(x,y,d); // 정령이 갈 수 있는 곳 까지 탐색하고 가장 아래 행값 반환
	}
	cout << ans;
	return 0;
}