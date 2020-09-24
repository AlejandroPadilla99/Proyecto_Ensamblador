#include<bits/stdc++.h>
#include<fstream>
using namespace std;

struct datos{
	string etiqueta;
	string codop;
	string operando;
	string comentario; //Usada solo en caso de que la línea sea un comentario
	bool error; //Usado para verificar errores
	int numeroLinea; //Usado para contabilizar el número de línea
};

struct operaciones{
	string CodigoDeOperacion, Operando, ModoDireccionamiento, CodMaquina; 
	int BytesCalcular, TotalBytes;
};


class LeerDocumento{
	//Atributos
	public:
	ifstream Archivo;
	ifstream Tabop;
	vector<datos> registro;
	vector <operaciones> RegistroTabop;
	vector <operaciones> registroCodop;
	
	//Metodos
	public:
		LeerDocumento(string Direccion);
		void ImprimirIntrucciones();
		void ImprimirTabop();
		void ImprimirCodigos();
		void ProcesarDocumento();
		void CargarTabop();
		bool ValidarValor(string s, int IntruccionValidar);
};

LeerDocumento::LeerDocumento(string Direccion){
	Archivo.open(Direccion);
	Tabop.open("DocumentosInternos/Tabop.txt");
}
//Permite imprimir
void LeerDocumento::ImprimirIntrucciones(){
	for(datos i: registro){
		if(i.error == true) cout << "Error linea" << i.numeroLinea << endl;
		else if(i.comentario != "NULL") cout << "Comentario" << endl;
		else cout << "Etiqueta = " << i.etiqueta << "\t codop = " <<  i.codop << "\t operando = " << i.operando << endl;
	}
}
void LeerDocumento::ImprimirTabop(){
	for(operaciones i : RegistroTabop){
		cout << i.CodigoDeOperacion << "\t" << i.Operando << "\t" << i.ModoDireccionamiento << "\t" << i.CodMaquina << "\t" << i.BytesCalcular << "\t" << i.TotalBytes << endl;
	}
}
void LeerDocumento::ImprimirCodigos(){
		for(operaciones i : registroCodop){
			cout << i.CodigoDeOperacion << "\t" << i.Operando << "\t" << i.ModoDireccionamiento << "\t";
			if (i.ModoDireccionamiento.length() < 8) cout << "\t";
			cout << i.CodMaquina << "\t" << i.BytesCalcular << "\t" << i.TotalBytes << endl;
		}
}
// Permite Procesar los diferentes campos del archivo
void LeerDocumento::ProcesarDocumento(){
	
	//Variables
	string item = ""; //Variable para guardar la línea de caracteres temporalmente
	datos DatosAux; //Estructura auxiliar para guardar las líneas ensamblador
	int Linea = 0; //Contador para indicar el numero de la linea del texto
	bool Terminar_programa = false; //Variable encargada de terminar el programa en caso de encontrar un END
	
	//Leer y procesar el archivo línea por línea
	while (getline (Archivo,item)) {
		
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

				//Condicional encargado de terminar la lectura de la lÃ­nea en caso de que las 3 etiquetas tengan un valor y mandar un error
				if(DatoIngresado > 2) {

					DatosAux.error = true;
					break;
				}

				//Validamos los tabuladores y espacios como el final de las etiquetas
				while (int(item[i]) != 9 && int(item[i]) != 32 && i < item.length()) {

					valor = valor + item[i];
					i++;
				}

				//En caso de que la palabra contenga algÃºn valor
				if (valor != "") {
					
					//Condicional que verifica si la palabra es igual a END para terminar el programa
					if (valor == "END") {

						Terminar_programa = true;
						break;
					}
					
					//Mandamos la cadena valor a validar	
					if (ValidarValor(valor, DatoIngresado)) {

						//Se guardan los datos optenidos en los campos correspondientes
						if(DatoIngresado == 0) DatosAux.etiqueta = valor;
						
						else if(DatoIngresado == 1){
							
							bool Codop_Encontrado = false; //Booleano que verifica que sí se haya encontrado el código de operación
							
							//Por cada línea del Tabop
							for (int j = 0; j < RegistroTabop.size(); j++) {
							
								//Si la el código de operación es encontrado en la línea actual del Tabop
								if (valor == RegistroTabop[j].CodigoDeOperacion) {
									
									Codop_Encontrado = true;
									DatosAux.codop = valor;
									
									//Convierte el código de operación a minúsculas para que coincida con la Tabop
									for (int k = 0; k < valor.length(); k++) {
										
										valor[k] = tolower(valor[k]);
									}
									
									//Inserta cada elemento que coincida con el código de operación en un registro
									do {
										
										registroCodop.push_back(RegistroTabop[j]);
										j++;
									} while (valor == RegistroTabop[j].CodigoDeOperacion);
									
									//Si el código no contiene un operando cuando debe, marca error
									if (registroCodop[registroCodop.size()-1].Operando == "Operando" &&  i >= item.length()-1) DatosAux.error = true;
									
									break;
								}
							}
							
							//Si no se encuentra el código de operación en el Tabop, el programa marcará un error
							if (!Codop_Encontrado) {
								
								DatosAux.error = true;
							}
						}
						
						else {
							
							//Si se ha agregado previamente un valor al registro encargado de guardar los datos de los códigos de operación utilizados
							if (registroCodop.size() > 0) {
							
								//Si el código contiene un operando cuando no debe, marca error
								if (registroCodop[registroCodop.size()-1].Operando == "No Operando") DatosAux.error = true;
								else DatosAux.operando = valor;
							}
						}
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
	
	
}

void LeerDocumento::CargarTabop(){
	
	string cadena = "";
	operaciones aux;
	int con;
	while(getline(Tabop,cadena)){
		
		aux.CodigoDeOperacion = "";
		aux.Operando = "";
		aux.ModoDireccionamiento = "";
		aux.CodMaquina = "";
		aux.BytesCalcular = 0;
		aux.TotalBytes = 0;
		con = 0;
		
		for(int i = 0; i < cadena.length(); i++) {
			string valor = "";
			
			while (int(cadena[i]) != 9 && i < cadena.length()) {
				valor = valor + cadena[i];
				i++;
			}
		
			if(con == 0) aux.CodigoDeOperacion = valor;
			else if(con == 1) aux.Operando = valor;
			else if(con == 2) aux.ModoDireccionamiento = valor;
			else if(con == 3) aux.CodMaquina = valor;
			else if(con == 4) istringstream(valor) >> aux.BytesCalcular;
			else istringstream(valor) >> aux.TotalBytes;
			con++;
		}
		RegistroTabop.push_back(aux);
	}
	
	
}
/*
	Método que valida todos los posibles errores en las etiquetas y comentarios.
	Parámetros:
	s = Cadena de texto a evaluar
	IntruccionValidar = tipo de etiqueta que se va a evaluar
*/
bool LeerDocumento::ValidarValor(string s, int IntruccionValidar) {

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
