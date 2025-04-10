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
#include <tuple>
using namespace std;
#define X first
#define Y second
int dx[] = { 0,1,0,-1 };
int dy[] = { 1,0,-1,0 }; // 동 남 서 북 순서
const int MXN = 5;
int K, M;
int AncientMap[MXN+2][MXN+2];
int board[MXN + 2][MXN + 2];
queue<int> WallNumber;
queue<pair<int,int>> q; // bfs 진행할 큐

vector<tuple<int, int, int, int>> Rot; // 열, 행, 1차 획득 가치, 회전한 각도

// 유물 조각은 1부터 7까지 존재
// 3개 이상 연결된 경우 유물이 되고 사라짐
// 유물의 가치는 모인 조각의 개수와 같다.
// 초기 유적지에서는 탐사진행 이전에 3개이상 연결된 유물 조각이 없다.

// 현재 board에 대해서 유물 획득 가치 계산
int bfs() {
	int ret = 0;
	bool vis[MXN + 2][MXN + 2];
	for (int tt = 0; tt < MXN; tt++) {
		fill(vis[tt], vis[tt] + MXN, 0);
	} // 방문 배열 초기화
	for (int xi = 0; xi < MXN; xi++) {
		for (int xj = 0; xj < MXN; xj++) {
			q.push({ xi,xj }); int val = board[xi][xj]; vis[xi][xj] = 1;
			int Umul_len = 1; // 유물 조각 크기
			while (!q.empty()) {
				auto cur = q.front(); q.pop();
				for (int dir = 0; dir < 4; dir++) {
					int nx = cur.X + dx[dir];
					int ny = cur.Y + dy[dir];
					if (nx < 0 || nx >= MXN || ny < 0 || ny >= MXN) continue;
					if (vis[nx][ny] != 0 || board[nx][ny] != val) continue;
					vis[nx][ny] = 1; q.push({ nx,ny });
					Umul_len++;
				}
			} // 현재 보고 있는 셀과 이어진 유물 탐색 종료
			if (Umul_len >= 3) {
				ret += Umul_len;
				//for (int ii = 0; ii < MXN; ii++) {
				//	for (int jj = 0; jj < MXN; jj++) {
				//		// 지워야할 유물들 좌표 큐에 push
				//		if (vis[ii][jj] == 1) Umul_info.push({ ii,jj });
				//	}
				//}
			}
		}
	}
	return ret;
}

int bfs_remove() {
	int ret = 0;
	int ccnt = 1;
	int vis[MXN + 2][MXN + 2];
	for (int tt = 0; tt < MXN; tt++) {
		fill(vis[tt], vis[tt] + MXN, 0);
	} // 방문 배열 초기화
	for (int xi = 0; xi < MXN; xi++) {
		for (int xj = 0; xj < MXN; xj++) {
			q.push({ xi,xj }); int val = board[xi][xj]; vis[xi][xj] = ccnt;
			int Umul_len = 1; // 유물 조각 크기
			while (!q.empty()) {
				auto cur = q.front(); q.pop();
				for (int dir = 0; dir < 4; dir++) {
					int nx = cur.X + dx[dir];
					int ny = cur.Y + dy[dir];
					if (nx < 0 || nx >= MXN || ny < 0 || ny >= MXN) continue;
					if (vis[nx][ny] != 0 || board[nx][ny] != val) continue;
					vis[nx][ny] = ccnt; q.push({ nx,ny });
					Umul_len++;
				}
			} // 현재 보고 있는 셀과 이어진 유물 탐색 종료
			if (Umul_len >= 3) {
				ret += Umul_len;
				for (int ii = 0; ii < MXN; ii++) {
					for (int jj = 0; jj < MXN; jj++) {
						// 지워야할 유물들 좌표 큐에 push
						if (vis[ii][jj] == ccnt ) {
							board[ii][jj] = 0; // 지워버림
						}
					}
				}
			}
			ccnt++;
		}
	}
	return ret;
}

