#include <iostream>
#include <string>
#include <cctype>
#include <stdexcept>
#include <limits>
#include <cstdlib>
#include <cmath>
#include <cstdio>
#include <sstream>

using namespace std;

/* ==========================
   Parser de expresiones (C++98)
   Soporta + - * / y paréntesis.
   ========================== */
class ExprParser {
    const string s;
    size_t p;
    void skip(){ while(p<s.size() && isspace((unsigned char)s[p])) p++; }

    double parseNumber(){
        skip();
        if(p>=s.size()) throw runtime_error("Se esperaba un numero.");
        size_t start=p;
        if(s[p]=='+'||s[p]=='-') p++;
        bool dig=false, dot=false;
        while(p<s.size()){
            unsigned char c=(unsigned char)s[p];
            if(isdigit(c)){ dig=true; p++; }
            else if(s[p]=='.' && !dot){ dot=true; p++; }
            else break;
        }
        if(!dig) throw runtime_error("Se esperaba un numero.");
        const char* base=s.c_str();
        char* endptr=NULL;
        double v=strtod(base+start,&endptr);
        if(endptr!=base+p) throw runtime_error("Numero invalido.");
        return v;
    }
    double factor(){
        skip();
        if(p>=s.size()) throw runtime_error("Factor incompleto.");
        if(s[p]=='('){
            p++; double v=expr(); skip();
            if(p>=s.size()||s[p]!=')') throw runtime_error("Falta ')'.");
            p++; return v;
        }
        return parseNumber();
    }
    double term(){
        double v=factor();
        for(;;){
            skip();
            if(p<s.size() && (s[p]=='*'||s[p]=='/')){
                char op=s[p++]; double r=factor();
                if(op=='*') v*=r;
                else{
                    if (fabs(r) < 1e-12) throw runtime_error("Division entre cero.");
                    v/=r;
                }
            }else break;
        }
        return v;
    }
    double expr(){
        double v=term();
        for(;;){
            skip();
            if(p<s.size() && (s[p]=='+'||s[p]=='-')){
                char op=s[p++]; double r=term();
                if(op=='+') v+=r; else v-=r;
            }else break;
        }
        return v;
    }
public:
    ExprParser(const string& in):s(in),p(0){}
    double parse(){ double v=expr(); skip(); if(p!=s.size()) throw runtime_error("Entrada invalida."); return v; }
};

/* ======= utilidades de entrada ======= */
void clearStdin(){ cin.clear(); cin.ignore(numeric_limits<streamsize>::max(), '\n'); }

double leerNumeroExpr(const string& prompt){
    for(;;){
        cout<<prompt;
        string line; getline(cin,line);
        if(line.empty()) { cout<<"Escribe un valor.\n"; continue; }
        try{ ExprParser p(line); return p.parse(); }
        catch(exception& e){ cout<<"Error: "<<e.what()<<". Intenta de nuevo.\n"; }
    }
}

int decidirEnteroDesdeDouble(double val, const string& campo){
    long long tr=(long long)val;
    if(val==(double)tr) return (int)tr;
    for(;;){
        cout<<"Ingresaste decimal para '"<<campo<<"': "<<val<<"\n";
        cout<<"1) Redondear ("<<(long long)(val + (val>=0?0.5:-0.5))<<")\n";
        cout<<"2) Truncar ("<<tr<<")\n";
        cout<<"3) Reingresar\n";
        cout<<"Opcion: ";
        string op; getline(cin,op);
        if(op=="1") return (int)(val + (val>=0?0.5:-0.5));
        if(op=="2") return (int)tr;
        if(op=="3"){ double v=leerNumeroExpr("Nuevo valor: "); return decidirEnteroDesdeDouble(v,campo); }
        cout<<"Opcion invalida.\n";
    }
}

