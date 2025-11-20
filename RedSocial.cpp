#include "RedSocial.h"
#include <iostream>
using namespace std;

RedSocial::RedSocial() : _amistadestotales(0), _idMasPopular(0) {}

const set<int> & RedSocial::usuarios() const{
        return _usuarios;                               //O(1)
}

string RedSocial::obtener_alias(int id) const{
    auto it = _idalias.find(id);                       //O(log n)
    return it-> second;                                //O(1)
}                          

const set<string> & RedSocial::obtener_amigos(int id) const{
    auto it = _amigos.find(id);                                 //O(log n)                   
    return it->second;                                          //O(1)                     
}

const set<string> & RedSocial::obtener_conocidos(int id) const{
    auto it = _conocidos.find(id);                                     //O(log n)                
    return it->second;                                                 //O(1)  
}

int RedSocial::cantidad_amistades() const{
    return _amistadestotales;                 //O(1)
}

void RedSocial::registrar_usuario(string alias, int id){
    _usuarios.insert(id);                                   //O(log n)
    _idalias[id] = alias;                                   //O(log n)
    _aliasid[alias] = id;                                   //O(log n)
    _amigos [id] = {};                                      //O(log n) 
    _cantamigos [id] = 0;                                   //O(log n)    
    _conocidos [id] = {};                                   //O(log n)
}

void RedSocial::eliminar_usuario(int id){
    string alias = obtener_alias(id);
    // para cada uno si era el "conocido" entre el y otro, y ninguno mas, ya no son conocidos
    for (auto it1 = _amigos[id].begin(); it1 != _amigos[id].end(); ++it1) {
        const string& amigo1 = *it1;
        int idAmigo1 = obtener_id(amigo1);
        for (auto it2 = _amigos[id].begin(); it2 != _amigos[id].end(); ++it2) {
            const string& amigo2 = *it2;
            if (amigo1 == amigo2) continue; 
            int idAmigo2 = obtener_id(amigo2);
            bool otroAmigoEnComun = false;
            const set<string>& amigosDeA1 = _amigos[idAmigo1];
            const set<string>& amigosDeA2 = _amigos[idAmigo2];
            for (auto itAmigoDeA1 = _amigos[idAmigo1].begin(); itAmigoDeA1 != _amigos[idAmigo1].end(); itAmigoDeA1++) {
                const string& posible = *itAmigoDeA1;
                if (_amigos[idAmigo2].find(posible) != _amigos[idAmigo2].end() && posible != obtener_alias(id)) {
                    otroAmigoEnComun = true;
                    break;
                }
            }
            if (!otroAmigoEnComun) {
            _conocidos[idAmigo1].erase(amigo2);
            _conocidos[idAmigo2].erase(amigo1);
            }
        }  
    }  
    //eliminar en las listas de conocidos y de amigos de todo el resto 
    for (auto itAmigos = _amigos[id].begin(); itAmigos != _amigos[id].end(); itAmigos++){
            const string& amigo = *itAmigos; 
            int idAmigo = obtener_id(amigo);
            _amigos[idAmigo].erase(alias);
            _cantamigos[idAmigo]--;
            _amistadestotales--;
        }
    _amigos.erase(id);                                      //O(log n)
    for (auto itConocidos = _conocidos[id].begin(); itConocidos != _conocidos[id].end(); itConocidos++){
            const string& conocido = *itConocidos;
            int idConocido = obtener_id(conocido);
            _conocidos[idConocido].erase(alias);
        }
    _conocidos.erase(id);                                   //O(log n)
    _usuarios.erase(id);                                    //O(log n)
    _idalias.erase(id);                                     //O(log n)
    _aliasid.erase(alias);                                  //O(log n)
    _cantamigos.erase(id);                                  //O(log n)     
    int popular = _idMasPopular;
    for (auto itUsuario = _usuarios.begin(); itUsuario != _usuarios.end(); itUsuario++) {
        int usuario = *itUsuario;
        if (_amigos[usuario].size()>_amigos[popular].size()){
             _idMasPopular = usuario; 
            }
        } 
}

