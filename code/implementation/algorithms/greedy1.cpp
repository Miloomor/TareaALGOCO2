// Greedy 1 - AniMarathon
// Criterio: densidad de satisfaccion = valor / (tiempo + energia).
// Para cada anime, elige el mejor prefijo por densidad. Ordena animes
// por su mejor densidad y selecciona greedily.
#include <bits/stdc++.h>
using namespace std;

struct Anime {
    string nombre;
    int q;
    long long bono;
    vector<int> t, c;
    vector<long long> v;
    vector<int> prefT, prefC;
    vector<long long> prefV;
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
        animes[i].prefT.resize(q + 1, 0);
        animes[i].prefC.resize(q + 1, 0);
        animes[i].prefV.resize(q + 1, 0);
        for (int j = 0; j < q; j++) {
            animes[i].prefT[j + 1] = animes[i].prefT[j] + animes[i].t[j];
            animes[i].prefC[j + 1] = animes[i].prefC[j] + animes[i].c[j];
            animes[i].prefV[j + 1] = animes[i].prefV[j] + animes[i].v[j];
        }
    }

    // Para cada anime, encontrar el prefijo con mejor densidad
    // Candidato: (densidad, indice_anime, k_elegido, tiempo, costo, satisfaccion)
    struct Candidato {
        double densidad;
        int idx, k, tiempo, costo;
        long long sat;
    };
    vector<Candidato> candidatos;

    for (int i = 0; i < n; i++) {
        double mejorDens = -1;
        int mejorK = 0;
        for (int k = 1; k <= animes[i].q; k++) {
            long long val = animes[i].prefV[k];
            if (k == animes[i].q) val += animes[i].bono;
            double peso = animes[i].prefT[k] + animes[i].prefC[k];
            double dens = (peso > 0) ? (double)val / peso : 1e18;
            if (dens > mejorDens) {
                mejorDens = dens;
                mejorK = k;
            }
        }
        if (mejorK > 0) {
            long long val = animes[i].prefV[mejorK];
            if (mejorK == animes[i].q) val += animes[i].bono;
            candidatos.push_back({mejorDens, i, mejorK,
                animes[i].prefT[mejorK], animes[i].prefC[mejorK], val});
        }
    }

    // Ordenar por densidad descendente
    sort(candidatos.begin(), candidatos.end(),
        [](const Candidato& a, const Candidato& b) { return a.densidad > b.densidad; });

    // Seleccion greedy
    int usedT = 0, usedC = 0;
    long long totalSat = 0;
    for (auto& cand : candidatos) {
        if (usedT + cand.tiempo <= M && usedC + cand.costo <= E) {
            usedT += cand.tiempo;
            usedC += cand.costo;
            totalSat += cand.sat;
        }
    }

    cout << totalSat << endl;
    return 0;
}