int leerEnteroExprLibre(const string& prompt, const string& campo){
    return decidirEnteroDesdeDouble(leerNumeroExpr(prompt), campo);
}
int leerEnteroRango(const string& prompt, int minV, int maxV, const string& campo){
    for(;;){
        int v=leerEnteroExprLibre(prompt,campo);
        if(v<minV||v>maxV){ cout<<"Debe estar entre "<<minV<<" y "<<maxV<<".\n"; continue; }
        return v;
    }
}

/* ======= Estructuras ======= */
struct Proceso {
    int id;
    string nombre;
    int prioridad;
    int memoria;
    Proceso* siguiente;

    // Campos de rúbrica (se guardan para CSV, no se muestran en consola)
    string variables;    // 6.1
    string condiciones;  // 6.1
    string datos;        // 6.2
    string pide;         // 6.2
    string evidencia;    // 6.3
    int r61, r62, r63;   // 0..3, -1 si no evaluado

    Proceso(): id(0), prioridad(0), memoria(0), siguiente(NULL),
               r61(-1), r62(-1), r63(-1) {}
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

/* ======= Lista de procesos ======= */
class ListaProcesos {
private:
    Proceso* cabeza;
    int contadorId;

public:
    ListaProcesos(): cabeza(NULL), contadorId(1) {}

    int insertarProceso(const string& nombre, int prioridad, int memoria,
                        const string& variables,
                        const string& condiciones,
                        const string& datos,
                        const string& pide)
    {
        Proceso* nuevo = new Proceso;
        nuevo->id = contadorId++;
        nuevo->nombre = nombre;
        nuevo->prioridad = prioridad;
        nuevo->memoria = memoria;
        nuevo->variables = variables;
        nuevo->condiciones = condiciones;
        nuevo->datos = datos;
        nuevo->pide = pide;
        nuevo->siguiente = NULL;

        if (!cabeza) cabeza = nuevo;
        else {
            Proceso* a = cabeza;
            while (a->siguiente) a = a->siguiente;
            a->siguiente = nuevo;
        }
        cout << "Proceso agregado. ID: " << nuevo->id << "\n";
        return nuevo->id;
    }

    Proceso* buscarPorId(int id) {
        Proceso* a = cabeza;
        while (a) {
            if (a->id == id) return a;
            a = a->siguiente;
        }
        return NULL;
    }

    void mostrarProcesos() {
        if (!cabeza) { cout << "No hay procesos.\n"; return; }
        Proceso* a = cabeza;
        cout << "\n--- LISTA DE PROCESOS ---\n";
        while (a) {
            cout << "ID: " << a->id
                 << " | Nombre: " << a->nombre
                 << " | Prioridad: " << a->prioridad
                 << " | Memoria: " << a->memoria << " KB\n";
            a = a->siguiente;
        }
    }

    void eliminarProceso(int id) {
        if (!cabeza) return;
        if (cabeza->id == id) {
            Proceso* t = cabeza;
            cabeza = cabeza->siguiente;
            delete t;
            cout << "Proceso eliminado.\n";
            return;
        }
        Proceso* a = cabeza;
        while (a->siguiente && a->siguiente->id != id) a = a->siguiente;
        if (a->siguiente) {
            Proceso* t = a->siguiente;
            a->siguiente = t->siguiente;
            delete t;
            cout << "Proceso eliminado.\n";
        } else cout << "ID no encontrado.\n";
    }

