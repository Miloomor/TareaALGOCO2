/*
 * Autor: Emilio Moran Valdebenito
 * Rol: 202473575-6
 * Tarea 2 - INF-221 Algoritmos y Complejidad (2026-1)
 */
// Greedy 2 - AniMarathon
// Criterio: seleccion marginal por capitulo. En cada paso, entre todos los
// animes, elige el proximo capitulo (siguiente del prefijo actual) con mejor
// ratio marginal satisfaccion / (tiempo + energia). Incluye bono si completa.
#include <bits/stdc++.h>
using namespace std;

struct Anime {
    string nombre;
    int q;
    long long bono;
    vector<int> t, c;
    vector<long long> v;
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, M, E;
    cin >> n >> M >> E;

    vector<Anime> animes(n);
    for (int i = 0; i < n; i++) {
        cin >> animes[i].nombre >> animes[i].q >> animes[i].bono;
        int q = animes[i].q;
        animes[i].t.resize(q); animes[i].c.resize(q); animes[i].v.resize(q);
        for (int j = 0; j < q; j++)
            cin >> animes[i].t[j] >> animes[i].c[j] >> animes[i].v[j];
    }

    // k[i] = cantidad de capitulos vistos del anime i
    vector<int> k(n, 0);
    int usedT = 0, usedC = 0;
    long long totalSat = 0;

    // Iterativamente agregar el mejor capitulo marginal
    while (true) {
        int bestIdx = -1;
        double bestRatio = -1;

        for (int i = 0; i < n; i++) {
            if (k[i] >= animes[i].q) continue; // anime completo
            int j = k[i]; // proximo capitulo
            int dt = animes[i].t[j];
            int dc = animes[i].c[j];
            if (usedT + dt > M || usedC + dc > E) continue; // no cabe

            long long dv = animes[i].v[j];
            // Si este es el ultimo capitulo, incluir bono
            if (j == animes[i].q - 1) dv += animes[i].bono;

            double peso = dt + dc;
            double ratio = (peso > 0) ? (double)dv / peso : 1e18;
            if (ratio > bestRatio) {
                bestRatio = ratio;
                bestIdx = i;
            }
        }

        if (bestIdx == -1) break; // no hay mas capitulos factibles

        int j = k[bestIdx];
        usedT += animes[bestIdx].t[j];
        usedC += animes[bestIdx].c[j];
        totalSat += animes[bestIdx].v[j];
        if (j == animes[bestIdx].q - 1) totalSat += animes[bestIdx].bono;
        k[bestIdx]++;
    }

    cout << totalSat << endl;
    return 0;
}
