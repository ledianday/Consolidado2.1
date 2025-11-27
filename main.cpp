#include <iostream>
#include <string>
using namespace std;

// --------------------------
// ESTRUCTURA DEL NODO
// --------------------------
struct Nodo {
    int id;
    string nombre;
    Nodo* izquierda;
    Nodo* derecha;

    Nodo(int _id, string _nombre) {
        id = _id;
        nombre = _nombre;
        izquierda = nullptr;
        derecha = nullptr;
    }
};

// --------------------------
// CLASE ARBOL BINARIO
// --------------------------
class Arbol {
private:
    Nodo* raiz;

    // Inserción recursiva
    Nodo* insertarRec(Nodo* nodo, int id, string nombre) {
        if (nodo == nullptr)
            return new Nodo(id, nombre);

        if (id < nodo->id)
            nodo->izquierda = insertarRec(nodo->izquierda, id, nombre);
        else
            nodo->derecha = insertarRec(nodo->derecha, id, nombre);

        return nodo;
    }

    // Búsqueda recursiva
    bool buscarRec(Nodo* nodo, int id) {
        if (nodo == nullptr)
            return false;
        if (id == nodo->id)
            return true;
        if (id < nodo->id)
            return buscarRec(nodo->izquierda, id);
        return buscarRec(nodo->derecha, id);
    }

    // Encontrar el menor del subárbol derecho (sucesor)
    Nodo* minValor(Nodo* nodo) {
        while (nodo->izquierda != nullptr)
            nodo = nodo->izquierda;
        return nodo;
    }

    // Eliminación recursiva
    Nodo* eliminarRec(Nodo* nodo, int id) {
        if (nodo == nullptr)
            return nodo;

        if (id < nodo->id)
            nodo->izquierda = eliminarRec(nodo->izquierda, id);
        else if (id > nodo->id)
            nodo->derecha = eliminarRec(nodo->derecha, id);
        else {
            // Caso 1: sin hijos
            if (nodo->izquierda == nullptr && nodo->derecha == nullptr) {
                delete nodo;
                return nullptr;
            }
            // Caso 2: un hijo
            else if (nodo->izquierda == nullptr) {
                Nodo* temp = nodo->derecha;
                delete nodo;
                return temp;
            }
            else if (nodo->derecha == nullptr) {
                Nodo* temp = nodo->izquierda;
                delete nodo;
                return temp;
            }
            // Caso 3: dos hijos
            Nodo* temp = minValor(nodo->derecha);
            nodo->id = temp->id;
            nodo->nombre = temp->nombre;
            nodo->derecha = eliminarRec(nodo->derecha, temp->id);
        }

        return nodo;
    }

    // Recorridos
    void inOrdenRec(Nodo* nodo) {
        if (nodo == nullptr) return;
        inOrdenRec(nodo->izquierda);
        cout << nodo->id << " - " << nodo->nombre << endl;
        inOrdenRec(nodo->derecha);
    }

    void preOrdenRec(Nodo* nodo) {
        if (nodo == nullptr) return;
        cout << nodo->id << " - " << nodo->nombre << endl;
        preOrdenRec(nodo->izquierda);
        preOrdenRec(nodo->derecha);
    }

    void postOrdenRec(Nodo* nodo) {
        if (nodo == nullptr) return;
        postOrdenRec(nodo->izquierda);
        postOrdenRec(nodo->derecha);
        cout << nodo->id << " - " << nodo->nombre << endl;
    }

    // Representación visual del árbol
    void imprimirArbol(Nodo* nodo, int espacio) {
        if (nodo == nullptr) return;

        espacio += 5;

        imprimirArbol(nodo->derecha, espacio);

        cout << endl;
        for (int i = 5; i < espacio; i++) cout << " ";
        cout << nodo->id << "(" << nodo->nombre << ")" << endl;

        imprimirArbol(nodo->izquierda, espacio);
    }

public:
    Arbol() {
        raiz = nullptr;
    }

    void insertar(int id, string nombre) {
        raiz = insertarRec(raiz, id, nombre);
    }

    bool buscar(int id) {
        return buscarRec(raiz, id);
    }

    void eliminar(int id) {
        raiz = eliminarRec(raiz, id);
    }

    void inOrden() { inOrdenRec(raiz); }
    void preOrden() { preOrdenRec(raiz); }
    void postOrden() { postOrdenRec(raiz); }

    void visualizar() {
        imprimirArbol(raiz, 0);
    }
};


// --------------------------
// PROGRAMA PRINCIPAL
// --------------------------
int main() {
    Arbol genealogia;

    // Insertar 10 miembros ficticios
    genealogia.insertar(50, "Akar");
    genealogia.insertar(30, "Namil");
    genealogia.insertar(20, "Harek");
    genealogia.insertar(40, "Suri");
    genealogia.insertar(70, "Tarek");
    genealogia.insertar(60, "Miran");
    genealogia.insertar(80, "Zahir");
    genealogia.insertar(10, "Orek");
    genealogia.insertar(35, "Luna");
    genealogia.insertar(45, "Mara");

    int opcion, id;
    string nombre;

    do {
        cout << "\n=== MENU GENEALOGIA ===\n";
        cout << "1. Insertar miembro\n";
        cout << "2. Buscar miembro\n";
        cout << "3. Eliminar miembro\n";
        cout << "4. Mostrar Inorden\n";
        cout << "5. Mostrar Preorden\n";
        cout << "6. Mostrar Postorden\n";
        cout << "7. Visualizar arbol\n";
        cout << "0. Salir\n";
        cout << "Opcion: ";
        cin >> opcion;

        switch (opcion) {
        case 1:
            cout << "ID: ";
            cin >> id;
            cout << "Nombre: ";
            cin >> nombre;
            genealogia.insertar(id, nombre);
            cout << "Miembro insertado.\n";
            break;

        case 2:
            cout << "ID a buscar: ";
            cin >> id;
            if (genealogia.buscar(id))
                cout << "Miembro encontrado.\n";
            else
                cout << "No existe.\n";
            break;

        case 3:
            cout << "ID a eliminar: ";
            cin >> id;
            genealogia.eliminar(id);
            cout << "Miembro eliminado.\n";
            break;

        case 4:
            cout << "\n--- Inorden ---\n";
            genealogia.inOrden();
            break;

        case 5:
            cout << "\n--- Preorden ---\n";
            genealogia.preOrden();
            break;

        case 6:
            cout << "\n--- Postorden ---\n";
            genealogia.postOrden();
            break;

        case 7:
            cout << "\n--- Arbol ---\n";
            genealogia.visualizar();
            break;
        }
    } while (opcion != 0);

    return 0;
}

