#include <fstream>
#include <vector>
#include <algorithm>
#include <iostream>
#include <string>
#include <queue>

using namespace std;

const int kNmax = 100005;

class Task {
 public:
	void solve() {
		read_input();
		print_output(get_result());
	}

 private:
	int n;
	int m;
	vector<int> adj[kNmax];

	void read_input() {
		ifstream fin("disjcnt.in");
		/* citesc numarul de noduri si de muchii */
		fin >> n >> m;
		/* creez listele de adiacenta */
		for (int i = 1, x, y; i <= m; i++) {
			fin >> x >> y;
			adj[x].push_back(y);
			adj[y].push_back(x);
		}
		fin.close();
	}

    void DFS (int node, int &start, vector<int> &idx, vector<int> &lowlink, 
				vector<int> &parent, vector<pair<int, int>> &critical_edges) {
		start++;

		/* atat idx, cat si lowlink vor primi valoarea lui start */
		idx[node] = start;
		lowlink[node] = start;

		/* initializez numarul de copii al nodului curent cu 0 */
		int children = 0;
        for (int neighbor : adj[node]) {
            if (idx[neighbor] == -1) {	/* deci il pot vizita */

				/* parintele nodului in care ma duc este nodul curent */
                parent[neighbor] = node;

				/* cresc numarul de copii */
				children++;

				/* pornesc un nou DFS */
				DFS(neighbor, start, idx, lowlink, parent, critical_edges);

				/* updatez lowlink */				
				lowlink[node] = min(lowlink[node], lowlink[neighbor]);

				/* 	daca nu au existat muchii de intoarcere spre node sau spre 
					un parinte al lui node => muchia (node, neighbor) este
					muchie critica */
				if (lowlink[neighbor] > idx[node]) {
					critical_edges.push_back(make_pair(node, neighbor));
				}
            }
			else {
				if (neighbor != parent[node]) {
					/* am gasit o muchie de intoarcere */
					lowlink[node] = min(lowlink[node], idx[neighbor]);
				}
			}
        }
    }

	void tarjan(vector<int> &idx, vector<int> &lowlink, vector<int> &parent,
				vector<pair<int, int>> &critical_edges) {
		/* momentul curent de start */
		int start = 0;
		for (int i = 1; i <= n; i++) {
			if (idx[i] == -1) {
				/* 	acest nod nu a fost descoperit
					pornim o noua cautare din accest nod */
				DFS(i, start, idx, lowlink, parent, critical_edges);
			}
		}
	}

	void BFS (int start, vector<bool> &visited, vector<int> &d) {
		queue<int> queue;
		visited[start] = true;
		/* adaug nodul de inceput in coada */
		queue.push(start);

		/* cat timp mai am noduri de adaugat */
		while (!queue.empty()) {
			/* determin nodul din varful cozii */
			start = queue.front();
			/* il adaug in vectorul in care retin parcurgerea BFS */
			d.push_back(start);
			/* elimin nodul din coada */
			queue.pop();
			/* parcurg toti vecinii */
			for (int i : adj[start]) {
				/* daca nodul nu a fost vizitat */
				if (!visited[i]) {
					/* il marchez ca vizitat si il adaug in coada */
					visited[i] = true;
					queue.push(i);
				}
			}
		}
	}

	int get_result() {
		/* 	ordinea de vizitare
			idx[node] = timpul de start a lui node in parcurgerea DFS */
		vector<int> idx(n + 1, -1);
		/* lowlink[node] = min { idx[x] | x este accesibil din node } */
		vector<int> lowlink(n + 1, 0);
		/* vector de parinti */
        vector<int> parent(n + 1, 0);
		/* vector in are retin muchiile critice (puntile) */
		vector<pair<int, int>> critical_edges;
		/* initialiez vectorul visited cu false */
		vector<bool> visited(n + 1, false);
		long number_of_pairs = 0;

		/* aplic Algoritmul lui Tarjan pentru a determina muchiile critice */
		tarjan(idx, lowlink, parent, critical_edges);

		/* 	parcurg toate muchiile critice si le elimin din graf (le sterg din
			listele de adiacenta) */
		for (int i = 0; i < critical_edges.size(); i++) {
			int x = critical_edges[i].first;
			int y = critical_edges[i].second;
			for (int j = 0; j < adj[x].size(); j++) {
				if (adj[x][j] == y) {
					adj[x].erase(adj[x].begin() + j);
					break;
				}
			}
			for (int j = 0; j < adj[y].size(); j++) {
				if (adj[y][j] == x) {
					adj[y].erase(adj[y].begin() + j);
					break;
				}
			}
		}

		/* 	parcurg toate componenetele conexe folosind BFS si retin 
			parcurgerile in vectorul d 
			pentru a determina numarul de perechi de noduri, am calculat suma
			combinarilor de n luate cate 2, unde n reprezinta numarul de noduri
			pe care le are fiecare componenta conexa */
		for (int i = 1; i <= n; i++) {
			if (!visited[i]) {
				vector<int> d;
				BFS(i, visited, d);
				long size = d.size();
				number_of_pairs += (size * (size - 1)) / 2;
			}
		}
		/* returnez numarul de perechi */
		return number_of_pairs;
 	}

	void print_output(int result) {
		ofstream fout("disjcnt.out");
		/* afisez rezultatul */
		fout << result << '\n';
		fout.close();
	}
};

int main() {
	Task *task = new Task();
	task->solve();
	delete task;
	return 0;
}
