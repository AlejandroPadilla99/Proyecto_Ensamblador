/*
	Descripcion: Programa diseñado para leer un archivo .txt, separar los direfentes
	elementos del mismo y ordenarlos de manera que cada línea sea una instrucción en
	código ensamblador.
*/

/*Inclusión de librerías*/
#include<bits/stdc++.h>
#include<fstream>

using namespace std;

/*Estructura encargada de guardar los datos de una línea de instrucciones ensamblador*/
struct datos{
	string etiqueta;
	string codop;
	string operando;
	string comentario; //Usada solo en caso de que la línea sea un comentario
	bool error; //Usado para verificar errores
	int numeroLinea; //Usado para contabilizar el número de línea
};

//Variables globales
vector <datos> registro;

/*
	Método que valida todos los posibles errores en las etiquetas y comentarios.
	Parámetros:
	s = Cadena de texto a evaluar
	IntruccionValidar = tipo de etiqueta que se va a evaluar
*/
bool ValidarValor(string s, int IntruccionValidar) {

	switch (IntruccionValidar) {

			//En caso de que la cadena sea una etiqueta
			case 0: {

				//Validacion de longitud
				if (s.length() > 8)	return false;

				else {

					//Verifica si el primer caracter no es una letra
					if ((int(s[0]) >= 0 && int(s[0]) <=64) ||(int(s[0]) >= 91 && int(s[0]) <= 96) || (int(s[0]) >= 123)) return false;

					else {

						for (int i = 1; i < s.length(); i++) {

							//Validacion de solo caracteres, números o guiones bajos
							if((int(s[i]) >= 0 && int(s[i]) <= 47) || (int(s[i]) >= 58 && int(s[i]) <= 64) || (int(s[i]) >= 91 && int(s[i]) <= 96 && int(s[i]) != 95) || (int(s[i]) >= 123)) return false;
						}
					}
				}

				//La cadena es valida
				return true;
			}

			//En caso de que la cadena sea un código de operación
			case 1: {

				//Validacion de longitud
				if (s.length() > 5) return false;
				
				else {

					//Verifica si el primer caracter no es una letra
					if ((int(s[0]) >= 0 &&  int(s[0]) <=64) || (int(s[0]) >= 91 && int(s[0]) <= 96) || (int(s[0]) >= 123)) return false;
					
					else {

						int PuntosCadena = 0; //Variable que verifica si ya existe un punto en la cadena

						//for que recorre toda la cadena
						for (int i = 1; i < s.length(); i++) {

							//Validacion de solo caracteres
							if((int(s[i]) >= 0 &&  int(s[i]) <=45) || (int(s[i]) >=47 && int(s[i]) <= 64) || (int(s[i]) >= 91 &&  int(s[i]) <= 96) || (int(s[i]) >= 123)) return false;
							
							else {

								//Verificación de que exista un solo punto en la cadena
								if (int(s[i]) == 46) PuntosCadena ++;
								if (PuntosCadena > 1) return false;
							}
						}
					}
				}
				//La cadena es valida
				return true;
			}

			//En caso de que la cadena sea un comentario
			case 3: {

				//Validacion de longitud
				if(s.length() > 80) return false;
				else return true;
			}
	}

	//La cadena es valida
	return true;
}

/*Método principal main*/
int main() {
	
	//Objetos
	ifstream File;
	File.open("P1ASM.txt");

	//Variables
	string item = ""; //Variable para guardar la línea de caracteres temporalmente
	datos DatosAux; //Estructura auxiliar para guardar las líneas ensamblador
	int Linea = 0; //Contador para indicar el numero de la linea del texto
	bool Terminar_programa = false; //Variable encargada de terminar el programa en caso de encontrar un END
	
	// Posibles errores al abrir el .txt
	if (File.fail()){

		cerr << "Error Al abrir el archivo";
		exit(1);
	}
	
	//Leer y procesar el archivo línea por línea
	while (getline (File,item)) {
		
		Linea++;
		
		//Inicializamos los campos de DatosAux en NULL
		DatosAux.etiqueta = "NULL";
		DatosAux.codop = "NULL";
		DatosAux.operando = "NULL";
		DatosAux.comentario = "NULL";
		DatosAux.error = false;
		DatosAux.numeroLinea = Linea;
		
		/*Parte del programa que separa las partes del archivo*/
		
		//Verifica si la linea es un comentario
		if (item[0] == ';') {
			
			//Valida que no haya errores en el comentario
			if (ValidarValor(item,3)) DatosAux.comentario = item;

			else {

				DatosAux.error = true;
			}

			registro.push_back(DatosAux);
		}

		else {
			
			int DatoIngresado= 0; // Contador para conocer el tipo de dato en el que se encuentra
			
			//Recorrer todos los caracteres de la linea
			for(int i = 0; i < item.length(); i++) {
				
				string valor = ""; //Cadena que guarda temporalmete los valores de las etiquetas

				//Condicional encargado de terminar la lectura de la línea en caso de que las 3 etiquetas tengan un valor y mandar un error
				if(DatoIngresado > 2) {

					DatosAux.error = true;
					break;
				}

				//Validamos los tabuladores y espacios como el final de las etiquetas
				while (int(item[i]) != 9 && int(item[i]) != 32 && i < item.length()) {

					valor = valor + item[i];
					i++;
				}

				//En caso de que la palabra contenga algún valor
				if (valor != "") {
					
					//Condicional que verifica si la palabra es igual a END para terminar el programa
					if (valor == "END" || valor == "end" || valor == "End") {

						Terminar_programa = true;
						break;
					}
					
					//Mandamos la cadena valor a validar	
					if (ValidarValor(valor, DatoIngresado)) {

						//Se guardan los datos optenidos en los campos correspondientes
						if(DatoIngresado == 0) DatosAux.etiqueta = valor;
						else if(DatoIngresado == 1) DatosAux.codop = valor;
						else if(DatoIngresado == 2) DatosAux.operando = valor;
					}

					else {

						//Marcamos que esta linea de codigo tiene un error
						DatosAux.error = true;
						break;
					}
					
				}

				DatoIngresado++;
			}
			
			//Si se encontro la palabra "END", termina el programa
			if(Terminar_programa) break;
			
			//Si la etiqueta "codop" esta vacia, marca un error
			if(DatosAux.codop == "") DatosAux.error = true;
			registro.push_back(DatosAux);
		}

	} //Fin de procesar el archivo
	
	
	//Imprimir los datos
	for (int i = 0; i < registro.size(); i++) {

		if(registro[i].error == true) cout << "Error linea: " <<registro[i].numeroLinea << endl;
		else if(registro[i].comentario != "NULL") cout << "Comentario" << endl;
		else cout << "Etiqueta = " << registro[i].etiqueta << "\t codop = " <<  registro[i].codop << "\t operando = " << registro[i].operando << endl;
		
	}
	
	return 1;
}
