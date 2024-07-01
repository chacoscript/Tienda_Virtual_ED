#include <iostream>
#include <cstdlib>
#include <ctime>
#include <windows.h>
#include <string>
#include <conio.h>
#include <fstream>
#include <iomanip>    // Para usar setw
using namespace std;

//Variables constantes
#define ARRIBA 72
#define ABAJO 80
#define ENTER 13

// Nodo para guardar cuentas personales
struct Usuario {
    string nombreUsuario;
    string contrasena;
    Usuario* sgte;
};

typedef Usuario* usuario;

// Estructura para obtener la fecha y hora actual
struct Fecha {
    int anio;
    int mes;
    int dia;
    int hora;
    int minutos;
    int segundos;
};

//NO MOD - Funcion para los colores en las opciones
void color(int color){
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

// Funci�n para obtener la fecha y hora actual
Fecha obtenerFechaHoraActual() {
    // Obteniendo la fecha y hora actual
    time_t tiempoActual = time(NULL);
    tm* fechaHora = localtime(&tiempoActual);

    // Creando una instancia de la estructura Fecha
    Fecha fechaActual;
    fechaActual.anio = fechaHora->tm_year + 1900; // tm_year devuelve a�os desde 1900
    fechaActual.mes = fechaHora->tm_mon + 1;      // tm_mon devuelve meses desde 0
    fechaActual.dia = fechaHora->tm_mday;
    fechaActual.hora = fechaHora->tm_hour;
    fechaActual.minutos = fechaHora->tm_min;
    fechaActual.segundos = fechaHora->tm_sec;

    return fechaActual;
}

//NO MOD - Funcion para la posicion de la impresion
void gotoxy(int x, int y){
    HANDLE hCon;
    hCon = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD dwPos;
    dwPos.X = x;
    dwPos.Y = y;
    SetConsoleCursorPosition(hCon, dwPos);
}

// Funci�n para obtener el ancho de la consola
int getConsoleWidth() {
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    int columns;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
    columns = csbi.srWindow.Right - csbi.srWindow.Left + 1;
    return columns;
}

// Funci�n para centrar texto en la consola
void centerText(string text) {
    int consoleWidth = getConsoleWidth();
    int padding = (consoleWidth - text.length()) / 2;
    cout << string(padding, ' ') << text << endl;
}

// Estructura para Art�culo del Carrito de Compras
struct ArticuloCarrito {
    string nombreArticulo;
    int idArticulo;
    int cantidad;
    double precio;
    double montoTotal;
    ArticuloCarrito* siguiente;
};

typedef ArticuloCarrito* carrito;

// Estructura para Art�culo del Inventario
struct ArticuloInventario {
    string nombreArticulo;
    int idArticulo;
    int cantidad;
    double precio;
    ArticuloInventario* siguiente;
    ArticuloInventario* anterior;
};

typedef ArticuloInventario* inventario;

// Funciones para gestionar el inventario
inventario crearArticulo(string nombreArticulo, int idArticulo, int cantidad, double precio) {
    inventario nuevoArticulo = new ArticuloInventario;
    nuevoArticulo->nombreArticulo = nombreArticulo;
    nuevoArticulo->idArticulo = idArticulo;
    nuevoArticulo->cantidad = cantidad;
    nuevoArticulo->precio = precio;
    nuevoArticulo->siguiente = NULL;
    nuevoArticulo->anterior = NULL;
    return nuevoArticulo;
}

//NO MOD - Función para los menús en común
int menu(const char* titulo, string opciones[], int n) {
    int opcion = 1;
    int tecla;
    bool flag = true;

    do {
        system("cls");
        color(10);
        gotoxy(42, 12);
        for (int i = 0; i < strlen(titulo) + 2; i++) {
            cout << "*";
        }

        gotoxy(43, 13);
        cout << titulo;

        gotoxy(42, 14);
        for (int i = 0; i < strlen(titulo) + 2; i++) {
            cout << "*";
        }

        color(7);
        for (int i = 0; i < n; i++) {
            if (15 + i == 14 + opcion) {
                color(10);
            }
            if (i + 1 == n) {
                gotoxy(38, 15 + i);
                cout << "[0] " << opciones[i];
            } else {
                gotoxy(38, 15 + i);
                cout << "[" << i + 1 << "] " << opciones[i];
            }
            color(7);
        }
        color(10);
        gotoxy(34, 14 + opcion);
        cout << "~>";

        color(7);
        do {
            tecla = getch();
        } while (tecla != ARRIBA && tecla != ABAJO && tecla != ENTER);

        switch (tecla) {
        case ARRIBA: {
            opcion--;
            if (opcion == 0) {
                opcion = n;
            }
        } break;

        case ABAJO: {
            opcion++;
            if (opcion > n) {
                opcion = 1;
            }
        } break;

        case ENTER: {
            flag = false;
        } break;
        }

    } while (flag);
    return opcion;
}

void agregarArticulo(inventario &cabeza, string nombreArticulo, int idArticulo, int cantidad, double precio) {
    inventario nuevoArticulo = crearArticulo(nombreArticulo, idArticulo, cantidad, precio);
    if (!cabeza) {
        cabeza = nuevoArticulo;
    } else {
        inventario temp = cabeza;
        while (temp->siguiente) {
            temp = temp->siguiente;
        }
        temp->siguiente = nuevoArticulo;
        nuevoArticulo->anterior = temp;
    }
}

void eliminarArticulo(inventario &cabeza, int idArticulo) {
    if (!cabeza) return;
    inventario temp = cabeza;
    while (temp && temp->idArticulo != idArticulo) {
        temp = temp->siguiente;
    }
    if (!temp) return;
    if (temp->anterior) temp->anterior->siguiente = temp->siguiente;
    if (temp->siguiente) temp->siguiente->anterior = temp->anterior;
    if (temp == cabeza) cabeza = temp->siguiente;
    delete temp;
}

void mostrarInventario(inventario cabeza) {
	system("cls");
    inventario temp = cabeza;
    int fila = 8;
    centerText("=========================================");
    centerText("===      Inventario de productos      ===");
    centerText("=========================================");
    
    gotoxy(5, 5);
    cout << "ID Art�culo\tNombre\t\t\tCantidad\tPrecio" << endl;
    cout <<"======================================================================================"<<endl;
    while (temp) {
        gotoxy(5, fila);
        cout << temp->idArticulo << "\t\t";

        // Ajustar el ancho del campo de nombre usando setw
        cout << setw(20) << left << temp->nombreArticulo;

        cout << "\t" << temp->cantidad << "\t\t" << temp->precio << endl;
        temp = temp->siguiente;
        fila++;
    }
    cout <<"======================================================================================"<<endl;
}

void guardarInventario(inventario cabeza, const string& nombreArchivo) {
    ofstream archivo(nombreArchivo.c_str()); // Convertir el nombre de archivo a const char*
    if (!archivo.is_open()) {
        cerr << "Error al abrir el archivo de inventario." << endl;
        return;
    }

    inventario temp = cabeza;
    while (temp) {
        archivo << temp->idArticulo << " " << temp->nombreArticulo << " " << temp->cantidad << " " << temp->precio << endl;
        temp = temp->siguiente;
    }
    archivo.close();
}

void cargarInventario(inventario &cabeza, const string& nombreArchivo) {
    ifstream archivo(nombreArchivo.c_str()); // Convertir el nombre de archivo a const char*
    if (!archivo.is_open()) {
        cerr << "Error al abrir el archivo de inventario." << endl;
        return;
    }

    string nombreArticulo;
    int idArticulo, cantidad;
    double precio;
    while (archivo >> idArticulo >> nombreArticulo >> cantidad >> precio) {
        agregarArticulo(cabeza, nombreArticulo, idArticulo, cantidad, precio);
    }
    archivo.close();
}

void editarArticulo(inventario &cabeza, int idArticulo) {
    if (!cabeza) return;
    inventario temp = cabeza;
    while (temp && temp->idArticulo != idArticulo) {
        temp = temp->siguiente;
    }
    if (!temp) {
        centerText("Art�culo no encontrado.");
        return;
    }

    centerText("Ingrese la nueva cantidad: ");
    cin >> temp->cantidad;
    centerText("Ingrese el nuevo precio: ");
    cin >> temp->precio;

    centerText("Art�culo actualizado exitosamente.");
}

// Funciones para gestionar el carrito de compras
carrito crearArticuloCarrito(string nombreArticulo, int idArticulo, int cantidad, double precio) {
    carrito nuevoArticulo = new ArticuloCarrito;
    nuevoArticulo->nombreArticulo = nombreArticulo;
    nuevoArticulo->idArticulo = idArticulo;
    nuevoArticulo->cantidad = cantidad;
    nuevoArticulo->precio = precio;
    nuevoArticulo->montoTotal = cantidad * precio;
    nuevoArticulo->siguiente = NULL;
    return nuevoArticulo;
}

double recalcularMontoTotalCarrito(carrito cabezaCarrito) {
    double montoTotal = 0.0;
    carrito temp = cabezaCarrito;
    while (temp) {
        montoTotal += temp->precio * temp->cantidad;
        temp = temp->siguiente;
    }
    return montoTotal;
}

void agregarAlCarrito(carrito &cabeza, inventario &inv, int idArticulo, int cantidad) {
	
	
    // Buscar el art�culo en el inventario
    inventario tempInv = inv;
    while (tempInv && tempInv->idArticulo != idArticulo) {
        tempInv = tempInv->siguiente;
    }

    if (tempInv && tempInv->cantidad >= cantidad) {
        // Agregar al carrito
        carrito nuevoArticulo = crearArticuloCarrito(tempInv->nombreArticulo, idArticulo, cantidad, tempInv->precio);
        if (!cabeza) {
            cabeza = nuevoArticulo;
        } else {
            carrito temp = cabeza;
            while (temp->siguiente) {
                temp = temp->siguiente;
            }
            temp->siguiente = nuevoArticulo;
        }
		recalcularMontoTotalCarrito(cabeza);
        // Actualizar inventario
        //tempInv->cantidad -= cantidad;
        guardarInventario(inv, "inventarioTienda.txt"); // Guardar inventario actualizado
    } else {
        cout << "Articulo no encontrado o cantidad insuficiente en el inventario." << endl;
    }
}

void mostrarCarrito(carrito cabeza) {
    carrito temp = cabeza;
    double totalCompra = 0.0;
    int fila = 8;
    centerText("====================================================");
    centerText("===              Carrito de Compras             ====");
    centerText("====================================================");
    gotoxy(5, 6);
    cout << "ID Articulo\tNombre\t\t\tCantidad\tPrecio\t\tMonto X Articulo" << endl;
    cout <<"==================================================================================================="<<endl;
    while (temp) {
    	
    	gotoxy(5, fila);
        cout << temp->idArticulo << "\t\t";

        // Ajustar el ancho del campo de nombre usando setw
        cout << setw(20) << left << temp->nombreArticulo;

        cout << "\t" << temp->cantidad << "\t\t" << temp->precio << "\t\t" << temp->montoTotal << endl;
    	fila ++;
        totalCompra += temp->montoTotal;
        temp = temp->siguiente;
    }
    cout <<"\n================================================================================================="<<endl;
    cout << "Total de la compra: " << totalCompra << endl;
}

void eliminarDelCarrito(carrito &cabeza, inventario &inv, int idArticulo) {
    if (!cabeza) return;
    carrito temp = cabeza;
    carrito anterior = NULL;
    while (temp && temp->idArticulo != idArticulo) {
        anterior = temp;
        temp = temp->siguiente;
    }
    if (!temp) return;
    if (anterior) {
        anterior->siguiente = temp->siguiente;
    } else {
        cabeza = temp->siguiente;
    }
    delete temp;

    // Restaurar el inventario
    inventario tempInv = inv;
    while (tempInv && tempInv->idArticulo != idArticulo) {
        tempInv = tempInv->siguiente;
    }
    if (tempInv) {
        tempInv->cantidad += temp->cantidad;
        guardarInventario(inv, "inventarioTienda.txt"); // Guardar inventario actualizado
    }
}

void actualizarInventario(inventario &inv, carrito cart) {
    carrito tempCarrito = cart;
    while (tempCarrito) {
        inventario tempInv = inv;
        while (tempInv && tempInv->idArticulo != tempCarrito->idArticulo) {
            tempInv = tempInv->siguiente;
        }
        if (tempInv) {
            tempInv->cantidad -= tempCarrito->cantidad;
        }
        tempCarrito = tempCarrito->siguiente;
    }
}

void realizarCompra(inventario &inv, carrito &cart, const string& nombreArchivo) {
	system("cls");
    mostrarCarrito(cart);
    char confirmacion;
    cout << "�Desea confirmar la compra? (s/n): ";
    cin >> confirmacion;
    if (confirmacion == 's' || confirmacion == 'S') {
        actualizarInventario(inv, cart);
        guardarInventario(inv, nombreArchivo);
        cout << "Compra realizada con �xito. Aqu� est� su boleta:" << endl;
        mostrarCarrito(cart);
        // Vaciar el carrito
        while (cart) {
            carrito temp = cart;
            cart = cart->siguiente;
            delete temp;
        }
    } else {
        cout << "Compra cancelada." << endl;
    }
}

void gestionarInventario() {
    int opcion;
    const char* titulo = "GESTION DE INVENTARIO";
    string opciones[] = { "Agregar Articulo", "Eliminar Articulo", "Mostrar Inventario", 
    "Editar Articulo", "Salir"};

    inventario cabeza = NULL; // Inicializar la lista de inventario
    const string nombreArchivo = "inventarioTienda.txt";
    cargarInventario(cabeza, nombreArchivo); // Cargar inventario desde el archivo

    do {
        opcion = menu(titulo, opciones, 5);
        switch (opcion) {
            case 1: {
            	system("cls");
                string nombreArticulo;
                int idArticulo, cantidad;
                double precio;
                cout<<"Ingrese el nombre del art�culo: ";
                cin >> nombreArticulo;
                cout<<"\nIngrese el ID del art�culo: ";
                cin >> idArticulo;
                cout<<"\nIngrese la cantidad: ";
                cin >> cantidad;
                cout<<"\nIngrese el precio: ";
                cin >> precio;
                agregarArticulo(cabeza, nombreArticulo, idArticulo, cantidad, precio);
                guardarInventario(cabeza, nombreArchivo); // Guardar inventario en el archivo
                break;
            }
            case 2: {
            	system("cls");
                int idArticulo;
                mostrarInventario(cabeza);
                cout<<"Ingrese el ID del art�culo a eliminar: ";
                cin >> idArticulo;
                eliminarArticulo(cabeza, idArticulo);
                guardarInventario(cabeza, nombreArchivo); // Guardar inventario en el archivo
                break;
            }
            case 3: {
                mostrarInventario(cabeza);
                system("pause");
                break;
            }
            case 4: {
            	mostrarInventario(cabeza);
                int idArticulo;
                cout<<"Ingrese el ID del art�culo a editar: ";
                cin >> idArticulo;
                editarArticulo(cabeza, idArticulo);
                guardarInventario(cabeza, nombreArchivo); // Guardar inventario en el archivo
                system("pause");
                break;
            }
            case 5: {
                system("cls");
                break;
            }
        }
    } while(opcion != 5);

    guardarInventario(cabeza, nombreArchivo); // Guardar inventario en el archivo al salir
}

usuario CrearUsuario() {
	system("cls"); 
    centerText("==================================");
    centerText("===      Tienda Virtual        ===");
    centerText("==================================");
   
usuario nuevo = new Usuario;

centerText("Ingresar nuevo nombre de usuario:");
cin >> nuevo->nombreUsuario;

centerText("Ingresar nueva contrase�a: ");
char caracter;
string contrasena = "";
while ((caracter = _getch()) != '\r') { // Leer caracteres hasta que se presione Enter
    if (caracter == '\b') { // Si es la tecla de retroceso
        if (!contrasena.empty()) { // Verificar si la contrase�a no est� vac�a
            cout << "\b \b"; // Borrar el �ltimo asterisco mostrado
            contrasena.erase(contrasena.size() - 1); // Eliminar el �ltimo car�cter de la contrase�a
        }
    } else {
        cout << '*'; // Mostrar asterisco en lugar del car�cter ingresado
        contrasena += caracter; // Agregar el car�cter a la contrase�a
    }
}
cout << endl;

nuevo->contrasena = contrasena;
nuevo->sgte = NULL;
return nuevo;
}

void menuCliente(inventario &inv, const string& nombreArchivo) {
	
	inventario cabezaInventario = NULL;
    cargarInventario(cabezaInventario, "inventarioTienda.txt");
    carrito carritoCliente = NULL;
    carrito cart = NULL;
    int opcion;
    do {
    	system("cls");
        centerText("===================================");
    	centerText("===         Menu Cliente        ===");
    	centerText("===================================");
    	centerText("1. Agregar Articulo al Carrito");
    	centerText("2. Eliminar Articulo del Carrito");
    	centerText("3. Mostrar Carrito");
    	centerText("4. Realizar Compra");
    	centerText("0. Salir");
    	centerText("===================================");
    	centerText("Ingrese una opcion: ");
    	gotoxy(72, 10);
        cin >> opcion;

        switch(opcion) {
            case 1: {
            	system("cls");
                int idArticulo, cantidad;
                mostrarInventario(cabezaInventario);   
                cout << "\nIngrese el ID del Articulo: ";
                cin >> idArticulo;
                cout << "Ingrese la cantidad: ";
                cin >> cantidad;

                inventario tempInv = inv;
                while (tempInv && tempInv->idArticulo != idArticulo) {
                    tempInv = tempInv->siguiente;
                }

                if (tempInv && tempInv->cantidad >= cantidad) {
                    agregarAlCarrito(carritoCliente, cabezaInventario, idArticulo, cantidad);
                    cout << "Articulo agregado al carrito." << endl;
                } else {
                    cout << "Articulo no encontrado o cantidad insuficiente." << endl;
                }
                break;
            }
            case 2: {
            	system("cls");
                int idArticulo;
                mostrarCarrito(carritoCliente);
                cout << "Ingrese el ID del Articulo a eliminar: ";
                cin >> idArticulo;
                eliminarDelCarrito(carritoCliente, cabezaInventario, idArticulo);
                cout << "Articulo eliminado del carrito." << endl;
                break;
            }
            case 3:{
            	system("cls");
            	mostrarCarrito(carritoCliente);
                system("pause");
				break;
			}
            case 4:{
            	realizarCompra(cabezaInventario, carritoCliente, "inventarioTienda.txt");
                system("pause");
				break;
			}
            case 0:{
            	cout << "Saliendo del menu cliente." << endl;
				break;
			}
            default:{
            	cout << "Opcion invalida." << endl;
				break;
			}
        }
    } while (opcion != 0);
}

void InsertarUsuarioEnArchivo(const string& nombreUsuario, const string& contrasena) {
ofstream archivo("usuarios.txt", ios::app); // Abrir el archivo en modo de a�adir al final
if (archivo.is_open()) {
    archivo << nombreUsuario << " " << contrasena << endl; // Escribir los datos del usuario en una l�nea del archivo
    archivo.close(); // Cerrar el archivo
} else {
    cerr << "Error al abrir el archivo de usuarios." << endl;
}
}

bool AutenticarUsuarioDesdeArchivo(const string& nombreUsuario, const string& contrasena) {
    ifstream archivo("usuarios.txt");
    string usuario, pass;
    while (archivo >> usuario >> pass) { // Se lee los datos pares de usuario y contrase�a del archivo
        if (usuario == nombreUsuario && pass == contrasena) {
            archivo.close(); // Cerramos el archivo
            return true; // Usuario autenticado
        }
    }
    archivo.close(); // Cerramos el archivo
    return false; // Usuario no encontrado o contrase�a incorrecta
}

void menuClient() {
    inventario cabezaInventario = NULL;
    cargarInventario(cabezaInventario, "inventarioTienda.txt");
    int opcion;
    const char* titulo = "MENU DEL CLIENTE";
    string opciones[] = { "Logearse", "Crear Cuenta", "Ingreso Directo", "Salir"};

    do {
        opcion = menu(titulo, opciones, 4);
        switch (opcion) {
        case 1: // Logearse
            system("cls"); // Limpiar la consola 
            centerText("==================================");
            centerText("===      Tienda Virtual        ===");
            centerText("=================================="); 
            {
                string nombreUsuario, contrasenaIngresada = "";
                centerText("Ingrese nombre de usuario: "); 
                gotoxy(72, 3);
                cin >> nombreUsuario;
                centerText("Ingrese contrasena: ");
                char caracter;
                gotoxy(69, 4);
                
                // Leer la contraseña sin mostrarla en pantalla
                while ((caracter = _getch()) != '\r') { // Leer caracteres hasta que se presione Enter
                    if (caracter == '\b') { // Si es la tecla de retroceso
                        if (!contrasenaIngresada.empty()) { // Verificar si la contraseña no está vacía
                            cout << "\b \b"; // Borrar el último asterisco mostrado
                            contrasenaIngresada.erase(contrasenaIngresada.size() - 1); // Eliminar el último carácter de la contraseña ingresada
                        }
                    } else {
                        cout << '*'; // Mostrar asterisco en lugar del carácter ingresado
                        contrasenaIngresada += caracter; // Agregar el carácter a la contraseña ingresada
                    }
                }
                cout << endl;

                if (AutenticarUsuarioDesdeArchivo(nombreUsuario, contrasenaIngresada)) {
                    centerText("Usuario autenticado exitosamente.");
                    menuCliente(cabezaInventario, "inventario.txt");
                } else {
                    centerText("Autenticacion fallida. Nombre de usuario o contrasena incorrectos.");
                    system("pause");
                }
            }
            break;
        case 2: // Crear Cuenta
            system("cls");
            {
                Usuario* nuevoUsuario = CrearUsuario();
                InsertarUsuarioEnArchivo(nuevoUsuario->nombreUsuario, nuevoUsuario->contrasena);
                delete nuevoUsuario;
            }
            break;
        case 3: // Ingreso Directo
            system("cls");
            menuCliente(cabezaInventario, "inventario.txt");
            break;
        case 4: // Salir
            system("cls");
            break;
        }
    } while(opcion != 4);
}


void menuAdm() {
    int opcion;
    const char* titulo = "OPCIONES DE ADMINISTRADOR";
    string opciones[] = { "Gestion de Inventario", "Gestion de Clientes", "Gestion de pedidos", 
    "Gestion de Promociones y Descuentos", "Salir"};

    do {
        opcion = menu(titulo, opciones, 5);
        switch (opcion) {
        case 1: // Llamar función de Inventario
            gestionarInventario();
            break;
        case 2: // Llamar función de Clientes
            //OpcionesCliente();
            break;
        case 3: // Llamar función de Pedidos
            //OpcionesCliente();
            break;
        case 4: // Llamar función de Promociones
            //OpcionesCliente();
            break;
        case 5: // Salir del menu
            system("cls");
        }break;
    } while(opcion != 5);
}

void OpcionesAdministrador(){
        system("cls"); // Limpiar la consola 
        centerText("==================================");
        centerText("===      Tienda Virtual        ===");
        centerText("=================================="); 
        string nombreUsuario, contrasena;
        centerText("Ingrese nombre de usuario: "); 
        gotoxy(72, 3);
        cin >> nombreUsuario;
        centerText("Ingrese contrase�a: ");
        char caracter;
        gotoxy(69, 4);
        string contrasenaIngresada = "";
        while ((caracter = _getch()) != '\r') { // Leer caracteres hasta que se presione Enter
            if (caracter == '\b') { // Si es la tecla de retroceso
                if (!contrasenaIngresada.empty()) { // Verificar si la contrase�a no est� vac�a
                    cout << "\b \b"; // Borrar el �ltimo asterisco mostrado
                    contrasenaIngresada.erase(contrasenaIngresada.size() - 1); // Eliminar el �ltimo car�cter de la contrase�a ingresada
                }
            } else {
                cout << '*'; // Mostrar asterisco en lugar del car�cter ingresado
                contrasenaIngresada += caracter; // Agregar el car�cter a la contrase�a ingresada
            }
        }
        cout << endl;

        if (AutenticarUsuarioDesdeArchivo(nombreUsuario, contrasenaIngresada)){
            centerText("Usuario autenticado exitosamente." );
            menuAdm();
            }
        else{
            centerText("Autenticacion fallida. Nombre de usuario o contrase�a incorrectos.");
            system("pause");
            return;
        }     
}

void menu1() {
    int opcion;
    const char* titulo = "TIENDA VIRTUAL";
    string opciones[] = { "Ingresar como ADMINISTRADOR", "Ingresar como CLIENTE", "Salir"};

    do {
        opcion = menu(titulo, opciones, 3);
        switch (opcion) {
        case 1: // Llamar función de Administrador
            OpcionesAdministrador();
            break;
        case 2: // Llamar función de Trabajador
            menuClient();
            break;
        case 3:
            system("cls");
            exit(0); // Salir del programa
            break;
        }
    } while (true);
}

int main() {
    Fecha fechaHoraActual = obtenerFechaHoraActual();

    carrito carritoCliente = NULL;

    system("cls"); // Limpiar la consola
    gotoxy(90,1);//a�adimos
    cout<<fechaHoraActual.dia<<"/"<<fechaHoraActual.mes<<"/"<<fechaHoraActual.anio<<"    "<< fechaHoraActual.hora<<
    ":"<<fechaHoraActual.minutos<< ":" << fechaHoraActual.segundos<<endl;

    menu1(); //Seleccion de Administrador o Cliente
    gotoxy(72, 7);
    return 0;
}
