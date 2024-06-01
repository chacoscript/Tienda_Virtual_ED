#include <iostream>
#include <cstdlib> 
#include <ctime> 

using namespace std;

struct nodo {
    int nro;
    string tipo;
    struct nodo* ant;
    struct nodo* sgte;
};

typedef struct nodo* TpLista; 


TpLista CrearNodo() {
    TpLista nuevo = new nodo;
    nuevo->nro = rand() % 201 + 100; 
    cout << "Ingrese el tipo del nodo: ";
    cin >> nuevo->tipo;
    nuevo->sgte = NULL;
    nuevo->ant = NULL;
    return nuevo;
}



void VerLista(TpLista lista) {
    int i = 0;
    TpLista t = lista, q = NULL;
    cout << "\n Lista de IDA" << endl << endl;
    do {
        cout << " " << i + 1 << ")  " << t->nro << " - " << t->tipo << endl;
        q = t;
        t = t->sgte;
        i++;
    } while (t != lista);

    cout << "************";
    i = 0;
    cout << "\n Lista de REGRESO" << endl << endl;
    t = q;

    do {
        cout << " " << i + 1 << ")  " << t->nro << " - " << t->tipo << endl;
        q = t;
        t = t->ant;
        i++;
    } while (t != lista->ant);
}

int ContarNodos(TpLista lista) {
    if (lista == NULL) {
        return 0;
    }

    int contador = 0;
    TpLista temp = lista;

    do {
        contador++;
        temp = temp->sgte;
    } while (temp != lista);

    return contador;
}

int main() {
	int contador=0;
    srand(time(NULL)); // Inicializar la semilla para la generación de números aleatorios
    TpLista lista = NULL;
    int opcion;

    do {
        cout << "==================================" << endl;
	    cout << "===      Tienda Virtual        ===" << endl;
	    cout << "==================================" << endl;
	    cout << "1. Agregar producto" << std::endl;
	    cout << "2. Ver carrito" << std::endl;
	    cout << "3. Salir" << std::endl;
	    cout << "==================================" << endl;
	    cout << "Seleccione una opción: ";
        cin >> opcion;

        switch (opcion) {
            case 1:
            	
                break;
            case 2:
                
                break;
            case 3: 
			   break;
            case 0:
                
                break;
            default:
                cout << "Opción no válida\n";
                break;
        }
    } while (opcion != 0);

    return 0;
}
