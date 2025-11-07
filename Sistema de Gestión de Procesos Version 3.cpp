#include <iostream>
#include <cstdlib>
#include <cstring>
#include <fstream>

using namespace std;

// ==================== ESTRUCTURA DEL PROCESO ====================
struct Proceso {
    int id;
    char nombre[50];
    int prioridad;
    int memoria;
    int tiempo_ejecucion;
    Proceso* siguiente;
    
    Proceso(int id_val, const char* nombre_val, int prioridad_val, int memoria_val, int tiempo_val) {
        id = id_val;
        strcpy(nombre, nombre_val);
        prioridad = prioridad_val;
        memoria = memoria_val;
        tiempo_ejecucion = tiempo_val;
        siguiente = NULL;
    }
};

// ==================== FUNCIONES DE VALIDACION ====================
bool esNumeroEntero(const char* entrada) {
    if (entrada[0] == '\0') return false;
    
    for (int i = 0; entrada[i] != '\0'; i++) {
        if (entrada[i] < '0' || entrada[i] > '9') {
            return false;
        }
    }
    return true;
}

int obtenerEnteroValido(const char* mensaje, int min, int max) {
    char entrada[100];
    int valor;
    
    while (true) {
        cout << mensaje;
        cin >> entrada;
        
        cin.clear();
        cin.ignore(1000, '\n');
        
        if (esNumeroEntero(entrada)) {
            valor = atoi(entrada);
            if (valor >= min && valor <= max) {
                return valor;
            }
        }
        
        cout << "Error: Debe ingresar un numero entre " << min << " y " << max << endl;
    }
}

void limpiarPantalla() {
    // Limpieza optimizada - solo 5 líneas en lugar de 50
    for (int i = 0; i < 1; i++) {
        cout << endl;
    }
}

void mostrarEncabezado() {
    cout << "========================================" << endl;
    cout << "    SISTEMA DE GESTION DE PROCESOS" << endl;
    cout << "========================================" << endl;
}

// ==================== LISTA ENLAZADA SIMPLE ====================
class ListaEnlazada {
private:
    Proceso* cabeza;
    int contadorId;
    
public:
    ListaEnlazada() {
        cabeza = NULL;
        contadorId = 1;
    }
    
    void insertarProceso(const char* nombre, int prioridad, int memoria, int tiempo) {
        Proceso* nuevo = new Proceso(contadorId, nombre, prioridad, memoria, tiempo);
        contadorId++;
        
        if (cabeza == NULL) {
            cabeza = nuevo;
        } else {
            Proceso* temp = cabeza;
            while (temp->siguiente != NULL) {
                temp = temp->siguiente;
            }
            temp->siguiente = nuevo;
        }
    }
    
    bool eliminarProceso(int id) {
        if (cabeza == NULL) {
            cout << "La lista esta vacia" << endl;
            return false;
        }
        
        if (cabeza->id == id) {
            Proceso* temp = cabeza;
            cabeza = cabeza->siguiente;
            cout << "Proceso '" << temp->nombre << "' eliminado" << endl;
            delete temp;
            return true;
        }
        
        Proceso* actual = cabeza;
        while (actual->siguiente != NULL && actual->siguiente->id != id) {
            actual = actual->siguiente;
        }
        
        if (actual->siguiente != NULL) {
            Proceso* temp = actual->siguiente;
            actual->siguiente = temp->siguiente;
            cout << "Proceso '" << temp->nombre << "' eliminado" << endl;
            delete temp;
            return true;
        }
        
        cout << "Proceso con ID " << id << " no encontrado" << endl;
        return false;
    }
    
    Proceso* buscarProceso(int id) {
        Proceso* temp = cabeza;
        while (temp != NULL) {
            if (temp->id == id) {
                return temp;
            }
            temp = temp->siguiente;
        }
        return NULL;
    }
    
    Proceso* buscarPorNombre(const char* nombre) {
        Proceso* temp = cabeza;
        while (temp != NULL) {
            if (strcmp(temp->nombre, nombre) == 0) {
                return temp;
            }
            temp = temp->siguiente;
        }
        return NULL;
    }
    
