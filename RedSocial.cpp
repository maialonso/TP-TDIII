#include "RedSocial.h"
#include <iostream>
using namespace std;

RedSocial::RedSocial(){
}

const set<int> & RedSocial::usuarios() const{
    // borrar todo esto y escribir algo correcto
    // este set "static" debe desaparecer y tiene que devolverse
    // algo de la estructura de representación de la clase
    static set<int> basura = {314};
    return basura;
}
string RedSocial::obtener_alias(int id) const{
    // borrar todo esto y escribir algo correcto
    return "basura";
}
const set<string> & RedSocial::obtener_amigos(int id) const{
    // borrar todo esto y escribir algo correcto
    // este set "static" debe desaparecer y tiene que devolverse
    // algo de la estructura de representación de la clase
    static set<string> basura = {"basura"};
    return basura;
}

const set<string> & RedSocial::obtener_conocidos(int id) const{
    // borrar todo esto y escribir algo correcto
    // este set "static" debe desaparecer y tiene que devolverse
    // algo de la estructura de representación de la clase
    static set<string> basura = {"basura"};
    return basura;
}
int RedSocial::cantidad_amistades() const{
    // Borrar esto y escribir algo correcto
    return -1;
}

void RedSocial::registrar_usuario(string alias, int id){
    // Completar
}

void RedSocial::eliminar_usuario(int id){
    // Completar
}

void RedSocial::amigar_usuarios(int id_A, int id_B){
    // Completar
}

void RedSocial::desamigar_usuarios(int id_A, int id_B){
    // Completar
}

int RedSocial::obtener_id(string alias) const{
    // Borrar esto y escribir algo correcto
    return -1;
}
const set<string> & RedSocial::conocidos_del_usuario_mas_popular() const{
    // este set "static" debe desaparecer y tiene que devolverse
    // algo de la estructura de representación de la clase
	static set<string> basura = {"basura"};
    return basura;
}

