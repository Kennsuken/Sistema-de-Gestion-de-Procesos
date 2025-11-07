#include <iostream>
#include <cstring>
#include <cstdlib>

using namespace std;

// ==================== ESTRUCTURAS ====================
struct Proceso {
    int id;
    char nombre[50];
    int prioridad;
    int memoria;
    Proceso* siguiente;
};

struct NodoCola {
    Proceso* proceso;
    NodoCola* siguiente;
};

struct BloqueMemoria {
    int id_proceso;
    int size;
    BloqueMemoria* siguiente;
};

// ==================== CLASE LISTA ====================
class ListaProcesos {
private:
    Proceso* cabeza;
    int contadorId;

public:
    ListaProcesos() {
        cabeza = NULL;
        contadorId = 1;
    }

    void insertarProceso(const char* nombre, int prioridad, int memoria) {
        Proceso* nuevo = new Proceso;
        nuevo->id = contadorId;
        strcpy(nuevo->nombre, nombre);
        nuevo->prioridad = prioridad;
        nuevo->memoria = memoria;
        nuevo->siguiente = NULL;
        
        if (cabeza == NULL) {
            cabeza = nuevo;
        } else {
            Proceso* actual = cabeza;
            while (actual->siguiente != NULL) {
                actual = actual->siguiente;
            }
            actual->siguiente = nuevo;
        }
        contadorId++;
        cout << "Proceso '" << nombre << "' agregado. ID: " << nuevo->id << endl;
    }

    Proceso* buscarPorId(int id) {
        Proceso* actual = cabeza;
        while (actual != NULL) {
            if (actual->id == id) {
                return actual;
            }
            actual = actual->siguiente;
        }
        return NULL;
    }

    void eliminarProceso(int id) {
        if (cabeza == NULL) {
            cout << "Lista vacia" << endl;
            return;
        }

        if (cabeza->id == id) {
            Proceso* temp = cabeza;
            cabeza = cabeza->siguiente;
            delete temp;
            cout << "Proceso " << id << " eliminado" << endl;
            return;
        }

        Proceso* actual = cabeza;
        while (actual->siguiente != NULL && actual->siguiente->id != id) {
            actual = actual->siguiente;
        }

        if (actual->siguiente != NULL) {
            Proceso* temp = actual->siguiente;
            actual->siguiente = actual->siguiente->siguiente;
            delete temp;
            cout << "Proceso " << id << " eliminado" << endl;
        } else {
            cout << "Proceso " << id << " no encontrado" << endl;
        }
    }

    void mostrarProcesos() {
        if (cabeza == NULL) {
            cout << "No hay procesos" << endl;
            return;
        }

        cout << "\n--- LISTA DE PROCESOS ---" << endl;
        Proceso* actual = cabeza;
        while (actual != NULL) {
            cout << "ID: " << actual->id 
                 << " | Nombre: " << actual->nombre 
                 << " | Prioridad: " << actual->prioridad 
                 << " | Memoria: " << actual->memoria << " KB" << endl;
            actual = actual->siguiente;
        }
    }
};

// ==================== CLASE COLA ====================
class ColaPrioridad {
private:
    NodoCola* frente;
    NodoCola* final;

public:
    ColaPrioridad() {
        frente = NULL;
        final = NULL;
    }

    void encolar(Proceso* proceso) {
        NodoCola* nuevo = new NodoCola;
        nuevo->proceso = proceso;
        nuevo->siguiente = NULL;
        
        if (frente == NULL) {
            frente = nuevo;
            final = nuevo;
        } else {
            NodoCola* actual = frente;
            NodoCola* anterior = NULL;
            
            while (actual != NULL && actual->proceso->prioridad >= proceso->prioridad) {
                anterior = actual;
                actual = actual->siguiente;
            }
            
            if (anterior == NULL) {
                nuevo->siguiente = frente;
                frente = nuevo;
            } else {
                anterior->siguiente = nuevo;
                nuevo->siguiente = actual;
                if (actual == NULL) {
                    final = nuevo;
                }
            }
        }
        cout << "Proceso '" << proceso->nombre << "' en cola. Prioridad: " << proceso->prioridad << endl;
    }

    Proceso* desencolar() {
        if (frente == NULL) {
            cout << "Cola vacia" << endl;
            return NULL;
        }
        
        NodoCola* temp = frente;
        Proceso* proceso = frente->proceso;
        frente = frente->siguiente;
        
        if (frente == NULL) {
            final = NULL;
        }
        
        delete temp;
        cout << "Ejecutando: " << proceso->nombre << endl;
        return proceso;
    }

    void mostrarCola() {
        if (frente == NULL) {
            cout << "Cola vacia" << endl;
            return;
        }

        cout << "\n--- COLA DE EJECUCION ---" << endl;
        NodoCola* actual = frente;
        while (actual != NULL) {
            cout << "Proceso: " << actual->proceso->nombre 
                 << " | Prioridad: " << actual->proceso->prioridad 
                 << " | Memoria: " << actual->proceso->memoria << " KB" << endl;
            actual = actual->siguiente;
        }
    }
};