    bool modificarPrioridad(int id, int nueva_prioridad) {
        Proceso* proceso = buscarProceso(id);
        if (proceso != NULL) {
            proceso->prioridad = nueva_prioridad;
            cout << "Prioridad del proceso " << id << " modificada a: " << nueva_prioridad << endl;
            return true;
        }
        cout << "Proceso no encontrado" << endl;
        return false;
    }
    
    void mostrarProcesos() {
        if (cabeza == NULL) {
            cout << "No hay procesos en el sistema" << endl;
            return;
        }
        
        cout << "\n=== LISTA DE PROCESOS ===" << endl;
        cout << "ID\tNombre\t\tPrioridad\tMemoria\tTiempo" << endl;
        cout << "----------------------------------------" << endl;
        
        Proceso* temp = cabeza;
        while (temp != NULL) {
            cout << temp->id << "\t" << temp->nombre << "\t\t" 
                 << temp->prioridad << "\t\t" << temp->memoria << "MB\t" 
                 << temp->tiempo_ejecucion << "s" << endl;
            temp = temp->siguiente;
        }
    }
    
    Proceso* getCabeza() {
        return cabeza;
    }
};

// ==================== COLA DE PRIORIDAD ====================
class ColaPrioridad {
private:
    Proceso* frente;
    
public:
    ColaPrioridad() {
        frente = NULL;
    }
    
    void encolar(Proceso* proceso) {
        Proceso* nuevo = new Proceso(proceso->id, proceso->nombre, proceso->prioridad, 
                                   proceso->memoria, proceso->tiempo_ejecucion);
        
        if (frente == NULL || nuevo->prioridad < frente->prioridad) {
            nuevo->siguiente = frente;
            frente = nuevo;
        } else {
            Proceso* actual = frente;
            while (actual->siguiente != NULL && 
                   actual->siguiente->prioridad <= nuevo->prioridad) {
                actual = actual->siguiente;
            }
            nuevo->siguiente = actual->siguiente;
            actual->siguiente = nuevo;
        }
    }
    
    Proceso* desencolar() {
        if (frente == NULL) {
            return NULL;
        }
        
        Proceso* temp = frente;
        frente = frente->siguiente;
        temp->siguiente = NULL;
        return temp;
    }
    
    void mostrarCola() {
        if (frente == NULL) {
            cout << "La cola de CPU esta vacia" << endl;
            return;
        }
        
        cout << "\n=== COLA DE CPU ===" << endl;
        cout << "ID\tNombre\t\tPrioridad\tMemoria\tTiempo" << endl;
        cout << "----------------------------------------" << endl;
        
        Proceso* temp = frente;
        while (temp != NULL) {
            cout << temp->id << "\t" << temp->nombre << "\t\t" 
                 << temp->prioridad << "\t\t" << temp->memoria << "MB\t" 
                 << temp->tiempo_ejecucion << "s" << endl;
            temp = temp->siguiente;
        }
    }
    
    bool estaVacia() {
        return frente == NULL;
    }
};

// ==================== PILA DE MEMORIA ====================
class PilaMemoria {
private:
    struct BloqueMemoria {
        int id_proceso;
        int tamano;
        BloqueMemoria* siguiente;
        
        BloqueMemoria(int id, int tam) {
            id_proceso = id;
            tamano = tam;
            siguiente = NULL;
        }
    };
    
    BloqueMemoria* tope;
    int memoria_total;
    int memoria_usada;
    
public:
    PilaMemoria(int total = 1024) {
        tope = NULL;
        memoria_total = total;
        memoria_usada = 0;
    }
    
