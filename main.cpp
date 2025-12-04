#include <iostream>
#include <string>
using namespace std;

/* ======================================================
   ESTRUCTURAS PRINCIPALES
   ====================================================== */

struct Miembro {
    int id;
    string nombre;
    string fecha;
    string descripcion;

    Miembro* padre;
    Miembro* hijo;
    Miembro* hermano;

    Miembro(int _id, const string& _nombre, const string& _fecha, const string& _desc)
        : id(_id), nombre(_nombre), fecha(_fecha), descripcion(_desc),
          padre(NULL), hijo(NULL), hermano(NULL) {}
};

struct IndexNode {
    int id;
    Miembro* persona;
    IndexNode* izq;
    IndexNode* der;

    IndexNode(int _id, Miembro* p)
        : id(_id), persona(p), izq(NULL), der(NULL) {}
};

/* ======================================================
   VARIABLE GLOBAL DEL ÁRBOL ÍNDICE
   ====================================================== */

IndexNode* raiz = NULL;

/* ======================================================
   VALIDACIONES
   ====================================================== */

bool parseEntero(const string& s, int &out) {
    if (s.empty()) return false;
    long num = 0;
    for (char c : s) {
        if (c < '0' || c > '9') return false;
        num = num * 10 + (c - '0');
        if (num > 2147483647L) return false;
    }
    out = (int)num;
    return true;
}

string leerLineaNoVacia(const string& msg) {
    string s;
    do {
        cout << msg;
        getline(cin, s);
        if (s.empty()) cout << "ERROR: campo vacío.\n";
    } while (s.empty());

    return s;
}

/* ======================================================
   OPERACIONES DEL ÁRBOL ÍNDICE (ABB)
   ====================================================== */

IndexNode* buscarIndex(IndexNode* nodo, int id) {
    if (!nodo) return NULL;
    if (id == nodo->id) return nodo;
    if (id < nodo->id) return buscarIndex(nodo->izq, id);
    return buscarIndex(nodo->der, id);
}

bool existeId(int id) {
    return buscarIndex(raiz, id) != NULL;
}

IndexNode* insertarIndex(IndexNode* nodo, IndexNode* nuevo) {
    if (!nodo) return nuevo;
    if (nuevo->id < nodo->id)
        nodo->izq = insertarIndex(nodo->izq, nuevo);
    else
        nodo->der = insertarIndex(nodo->der, nuevo);
    return nodo;
}

IndexNode* minimoIndex(IndexNode* nodo) {
    while (nodo && nodo->izq) nodo = nodo->izq;
    return nodo;
}

IndexNode* eliminarIndex(IndexNode* nodo, int id) {
    if (!nodo) return NULL;

    if (id < nodo->id)
        nodo->izq = eliminarIndex(nodo->izq, id);
    else if (id > nodo->id)
        nodo->der = eliminarIndex(nodo->der, id);
    else {
        if (!nodo->izq) {
            IndexNode* r = nodo->der;
            delete nodo;
            return r;
        }
        if (!nodo->der) {
            IndexNode* l = nodo->izq;
            delete nodo;
            return l;
        }
        IndexNode* s = minimoIndex(nodo->der);
        nodo->id = s->id;
        nodo->persona = s->persona;
        nodo->der = eliminarIndex(nodo->der, s->id);
    }
    return nodo;
}

/* ======================================================
   OPERACIONES DE ARBOL GENEALÓGICO
   ====================================================== */

void anexarComoHijo(Miembro* padre, Miembro* nuevo) {
    nuevo->hermano = padre->hijo;
    padre->hijo = nuevo;
    nuevo->padre = padre;
}

void quitarDePadre(Miembro* padre, Miembro* hijo) {
    Miembro* act = padre->hijo;
    Miembro* ant = NULL;

    while (act) {
        if (act == hijo) {
            if (ant) ant->hermano = act->hermano;
            else padre->hijo = act->hermano;
            return;
        }
        ant = act;
        act = act->hermano;
    }
}

void liberarSubArbol(Miembro* m) {
    if (!m) return;

    Miembro* h = m->hijo;
    while (h) {
        Miembro* sig = h->hermano;
        liberarSubArbol(h);
        h = sig;
    }

    delete m;
}

/* ======================================================
   BUSCAR MIEMBRO POR ID
   ====================================================== */

Miembro* buscarMiembro(int id) {
    IndexNode* n = buscarIndex(raiz, id);
    return n ? n->persona : NULL;
}

/* ======================================================
   RECOLECTAR IDS DE UNA SUBRAMA
   ====================================================== */

void recolectarIds(Miembro* r, int out[], int &n) {
    if (!r) return;
    out[n++] = r->id;
    for (Miembro* h = r->hijo; h; h = h->hermano)
        recolectarIds(h, out, n);
}

/* ======================================================
   FUNCIONES PRINCIPALES
   ====================================================== */

void registrarMiembro() {
    string idS;
    int id;

    while (true) {
        cout << "ID (solo números): ";
        cin >> idS;

        if (!parseEntero(idS, id)) { cout << "ID inválido.\n"; continue; }
        if (existeId(id)) { cout << "Error: ID repetido.\n"; continue; }
        break;
    }

    cin.ignore();
    string nombre = leerLineaNoVacia("Nombre: ");
    string fecha = leerLineaNoVacia("Fecha de nacimiento: ");
    string desc = leerLineaNoVacia("Descripción: ");

    Miembro* padre = NULL;
    string idPadreS;
    int idPadre;

    while (true) {
        cout << "ID del padre (0 si no tiene): ";
        cin >> idPadreS;

        if (!parseEntero(idPadreS, idPadre)) { cout << "Inválido.\n"; continue; }
        if (idPadre == 0) break;

        padre = buscarMiembro(idPadre);
        if (!padre) { cout << "No existe ese padre.\n"; continue; }

        break;
    }
    cin.ignore();

    Miembro* nuevo = new Miembro(id, nombre, fecha, desc);

    if (padre) anexarComoHijo(padre, nuevo);

    raiz = insertarIndex(raiz, new IndexNode(id, nuevo));

    cout << "Miembro registrado.\n";
}

