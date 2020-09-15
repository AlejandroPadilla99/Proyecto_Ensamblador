/*
	Descripcion: Programa diseñado para leer un archivo .txt para separar los direfentes
	elementos del mismo.
 
 
 */

#include<bits/stdc++.h>
#include<fstream>

using namespace std;

struct datos{
	string etiqueta;
	string codop;
	string operando;
	bool error;
	int numeroLinea;
};


//Variables globales
vector <datos> registro;
bool ValidarValor(string s, int IntruccionValidar){
	switch (IntruccionValidar){
			case 0:{
				//Validacion de longitud
				if(s.length() > 8)	return false;
				else{
					//Validacio de primer caracter MAL
					if ((int(s[0]) >= 32 &&  int(s[0]) <=64) ||(int(s[0]) >= 91 && int(s[0]) <= 96) || (int(s[0]) >= 123)) return false;
				}
				//La cadena es valida
				return true;
			}
			case 1:{
				//Validacion de longitud
				if(s.length() > 5) return false;
				else{
					//Validacion de primer caracter MAl
					if ((int(s[0]) >= 32 &&  int(s[0]) <=64) || (int(s[0]) >= 91 && int(s[0]) <= 96) || (int(s[0]) >= 123)) return false;
					else{
						int PuntosCadena = 0;
						for(int i = 0; i < s.length(); i++){
							//Validacion de solo caracteres
							if((int(s[i]) >= 32 &&  int(s[i]) <=45) || (int(s[i]) >=47 && int(s[i]) <= 64) || (int(s[i]) >= 91 &&  int(s[i]) <= 96) || (int(s[i]) >= 123)) return false;
							else{
								if(int(s[i]) == 46) PuntosCadena ++;
								if(PuntosCadena > 1) return false;
								}
						}
					}
				}
				//La cadena es valida
				return true;
			}
			case 2:{
				
				}
	}
	//La cadena es valida
	return true;
}

int main(){
	
	//Objetos
	ifstream File;
	File.open("error.txt");
	
	
	if(File.fail()){// Posibles errores al abrir el .txt
		cerr << "Error Al abrir el archivo";
		exit(1);
	}
	
	string item;
	datos DatosAux;
	int Linea;
	
	//Leer procesar el archivo
	while(getline (File,item)){
		Linea++;
		//Separar las partes del archivo
		if(item[0] != ';'){
			
			//Inicializamos los campos de DatosAux en NULL
			DatosAux.etiqueta = "";
			DatosAux.codop = "";
			DatosAux.operando = "";
			DatosAux.error = false;
			DatosAux.numeroLinea = Linea;
			
			// Contador para los datos obtenidos
			int DatoIngresado= 0;
			
			//Recorrer todos los caracteres de la linea
			for(int i = 0; i < item.length(); i++){
				
				string valor = "";
				
				//Validamos que no exitan tabuladores o saltos de linea
				while(int(item[i]) != 9 && int(item[i]) != 32 && i < item.length()){
					valor = valor + item[i];
					i++;
				}
				if(valor != ""){
						
					
					if(ValidarValor(valor, DatoIngresado)){
						//Se guardan los datos optenidos en los campos correspondientes
						if(DatoIngresado == 0) DatosAux.etiqueta = valor;
						if(DatoIngresado == 1) DatosAux.codop = valor;
						if(DatoIngresado == 2) DatosAux.operando = valor;
					}else {
						//Marcamos que esta linea de codigo tiene un error
						DatosAux.error = true;
						break;
					}
					
				}
				DatoIngresado++;
			}
			
			
			registro.push_back(DatosAux);
		}
	}//Fin de procesar el archivo
	
	for(int i = 0; i < registro.size(); i++){
		if(registro[i].error == true) cout << "Error linea: " <<registro[i].numeroLinea << endl;
		else cout << "Etiqueta = " << registro[i].etiqueta << "\t codop = " <<  registro[i].codop << "\t operando = " << registro[i].operando << endl;
		
	}
	
	return 1;
}