    bool asignarMemoria(int id_proceso, int tamano) {
        if (memoria_usada + tamano > memoria_total) {
            cout << "Error: Memoria insuficiente. Necesita: " << tamano 
                 << "MB, Disponible: " << (memoria_total - memoria_usada) << "MB" << endl;
            return false;
        }
        
        BloqueMemoria* nuevo = new BloqueMemoria(id_proceso, tamano);
        nuevo->siguiente = tope;
        tope = nuevo;
        memoria_usada += tamano;
        
        cout << "Memoria asignada: " << tamano << "MB al proceso ID: " << id_proceso << endl;
        return true;
    }
    
    bool liberarMemoria() {
        if (tope == NULL) {
            cout << "No hay memoria asignada para liberar" << endl;
            return false;
        }
        
        BloqueMemoria* temp = tope;
        tope = tope->siguiente;
        
        cout << "Memoria liberada: " << temp->tamano << "MB del proceso ID: " << temp->id_proceso << endl;
        memoria_usada -= temp->tamano;
        delete temp;
        return true;
    }
    
    bool liberarMemoriaProceso(int id_proceso) {
        if (tope == NULL) {
            return false;
        }
        
        if (tope->id_proceso == id_proceso) {
            BloqueMemoria* temp = tope;
            tope = tope->siguiente;
            memoria_usada -= temp->tamano;
            delete temp;
            return true;
        }
        
        BloqueMemoria* actual = tope;
        while (actual->siguiente != NULL && actual->siguiente->id_proceso != id_proceso) {
            actual = actual->siguiente;
        }
        
        if (actual->siguiente != NULL) {
            BloqueMemoria* temp = actual->siguiente;
            actual->siguiente = temp->siguiente;
            memoria_usada -= temp->tamano;
            delete temp;
            return true;
        }
        
        return false;
    }
    
    void mostrarEstado() {
        cout << "\n=== ESTADO DE MEMORIA ===" << endl;
        cout << "Memoria total: " << memoria_total << "MB" << endl;
        cout << "Memoria usada: " << memoria_usada << "MB" << endl;
        cout << "Memoria disponible: " << (memoria_total - memoria_usada) << "MB" << endl;
        
        if (tope == NULL) {
            cout << "No hay bloques de memoria asignados" << endl;
            return;
        }
        
        cout << "\nBloques de memoria asignados:" << endl;
        cout << "ID Proceso\tTamano (MB)" << endl;
        cout << "-----------------------" << endl;
        
        BloqueMemoria* temp = tope;
        while (temp != NULL) {
            cout << temp->id_proceso << "\t\t" << temp->tamano << endl;
            temp = temp->siguiente;
        }
    }
};

// ==================== SISTEMA PRINCIPAL ====================
class SistemaGestionProcesos {
private:
    ListaEnlazada gestorProcesos;
    ColaPrioridad planificadorCPU;
    PilaMemoria gestorMemoria;
    
public:
    void cargarProcesosEjemplo() {
        // Carga silenciosa sin mensajes
        gestorProcesos.insertarProceso("System", 1, 100, 0);
        gestorProcesos.insertarProceso("Explorer", 2, 50, 30);
        gestorProcesos.insertarProceso("Browser", 3, 200, 45);
        gestorProcesos.insertarProceso("Editor", 2, 80, 25);
        gestorProcesos.insertarProceso("MediaPlayer", 4, 120, 35);
    }
    
    void mostrarMenuPrincipal() {
        mostrarEncabezado();
        cout << "1. Gestion de Procesos" << endl;
        cout << "2. Planificador de CPU" << endl;
        cout << "3. Gestion de Memoria" << endl;
        cout << "4. Ejecutar Simulacion Completa" << endl;
        cout << "5. Guardar Estado" << endl;
        cout << "6. Cargar Estado" << endl;
        cout << "7. Salir" << endl;
        cout << "========================================" << endl;
    }
    
