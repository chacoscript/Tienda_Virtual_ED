#include <iostream>
#include <cstdlib>
#include <ctime>
#include <windows.h>
#include <string>
#include <conio.h>
#include <fstream>
#include <iomanip>    // Para usar setw
using namespace std;

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

// Función para obtener la fecha y hora actual
Fecha obtenerFechaHoraActual() {
    // Obteniendo la fecha y hora actual
    time_t tiempoActual = time(NULL);
    tm* fechaHora = localtime(&tiempoActual);

    // Creando una instancia de la estructura Fecha
    Fecha fechaActual;
    fechaActual.anio = fechaHora->tm_year + 1900; // tm_year devuelve años desde 1900
    fechaActual.mes = fechaHora->tm_mon + 1;      // tm_mon devuelve meses desde 0
    fechaActual.dia = fechaHora->tm_mday;
    fechaActual.hora = fechaHora->tm_hour;
    fechaActual.minutos = fechaHora->tm_min;
    fechaActual.segundos = fechaHora->tm_sec;

    return fechaActual;
}

void gotoxy(int x, int y) {
    COORD coord;
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

// Función para obtener el ancho de la consola
int getConsoleWidth() {
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    int columns;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
    columns = csbi.srWindow.Right - csbi.srWindow.Left + 1;
    return columns;
}

// Función para centrar texto en la consola
void centerText(string text) {
    int consoleWidth = getConsoleWidth();
    int padding = (consoleWidth - text.length()) / 2;
    cout << string(padding, ' ') << text << endl;
}

// Estructura para Artículo del Carrito de Compras
struct ArticuloCarrito {
    string nombreArticulo;
    int idArticulo;
    int cantidad;
    double precio;
    double montoTotal;
    ArticuloCarrito* siguiente;
};

typedef ArticuloCarrito* carrito;

// Estructura para Artículo del Inventario
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
    cout << "ID Artículo\tNombre\t\t\tCantidad\tPrecio" << endl;
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
        centerText("Artículo no encontrado.");
        return;
    }

    centerText("Ingrese la nueva cantidad: ");
    cin >> temp->cantidad;
    centerText("Ingrese el nuevo precio: ");
    cin >> temp->precio;

    centerText("Artículo actualizado exitosamente.");
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
	
	
    // Buscar el artículo en el inventario
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
    cout << "¿Desea confirmar la compra? (s/n): ";
    cin >> confirmacion;
    if (confirmacion == 's' || confirmacion == 'S') {
        actualizarInventario(inv, cart);
        guardarInventario(inv, nombreArchivo);
        cout << "Compra realizada con éxito. Aquí está su boleta:" << endl;
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


void menu1() {
	system("cls"); 
    centerText("==================================");
    centerText("===      Tienda Virtual        ===");
    centerText("==================================");
    centerText("1. Ingresar como administrador");
    centerText("2. Ingresar como Cliente");
    centerText("0. Salir");
    centerText("==================================");

    // Solicitar la selección de opción
    centerText("Seleccione una opcion: ");
}

void menuAdm() {
    system("cls"); // Limpiar la consola

    centerText("============================================");
    centerText("===      Opciones de Administrador       ===");
    centerText("============================================");
    centerText("1. Gestión de Inventario");
    centerText("2. Gestión de Clientes");
    centerText("3. Gestión de Pedidos");
    centerText("4. Gestión de Promociones y descuentos");
    centerText("0. Salir");
    centerText("==================================");

    // Solicitar la selección de opción
    centerText("Seleccione una opcion: ");
}

void gestionarInventario() {
    inventario cabeza = NULL; // Inicializar la lista de inventario
    const string nombreArchivo = "inventarioTienda.txt";
    cargarInventario(cabeza, nombreArchivo); // Cargar inventario desde el archivo

    int opcionInventario;
    do {
        system("cls");
        centerText("==================================");
        centerText("===  Gestión de Inventario     ===");
        centerText("==================================");
        centerText("1. Agregar Artículo");
        centerText("2. Eliminar Artículo");
        centerText("3. Mostrar Inventario");
        centerText("4. Editar Artículo");
        centerText("0. Salir");
        centerText("==================================");
        centerText("Seleccione una opción: ");
        gotoxy(72, 9);
        cin >> opcionInventario;

        switch (opcionInventario) {
            case 1: {
            	system("cls");
                string nombreArticulo;
                int idArticulo, cantidad;
                double precio;
                cout<<"Ingrese el nombre del artículo: ";
                cin >> nombreArticulo;
                cout<<"\nIngrese el ID del artículo: ";
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
                cout<<"Ingrese el ID del artículo a eliminar: ";
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
                cout<<"Ingrese el ID del artículo a editar: ";
                cin >> idArticulo;
                editarArticulo(cabeza, idArticulo);
                guardarInventario(cabeza, nombreArchivo); // Guardar inventario en el archivo
                system("pause");
                break;
            }
        }
    } while (opcionInventario != 0);

    guardarInventario(cabeza, nombreArchivo); // Guardar inventario en el archivo al salir
}



void menuClient() {
    system("cls"); // Limpiar la consola

    centerText("=======================================");
    centerText("===      Opciones de Cliente        ===");
    centerText("=======================================");
    centerText("1. Logearse");
    centerText("2. Crear cuenta");
    centerText("3. Ingreso directo");
    centerText("0. Salir");
    centerText("==================================");

    // Solicitar la selección de opción
    centerText("Seleccione una opción: ");
}

usuario CrearUsuario() {
	system("cls"); 
    centerText("==================================");
    centerText("===      Tienda Virtual        ===");
    centerText("==================================");
   
usuario nuevo = new Usuario;

centerText("Ingresar nuevo nombre de usuario:");
cin >> nuevo->nombreUsuario;

centerText("Ingresar nueva contraseña: ");
char caracter;
string contrasena = "";
while ((caracter = _getch()) != '\r') { // Leer caracteres hasta que se presione Enter
    if (caracter == '\b') { // Si es la tecla de retroceso
        if (!contrasena.empty()) { // Verificar si la contraseña no está vacía
            cout << "\b \b"; // Borrar el último asterisco mostrado
            contrasena.erase(contrasena.size() - 1); // Eliminar el último carácter de la contraseña
        }
    } else {
        cout << '*'; // Mostrar asterisco en lugar del carácter ingresado
        contrasena += caracter; // Agregar el carácter a la contraseña
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
ofstream archivo("usuarios.txt", ios::app); // Abrir el archivo en modo de añadir al final
if (archivo.is_open()) {
    archivo << nombreUsuario << " " << contrasena << endl; // Escribir los datos del usuario en una línea del archivo
    archivo.close(); // Cerrar el archivo
} else {
    cerr << "Error al abrir el archivo de usuarios." << endl;
}
}

bool AutenticarUsuarioDesdeArchivo(const string& nombreUsuario, const string& contrasena) {
ifstream archivo("usuarios.txt");
string usuario, pass;
while (archivo >> usuario >> pass) { // Se lee los datos pares de usuario y contraseña del archivo
    if (usuario == nombreUsuario && pass == contrasena) {
        archivo.close(); // Cerramos el archivo
        return true; // Usuario autenticado
    }
}
archivo.close(); // Cerramos el archivo
return false; // Usuario no encontrado o contraseña incorrecta
}

int main() {
Fecha fechaHoraActual = obtenerFechaHoraActual();

inventario cabezaInventario = NULL;
cargarInventario(cabezaInventario, "inventarioTienda.txt");
carrito carritoCliente = NULL;

system("cls"); // Limpiar la consola
gotoxy(90,1);//añadimos
cout<<fechaHoraActual.dia<<"/"<<fechaHoraActual.mes<<"/"<<fechaHoraActual.anio<<"    "<< fechaHoraActual.hora << ":"<<fechaHoraActual.minutos<< ":" << fechaHoraActual.segundos<<endl;
int opcion,opcionAdm,opcionClient;

do {
    menu1();
    gotoxy(72, 7);
    cin >> opcion;

    switch (opcion) {
        case 1:{
            system("cls"); // Limpiar la consola 
            centerText("==================================");
            centerText("===      Tienda Virtual        ===");
            centerText("=================================="); 
            string nombreUsuario, contrasena;
            centerText("Ingrese nombre de usuario: "); 
            gotoxy(72, 3);
            cin >> nombreUsuario;
            centerText("Ingrese contraseña: ");
            char caracter;
            gotoxy(69, 4);
            string contrasenaIngresada = "";
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

            if (AutenticarUsuarioDesdeArchivo(nombreUsuario, contrasenaIngresada))
                centerText("Usuario autenticado exitosamente." );
            else{
                centerText("Autenticacion fallida. Nombre de usuario o contraseña incorrectos.");
                system("pause");
                break;
            }
            
            do{
                menuAdm();
                gotoxy(72, 9);
                cin>>opcionAdm;
                switch (opcionAdm){
                    case 1:{
                        gestionarInventario(); // Llamar a la función de gestión de inventario
                        break;
                    }
                    
                    case 2:{
                        break;
                    }
                    
                    case 0:{
                        break;
                    }
                    
                    case 4:{
                        break;
                    }            
                }
            } while (opcionAdm != 0);                
            break;
        }
            
        case 2:{
            system("cls"); // Limpiar la consola
            do{
                menuClient();
                gotoxy(72, 9);
                cin>>opcionClient;
                switch(opcionClient){
                    case 1:{
                    system("cls"); // Limpiar la consola 
		            centerText("==================================");
		            centerText("===      Tienda Virtual        ===");
		            centerText("=================================="); 
		            string nombreUsuario, contrasena;
		            centerText("Ingrese nombre de usuario: "); 
		            gotoxy(72, 3);
		            cin >> nombreUsuario;
		            centerText("Ingrese contraseña: ");
		            char caracter;
		            gotoxy(69, 4);
		            string contrasenaIngresada = "";
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
		
		            if (AutenticarUsuarioDesdeArchivo(nombreUsuario, contrasenaIngresada))
		                centerText("Usuario autenticado exitosamente." );
		            else{
		                centerText("Autenticacion fallida. Nombre de usuario o contraseña incorrectos.");
		                system("pause");
		                break;
		            }
					
					menuCliente(cabezaInventario, "inventario.txt");	
                        break;
                    }
                        
                    case 2:{
                        usuario nuevoUsuario = CrearUsuario();
                        InsertarUsuarioEnArchivo(nuevoUsuario->nombreUsuario, nuevoUsuario->contrasena);
                        break;
                    }
                        
                    case 3:{
                    	menuCliente(cabezaInventario, "inventario.txt");
                        break;
                    }                   
                }
            }while (opcionClient != 0);
            break;
        }
            
    
        case 0:{
            break;
        }
            
        default:
            centerText("Opción no válida\n");
            break;
    }
} while (opcion != 0);

return 0;
}

