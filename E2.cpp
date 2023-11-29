#include <iostream>
#include <fstream>
#include <vector>
#include <limits>
#include <cmath>

using namespace std;

const int INF = numeric_limits<int>::max();

// Estructura para representar una colonia en un plano coordenado
struct Colonia {
    int x, y;
};

// Función para encontrar el árbol de expansión mínima usando el algoritmo de Prim
void primMST(const vector<vector<int>>& grafo, vector<pair<int, int>>& cableado) {
    int n = grafo.size();
    vector<int> costoMinimo(n, INF);
    vector<bool> incluido(n, false);
    costoMinimo[0] = 0;

    for (int i = 0; i < n - 1; ++i) {
        int u = -1;
        for (int j = 0; j < n; ++j) {
            if (!incluido[j] && (u == -1 || costoMinimo[j] < costoMinimo[u]))
                u = j;
        }

        incluido[u] = true;

        for (int v = 0; v < n; ++v) {
            if (grafo[u][v] != 0 && !incluido[v] && grafo[u][v] < costoMinimo[v]) {
                costoMinimo[v] = grafo[u][v];
                cableado.push_back({u, v});
            }
        }
    }
}

// Función para encontrar la ruta más corta que visita cada colonia exactamente una vez y regresa a la colonia origen (Problema del Viajante)
void tsp(const vector<vector<int>>& grafo, const vector<pair<int, int>>& cableado, vector<int>& ruta) {
    int n = grafo.size();
    vector<bool> visitado(n, false);
    ruta.resize(n + 1);
    int distanciaTotal = 0;

    // Empezar desde la colonia 0
    int actual = 0;
    ruta[0] = actual;
    visitado[actual] = true;

    for (int i = 1; i < n; ++i) {
        int siguiente = -1;
        for (int j = 0; j < n; ++j) {
            if (!visitado[j] && (siguiente == -1 || grafo[actual][j] < grafo[actual][siguiente]))
                siguiente = j;
        }

        ruta[i] = siguiente;
        distanciaTotal += grafo[actual][siguiente];
        visitado[siguiente] = true;
        actual = siguiente;
    }

    // Regresar a la colonia origen
    ruta[n] = 0;
    distanciaTotal += grafo[actual][0];

    cout << "1. Forma aptima de cablear las colonias con fibra optica:\n";
    for (const auto& arco : cableado) {
        cout << "(" << char('A' + arco.first) << "," << char('A' + arco.second) << ") ";
    }
    cout << "\n\n";

    cout << "2. Ruta del repartidor:\n";
    for (const auto& colonia : ruta) {
        cout << char('A' + colonia) << " ";
    }
    cout << "\n\n";

    // 3. Distancia al servidor más cercano
    int indiceCentralMasCercana = 0;  // Suponemos que la primera central es la más cercana
    double distanciaMinima = numeric_limits<double>::max();

    for (int i = 0; i < n; ++i) {
        double distancia = hypot(grafo[i][0], grafo[i][1]);  // Distancia al servidor (0, 0)
        if (distancia < distanciaMinima) {
            distanciaMinima = distancia;
            indiceCentralMasCercana = i;
        }
    }

    cout << "3. Distancia al servidor mas cercano desde la colonia " << char('A' + indiceCentralMasCercana)
         << ": " << distanciaMinima << " kilometros\n\n";
}

int main() {
    ifstream archivo("entrada.txt");
    if (!archivo.is_open()) {
        cerr << "Error al abrir el archivo de entrada.\n";
        return 1;
    }

    int n;
    archivo >> n;

    // Leer la matriz de adyacencia
    vector<vector<int>> grafo(n, vector<int>(n));
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            archivo >> grafo[i][j];
        }
    }

    // 1. Encontrar la forma óptima de cablear las colonias con fibra óptica
    vector<pair<int, int>> cableado;
    primMST(grafo, cableado);

    // 2 y 3. Encontrar la ruta más corta para el repartidor y la distancia al servidor más cercano
    vector<int> ruta;
    tsp(grafo, cableado, ruta);

    archivo.close();
    return 0;
}