void RedSocial::amigar_usuarios(int id_A, int id_B){          
    string aliasA = obtener_alias(id_A);
    string aliasB = obtener_alias(id_B);
    auto it1 = _amigos.find(id_A); 
    // si no son amigos, se amigan
    if (it1 -> second.find (aliasB) == it1 -> second.end()){ 
        _amigos[id_A].insert(aliasB);
        _cantamigos[id_A]++;
        _amigos[id_B].insert(aliasA);
        _cantamigos[id_B]++;
        // si eran conocidos dejan de serlo
        _conocidos[id_A].erase(aliasB);
        _conocidos[id_B].erase(aliasA);
        // hay una nueva amistad
        _amistadestotales ++;
        // cada uno de sus amigos, si no es ya amigo del otro pasa a ser conocido del otro (y la recíproca)
        for (auto it2 = _amigos[id_A].begin(); it2 != _amigos[id_A].end(); it2++){
            const string& amigoA = *it2;
            if (amigoA == aliasB) continue;
            if (_amigos[id_B].find(amigoA) == _amigos[id_B].end()) {
                _conocidos[id_B].insert(amigoA);
                int idc = obtener_id(amigoA);
                _conocidos[idc].insert(aliasB);
            }
        }
        for (auto it3 = _amigos[id_B].begin(); it3 != _amigos[id_B].end(); it3++){
            const string& amigoB = *it3;
            if (amigoB == aliasA) continue;
            if (_amigos[id_A].find(amigoB) == _amigos[id_A].end()) {
                _conocidos[id_A].insert(amigoB);
                int idk = obtener_id(amigoB);
                _conocidos[idk].insert(aliasA);
            } 
        }
        int popular = _idMasPopular;
        if (_idMasPopular == 0 || _amigos[id_A].size() > _amigos[popular].size()){
            _idMasPopular = id_A;
        }
        if (_amigos[id_B].size() > _amigos[popular].size()){
            _idMasPopular = id_B;
        }
    }
    else{
        return; 
    }
}

void RedSocial::desamigar_usuarios(int id_A, int id_B){
    string aliasA = obtener_alias(id_A);
    string aliasB = obtener_alias(id_B);
    // dejan de ser amigos
    auto it1 = _amigos.find(id_A); 
    it1 -> second.erase(aliasB);
    auto it2 = _cantamigos.find(id_A); 
    it2 -> second --;
    auto it3 = _amigos.find(id_B); 
    it3 -> second.erase(aliasA);
    auto it4 = _cantamigos.find(id_B); 
    it4 -> second --;   
    // hay una amistad menos
    _amistadestotales --;
    // si tienen un amigo en comun no son mas amigos pero son conocidos 
    bool amigoComun = false;
    for (auto it1 = _amigos[id_A].begin(); it1 != _amigos[id_A].end(); it1++){
        const string& amigoA = *it1;
        for (auto it2 = _amigos[id_B].begin(); it2 != _amigos[id_B].end(); it2++){
        const string& amigoB = *it2;
        if (amigoA == amigoB) amigoComun = true;
        }
    }
    if(amigoComun){
        _conocidos[id_A].insert(aliasB);
        _conocidos[id_B].insert(aliasA);
    }
    // para cada uno de sus amigos si la unica persona que tenian en comun dejan de ser conocidos, si tienen otra persona en comun siguen siendo conocidos FALTAAAAAAAAAAAAAAAAAAAAA
    for (auto itAmigosA = _amigos[id_A].begin(); itAmigosA != _amigos[id_A].end(); itAmigosA++) {
    const string& amigoA = *itAmigosA;
    int idAmigoA = obtener_id(amigoA);
    bool sigueConocido = false;
    // busco si ese amigo de A comparte algún amigo con B
    for (auto itAmigosDeAmigo1 = _amigos[idAmigoA].begin(); itAmigosDeAmigo1 != _amigos[idAmigoA].end(); itAmigosDeAmigo1++) {
        const string& posible = *itAmigosDeAmigo1;
        if (_amigos[id_B].find(posible) != _amigos[id_B].end()) {
            sigueConocido = true;
            }
        }
    if (!sigueConocido){
        _conocidos[idAmigoA].erase(aliasB);
        _conocidos[id_B].erase(amigoA);
        }
    }
    for (auto itAmigosB = _amigos[id_B].begin(); itAmigosB != _amigos[id_B].end(); itAmigosB++) {
    const string& amigoB = *itAmigosB;
    int idAmigoB = obtener_id(amigoB);
    bool sigueConocido = false;
    // busco si ese amigo de B comparte algún amigo con A
    for (auto itAmigosDeAmigo2 = _amigos[idAmigoB].begin(); itAmigosDeAmigo2 != _amigos[idAmigoB].end(); itAmigosDeAmigo2++) {
        const string& posible = *itAmigosDeAmigo2;
        if (_amigos[id_A].find(posible) != _amigos[id_A].end()) {
            sigueConocido = true;
            }
        }
    if (!sigueConocido){
        _conocidos[idAmigoB].erase(aliasA);
        _conocidos[id_A].erase(amigoB);
        }
    }   
    int popular = _idMasPopular;
    for (auto itUsuario = _usuarios.begin(); itUsuario != _usuarios.end(); itUsuario++) {
        int usuario = *itUsuario;
        if (_amigos[usuario].size()>_amigos[popular].size()){
            _idMasPopular = usuario;
            }
        }  
}

int RedSocial::obtener_id(string alias) const{                      
    return _aliasid.at(alias);
}

const set<string> & RedSocial::conocidos_del_usuario_mas_popular() const{
    int popular = _idMasPopular;                                                //O(1)
    return _conocidos.at(popular);                                              //O(1)
}
