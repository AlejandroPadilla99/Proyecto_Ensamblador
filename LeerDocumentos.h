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
	vector <operaciones> RegistroDirecciones;
	vector <string> PilaErrores;
	
	//Metodos
	public:
		LeerDocumento(string Direccion);
		void ImprimirIntrucciones();
		void ImprimirTabop();
		void ImprimirCodigos();
		void ImprimirPilaErores();
		void ProcesarDocumento();
		void CargarTabop();
		bool ValidarCodop(datos LineaCodigo);
		int ValidarOperando(string Operando, string Dir);
		bool ValidarSistemaNumerico(string Operando, int n);
		bool ValidarValor(string s, int IntruccionValidar);
};

LeerDocumento::LeerDocumento(string Direccion){
	Archivo.open(Direccion);
	Tabop.open("DocumentosInternos/Tabop.txt");
}
//Permite imprimir
void LeerDocumento::ImprimirIntrucciones(){
	int con = 0;
	for(datos i: registro){
		if(i.error == true){
			cout << "Error linea " << i.numeroLinea << " " << PilaErrores[con] << endl;
			con++;
		} 
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
void LeerDocumento:: ImprimirPilaErores(){
	for(string i: PilaErrores){
		cout << i << endl;
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
						
						else if(DatoIngresado == 1) DatosAux.codop = valor;
						
						else DatosAux.operando = valor;
							/*
							//Si se ha agregado previamente un valor al registro encargado de guardar los datos de los códigos de operación utilizados
							if (registroCodop.size() > 0) {
							
								//Si el código contiene un operando cuando no debe, marca error
								if (registroCodop[registroCodop.size()-1].Operando == "No Operando"){
									PilaErrores.push_back("El codop "+ DatosAux.codop + " no debe contener un operando");
									DatosAux.error = true;
								}
								else DatosAux.operando = valor;
							}
							* */
						
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
			
			DatoIngresado++;
			if(DatoIngresado >= 2 && !DatosAux.error) {
				if (!ValidarCodop(DatosAux)) DatosAux.error = true;
			}
			
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

bool LeerDocumento::ValidarCodop(datos LineaCodigo){
	
	bool Codop_Encontrado = false; //Booleano que verifica que sí se haya encontrado el código de operación
	
							
	//Por cada línea del Tabop
	for (int j = 0; j < RegistroTabop.size(); j++) {
	
		//Si el código de operación es encontrado en la línea actual del Tabop
		if (LineaCodigo.codop == RegistroTabop[j].CodigoDeOperacion) {
			
			string aux = LineaCodigo.codop;
			Codop_Encontrado = true;
			
			//Convierte el código de operación a minúsculas para que coincida con la Tabop
			for (int k = 0; k < aux.length(); k++) {
				
				aux[k] = tolower(aux[k]);
			}
			
			//Inserta cada elemento que coincida con el código de operación en un registro
			do {
				//Validacion del operando
				if(ValidarOperando(LineaCodigo.operando, RegistroTabop[j].ModoDireccionamiento) == -1){
					// Error
					
				}else{
					RegistroDirecciones.push_back(RegistroTabop[j]);
				}
				
				
				
				registroCodop.push_back(RegistroTabop[j]);
				j++;
			} while (aux == RegistroTabop[j].CodigoDeOperacion);
			
			//Si el código no contiene un operando cuando debe, marca error
			if (registroCodop[registroCodop.size()-1].Operando == "Operando" && LineaCodigo.operando == "NULL") {
			
				PilaErrores.push_back("El codop "+ LineaCodigo.codop + " debe contener un operando");
				return false;
			}
			else if (registroCodop[registroCodop.size()-1].Operando == "No Operando" && LineaCodigo.operando != "NULL") {
				
				PilaErrores.push_back("El codop "+ LineaCodigo.codop + " no debe contener un operando");
				return false;
			}
			
			return true;
		}
	}
	
	
	//Si no se encuentra el código de operación en el Tabop, el programa marcará un error
	if (!Codop_Encontrado) {
		PilaErrores.push_back("El codop " + LineaCodigo.codop + " no se reconoce como un codop valido");
		return false;
	}
	return true;
}
int LeerDocumento::ValidarOperando(string Operando, string Dir){
	//cout << Operando <<  " === " << Dir;
	
	int n;
	if(Dir == "Inmediato" && Operando[0] == '#'){
		//Binario
		if(Operando[1] == '%'){
			if(!ValidarSistemaNumerico(Operando.substr(2),1)){
				cout << "Error Sistema Binario" << endl;
			}else{
				if (Operando.length() < 11) {
					
					return 8;
				}
				else if (Operando.length() < 19) {
					
					return 16;
				}
				else return -1;
				
			}
		}
		//Octal
		else if(Operando[1] == '@'){
			if(!ValidarSistemaNumerico(Operando.substr(2),2)){
				cout << "Error Sistema Octal" << endl;
			}else{
				istringstream(Operando.substr(2)) >> oct >> n;
				if(n > -1 || n < 156){
					return 8;
				}else if(n < 65536 ){
					return 16;
				}else{
					cout << "Eror desvordamiento" << endl;
				}
			}
		}
		//Hexa
		else if(Operando[1] == '$'){
			if(!ValidarSistemaNumerico(Operando.substr(2),4)){
				cout << "Error Sistema Hexa" << endl;
			}else{
				istringstream(Operando.substr(2)) >> hex >> n;
				if(n > -1 || n < 156){
					return 8;
				}else if(n < 65536 ){
					return 16;
				}else{
					cout << "Eror desvordamiento" << endl;
				}
				
			}
		}
		else{
			if(!ValidarSistemaNumerico(Operando.substr(1),3)){
				cout << "Error Sistema Decimal" << endl;
			}else{
				istringstream(Operando.substr(1)) >> dec >> n;
				if(n > -1 || n < 156){
					return 8;
				}else if(n < 65536 ){
					return 16;
				}else{
					cout << "Eror desvordamiento" << endl;
				}
				
			}
		}
	} 
	else if(Dir == "Directo" || Dir == "Extendido" ){
		
		//Binario
		if(Operando[0] == '%'){
			if(!ValidarSistemaNumerico(Operando.substr(1),1)){
				cout << "Error Sistema Binario" << endl;
			}else{
				cout << "<3 BIN";
				
			}
		}
		//Octal
		else if(Operando[0] == '@'){
			if(!ValidarSistemaNumerico(Operando.substr(1),2)){
				cout << "Error Sistema Octal" << endl;
			}else{
				istringstream(Operando.substr(1)) >> oct >> n;
				if(n > -1 || n < 156){
					return 8;
				}else{
					cout << "Eror desvordamiento" << endl;
				}
			}
		}
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		//Hexa
		else if(Operando[0] == '$'){
			if(!ValidarSistemaNumerico(Operando.substr(1),4)){
				cout << "Error Sistema Hexa" << endl;
			}else{
				istringstream(Operando.substr(1)) >> hex >> n;
				if(n > -1 || n < 156){
					return 8;
				}else{
					cout << "Eror desvordamiento" << endl;
				}
				
			}
		}
		else{
			if(!ValidarSistemaNumerico(Operando.substr(0),3)){
				cout << "Error Sistema Decimal" << endl;
			}else{
				istringstream(Operando.substr(0)) >> dec >> n;
				if(n > -1 || n < 156){
					return 8;
				}else{
					cout << "Eror desvordamiento" << endl;
				}
				
			}
		}
	}
	else if (Dir == "Extendido") {
		
		//Binario
		if(Operando[0] == '%'){
			if(!ValidarSistemaNumerico(Operando.substr(1),1)){
				cout << "Error Sistema Binario" << endl;
			}else{
				cout << "<3 BIN";
				
			}
		}
		//Octal
		else if(Operando[0] == '@'){
			if(!ValidarSistemaNumerico(Operando.substr(1),2)){
				cout << "Error Sistema Octal" << endl;
			}else{
				istringstream(Operando.substr(1)) >> oct >> n;
				if(n > 255 || n < 65536 ){
					//Extendido////////////////////////////////////////////////
					return 16;
				}else{
					cout << "Eror desvordamiento" << endl;
				}
			}
		}
		//Hexa
		else if(Operando[0] == '$'){
			if(!ValidarSistemaNumerico(Operando.substr(1),4)){
				cout << "Error Sistema Hexa" << endl;
			}else{
				if(n > 255 || n < 65536 ){
					//Extendido////////////////////////////////////////////////
					return 16;
				}else{
					cout << "Eror desvordamiento" << endl;
				}
				
			}
		}
		else{
			if(!ValidarSistemaNumerico(Operando.substr(0),3)){
				cout << "Error Sistema Decimal" << endl;
			}else{
				if(n > 255 || n < 65536 ){
					//Extendido////////////////////////////////////////////////
					return 16;
				}else{
					cout << "Eror desvordamiento" << endl;
				}
				
			}
		}
	}
	else if(Dir == "IDX"){
		//if(Operando[0] == ',' || int(Operando[0]){}
	}
	else if(Dir == "IDX1"){}
	else if(Dir == "IDX2"){}
	else if(Dir == "[IDX2]"){}
	
	
	return -1;
}
bool LeerDocumento::ValidarSistemaNumerico(string operando, int n){
		switch(n){
			//Binario
			case 1:{
				for(char i : operando){
					if(i != '0' && i != '1') return false;
				}
				
				break;
			}
			//Octal
			case 2:{
				for(char i : operando){
					if(int(i) <  48  ||  int(i) > 55) return false;
				}
				
				break;
			}
			//Decimal
			case 3:{
				for(char i : operando){
					if(int(i) < 48 || int(i) > 57) return false;
				}
				
				break;
			}
			//Hexa
			case 4:{
				for(char i : operando){
					if( (int(i) < 48 || int(i) >57) && (int(i) < 65 || int(i) > 70) && (int(i) < 97 || int(i) > 102) ) return false;
				}
				
				break;
			}
				
		}

	return true;
}
bool LeerDocumento::ValidarValor(string s, int IntruccionValidar) {
	
	switch (IntruccionValidar) {

			//En caso de que la cadena sea una etiqueta
			case 0: {

				//Validacion de longitud
				if (s.length() > 8){
					PilaErrores.push_back("La etiqueta " + s + " debe contener 8 o menos caracteres"); 
					return false;
				}

				else {

					//Verifica si el primer caracter no es una letra
					if ((int(s[0]) >= 0 && int(s[0]) <=64) ||(int(s[0]) >= 91 && int(s[0]) <= 96) || (int(s[0]) >= 123)){
						PilaErrores.push_back("EL primer caracter de la etiqueta " + s + " debe ser una letra "); 
						return false;
					} 

					else {

						for (int i = 1; i < s.length(); i++) {

							//Validacion de solo caracteres, números o guiones bajos
							if((int(s[i]) >= 0 && int(s[i]) <= 47) || (int(s[i]) >= 58 && int(s[i]) <= 64) || (int(s[i]) >= 91 && int(s[i]) <= 96 && int(s[i]) != 95) || (int(s[i]) >= 123)){
								PilaErrores.push_back("La etiqueta " + s + " debe contener solo letras, numeros o guiones bajos"); 
								return false;
							}
						}
					}
				}

				//La cadena es valida
				return true;
				
				break;
			}

			//En caso de que la cadena sea un código de operación
			case 1: {

				//Validacion de longitud
				if (s.length() > 5){ 
					PilaErrores.push_back("El codigo de operacion " + s + " debe contener 5 o menos caracteres"); 
					return false;
				}
				
				else {

					//Verifica si el primer caracter no es una letra
					if ((int(s[0]) >= 0 &&  int(s[0]) <=64) || (int(s[0]) >= 91 && int(s[0]) <= 96) || (int(s[0]) >= 123)) {
						PilaErrores.push_back("EL primer caracter del codigo de operacion " + s + " debe ser una letra");
						return false;
					}
					
					else {

						int PuntosCadena = 0; //Variable que verifica si ya existe un punto en la cadena

						//for que recorre toda la cadena
						for (int i = 1; i < s.length(); i++) {
							
							//Validacion de solo caracteres
							if((int(s[i]) >= 0 &&  int(s[i]) <=45) || (int(s[i]) >=47 && int(s[i]) <= 64) || (int(s[i]) >= 91 &&  int(s[i]) <= 96) || (int(s[i]) >= 123)) {
								PilaErrores.push_back("El codigo de operacion " + s + " solo puede contener caracteres o puntos'");
								return false;
							}
							
							else {

								//Verificación de que exista un solo punto en la cadena
								if (int(s[i]) == 46) PuntosCadena ++;
								if (PuntosCadena > 1) {
									PilaErrores.push_back("El codigo de operacion " + s + " no puede contener mas de un punto'");
									return false;
								}
							}
							//cout << endl;
						}
					}
				}
				//La cadena es valida
				return true;
				
				break;
			}
			case 2:{
				//cout << "hola";
				return true;
				break;
			}
			//En caso de que la cadena sea un comentario
			case 3: {

				//Validacion de longitud
				if(s.length() > 80) {
					PilaErrores.push_back("La etiqueta " + s + " debe contener 80 o menos caracteres");  
					return false;
				}
				else return true;
				
				break;
			}
			
	}

	//La cadena es valida
	return true;
}
