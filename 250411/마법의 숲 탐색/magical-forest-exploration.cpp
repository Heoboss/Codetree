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
int R, C, K,ans,color=1;
#define X first
#define Y second
int dx[] = { -1,0,1,0 };
int dy[] = { 0,1,0,-1 }; // 북 동 남 서 
int MagicMap[75][75];
queue<pair<int, int>> GolRem; // 출발하는 열, 출구 방향 정보
queue<pair<int, int>> q; // bfs 진행할 큐
queue<pair<int, int>> is_chulgu;

// 격자 맨 윗줄이 1행이므로 나중에 행값 ans에 더할 때 +1해서 더해줘야함
// 골렘 중심은 (R + 2, C) 에서 시작
// 격자 크기는 (R + 2, C + 2)로 상정
// 골렘이 이동 완료했는데 행이 1 이하라면 숲을 clear 해야함
// 출구는 -1로 표기
bool move_to_nam(int xx, int yy) { // (x,y)는 중심좌표
	if (xx + 2 > R + 1) return false; // 골렘이 벽 바깥이면
	if (abs(MagicMap[xx + 2][yy]) >= 1 || abs(MagicMap[xx + 1][yy - 1]) >= 1 || abs(MagicMap[xx + 1][yy + 1]) >= 1) return false;
	return true;
}

bool move_to_seo(int xx, int yy) {
	if (yy - 2 < 1 || xx + 2 > R + 1) return false;
	if (abs(MagicMap[xx][yy - 2]) >= 1 || abs(MagicMap[xx - 1][yy - 1]) >= 1 || abs(MagicMap[xx + 1][yy - 1]) >= 1) return false;
	if (abs(MagicMap[xx + 2][yy - 1]) >= 1 || abs(MagicMap[xx + 1][yy - 2]) >= 1)return false;
	return true;
}

bool move_to_dong(int xx, int yy) {
	if (yy + 2 > C || xx + 2 > R + 1) return false;
	if (abs(MagicMap[xx][yy + 2]) >= 1 || abs(MagicMap[xx - 1][yy + 1]) >= 1 || abs(MagicMap[xx + 1][yy + 1]) >= 1) return false;
	if (abs(MagicMap[xx + 2][yy + 1]) >= 1 || abs(MagicMap[xx + 1][yy + 2]) >= 1)return false;
	return true;
}

void GolRem_to_map(int xx, int yy, int dd) {
	MagicMap[xx][yy] = color;
	for (int dir = 0; dir < 4; dir++) {
		int nx = xx + dx[dir];
		int ny = yy + dy[dir];
		if (dir == dd) MagicMap[nx][ny] = -color;
		else MagicMap[nx][ny] = color;
	}
	color++;
}

void print_MagicMap() {
	cout << "-----------------------------------\n";
	for (int iii = 0; iii < R + 2; iii++) {
		for (int jjj = 0; jjj < C + 2; jjj++) {
			int val = MagicMap[iii][jjj];
			if (val < 0) {
				// 빨간색
				cout << "\033[31m" << -val << "\033[0m" << ' ';
			}
			else if (val > 0) {
				// 초록색 (선택)
				cout << "\033[32m" << val << "\033[0m" << ' ';
			}
			else {
				// 기본 색
				cout << val << ' ';
			}
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
	q.push({ xx,yy }); vis[xx][yy] = cnt;
	int max_row = 1;
	while (!q.empty() || !is_chulgu.empty()) {
		if (q.empty()) { // 큐가 비었다면 출구를 넣어줌
			q.push(is_chulgu.front()); is_chulgu.pop();
		}
		auto ccur = q.front(); q.pop(); 
		max_row = max(max_row, ccur.X - 1); // 방문할 때마다 최대 행 갱신!
		//if (MagicMap[ccur.X][ccur.Y] == 2) cnt++;
		for (int dir = 0; dir < 4; dir++) {
			int nx = ccur.X + dx[dir];
			int ny = ccur.Y + dy[dir];
			// 골렘이 아니거나 방문했다면 pass (O)
			if (MagicMap[nx][ny] == 0 || vis[nx][ny] != 0) continue;

			// 다음꺼가 나의출구일때 나중에 큐에 넣어주자
			if (MagicMap[nx][ny]<0 && MagicMap[nx][ny] == -MagicMap[ccur.X][ccur.Y]) { 
				is_chulgu.push({nx,ny});
				vis[nx][ny] = 1;
				continue; 
			}
			// 나는 출구고 다음도 출구면
			if (MagicMap[ccur.X][ccur.Y] < 0 && MagicMap[nx][ny] < 0) {
				is_chulgu.push({ nx,ny });
				vis[nx][ny] = 1;
				continue;
			}
			// 나는 출구인데 다음것이 출구가 아니고 내 골렘이면
			if (MagicMap[ccur.X][ccur.Y] < 0 && MagicMap[nx][ny] == -MagicMap[ccur.X][ccur.Y]) {
				q.push({ nx,ny }); vis[nx][ny] = 1;
				continue;
			}
			// 나는 출구가 아닌데 다음꺼가 나와 같다면
			if (MagicMap[ccur.X][ccur.Y]>0 && MagicMap[nx][ny] == MagicMap[ccur.X][ccur.Y]) {
				vis[nx][ny] = 1;
				q.push({ nx,ny });
				continue;
			}
			// 나는 출구인데 다음걸 방문도 안했고 다음 넘어갈수있는 골렘이면
			if (MagicMap[ccur.X][ccur.Y] < 0 && MagicMap[nx][ny] != -MagicMap[ccur.X][ccur.Y]) {
				q.push({ nx,ny });
			}
			
		}
	}
	/*for (int i = R + 1; i >= 2; i--) {
		for (int j = 1; j < C + 1; j++) {
			if (vis[i][j] != 0) return i-1;
		}
	}*/
	return max_row;
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
		bool flag = false;
		while (1) {
			while (1) {
				if (move_to_nam(x, y)) {
					x++;
					flag = false;
				}
				else {
					flag = true;
					break;
				}
			}
			while (1) {
				if (move_to_seo(x, y)) {
					x++; y--;
					d = (d + 3) % 4;
					flag = false;
				}
				else break;
			}
			while (1) {
				if (move_to_dong(x, y)) {
					x++; y++;
					d = (d + 1) % 4;
					flag = false;
				}
				else break;
			}
			if (flag) break;
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