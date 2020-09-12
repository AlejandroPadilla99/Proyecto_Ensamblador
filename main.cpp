#include<bits/stdc++.h>
#include<fstream>
using namespace std;
struct datos{
	string etiqueta;
	string codop;
	string operando;
};


//Variables globales
vector <datos> registro;

int main(){
	
	//Objetos
	ifstream File;
	File.open("P1ASM.txt");
	
	//ver que existan errores
	if(File.fail()){
		cerr << "Error Al abrir el archivo";
		exit(1);
	}
	string item;
	datos aux;
	//Leer el archivo
	while(getline (File,item)){
		
		//Separar las partes del archivo
		if(item[0] != ';'){
			//variables
			aux.etiqueta = "";
			aux.codop = "";
			aux.operando = "";
			// aux y contadores
			int con_variable = 0;
			
			//recorrer todos los caracteres de la linea
			for(int i = 0; i < item.length(); i++){
				
				string valor = "";
					
				while(int(item[i]) != 9 && int(item[i]) != 32 && i < item.length()){
					valor = valor + item[i];
					//cout << valor << endl;
					i++;
				}
				if(valor != ""){
					if(con_variable == 0) aux.etiqueta = valor;
					if(con_variable == 1) aux.codop = valor;
					if(con_variable == 2) aux.operando = valor;
				}
				con_variable++;
			}
			
			//Estructura llena
			registro.push_back(aux);
		}
	}
	
	for(int i = 0; i < registro.size(); i++){
		cout << "Etiqueta = " << registro[i].etiqueta << "\t codop = " <<  registro[i].codop << "\t operando = " << registro[i].operando << endl;
	}
	
	return 1;
}
