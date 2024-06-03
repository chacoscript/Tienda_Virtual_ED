#include <iostream>
#include <cstdlib> 
#include <ctime> 
#include <windows.h> // Para obtener información sobre la consola
#include <string>
#include <conio.h>
#include <fstream>// Para manejo de archivos

using namespace std;
//Nodo para guardar cuentas personales
struct Usuario {
    string nombreUsuario;
    string contrasena;
    Usuario* sgte;
};

typedef Usuario* usuario;
//Structura para obtener la fecha y hora actual
struct fecha{
	int anio;
	int mes;
	int dia;
	int hora;
	int minutos;
	int segundos;
};

void gotoxy(int x, int y) {
    COORD coord;
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}
typedef struct fecha* Fecha;

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

void menu1(){
	system("cls"); // Limpiar la consola

    centerText("==================================");
    centerText("===      Tienda Virtual        ===");
    centerText("=================================="); 
    centerText("1. Ingresar como administrador");
    centerText("2. Ingresar como Cliente");
    centerText("0. Salir");
    centerText("==================================");

        // Solicitar la selección de opción
        centerText("Seleccione una opción: ");
}
void menuAdm(){
	system("cls"); // Limpiar la consola

    centerText("==================================");
    centerText("===      Tienda Virtual        ===");
    centerText("=================================="); 
    centerText("1. Gestion de Inventario");
    centerText("2. Gestion de Clientes");
    centerText("3. Gestion de Pedidos");
    centerText("0. Salir");
    centerText("==================================");

        // Solicitar la selección de opción
        centerText("Seleccione una opción: ");
}

void menuClient(){
	system("cls"); // Limpiar la consola

    centerText("==================================");
    centerText("===      Tienda Virtual        ===");
    centerText("=================================="); 
    centerText("1. Logearse");
    centerText("2. Crear cuenta");
    centerText("3. Ingreso directo");
    centerText("0. Salir");
    centerText("==================================");

        // Solicitar la selección de opción
        centerText("Seleccione una opción: ");
}

usuario CrearUsuario() {
	
	centerText("==================================");
    centerText("===      Tienda Virtual        ===");
    centerText("=================================="); 
    usuario nuevo = new Usuario;
    
    centerText("Ingresar nuevo nombre de usuario:");
    cin >> nuevo->nombreUsuario;
    centerText("Ingresar nueva contrasena: ");
    char caracter;
    string contrasena = "";
    while ((caracter = _getch()) != '\r') { // Leer caracteres hasta que se presione Enter
        if (caracter == '\b') { // Si es la tecla de retroceso
            if (!contrasena.empty()) { // Verificar si la contraseña no está vacía
                cout << "\b \b"; // Borrar el último asterisco mostrado
                contrasena.erase(contrasena.size() - 1); // Eliminar el último carácter de la contraseña
            }
        } else {
            cout << '*'; // Mostrar asterisco en lugar del caracter ingresado
            contrasena += caracter; // Agregar el carácter a la contraseña
        }
    }
    cout << endl;

    nuevo->contrasena = contrasena;
    nuevo->sgte = NULL;
    return nuevo;
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
    srand(time(NULL)); // Inicializar la semilla para la generación de números aleatorios
    int opcion,opcionAdm,opcionClient;

    do {
        menu1();
        gotoxy(72, 7);
        cin >> opcion;

        switch (opcion) {
            case 1:{
            	system("cls"); // Limpiar la consola 
            	
            	string nombreUsuario, contrasena;
			    cout << "Ingrese nombre de usuario: ";
			    cin >> nombreUsuario;
			
			    cout << "Ingrese contrasena: ";
			    char caracter;
			    string contrasenaIngresada = "";
			    while ((caracter = _getch()) != '\r') { // Leer caracteres hasta que se presione Enter
			        if (caracter == '\b') { // Si es la tecla de retroceso
			            if (!contrasenaIngresada.empty()) { // Verificar si la contraseña no está vacía
			                cout << "\b \b"; // Borrar el último asterisco mostrado
			                contrasenaIngresada.erase(contrasenaIngresada.size() - 1); // Eliminar el último carácter de la contraseña ingresada
			            }
			        } else {
			            cout << '*'; // Mostrar asterisco en lugar del caracter ingresado
			            contrasenaIngresada += caracter; // Agregar el carácter a la contraseña ingresada
			        }
			    }
			    cout << endl;
			
			    if (AutenticarUsuarioDesdeArchivo(nombreUsuario, contrasenaIngresada))
			        cout << "Usuario autenticado exitosamente." << endl;
			    else{
			    	cout << "Autenticacion fallida. Nombre de usuario o contrasena incorrectos." << endl;
			    	system("pause");
			    	break;
				}
			            
			            	
            	do{
            		menuAdm();
            		gotoxy(72, 8);
            		cin>>opcionAdm;
            		switch (opcionAdm){
            			case 1:{
							break;
						}
            				
            			case 2:{
							break;
						}
            				
						case 0:{
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
            		gotoxy(72, 8);
					cin>>opcionClient;
					switch(opcionClient){
						case 1:{
							break;
						}
							
						case 2:{
							
							usuario nuevoUsuario = CrearUsuario();
   	 						InsertarUsuarioEnArchivo(nuevoUsuario->nombreUsuario, nuevoUsuario->contrasena);
							break;
						}
							
						case 3:{
							break;
						} 
									
					}
				}while (opcionClient != 0);
				break;
			}
            	
            case 3:{
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