void rot(int x, int y) {
	int tmp[MXN + 2][MXN + 2];
	for (int ii = 0; ii < 3; ii++) {
		for (int jj = 0; jj < 3; jj++) {
			tmp[x - 1 + jj][y - 1 + 2 -ii] = board[x - 1 + ii][y - 1 + jj];
		}
	}
	for (int ii = x - 1; ii <= x + 1; ii++) {
		for (int ij = y - 1; ij <= y + 1; ij++) {
			board[ii][ij] = tmp[ii][ij];
		}
	}
	return;
}

void copy_to_board() {
	for (int iii = 0; iii < MXN; iii++) {
		for (int jjj = 0; jjj < MXN; jjj++) {
			board[iii][jjj] = AncientMap[iii][jjj];
		}
	}
	return;
}

void copy_to_map() {
	for (int xi = 0; xi < MXN; xi++) {
		for (int xj = 0; xj < MXN; xj++) {
			AncientMap[xi][xj] = board[xi][xj];
		}
	}
	return;
}

void print_board() {
	cout << "---------------------------\n";
	for (int oi = 0; oi < MXN; oi++) {
		for (int oj = 0; oj < MXN; oj++) {
			cout << board[oi][oj] << ' ';
		}
		cout << '\n';
	}
}

int main() {
	ios::sync_with_stdio(0);
	cin.tie(0);
	cin >> K >> M;
	for (int i = 0; i < MXN; i++) {
		for (int j = 0; j < MXN; j++) {
			cin >> AncientMap[i][j];
		}
	}
	for (int i = 0; i < M; i++) {
		int tmp;
		cin >> tmp;
		WallNumber.push(tmp);
	}
	while (K--) {
		int ans = 0;
		for (int i = 1; i < MXN - 1; i++) {
			for (int j = 1; j < MXN - 1; j++) {
				copy_to_board();
				for (int t = 1; t < 4; t++)
				{
					// (i,j)를 중심으로 t번만큼 돌린다음 유물가치 탐색했음
					// t : 회전 횟수를 나타냄 t = 3 -> 270도 회전
					rot(i, j); // 현재 좌표에 대해서 한번 돌림
					int gachi = bfs();
					if ((int)Rot.size() < 1 && gachi != 0) { // 아직 들어간게 없으면
						Rot.push_back(make_tuple(j, i, gachi, t));
					}
					else if((int)Rot.size() >= 1 && gachi != 0){ // 들어간게 하나라도 있다면
						int worth = get<2>(Rot[0]);
						int cnt = get<3>(Rot[0]);
						if (gachi > worth) { //현재 경우가 가치가 더 크다면 바로 수정
							Rot[0] = make_tuple(j, i, gachi, t);
						}
						else if (gachi == worth) {
							if (t < cnt) { // 현재 경우가 회전 횟수가 더 적으면
								Rot[0] = make_tuple(j, i, gachi, t);
							}
							else if (t == cnt) { // 같다면
								Rot.push_back(make_tuple(j, i, gachi, t));
							}
						}
					} 
				}
			}
		} // 유물 가치와 회전 횟수에 따라 Rot에 후보군들 저장

		// 최적의 구간 선택을 위해 정렬
		if ((int)Rot.size() < 1) {
			return 0;
		}
		sort(Rot.begin(), Rot.end());
		int t = get<3>(Rot[0]);
		copy_to_board();
		while (t--) {
			rot(get<1>(Rot[0]), get<0>(Rot[0]));
		}
		ans += get<2>(Rot[0]);
		bfs_remove(); // Umul_info에도 담고 지워버렸음
		while (1) {
			for (int tj = 0; tj < MXN; tj++) {
				for (int ti = MXN - 1; ti >= 0; ti--) {
					if (board[ti][tj] == 0) {
						if(WallNumber.empty()) continue;
						board[ti][tj] = WallNumber.front();
						WallNumber.pop();
					}
				}
			}
			// board에 0이 있다면 유적 벽면 숫자로 다 채웠음
			int condi = bfs_remove();
			ans += condi;
			if (condi == 0) break;
		}
		cout << ans << ' ';
		copy_to_map();
		Rot.clear();

	}
	
	return 0;
}