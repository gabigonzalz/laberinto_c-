#include <iostream> // Entrada y salida de datos, como cin y cout
#include <vector> // Contenedor de tipo vector para manejar matrices dinámicas
#include <stack> // Contenedor de tipo stack para manejar pilas LIFO
#include <algorithm> // Funciones de algoritmos estándar como shuffle
#include <random> // Generación de números aleatorios
#include <queue> // Contenedor de tipo queue para manejar colas FIFO

using namespace std;

// Definimos la estructura Celda para almacenar las coordenadas x e y de una celda en el laberinto
struct Celda {
    int x, y;
    Celda(int _x, int _y) : x(_x), y(_y) {} // Constructor que inicializa x e y
};

// ---------------------------- Prototipado de funciones ----------------------------
void crearCaminoDFS(int** matriz, int filas, int columnas, int x, int y);
int** generarLaberinto(int filas, int columnas);
bool encontrarCaminoBFS(int** matriz, int filas, int columnas, int x, int y, int fin_x, int fin_y);
void imprimirLaberinto(int filas, int columnas, int **matriz, int inicio_x, int inicio_y, int fin_x, int fin_y);
int hacerImpar(int num);

// ---------------------------- Flujo principal del programa --------------------------
int main() {
    // Definimos el tamano de el laberitno
    int filas = 15, columnas = 31;
    filas = hacerImpar(filas);
    columnas = hacerImpar(columnas);

    // Definimos punto de inicio y fin
    int inicio_x = 2, inicio_y = 2;
    int fin_x = columnas - 3, fin_y = filas - 3;

    // Generamos el laberinto
    int **matriz = generarLaberinto(filas, columnas);

    // Generamos la resolucion
    if (!encontrarCaminoBFS(matriz, filas, columnas, inicio_x, inicio_y, fin_x, fin_y)) {
        cout << "No se encontró un camino\n"; // Si no se encuentra un camino, se notifica al usuario
    }

    // Imprime el resultado
    imprimirLaberinto(filas, columnas, matriz, inicio_x, inicio_y, fin_x, fin_y); // Imprime el laberinto

    // Gestion de memoria (pq matrices dinamicas)
    // Elimina todas las filas en la memoria
    for (int i = 0; i < filas; i++) {
        delete[] matriz[i];
    }
    // Libera la memoria de matriz
    delete[] matriz;

    return 0;
}

// ---------------------------- Definicion de funciones ----------------------------
// Función para asegurar que un número sea impar
int hacerImpar(int num) {
    if (num % 2 == 0) {
        num += 1;
    }
    return num;
}

// Función para crear caminos en el laberinto usando DFS y PILA
void crearCaminoDFS(int** matriz, int filas, int columnas, int x, int y) {
    // Definimos las direccions y la pila
    vector<Celda> direcciones = { Celda(2, 0), Celda(-2, 0), Celda(0, 2), Celda(0, -2) }; 
    stack<Celda> pila;
    // Empuja la celda inicial a la pila
    pila.push(Celda(x, y)); 
    matriz[y][x] = 0; // Y marcamos en la matriz
    // Generador de números aleatorios
    random_device rd; // Semilla
    mt19937 g(rd()); // definimos el tipo de generador (Mersenne Twister)

    while (!pila.empty()) {
        Celda actual = pila.top();
        pila.pop();
        shuffle(direcciones.begin(), direcciones.end(), g); // Mezcla aleatoriamente las direcciones

        for (Celda dir : direcciones) {
            int nx = actual.x + dir.x;
            int ny = actual.y + dir.y;

            if (nx > 0 && nx < columnas - 1 && ny > 0 && ny < filas - 1 && matriz[ny][nx] == 1) {
                matriz[ny][nx] = 0; // Marca la nueva celda como parte del camino
                matriz[ny - dir.y / 2][nx - dir.x / 2] = 0; // Marca la celda intermedia como parte del camino
                pila.push(Celda(nx, ny)); // Empuja la nueva celda a la pila
            }
        }
    }
}

// Función para generar un laberinto inicializado con paredes (1) y caminos (0)
int** generarLaberinto(int filas, int columnas) {
    // Por cada fila y columna definimos la matriz dinamica
    int **matriz = new int*[filas];
    for (int i = 0; i < filas; i++) {
        matriz[i] = new int[columnas];
        for (int j = 0; j < columnas; j++) {
            matriz[i][j] = 1; // Inicializa todas las celdas como paredes
        }
    }
    
    // Comienza a crear caminos desde la celda (2, 2)
    crearCaminoDFS(matriz, filas, columnas, 2, 2); 

    return matriz;
}

// Función para encontrar un camino desde el inicio hasta el fin usando BFS y COLA
bool encontrarCaminoBFS(int** matriz, int filas, int columnas, int x, int y, int fin_x, int fin_y) {
    // Definimos la COLA y las direcciones
    queue<Celda> cola;
    // Empuja la primera celda
    cola.push(Celda(x, y));
    matriz[y][x] = 2; // Marcamos en la matriz
    vector<Celda> direcciones = { Celda(1, 0), Celda(-1, 0), Celda(0, 1), Celda(0, -1) };
    
    while (!cola.empty()) {
        Celda actual = cola.front();
        cola.pop();

        // Checkeamos si se llego al final
        if (actual.x == fin_x && actual.y == fin_y) {
            return true;
        }

        for (Celda dir : direcciones) {
            int nx = actual.x + dir.x;
            int ny = actual.y + dir.y;

            if (nx >= 0 && nx < columnas && ny >= 0 && ny < filas && matriz[ny][nx] == 0) {
                matriz[ny][nx] = 2; // Marca la celda como visitada
                cola.push(Celda(nx, ny)); // Encola la nueva celda
            }
        }
    }

    return false; // No se encontró un camino (por si acaso)
}

// Función para imprimir el laberinto
void imprimirLaberinto(int filas, int columnas, int **matriz, int inicio_x, int inicio_y, int fin_x, int fin_y) {
    for (int i = 0; i < filas; i++) {
        for (int j = 0; j < columnas; j++) {
            if (i == inicio_y && j == inicio_x) {
                cout << "O "; // Marca el inicio con 'O'
            } else if (i == fin_y && j == fin_x) {
                cout << "O "; // Marca el fin con 'O'
            } else if (matriz[i][j] == 2) {
                cout << "x "; // Marca el camino encontrado con 'x'
            } else if (matriz[i][j] == 1) {
                char a = 178;
                cout << a << a; // Imprime las paredes
            } else if (matriz[i][j] == 0) {
                cout << "  "; // Imprime los caminos disponibles
            }
        }
        cout << "\n";
    }
}