    void menuGestionProcesos() {
        int opcion;
        do {
            limpiarPantalla();
            mostrarEncabezado();
            cout << "*** GESTION DE PROCESOS ***" << endl;
            cout << "1. Insertar proceso" << endl;
            cout << "2. Eliminar proceso" << endl;
            cout << "3. Buscar por ID" << endl;
            cout << "4. Buscar por nombre" << endl;
            cout << "5. Modificar prioridad" << endl;
            cout << "6. Mostrar procesos" << endl;
            cout << "7. Volver al menu principal" << endl;
            
            opcion = obtenerEnteroValido("\nSeleccione opcion (1-7): ", 1, 7);
            
            switch (opcion) {
                case 1: {
                    char nombre[50];
                    int prioridad, memoria, tiempo;
                    
                    cout << "\nNombre del proceso: ";
                    cin >> nombre;
                    cin.ignore(1000, '\n');
                    
                    prioridad = obtenerEnteroValido("Prioridad (1-5, 1=mas alta): ", 1, 5);
                    memoria = obtenerEnteroValido("Memoria requerida (MB): ", 1, 1000);
                    tiempo = obtenerEnteroValido("Tiempo de ejecucion (segundos): ", 1, 300);
                    
                    gestorProcesos.insertarProceso(nombre, prioridad, memoria, tiempo);
                    cout << "Proceso creado exitosamente!" << endl;
                    break;
                }
                case 2: {
                    gestorProcesos.mostrarProcesos();
                    if (gestorProcesos.getCabeza() != NULL) {
                        int id = obtenerEnteroValido("\nID del proceso a eliminar: ", 1, 100);
                        gestorProcesos.eliminarProceso(id);
                    }
                    break;
                }
                case 3: {
                    int id = obtenerEnteroValido("ID del proceso a buscar: ", 1, 100);
                    Proceso* p = gestorProcesos.buscarProceso(id);
                    if (p != NULL) {
                        cout << "\nProceso encontrado:" << endl;
                        cout << "Nombre: " << p->nombre << endl;
                        cout << "Prioridad: " << p->prioridad << endl;
                        cout << "Memoria: " << p->memoria << "MB" << endl;
                        cout << "Tiempo: " << p->tiempo_ejecucion << "s" << endl;
                    } else {
                        cout << "Proceso no encontrado" << endl;
                    }
                    break;
                }
                case 4: {
                    char nombre[50];
                    cout << "Nombre del proceso a buscar: ";
                    cin >> nombre;
                    cin.ignore(1000, '\n');
                    
                    Proceso* p = gestorProcesos.buscarPorNombre(nombre);
                    if (p != NULL) {
                        cout << "\nProceso encontrado:" << endl;
                        cout << "ID: " << p->id << endl;
                        cout << "Prioridad: " << p->prioridad << endl;
                        cout << "Memoria: " << p->memoria << "MB" << endl;
                        cout << "Tiempo: " << p->tiempo_ejecucion << "s" << endl;
                    } else {
                        cout << "Proceso no encontrado" << endl;
                    }
                    break;
                }
                case 5: {
                    gestorProcesos.mostrarProcesos();
                    if (gestorProcesos.getCabeza() != NULL) {
                        int id = obtenerEnteroValido("\nID del proceso: ", 1, 100);
                        int nueva_prioridad = obtenerEnteroValido("Nueva prioridad (1-5): ", 1, 5);
                        gestorProcesos.modificarPrioridad(id, nueva_prioridad);
                    }
                    break;
                }
                case 6:
                    gestorProcesos.mostrarProcesos();
                    break;
                case 7:
                    cout << "Volviendo al menu principal..." << endl;
                    break;
            }
            
            if (opcion != 7) {
                cout << "\nPresione Enter para continuar...";
                cin.get();
            }
            
        } while (opcion != 7);
    }
    