    void exportarCSV(const char* filename){
        FILE* f = fopen(filename,"wb");
        if(!f){ cout<<"No se pudo crear "<<filename<<"\n"; return; }
        fprintf(f,"ID,Nombre,Prioridad,MemoriaKB,Variables,Condiciones,Datos,Pide,R6.1,R6.2,R6.3,Evidencia\n");
        Proceso* a = cabeza;
        while(a){
            string v=a->variables, c=a->condiciones, d=a->datos, p=a->pide, ev=a->evidencia, n=a->nombre;
            size_t k;
            for(k=0;k<v.size();k++) if(v[k]==',') v[k]=';';
            for(k=0;k<c.size();k++) if(c[k]==',') c[k]=';';
            for(k=0;k<d.size();k++) if(d[k]==',') d[k]=';';
            for(k=0;k<p.size();k++) if(p[k]==',') p[k]=';';
            for(k=0;k<ev.size();k++) if(ev[k]==',') ev[k]=';';
            for(k=0;k<n.size();k++) if(n[k]==',') n[k]=';';
            fprintf(f,"%d,%s,%d,%d,%s,%s,%s,%s,%d,%d,%d,%s\n",
                    a->id, n.c_str(), a->prioridad, a->memoria,
                    v.c_str(), c.c_str(), d.c_str(), p.c_str(),
                    (a->r61<0? -1:a->r61), (a->r62<0? -1:a->r62), (a->r63<0? -1:a->r63),
                    ev.c_str());
            a=a->siguiente;
        }
        fclose(f);
        cout<<"Exportado a "<<filename<<"\n";
    }
};

/* ======= Cola de prioridad ======= */
class ColaPrioridad {
    NodoCola* frente;
public:
    ColaPrioridad() : frente(NULL) {}

    void encolar(Proceso* p) {
        NodoCola* nuevo = new NodoCola;
        nuevo->proceso = p;
        nuevo->siguiente = NULL;

        if (!frente || frente->proceso->prioridad < p->prioridad) {
            nuevo->siguiente = frente;
            frente = nuevo;
        } else {
            NodoCola* a = frente;
            while (a->siguiente && a->siguiente->proceso->prioridad >= p->prioridad)
                a = a->siguiente;
            nuevo->siguiente = a->siguiente;
            a->siguiente = nuevo;
        }
        // Evidencia interna (6.3). Se guarda para CSV, no se imprime:
        p->evidencia += "encolar: metodo=cola_prioridad(descendente); justificacion=atender mayor prioridad primero\n";
        cout << "Proceso encolado.\n";
    }

    Proceso* desencolar() {
        if (!frente) {
            cout << "Cola vacia.\n";
            return NULL;
        }
        NodoCola* t = frente;
        Proceso* p = t->proceso;
        frente = frente->siguiente;
        delete t;

        // Evidencia interna (6.3)
        p->evidencia += "ejecutar: metodo=cola_prioridad; justificacion=orden por prioridad; resultado=ok\n";

        cout << "Ejecutando: " << p->nombre << "\n";
        return p;
    }

    void mostrar() {
        if (!frente) { cout << "Cola vacia.\n"; return; }
        NodoCola* a = frente;
        cout << "\n--- COLA ---\n";
        while (a) {
            cout << "Proceso: " << a->proceso->nombre
                 << " | Prioridad: " << a->proceso->prioridad
                 << " | Memoria: " << a->proceso->memoria << " KB\n";
            a = a->siguiente;
        }
    }
};

/* ======= Pila de memoria ======= */
class PilaMemoria {
    BloqueMemoria* tope;
    int total, usado;
public:
    PilaMemoria() : tope(NULL), total(1024), usado(0) {}

    void asignar(int id, int size) {
        if (size<=0){ cout<<"Tamano invalido.\n"; return; }
        if (usado + size > total) {
            cout << "Memoria insuficiente.\n";
            return;
        }
        BloqueMemoria* nuevo = new BloqueMemoria;
        nuevo->id_proceso = id;
        nuevo->size = size;
        nuevo->siguiente = tope;
        tope = nuevo;
        usado += size;
        cout << "Memoria asignada.\n";
    }

    void liberar() {
        if (!tope) { cout << "Nada que liberar.\n"; return; }
        BloqueMemoria* t = tope;
        usado -= t->size;
        tope = t->siguiente;
        delete t;
        cout << "Memoria liberada.\n";
    }