// ==================== CLASE PILA ====================
class PilaMemoria {
private:
    BloqueMemoria* tope;
    int memoriaTotal;
    int memoriaUsada;

public:
    PilaMemoria() {
        tope = NULL;
        memoriaTotal = 1024;
        memoriaUsada = 0;
    }

    bool asignarMemoria(int id_proceso, int size) {
        if (memoriaUsada + size > memoriaTotal) {
            cout << "Memoria insuficiente. Necesita: " << size 
                 << " KB, Libre: " << (memoriaTotal - memoriaUsada) << " KB" << endl;
            return false;
        }

        BloqueMemoria* nuevo = new BloqueMemoria;
        nuevo->id_proceso = id_proceso;
        nuevo->size = size;
        nuevo->siguiente = tope;
        tope = nuevo;
        memoriaUsada += size;
        
        cout << "Memoria asignada - Proceso: " << id_proceso 
             << " | Size: " << size << " KB" 
             << " | Usada: " << memoriaUsada << "/" << memoriaTotal << " KB" << endl;
        return true;
    }

    void liberarMemoria() {
        if (tope == NULL) {
            cout << "No hay memoria asignada" << endl;
            return;
        }

        BloqueMemoria* temp = tope;
        memoriaUsada -= tope->size;
        cout << "Memoria liberada - Proceso: " << tope->id_proceso 
             << " | Size: " << tope->size << " KB" << endl;
        
        tope = tope->siguiente;
        delete temp;
    }

    void mostrarMemoria() {
        cout << "\n--- ESTADO DE MEMORIA ---" << endl;
        cout << "Memoria total: " << memoriaTotal << " KB" << endl;
        cout << "Memoria usada: " << memoriaUsada << " KB" << endl;
        cout << "Memoria libre: " << (memoriaTotal - memoriaUsada) << " KB" << endl;
        
        cout << "Bloques de memoria:" << endl;
        BloqueMemoria* actual = tope;
        while (actual != NULL) {
            cout << "  Proceso: " << actual->id_proceso 
                 << " | Size: " << actual->size << " KB" << endl;
            actual = actual->siguiente;
        }
    }
};

// ==================== MAIN ====================
void mostrarMenu() {
    cout << "\n=== SISTEMA GESTION PROCESOS ===" << endl;
    cout << "1. Agregar proceso" << endl;
    cout << "2. Mostrar procesos" << endl;
    cout << "3. Eliminar proceso" << endl;
    cout << "4. Buscar proceso" << endl;
    cout << "5. Encolar proceso" << endl;
    cout << "6. Ejecutar proceso" << endl;
    cout << "7. Mostrar cola" << endl;
    cout << "8. Asignar memoria" << endl;
    cout << "9. Liberar memoria" << endl;
    cout << "10. Mostrar memoria" << endl;
    cout << "0. Salir" << endl;
    cout << "Opcion: ";
}

int main() {
    ListaProcesos lista;
    ColaPrioridad cola;
    PilaMemoria pila;
    int opcion;

    do {
        mostrarMenu();
        cin >> opcion;

        switch (opcion) {
            case 1: {
                char nombre[50];
                int prioridad, memoria;
                cout << "Nombre proceso: ";
                cin.ignore();
                cin.getline(nombre, 50);
                cout << "Prioridad (1-10): ";
                cin >> prioridad;
                cout << "Memoria (KB): ";
                cin >> memoria;
                lista.insertarProceso(nombre, prioridad, memoria);
                break;
            }
            case 2:
                lista.mostrarProcesos();
                break;
            case 3: {
                int id;
                cout << "ID a eliminar: ";
                cin >> id;
                lista.eliminarProceso(id);
                break;
            }
            case 4: {
                int id;
                cout << "ID a buscar: ";
                cin >> id;
                Proceso* encontrado = lista.buscarPorId(id);
                if (encontrado != NULL) {
                    cout << "Encontrado: " << encontrado->nombre 
                         << " | Prioridad: " << encontrado->prioridad << endl;
                } else {
                    cout << "No encontrado" << endl;
                }
                break;
            }
            case 5: {
                int id;
                cout << "ID a encolar: ";
                cin >> id;
                Proceso* proceso = lista.buscarPorId(id);
                if (proceso != NULL) {
                    cola.encolar(proceso);
                } else {
                    cout << "Proceso no existe" << endl;
                }
                break;
            }
            case 6: {
                Proceso* ejecutado = cola.desencolar();
                if (ejecutado != NULL) {
                    cout << "Ejecucion completada" << endl;
                }
                break;
            }
            case 7:
                cola.mostrarCola();
                break;
            case 8: {
                int id, memoria;
                cout << "ID proceso: ";
                cin >> id;
                cout << "Memoria (KB): ";
                cin >> memoria;
                pila.asignarMemoria(id, memoria);
                break;
            }
            case 9:
                pila.liberarMemoria();
                break;
            case 10:
                pila.mostrarMemoria();
                break;
            case 0:
                cout << "Saliendo..." << endl;
                break;
            default:
                cout << "Opcion invalida" << endl;
        }
    } while (opcion != 0);

    return 0;
}
