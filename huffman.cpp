#include <iostream>
#include <unordered_map>
#include <queue>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

// Nodo del árbol de Huffman
struct Nodo {
    char caracter;
    int frecuencia;
    Nodo* izquierda;
    Nodo* derecha;

    Nodo(char c, int f) : caracter(c), frecuencia(f), izquierda(nullptr), derecha(nullptr) {}
};

// Comparador para la cola de prioridad
struct Comparador {
    bool operator()(Nodo* a, Nodo* b) {
        return a->frecuencia > b->frecuencia;
    }
};

// Función para generar los códigos de Huffman
void generarCodigos(Nodo* raiz, string codigo, unordered_map<char, string>& codigos) {
    if (!raiz) return;
    if (!raiz->izquierda && !raiz->derecha) {
        codigos[raiz->caracter] = codigo;
    }
    generarCodigos(raiz->izquierda, codigo + "0", codigos);
    generarCodigos(raiz->derecha, codigo + "1", codigos);
}

// Función para construir el árbol de Huffman
Nodo* construirArbolHuffman(const unordered_map<char, int>& frecuencias) {
    priority_queue<Nodo*, vector<Nodo*>, Comparador> pq;
    for (auto& par : frecuencias) {
        pq.push(new Nodo(par.first, par.second));
    }
    while (pq.size() > 1) {
        Nodo* izquierda = pq.top(); pq.pop();
        Nodo* derecha = pq.top(); pq.pop();
        Nodo* nuevoNodo = new Nodo('\0', izquierda->frecuencia + derecha->frecuencia);
        nuevoNodo->izquierda = izquierda;
        nuevoNodo->derecha = derecha;
        pq.push(nuevoNodo);
    }
    return pq.top();
}

// Función para obtener los códigos de Huffman canónicos
unordered_map<char, string> huffmanCanonico(const string& palabra) {
    unordered_map<char, int> frecuencias;
    for (char c : palabra) {
        frecuencias[c]++;
    }

    Nodo* raiz = construirArbolHuffman(frecuencias);

    unordered_map<char, string> codigos;
    generarCodigos(raiz, "", codigos);

    vector<pair<string, char>> codigosOrdenados;
    for (auto& par : codigos) {
        codigosOrdenados.push_back({par.second, par.first});
    }

    sort(codigosOrdenados.begin(), codigosOrdenados.end(), [](const pair<string, char>& a, const pair<string, char>& b) {
        if (a.first.length() == b.first.length()) {
            return a.first < b.first;
        }
        return a.first.length() < b.first.length();
    });

    unordered_map<char, string> codigosCanonicamenteOrdenados;
    string codigoActual = "";
    for (auto& par : codigosOrdenados) {
        if (codigoActual.empty()) {
            codigoActual = string(par.first.length(), '0');
        } else {
            int i = codigoActual.length() - 1;
            while (i >= 0 && codigoActual[i] == '1') {
                codigoActual[i] = '0';
                i--;
            }
            if (i >= 0) {
                codigoActual[i] = '1';
            } else {
                codigoActual = "1" + codigoActual;
            }
        }
        codigosCanonicamenteOrdenados[par.second] = codigoActual;
    }

    return codigosCanonicamenteOrdenados;
}

int main() {
    string palabra;
    cout << "Ingrese una palabra: ";
    cin >> palabra;

    unordered_map<char, string> codigos = huffmanCanonico(palabra);

    // Mostrar los códigos de manera organizada
    cout << "\nCodigos de Huffman canonicos:\n";
    cout << "Caracter | Codigo\n";
    cout << "-----------------\n";
    for (auto& par : codigos) {
        cout << "   " << par.first << "    | " << par.second << "\n";
    }

    // Mostrar los códigos concatenados
    cout << "\nCodigos concatenados:\n";
    for (char c : palabra) {
        cout << codigos[c] << " " ;
    }
    cout << "\n";

    return 0;
}
