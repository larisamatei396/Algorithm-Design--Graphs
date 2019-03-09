#include <fstream>
#include <vector>
#include <algorithm>
#include <iostream>
#include <string>

using namespace std;

const int kNmax = 501;

class Task {
 public:
	void solve() {
		read_input();
		print_output(get_result());
	}

 private:
	int N;
	int M;
    long Q;
	/* matricea de adiacenta */
    int distances[kNmax][kNmax];
	/* vector in care retin query-urile */
	vector<pair<int, int>> edges;

	void read_input() {
		ifstream fin("revedges.in");
		fin >> N >> M >> Q;
		/* initializez matricea de adiacenta cu -1 si 0 pe diagonala principala */
		for (int i = 1; i <= N; i++)
			for (int j = 1; j <= N; j++) {
				if (i == j)
					distances[i][j] = 0;
				else
					distances[i][j] = -1;
			}
		for (int i = 1, x, y; i <= M; i++) {
			fin >> x >> y;
			/* 	daca am muchie de la x la y, pun 0 in matricea de adiacenta, iar
				pentru muchia de la y la x pun 1 */
			if (distances[x][y] == -1) {
				distances[x][y] = 0;
				distances[y][x] = 1;
			}
			else 
				distances[x][y] = 0;
		}
		/* citesc cele Q query-uri */
		for (int i = 1, x, y; i <= Q; i++) {
			fin >> x >> y;
			edges.push_back(make_pair(x, y));
		}
		fin.close();
	}

    vector<int> get_result() {
		vector<int> result;

		/* Algoritmul lui Floyd-Warshall */
		for (int k = 1; k <= N; k++)
			/* Fixez nodul intermediar, k */
			for (int i = 1; i <= N; i++)
				for (int j = 1; j <= N; j++) {
					if (i == j)
						continue;
					/* 	Daca nu exista drum de la i la k sau de la k la j, 
						sarim peste */
					if (distances[i][k] == -1 || distances[k][j] == -1)
						continue;
					/* 	Fixez cele doua capete ale drumului pe care updatez (i, j).
						Daca nu am drum sau daca obtin un cost mai mic intre i, j, 
						updatez */
					if (distances[i][j] == -1 || 
						distances[i][j] > distances[i][k] + distances[k][j])
						distances[i][j] = distances[i][k] + distances[k][j];
				}

		/* 	parcurg vectorul de query-uri si adaug rezultatul intr-un vector 
			(adaug numarul de muchii pe care trebuie sa le intorc pentru a
			avea drum de la x la y) */
		for (int i = 0; i < edges.size(); i++) {
			int x = edges[i].first;
			int y = edges[i].second;
			result.push_back(distances[x][y]);
		}
        return result;
    }

	void print_output(vector<int> result) {
		ofstream fout("revedges.out");
		/* afisez vectorul care contine rezultatele */
		for (int i = 0; i < result.size(); i++)
			fout << result[i] << '\n';
		fout.close();
	}
};

int main() {
	Task *task = new Task();
	task->solve();
	delete task;
	return 0;
}