void consultarMiembro() {
    string idS;
    cout << "ID a consultar: ";
    getline(cin, idS);

    int id;
    if (!parseEntero(idS, id)) { cout << "ID inválido.\n"; return; }

    Miembro* m = buscarMiembro(id);
    if (!m) { cout << "No existe.\n"; return; }

    cout << "\n--- Datos ---\n";
    cout << "ID: " << m->id << "\n";
    cout << "Nombre: " << m->nombre << "\n";
    cout << "Nacimiento: " << m->fecha << "\n";
    cout << "Descripción: " << m->descripcion << "\n";

    if (m->padre)
        cout << "Padre: " << m->padre->nombre << " (ID " << m->padre->id << ")\n";
    else
        cout << "Padre: ninguno\n";

    int cont = 0;
    for (Miembro* h = m->hijo; h; h = h->hermano) cont++;
    cout << "Número de hijos: " << cont << "\n";
}

void mostrarAncestros() {
    string idS;
    cout << "ID: ";
    getline(cin, idS);

    int id;
    if (!parseEntero(idS, id)) { cout << "ID inválido.\n"; return; }

    Miembro* m = buscarMiembro(id);
    if (!m) { cout << "No existe.\n"; return; }
    if (!m->padre) { cout << "No tiene padre registrado.\n"; return; }

    cout << "Ancestros: ";
    Miembro* p = m->padre;
    bool primero = true;

    while (p) {
        if (!primero) cout << " <- ";
        cout << p->nombre << " (ID " << p->id << ")";
        primero = false;
        p = p->padre;
    }
    cout << "\n";
}

void mostrarDescendientesRec(Miembro* m, int nivel) {
    if (!m) return;
    for (Miembro* h = m->hijo; h; h = h->hermano) {
        for (int i = 0; i < nivel; i++) cout << "  ";
        cout << h->nombre << " (ID " << h->id << ")\n";
        mostrarDescendientesRec(h, nivel + 1);
    }
}

void mostrarDescendientes() {
    string idS;
    cout << "ID: ";
    getline(cin, idS);
    int id;

    if (!parseEntero(idS, id)) { cout << "ID inválido.\n"; return; }

    Miembro* m = buscarMiembro(id);
    if (!m) { cout << "No existe.\n"; return; }

    cout << "Descendientes de " << m->nombre << ":\n";
    mostrarDescendientesRec(m, 1);
}

void modificarMiembro() {
    string idS;
    cout << "ID del miembro a modificar: ";
    getline(cin, idS);

    int id;
    if (!parseEntero(idS, id)) { cout << "ID inválido.\n"; return; }

    Miembro* m = buscarMiembro(id);
    if (!m) { cout << "No existe.\n"; return; }

    cout << "Nuevo nombre: ";
    getline(cin, m->nombre);

    cout << "Nueva fecha: ";
    getline(cin, m->fecha);

    cout << "Nueva descripción: ";
    getline(cin, m->descripcion);

    cout << "Actualizado.\n";
}

void eliminarMiembroRama() {
    string idS;
    cout << "ID a eliminar: ";
    getline(cin, idS);

    int id;
    if (!parseEntero(idS, id)) { cout << "ID inválido.\n"; return; }

    Miembro* m = buscarMiembro(id);
    if (!m) { cout << "No existe.\n"; return; }

    // Quitar referencia del padre
    if (m->padre) quitarDePadre(m->padre, m);

    // Recolectar IDs
    int ids[1000], n = 0;
    recolectarIds(m, ids, n);

    // Liberar genealogía
    liberarSubArbol(m);

    // Eliminar nodos del índice
    for (int i = 0; i < n; i++)
        raiz = eliminarIndex(raiz, ids[i]);

    cout << "Miembro y rama eliminados.\n";
}

void mostrarTodosRec(IndexNode* nodo) {
    if (!nodo) return;
    mostrarTodosRec(nodo->izq);

    Miembro* m = nodo->persona;
    int c = 0;
    for (Miembro* h = m->hijo; h; h = h->hermano) c++;

    cout << "ID " << nodo->id << ": " << m->nombre << " | Hijos: " << c << "\n";

    mostrarTodosRec(nodo->der);
}

void mostrarTodos() {
    cout << "=== Miembros ===\n";
    mostrarTodosRec(raiz);
}

/* ======================================================
   MENÚ PRINCIPAL
   ====================================================== */

int main() {
    cout << "Sistema Genealógico";

    while (true) {
        cout << "\n--- MENU ---\n";
        cout << "1. Registrar miembro\n";
        cout << "2. Consultar miembro\n";
        cout << "3. Ver ancestros\n";
        cout << "4. Ver descendientes\n";
        cout << "5. Eliminar miembro (rama completa)\n";
        cout << "6. Mostrar todos\n";
        cout << "7. Modificar datos\n";
        cout << "0. Salir\n";
        cout << "Opción: ";

        int op;
        cin >> op;
        cin.ignore();

        switch (op) {
            case 1: registrarMiembro(); break;
            case 2: consultarMiembro(); break;
            case 3: mostrarAncestros(); break;
            case 4: mostrarDescendientes(); break;
            case 5: eliminarMiembroRama(); break;
            case 6: mostrarTodos(); break;
            case 7: modificarMiembro(); break;
            case 0: cout << "Adiós.\n"; return 0;
            default: cout << "Opción inválida.\n"; break;
        }
    }
}