    void mostrar() {
        cout << "\n--- MEMORIA ---\n";
        cout << "Total: 1024 KB\n";
        cout << "Usado: " << usado << " KB\n";
        cout << "Libre: " << total - usado << " KB\n";
        BloqueMemoria* a = tope;
        while (a) {
            cout << "Proceso: " << a->id_proceso << " | Bloque: " << a->size << " KB\n";
            a = a->siguiente;
        }
    }
};

/* ======= Menú ======= */
void menu(){
    cout << "\n=== SISTEMA ===\n";
    cout << "1. Agregar proceso\n";
    cout << "2. Mostrar procesos\n";
    cout << "3. Eliminar proceso\n";
    cout << "4. Buscar proceso\n";
    cout << "5. Encolar proceso\n";
    cout << "6. Ejecutar proceso\n";
    cout << "7. Mostrar cola\n";
    cout << "8. Asignar memoria\n";
    cout << "9. Liberar memoria\n";
    cout << "10. Mostrar memoria\n";
    cout << "11. Exportar CSV\n";
    cout << "0. Salir\n";
}

int main(){
    ListaProcesos lista;
    ColaPrioridad cola;
    PilaMemoria pila;

    for(;;){
        cout << flush; // asegura que el menú se imprima de inmediato
        menu();
        int op = leerEnteroRango("Opcion: ",0,11,"Opcion");
        if(op==0) break;

        if (op == 1) {
            string nombre, variables, condiciones, datos, pide;
            cout << "Nombre: "; getline(cin, nombre);
            int pr = leerEnteroRango("Prioridad (1-10): ", 1, 10, "Prioridad");
            int mem = leerEnteroExprLibre("Memoria (KB): ", "Memoria");
            cout << "Variables del problema (ej. prio[1..10], mem>0): ";
            getline(cin, variables);
            cout << "Condiciones/Hipotesis (ej. mem_total=1024KB): ";
            getline(cin, condiciones);
            cout << "Datos dados (breve): ";
            getline(cin, datos);
            cout << "Que se pide (breve): ";
            getline(cin, pide);

            int idNuevo = lista.insertarProceso(nombre, pr, mem, variables, condiciones, datos, pide);

            Proceso* p = lista.buscarPorId(idNuevo);
            if(p){
                p->evidencia += "creacion: metodo=lista_enlazada; justificacion=insertar al final; resultado=ok\n";
            }
        }
        else if (op == 2) {
            lista.mostrarProcesos();
        }
        else if (op == 3) {
            lista.eliminarProceso(leerEnteroExprLibre("ID: ", "ID"));
        }
        else if (op == 4) {
            int id = leerEnteroExprLibre("ID: ", "ID");
            Proceso* e = lista.buscarPorId(id);
            if (e) {
                cout << "Encontrado: " << e->nombre
                     << " | Prioridad: " << e->prioridad
                     << " | Memoria: " << e->memoria << " KB\n";
            } else {
                cout << "No encontrado.\n";
            }
        }
        else if (op == 5) {
            int id = leerEnteroExprLibre("ID: ", "ID");
            Proceso* p = lista.buscarPorId(id);
            if (p) cola.encolar(p);
            else cout << "No existe.\n";
        }
        else if (op == 6) {
            cola.desencolar();
        }
        else if (op == 7) {
            cola.mostrar();
        }
        else if (op == 8) {
            int id = leerEnteroExprLibre("ID: ", "ID");
            int tam = leerEnteroExprLibre("Tamano (KB): ", "Memoria");
            pila.asignar(id, tam);

            Proceso* p = lista.buscarPorId(id);
            if (p){
                ostringstream oss;
                oss << "memoria: metodo=pila(LIFO); justificacion=liberar en orden inverso; size=" << tam << "KB\n";
                p->evidencia += oss.str(); // solo se guarda para CSV
            }
        }
        else if (op == 9) {
            pila.liberar();
        }
        else if (op == 10) {
            pila.mostrar();
        }
        else if (op == 11) {
            lista.exportarCSV("reporte_procesos.csv");
        }
    }

    cout<<"Hasta pronto.\n";
    return 0;
}

