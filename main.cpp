/*
	Descripcion: Programa diseñado para leer un archivo .txt, separar los direfentes
	elementos del mismo y ordenarlos de manera que cada línea sea una instrucción en
	código ensamblador.
*/

/*Inclusión de librerías*/
#include<bits/stdc++.h>
#include<fstream>
#include<sstream>

//Clases locales
#include "LeerDocumentos.h"


using namespace std;

/*Método principal main*/
int main() {
	
	LeerDocumento Archivo("DocumentosInternos/P1ASM.txt");
	
	Archivo.CargarTabop();
	//Archivo.ImprimirTabop();
	
	
	Archivo.ProcesarDocumento();
	Archivo.ImprimirIntrucciones();
	
	//Archivo.ImprimirCodigos();
	
	return 0;
}
