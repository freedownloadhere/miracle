#include <iostream>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <chrono>

using namespace std;

struct coords {
	size_t x = 0, y = 0;
};

static bool waitForMiracle(
	vector<vector<char>>& mat, 
	const size_t n, const size_t m, 
	const coords obstacle,
	coords pos
) {
	const auto start = chrono::high_resolution_clock::now();
	const coords d[4] = { {-1, 0}, {0, 1}, {1, 0}, {0, -1} };

	int dir = 0;
	bool isAlreadyObstacle = (mat[obstacle.x][obstacle.y] == '#');

	cout << "performing miracle on " << obstacle.x << ' ' << obstacle.y << ": ";

	mat[obstacle.x][obstacle.y] = '#';

	while (pos.x >= 0 && pos.y >= 0 && pos.x < n && pos.y < m) {
		if (mat[pos.x][pos.y] == '#') {
			pos.x -= d[dir].x;
			pos.y -= d[dir].y;
			dir++;
			dir %= 4;
			continue;
		}

		pos.x += d[dir].x;
		pos.y += d[dir].y;

		const auto now = chrono::high_resolution_clock::now();
		const auto timePassed = chrono::duration_cast<chrono::milliseconds>(now - start).count();

		if (timePassed > 100) {
			cout << "congrats! it's a loop :)" << endl;
			if(!isAlreadyObstacle)
				mat[obstacle.x][obstacle.y] = '.';
			return true;
		}
	}

	cout << "no loop :(" << endl;
	if(!isAlreadyObstacle)
		mat[obstacle.x][obstacle.y] = '.';
	return false;
}

static void solve() {
	ifstream fin("input.txt");
	string buf;
	vector<vector<char>> mat;
	while (getline(fin, buf)) {
		const size_t n = buf.size();
		if (!n) break;
		mat.push_back(vector<char>(n));
		for (size_t i = 0; i < n; i++)
			mat.back()[i] = buf[i];
	}

	const size_t n = mat.size();
	const size_t m = mat[0].size();
	coords pos;
	int ans = 0;

	for (size_t i = 0; i < n; i++)
		for (size_t j = 0; j < m; j++)
			if (mat[i][j] == '^')
				pos = { i, j };

	for (size_t i = 0; i < n; i++)
		for (size_t j = 0; j < m; j++)
			if (mat[i][j] != '^')
				ans += waitForMiracle(mat, n, m, { i, j }, pos);

	cout << endl << "answer is: " << ans << endl;
}

int main() {
	solve();
	return 0;
}
