/*
 * Autor: Emilio Moran Valdebenito
 * Rol: 202473575-6
 * Tarea 2 - INF-221 Algoritmos y Complejidad (2026-1)
 */
// Fuerza Bruta - AniMarathon
// Enumera todos los prefijos posibles para cada anime (backtracking con poda).
#include <bits/stdc++.h>
using namespace std;

struct Anime {
    string nombre;
    int q;
    long long bono;
    vector<int> t, c;       // duracion y costo por capitulo
    vector<long long> v;    // satisfaccion por capitulo
    // Prefijos acumulados
    vector<int> prefT, prefC;
    vector<long long> prefV;
};

int n, M, E;
vector<Anime> animes;
long long mejorSat;

// Backtracking: decide cuantos capitulos ver del anime idx
void solve(int idx, int usedT, int usedC, long long sat) {
    if (idx == n) {
        mejorSat = max(mejorSat, sat);
        return;
    }
    const Anime& a = animes[idx];
    // Probar cada prefijo k = 0..q
    for (int k = 0; k <= a.q; k++) {
        int newT = usedT + a.prefT[k];
        int newC = usedC + a.prefC[k];
        if (newT > M || newC > E) break; // poda: capitulos son incrementales
        long long newSat = sat + a.prefV[k];
        if (k == a.q) newSat += a.bono; // bono por completar
        solve(idx + 1, newT, newC, newSat);
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    // Lectura de entrada
    cin >> n >> M >> E;
    animes.resize(n);
    for (int i = 0; i < n; i++) {
        cin >> animes[i].nombre >> animes[i].q >> animes[i].bono;
        int q = animes[i].q;
        animes[i].t.resize(q);
        animes[i].c.resize(q);
        animes[i].v.resize(q);
        for (int j = 0; j < q; j++)
            cin >> animes[i].t[j] >> animes[i].c[j] >> animes[i].v[j];

        // Calcular prefijos acumulados (indice 0 = no ver nada)
        animes[i].prefT.resize(q + 1, 0);
        animes[i].prefC.resize(q + 1, 0);
        animes[i].prefV.resize(q + 1, 0);
        for (int j = 0; j < q; j++) {
            animes[i].prefT[j + 1] = animes[i].prefT[j] + animes[i].t[j];
            animes[i].prefC[j + 1] = animes[i].prefC[j] + animes[i].c[j];
            animes[i].prefV[j + 1] = animes[i].prefV[j] + animes[i].v[j];
        }
    }

    // Resolver
    mejorSat = 0;
    solve(0, 0, 0, 0);

    cout << mejorSat << endl;
    return 0;
}