    void menuPlanificadorCPU() {
        int opcion;
        do {
            limpiarPantalla();
            mostrarEncabezado();
            cout << "*** PLANIFICADOR DE CPU ***" << endl;
            cout << "1. Cargar procesos a la cola de CPU" << endl;
            cout << "2. Mostrar cola de CPU actual" << endl;
            cout << "3. Ejecutar siguiente proceso" << endl;
            cout << "4. Ejecutar todos los procesos" << endl;
            cout << "5. Volver al menu principal" << endl;
            
            opcion = obtenerEnteroValido("\nSeleccione opcion (1-5): ", 1, 5);
            
            switch (opcion) {
                case 1:
                    cargarProcesosACola();
                    break;
                case 2:
                    planificadorCPU.mostrarCola();
                    break;
                case 3:
                    ejecutarSiguienteProceso();
                    break;
                case 4:
                    ejecutarTodosProcesos();
                    break;
                case 5:
                    cout << "Volviendo al menu principal..." << endl;
                    break;
            }
            
            if (opcion != 5) {
                cout << "\nPresione Enter para continuar...";
                cin.get();
            }
            
        } while (opcion != 5);
    }
    
    void cargarProcesosACola() {
        Proceso* actual = gestorProcesos.getCabeza();
        int contador = 0;
        
        while (actual != NULL) {
            planificadorCPU.encolar(actual);
            actual = actual->siguiente;
            contador++;
        }
        
        cout << contador << " procesos cargados en la cola de CPU" << endl;
    }
    
    void ejecutarSiguienteProceso() {
        if (planificadorCPU.estaVacia()) {
            cout << "No hay procesos en la cola de CPU" << endl;
            return;
        }
        
        Proceso* p = planificadorCPU.desencolar();
        cout << "\n*** EJECUTANDO PROCESO ***" << endl;
        cout << "Proceso: " << p->nombre << " (ID: " << p->id << ")" << endl;
        cout << "Prioridad: " << p->prioridad << endl;
        cout << "Memoria: " << p->memoria << "MB" << endl;
        cout << "Tiempo: " << p->tiempo_ejecucion << "s" << endl;
        
        cout << "Ejecutando...";
        for (int i = 0; i < 1000000; i++);
        cout << " COMPLETADO!" << endl;
        
        gestorMemoria.liberarMemoriaProceso(p->id);
        delete p;
    }
    
    void ejecutarTodosProcesos() {
        if (planificadorCPU.estaVacia()) {
            cout << "No hay procesos en la cola de CPU" << endl;
            return;
        }
        
        int contador = 0;
        while (!planificadorCPU.estaVacia()) {
            ejecutarSiguienteProceso();
            contador++;
            cout << "----------------------------------------" << endl;
        }
        cout << "\nSe ejecutaron " << contador << " procesos" << endl;
    }
    
    void menuGestionMemoria() {
        int opcion;
        do {
            limpiarPantalla();
            mostrarEncabezado();
            cout << "*** GESTION DE MEMORIA ***" << endl;
            cout << "1. Asignar memoria a proceso" << endl;
            cout << "2. Liberar ultimo bloque de memoria" << endl;
            cout << "3. Liberar memoria de proceso especifico" << endl;
            cout << "4. Mostrar estado de memoria" << endl;
            cout << "5. Volver al menu principal" << endl;
            
            opcion = obtenerEnteroValido("\nSeleccione opcion (1-5): ", 1, 5);
            
            switch (opcion) {
                case 1: {
                    gestorProcesos.mostrarProcesos();
                    if (gestorProcesos.getCabeza() != NULL) {
                        int id = obtenerEnteroValido("\nID del proceso: ", 1, 100);
                        int tamano = obtenerEnteroValido("Tamano de memoria a asignar (MB): ", 1, 500);
                        
                        Proceso* p = gestorProcesos.buscarProceso(id);
                        if (p != NULL) {
                            gestorMemoria.asignarMemoria(id, tamano);
                        } else {
                            cout << "Proceso no encontrado" << endl;
                        }
                    }
                    break;
                }
                case 2:
                    gestorMemoria.liberarMemoria();
                    break;
                case 3: {
                    int id = obtenerEnteroValido("ID del proceso: ", 1, 100);
                    if (gestorMemoria.liberarMemoriaProceso(id)) {
                        cout << "Memoria liberada correctamente" << endl;
                    } else {
                        cout << "No se encontro memoria asignada a ese proceso" << endl;
                    }
                    break;
                }
                case 4:
                    gestorMemoria.mostrarEstado();
                    break;
                case 5:
                    cout << "Volviendo al menu principal..." << endl;
                    break;
            }
            
            if (opcion != 5) {
                cout << "\nPresione Enter para continuar...";
                cin.get();
            }
            
        } while (opcion != 5);
    }
    
