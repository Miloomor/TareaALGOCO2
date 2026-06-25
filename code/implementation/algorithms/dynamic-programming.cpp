/*
 * Autor: Emilio Moran Valdebenito
 * Rol: 202473575-6
 * Tarea 2 - INF-221 Algoritmos y Complejidad (2026-1)
 */
// Programacion Dinamica - AniMarathon
// Knapsack 2D (tiempo, energia) con grupos de items (prefijos por anime).
// dp[m][e] = max satisfaccion usando exactamente m minutos y e energia.
// Se recorre cada anime y para cada estado se prueba cada prefijo.
#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, M, E;
    cin >> n >> M >> E;

    // Leer animes y precalcular prefijos
    struct Anime {
        int q;
        long long bono;
        vector<int> prefT, prefC;
        vector<long long> prefV;
    };
    vector<Anime> animes(n);

    for (int i = 0; i < n; i++) {
        string nombre;
        cin >> nombre >> animes[i].q >> animes[i].bono;
        int q = animes[i].q;
        animes[i].prefT.resize(q + 1, 0);
        animes[i].prefC.resize(q + 1, 0);
        animes[i].prefV.resize(q + 1, 0);
        for (int j = 0; j < q; j++) {
            int t, c; long long v;
            cin >> t >> c >> v;
            animes[i].prefT[j + 1] = animes[i].prefT[j] + t;
            animes[i].prefC[j + 1] = animes[i].prefC[j] + c;
            animes[i].prefV[j + 1] = animes[i].prefV[j] + v;
        }
    }

    // DP 2D: dp[m][e] = max satisfaccion con m minutos y e energia usados
    // Inicializar con -1 (estado no alcanzable), dp[0][0] = 0
    vector<vector<long long>> dp(M + 1, vector<long long>(E + 1, -1));
    dp[0][0] = 0;

    // Procesar cada anime (grouped knapsack)
    for (int i = 0; i < n; i++) {
        // Recorrer en orden inverso para no reusar estados del mismo anime
        // Usamos tabla auxiliar para manejar el grupo
        vector<vector<long long>> ndp(M + 1, vector<long long>(E + 1, -1));

        for (int m = 0; m <= M; m++) {
            for (int e = 0; e <= E; e++) {
                if (dp[m][e] < 0) continue;
                // Opcion: no ver nada de este anime (k=0)
                ndp[m][e] = max(ndp[m][e], dp[m][e]);
                // Probar cada prefijo k = 1..q
                for (int k = 1; k <= animes[i].q; k++) {
                    int nm = m + animes[i].prefT[k];
                    int ne = e + animes[i].prefC[k];
                    if (nm > M || ne > E) break;
                    long long val = dp[m][e] + animes[i].prefV[k];
                    if (k == animes[i].q) val += animes[i].bono;
                    ndp[nm][ne] = max(ndp[nm][ne], val);
                }
            }
        }
        dp = move(ndp);
    }

    // Encontrar maximo en toda la tabla
    long long ans = 0;
    for (int m = 0; m <= M; m++)
        for (int e = 0; e <= E; e++)
            if (dp[m][e] > ans) ans = dp[m][e];

    cout << ans << endl;
    return 0;
}
