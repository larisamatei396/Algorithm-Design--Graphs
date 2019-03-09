#include <fstream>
#include <iostream>
#include <vector>
#include <algorithm>
#include <queue>
using namespace std;

const int kNmax = 100005;

struct comparator {
  bool operator() (const int &a, const int &b) const {
    if (a > b)
      return true;
    return false;
  }
};

class Task {
  public:
    void solve() {
      read_input();
      print_output(get_result());
    }

  private:
    int N;
    int M;
    priority_queue<int, vector<int>, comparator> adj[kNmax];
    vector<int> d;

    void read_input() {
      ifstream fin("minlexbfs.in");
      /* citesc numarul de noduri si de muchii */
      fin >> N >> M;
      /* creez listele de adiacenta */
      for (int i = 1, X, Y; i <= M; i++) {
        fin >> X >> Y;
        /* graf neorientat => X este vecinul lui Y si Y este vecinul lui X */
        adj[X].push(Y);
        adj[Y].push(X);
      }
      fin.close();
    }

    vector<int> BFS (int start) {
      /* initializez vectorul visited cu false */
      vector<bool> visited(N + 1, false);
      queue<int> queue;
      visited[start] = true;
      /* adaug nodul de inceput in coada */
      queue.push(start);

      /* cat timp mai am noduri de prelucrat */ 
      while (!queue.empty()) {
        /* determin nodul din varful cozii */
        start = queue.front();
        /* il adaug in vectorul in care retin parcurgerea BFS */
        d.push_back(start);
        /* elimin nodul din coada */
        queue.pop();
      /* parcurg toti vecinii */
      while (!adj[start].empty()) {
        int i = adj[start].top();
        adj[start].pop();
          /* daca nodul nu a fost vizitat */
          if (!visited[i]) {
            /* il marchez ca vizitat si il adaug in coada */
            visited[i] = true;
            queue.push(i);
          }
        }
      }
      /* returnez parcurgerea BFS */
      return d;
    }

    vector<int> get_result() {
      /* parcurg graful incepand cu nodul 1 */
      d = BFS(1);
      return d;
    }

    /* afisez rezultatul parcurgerii BFS */
    void print_output(vector<int> result) {
      ofstream fout("minlexbfs.out");
      for (int i = 0; i < N; i++) {
        fout << result[i] << (i == N ? '\n' : ' ');
      }
      fout.close();
    }
};

  int main() {
    Task *task = new Task();
    task->solve();
    delete task;
    return 0;
  }