    void ejecutarSimulacionCompleta() {
        limpiarPantalla();
        mostrarEncabezado();
        cout << "*** SIMULACION COMPLETA ***" << endl;
        
        cout << "\n1. ESTADO INICIAL DE PROCESOS:" << endl;
        gestorProcesos.mostrarProcesos();
        
        cout << "\n2. ESTADO INICIAL DE MEMORIA:" << endl;
        gestorMemoria.mostrarEstado();
        
        cout << "\n3. CARGANDO PROCESOS A COLA DE CPU..." << endl;
        cargarProcesosACola();
        planificadorCPU.mostrarCola();
        
        cout << "\n4. EJECUTANDO PROCESOS..." << endl;
        ejecutarTodosProcesos();
        
        cout << "\n5. ESTADO FINAL:" << endl;
        gestorMemoria.mostrarEstado();
        
        cout << "\nPresione Enter para continuar...";
        cin.get();
    }
    
    void guardarEstado() {
        ofstream archivo("estado_sistema.txt");
        
        if (!archivo) {
            cout << "Error al crear archivo de estado" << endl;
            return;
        }
        
        Proceso* actual = gestorProcesos.getCabeza();
        while (actual != NULL) {
            archivo << actual->id << "," << actual->nombre << "," 
                   << actual->prioridad << "," << actual->memoria << "," 
                   << actual->tiempo_ejecucion << endl;
            actual = actual->siguiente;
        }
        
        archivo.close();
        cout << "Estado guardado en 'estado_sistema.txt'" << endl;
    }
    
    void cargarEstado() {
        ifstream archivo("estado_sistema.txt");
        
        if (!archivo) {
            cout << "No se encontro archivo de estado" << endl;
            return;
        }
        
        char linea[256];
        while (archivo.getline(linea, 256)) {
            char* token = strtok(linea, ",");
            if (token != NULL) {
                token = strtok(NULL, ",");
                char nombre[50];
                if (token != NULL) {
                    strcpy(nombre, token);
                    
                    token = strtok(NULL, ",");
                    int prioridad = atoi(token);
                    
                    token = strtok(NULL, ",");
                    int memoria = atoi(token);
                    
                    token = strtok(NULL, ",");
                    int tiempo = atoi(token);
                    
                    gestorProcesos.insertarProceso(nombre, prioridad, memoria, tiempo);
                }
            }
        }
        
        archivo.close();
        cout << "Estado cargado desde 'estado_sistema.txt'" << endl;
    }
    
    void ejecutar() {
        // Carga silenciosa al inicio
        cargarProcesosEjemplo();
        
        int opcion;
        do {
            // Solo limpiar pantalla minimamente
            limpiarPantalla();
            mostrarMenuPrincipal();
            
            opcion = obtenerEnteroValido("Seleccione una opcion (1-7): ", 1, 7);
            
            switch (opcion) {
                case 1:
                    menuGestionProcesos();
                    break;
                case 2:
                    menuPlanificadorCPU();
                    break;
                case 3:
                    menuGestionMemoria();
                    break;
                case 4:
                    ejecutarSimulacionCompleta();
                    break;
                case 5:
                    guardarEstado();
                    cout << "\nPresione Enter para continuar...";
                    cin.get();
                    break;
                case 6:
                    cargarEstado();
                    cout << "\nPresione Enter para continuar...";
                    cin.get();
                    break;
                case 7:
                    cout << "Saliendo del sistema. ¡Hasta luego!" << endl;
                    break;
            }
            
        } while (opcion != 7);
    }
};

// ==================== FUNCION PRINCIPAL OPTIMIZADA ====================
int main() {
    // Inicio inmediato sin espacios en blanco
    SistemaGestionProcesos sistema;
    sistema.ejecutar();
    return 0;
}
