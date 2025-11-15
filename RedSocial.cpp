#include "RedSocial.h"
#include <iostream>
using namespace std;

RedSocial::RedSocial() : _amistadestotales(0) {}

const set<int> & RedSocial::usuarios() const{
        return _usuarios;                                           //O(1)
}

string RedSocial::obtener_alias(int id) const{
    auto it = _idalias.find(id);                                      //O(log n)
    return it-> second;                                              //O(1)
}                          

const set<string> & RedSocial::obtener_amigos(int id) const{
    auto it = _amigos.find(id);                                        //O(log n)                                        //O(1)
    return it->second;                                                 //O(1)                                                     //O(1)
}

const set<string> & RedSocial::obtener_conocidos(int id) const{
    auto it = _conocidos.find(id);                                        //O(log n)                                        //O(1)
    return it->second;                                                 //O(1)  
}

int RedSocial::cantidad_amistades() const{
    return _amistadestotales;                   //O(1)
}

void RedSocial::registrar_usuario(string alias, int id){
    _usuarios.insert(id);                                   //O(log n)
    _idalias[id] = alias;                                   //O(log n)
    _aliasid[alias] = id;                                   //O(log n)
    _amigos [id] = {};                                   //O(log n) 
    _cantamigos [id] = 0;                                   //O(log n)    
    _conocidos [id] = {};                                   //O(1)
}

void RedSocial::eliminar_usuario(int id){
    string alias = obtener_alias(id);
    _usuarios.erase(id);                                    //O(log n)
    _idalias.erase(id);                                         //O(log n)
    _aliasid.erase(alias); 
    _amigos.erase(id);                                      //O(log n)
    _conocidos.erase(id);
    _cantamigos.erase(id);
    //eliminar en las listas de conocidos y de amigos de todo el resto
}

void RedSocial::amigar_usuarios(int id_A, int id_B){            //ESTA RARO EL ENUNCIADO???
    
    string aliasA = obtener_alias(id_A);
    string aliasB = obtener_alias(id_B);
    
    auto it1 = _amigos.find(id_A); 
    if (it1 -> second.find (aliasB) == it1 -> second.end()){ 

        _amigos[id_A].insert(aliasB);
        _cantamigos[id_A]++;
        _amigos[id_B].insert(aliasA);
        _cantamigos[id_B]++;

        _conocidos[id_A].erase(aliasB);
        _conocidos[id_B].erase(aliasA);

        _amistadestotales ++;

        for (auto it2 = _amigos[id_A].begin(); it2 != _amigos[id_A].end(); it2++){
            const string& amigoA = *it2;
            if (amigoA == aliasB) continue;
            if (_amigos[id_B].find(amigoA) == _amigos[id_B].end()) {
                _conocidos[id_B].insert(amigoA);
            }
            int idc = obtener_id(amigoA);
            if (_amigos[idc].find(aliasB) == _amigos[idc].end()) {
                _conocidos[idc].insert(aliasB);
            }
        }

        for (auto it3 = _amigos[id_B].begin(); it3 != _amigos[id_B].end(); it3++){
            const string& amigoB = *it3;
            if (amigoB == aliasA) continue;
            if (_amigos[id_A].find(amigoB) == _amigos[id_A].end()) {
                _conocidos[id_A].insert(amigoB);
            }
            int idk = obtener_id(amigoB);
            if (_amigos[idk].find(aliasA) == _amigos[idk].end()) {
                _conocidos[idk].insert(aliasA);
            }
        }
    }
    else{
        return; 
    }
}


void RedSocial::desamigar_usuarios(int id_A, int id_B){
    string aliasA = obtener_alias(id_A);
    string aliasB = obtener_alias(id_B);
    auto it1 = _amigos.find(id_A); 
    it1 -> second.erase(aliasB);
    auto it2 = _cantamigos.find(id_A); 
    it2 -> second --;
    auto it3 = _amigos.find(id_B); 
    it3 -> second.erase(aliasA);
    auto it4 = _cantamigos.find(id_B); 
    it4 -> second --;   
    _amistadestotales --;
}

int RedSocial::obtener_id(string alias) const{                      //ES LEGAL UNORDERED MAP????
    return _aliasid.at(alias);
}

const set<string> & RedSocial::conocidos_del_usuario_mas_popular() const{
    static const set<string> vacio;
    return vacio;
}

