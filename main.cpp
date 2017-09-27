/* 
 * File:   main.cpp
 * Author: edward
 *
 * Created on 27 de septiembre de 2010, 09:39 AM
 */

#include <cstdlib>
#include <iostream>
#include <time.h>
//#include "unistd.h"

using namespace std;

#define COLUMNAS 9
#define FILAS 9
#define SIN_BOMBA -2
#define BOMBA -1
#define SIN_JUGAR -3

#define ESTADO_PERDEDOR 0
#define ESTADO_GANADOR  1
#define ESTADO_EN_JUEGO 2

int campo[FILAS][COLUMNAS];
bool jugadas[FILAS][COLUMNAS];
//Casilla jugada

int njugadas;
int estado;

float version = 0.1;
int build = 4;

/*
 * 
 */
void iniciaArr(){
    int i,j;
    for( i = 0; i < FILAS; i++){
        for(j = 0; j < COLUMNAS; j++){
            campo[i][j] = SIN_BOMBA;
            jugadas[i][j] = false; //No se ha jugado en casilla
        }
    }
}

void agregaBombas(int nbombas){
    int i,x,y;// contador de bombas
    i = 1;
    while( i <= nbombas){
        x = rand()%FILAS;
        y = rand()%COLUMNAS;
        campo[x][y] = BOMBA;
        i++;
    }
}

void print(){
    int i,j;
    cout << "[x] 0  1  2  3  4  5  6  7  8" << endl;
    cout << "------------------------------" << endl;
    for( i = 0; i < FILAS; i++){
        cout << "[" << i << "]";
        for(j = 0; j < COLUMNAS; j++){
            if( jugadas[i][j] ){
                if( campo[i][j] == BOMBA){
                    cout << " * ";
                }else{
                    cout << " " << campo[i][j] << " ";
                }
            }else{
                cout << " - ";
            }
        }
        cout << endl;
    }
}

int contar2(int x, int y){
    int nbombas = 0;
    for(int i = x-1; i < x+2; i++){
        for(int j = y-1; j < y+2; j++){
            if( i >= 0 && i < FILAS && j >= 0 && j < COLUMNAS ){
                if( campo[i][j] == BOMBA){ //Existe bomba
                    nbombas++;
                }
            }
        }
    }
    return nbombas;
}

void establecerNumeros(){
    int i,j, numero;
    for( i = 0; i < FILAS; i++){
        for(j = 0; j < COLUMNAS; j++){
            if( campo[i][j] == SIN_BOMBA){ //no tiene bomba
                numero = contar2(i,j);
                campo[i][j] = numero;
            }
        }
    }
}

void printAll(){
    for(int i = 0; i < FILAS; i++){
        for(int j = 0; j < COLUMNAS; j++){
            if( campo[i][j] == BOMBA){
                cout << " * ";
            }else{
                cout << " " << campo[i][j] << " ";
            }
        }
        cout << endl;
    }
}

void jugada(int x, int y){
    if( x >= 0 && x < FILAS && y >= 0 && y < COLUMNAS ){
        jugadas[x][y] = true;
        if( campo[x][y] == BOMBA){
            estado = ESTADO_PERDEDOR;
        }
    }else{
        cout << "Imposible jugada" << endl;
        system("pause");
    }
}

int contarJugadas(){
    int numero = 0;
    for(int i = 0; i < FILAS; i++){
        for(int j = 0; j < COLUMNAS; j++){
            if ( jugadas[i][j]){
                numero++;
            }
        }
    }
    return numero;
}

void jugadaEnGrupo(int x, int y){
    if( x >= 0 && x < FILAS && y >= 0 && y < COLUMNAS ){
        jugadas[x][y] = true;
        if( campo[x][y] == BOMBA){
            estado = ESTADO_PERDEDOR;
        }else if( campo[x][y] == 0){//Cero bombas alrededor
            for(int i = x-1; i < x+2; i++){
                for(int j = y-1; j < y+2; j++){
                    if( i >= 0 && i < FILAS && j >= 0 && j < COLUMNAS ){
                        if( campo[i][j] == 0 && !jugadas[i][j]){ //Existe bomba
                            jugadaEnGrupo(i,j);
                        }else{
                            jugadas[i][j] = true;
                        }
                    }
                }
            }
        }
    }
}

int getMenu(){
    cout << "1. Jugar Buscaminas" << endl;
    cout << "2. Sobre el autor" << endl;
    cout << "3. Salir" << endl;
    cout << "Opcion: ";
    int opt;
    cin >> opt;
    return opt;
}

void aboutMe(){
    cout << "By Edward -> edward1738atgmail.com" << endl;
    cout << "Version:" << version << " Build:" << build << endl;
}

void iniciarJuego(){
    int nbombas = -1;
    while( nbombas < 0 || nbombas > 70 ){
        cout << "Numero de bombas: ";
        cin >> nbombas;
    }

    int x,y;
    estado = ESTADO_EN_JUEGO;
    njugadas = 0;
    iniciaArr();

    //PID es un identificador de procesos, cada vez
    // que un nuevo proceso se ejectua se le asigna un PID
    // diferente - Se necesita libreria - unistd.h
    //srand( getpid() );
    srand ( time(NULL) );
    agregaBombas( nbombas );
    establecerNumeros();

    while( estado == ESTADO_EN_JUEGO){ //estado en juego
        aboutMe();
        print();
        cout << "Fila: ";
        cin >> x;
        cout << "Columna: ";
        cin >> y;
        jugadaEnGrupo(x,y);
        system("cls");
        njugadas = contarJugadas();
        if( njugadas == ( FILAS*COLUMNAS - nbombas) ){
            estado = ESTADO_GANADOR;
        }
    }

    if( estado == ESTADO_PERDEDOR){
        print();
        cout << "perdiste" << endl;
        cout << "Solucion: " << endl;
        printAll();
    }else{
        cout << "Ganaste !" << endl;
        printAll();
    }
}

int main(int argc, char** argv) {
    int opt = 0;
    do{
        opt = getMenu();
        switch(opt){
            case 1: iniciarJuego(); break;
            case 2: aboutMe(); break;
        }
        system("pause");
        system("cls");
    }while( opt != 3);
    return 0;
}

