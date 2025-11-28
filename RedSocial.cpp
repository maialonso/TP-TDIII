#include "RedSocial.h"
#include <iostream>
using namespace std;

RedSocial::RedSocial() : _amistadestotales(0), _idMasPopular(0) {}      // O(1)


// Complejidad: O(1).
const set<int> & RedSocial::usuarios() const{
    return _usuarios;                               // O(1)
}


// Complejidad: O(log n), donde n es la cantidad total de usuarios.
string RedSocial::obtener_alias(int id) const{
    auto it = _idalias.find(id);                    // O(log n)
    return it->second;                              // O(1)
}                          


// Complejidad: O(log n), donde n es la cantidad total de usuarios.
const set<string> & RedSocial::obtener_amigos(int id) const{
    auto it = _amigos.find(id);                     // O(log n)                   
    return it->second;                              // O(1)                     
}


// Complejidad: O(log n) promedio, donde n es la cantidad total de usuarios.
const set<string> & RedSocial::obtener_conocidos(int id) const{
    auto it = _conocidos.find(id);                  // O(log n)             
    return it->second;                              // O(1)  
}


// Complejidad: O(1).
int RedSocial::cantidad_amistades() const{
    return _amistadestotales;                       // O(1)
}


// Complejidad: O(log n) + O(1) promedio, donde n es la cantidad total de usuarios.
void RedSocial::registrar_usuario(string alias, int id){
    
    _usuarios.insert(id);                           // O(log n)
    _idalias[id] = alias;                           // O(log n)
    _aliasid[alias] = id;                           // O(1) promedio
    _amigos[id] = {};                               // O(log n)
    _cantamigos[id] = 0;                            // O(log n)    
    _conocidos[id] = {};                            // O(log n)
}


// Complejidad sin requerimiento: O(d^2 · c · log n + (d + f + n) · log n), donde:
// n = cantidad total de usuarios,
// d = cantidad de amigos del usuario id (|_amigos[id]|),
// c = máxima cantidad de amigos de un amigo de id,
// f = cantidad de conocidos del usuario id (|_conocidos[id]|),
// g = cantidad de conocidos del usuario más popular (|_conocidos_mas_popular|).
void RedSocial::eliminar_usuario(int id){       

    string alias = obtener_alias(id);                                                                                        // O(log n)
    
    // para cada uno, si era el "conocido" entre él y otro, y ninguno más, ya no son conocidos
    for (auto it1 = _amigos[id].begin(); it1 != _amigos[id].end(); ++it1) {                                                 // O(d)
        const string& amigo1 = *it1;                                                                                        // O(1)
        int idAmigo1 = obtener_id(amigo1);                                                                                  // O(1) promedio            
        
        for (auto it2 = _amigos[id].begin(); it2 != _amigos[id].end(); ++it2) {                                             // O(d)
            const string& amigo2 = *it2;                                                                                    // O(1)
            
            if (amigo1 == amigo2) continue;                                                                                 // O(1)
            int idAmigo2 = obtener_id(amigo2);                                                                              // O(1) promedio
            bool otroAmigoEnComun = false;                                                                                  // O(1)
            const set<string>& amigosDeA1 = _amigos[idAmigo1];                                                              // O(log n)
            const set<string>& amigosDeA2 = _amigos[idAmigo2];                                                              // O(log n)
            
            for (auto itAmigoDeA1 = _amigos[idAmigo1].begin();
                 itAmigoDeA1 != _amigos[idAmigo1].end(); itAmigoDeA1++) {                                                   // O(c)
                const string& posible = *itAmigoDeA1;                                                                       // O(1)
                if (_amigos[idAmigo2].find(posible) != _amigos[idAmigo2].end() &&                                           // O(log n)
                    posible != obtener_alias(id)) {                                                                         // O(log n)
                    otroAmigoEnComun = true;                                                                                // O(1)        
                    break;                                                                                                  // O(1)
                }
            }
            
            if (!otroAmigoEnComun) {                                                                                        // O(1)
                _conocidos[idAmigo1].erase(amigo2);                                                                         // O(log n)
                _conocidos[idAmigo2].erase(amigo1);                                                                         // O(log n)
            }
        }                                                                                                                   // doble for: O(d^2 · (c · log n))
    }  
    
    // eliminar en las listas de amigos y contadores de todo el resto 
    for (auto itAmigos = _amigos[id].begin(); itAmigos != _amigos[id].end(); itAmigos++){                                   // O(d)
        const string& amigo = *itAmigos;                                                                                    // O(1)
        int idAmigo = obtener_id(amigo);                                                                                    // O(1) promedio
        _amigos[idAmigo].erase(alias);                                                                                      // O(log n)
        _cantamigos[idAmigo]--;                                                                                             // O(log n)
        _amistadestotales--;                                                                                                // O(1)
    }
    _amigos.erase(id);                                                                                                      // O(log n)
    
    // eliminar en las listas de conocidos de todo el resto
    for (auto itConocidos = _conocidos[id].begin();
         itConocidos != _conocidos[id].end(); itConocidos++){                                                               // O(f)
        const string& conocido = *itConocidos;                                                                              // O(1)
        int idConocido = obtener_id(conocido);                                                                              // O(1) promedio        
        _conocidos[idConocido].erase(alias);                                                                                // O(log n)
    }
    
    _conocidos.erase(id);                                                                                                   // O(log n)
    _usuarios.erase(id);                                                                                                    // O(log n)
    _idalias.erase(id);                                                                                                     // O(log n)
    _aliasid.erase(alias);                                                                                                  // O(log n)
    _cantamigos.erase(id);                                                                                                  // O(log n)     
    
    if (_usuarios.empty()) {                                                                                                // O(1)
        _idMasPopular = 0;                                                                                                  // O(1)
        _conocidos_mas_popular.clear();                                                                                     // O(g)

    } else {
        int popular = *_usuarios.begin();                                                                                   // O(1)
        for (auto itUsuario = _usuarios.begin(); itUsuario != _usuarios.end(); ++itUsuario) {                               // O(n)
            int usuario = *itUsuario;                                                                                       // O(1)
            if (_amigos[usuario].size() > _amigos[popular].size()) {                                                        // O(n)
                popular = usuario;                                                                                          // O(1)
            }
        }

        _idMasPopular = popular;                                                                                            // O(1)
        _conocidos_mas_popular = _conocidos.at(_idMasPopular);                                                              // O(log n)
    }                                                                 
}


// Complejidad sin requerimiento: O((a + b) · log n), donde:
    // n = cantidad total de usuarios,
    // a = cantidad de amigos de id_A,
    // b = cantidad de amigos de id_B.
void RedSocial::amigar_usuarios(int id_A, int id_B){   

    string aliasA = obtener_alias(id_A);                                                    // O(log n)
    string aliasB = obtener_alias(id_B);                                                    // O(log n)

    auto it1 = _amigos.find(id_A);                                                          // O(log n)    
    
    // si no son amigos, se amigan                                  
    if (it1->second.find(aliasB) == it1->second.end()){                                     // O(log a)

        _amigos[id_A].insert(aliasB);                                                       // O(log n) + O(log a)
        _cantamigos[id_A]++;                                                                // O(log n)

        _amigos[id_B].insert(aliasA);                                                       // O(log n) + O(log b)
        _cantamigos[id_B]++;                                                                // O(log n)    

        // si eran conocidos dejan de serlo
        _conocidos[id_A].erase(aliasB);                                                     // O(log n) 
        _conocidos[id_B].erase(aliasA);                                                     // O(log n)        

        // hay una nueva amistad
        _amistadestotales++;                                                                // O(1)

        // cada uno de sus amigos, si no es ya amigo del otro pasa a ser conocido del otro (y la recíproca)
        for (auto it2 = _amigos[id_A].begin(); it2 != _amigos[id_A].end(); it2++){          // O(a)
            const string& amigoA = *it2;                                                    // O(1)
            if (amigoA == aliasB) continue;                                                 // O(1)
            if (_amigos[id_B].find(amigoA) == _amigos[id_B].end()) {                        // O(log n) + O(log b)
                _conocidos[id_B].insert(amigoA);                                            // O(log n)
                int idc = obtener_id(amigoA);                                               // O(1) promedio
                _conocidos[idc].insert(aliasB);                                             // O(log n)
            }
        }

        for (auto it3 = _amigos[id_B].begin(); it3 != _amigos[id_B].end(); it3++){          // O(b)
            const string& amigoB = *it3;                                                    // O(1)
            if (amigoB == aliasA) continue;                                                 // O(1)    
            if (_amigos[id_A].find(amigoB) == _amigos[id_A].end()) {                        // O(log n) + O(log a)
                _conocidos[id_A].insert(amigoB);                                            // O(log n)
                int idk = obtener_id(amigoB);                                               // O(1) promedio
                _conocidos[idk].insert(aliasA);                                             // O(log n)
            } 
        }

        int popular = _idMasPopular;                                                        // O(1)
        if (_idMasPopular == 0 || _amigos[id_A].size() > _amigos[popular].size()){          // O(log n) + O(1)
            _idMasPopular = id_A;                                                           // O(1)
        }   
        if (_amigos[id_B].size() > _amigos[popular].size()){                                // O(log n) + O(1)
            _idMasPopular = id_B;                                                           // O(1)
        }
        _conocidos_mas_popular = _conocidos.at(_idMasPopular);                              // O(log n)
    }
    else{
        return;                                                                             // O(1)
    }
}


// Complejidad sin requerimiento: O(a·b + a·c·log b + b·d·log a + n·log n), donde:
    // a = cantidad de amigos de id_A,
    // b = cantidad de amigos de id_B,
    // c = máximo número de amigos de un amigo de A,
    // d = máximo número de amigos de un amigo de B,
    // n = cantidad total de usuarios.
void RedSocial::desamigar_usuarios(int id_A, int id_B){

    string aliasA = obtener_alias(id_A);                                                    // O(log n)
    string aliasB = obtener_alias(id_B);                                                    // O(log n)

    // dejan de ser amigos
    auto it1 = _amigos.find(id_A);                                                          // O(log n)
    it1->second.erase(aliasB);                                                              // O(log a)
    auto it2 = _cantamigos.find(id_A);                                                      // O(log n)
    it2->second--;                                                                          // O(1)
    auto it3 = _amigos.find(id_B);                                                          // O(log n)
    it3->second.erase(aliasA);                                                              // O(log b)
    auto it4 = _cantamigos.find(id_B);                                                      // O(log n)
    it4->second--;                                                                          // O(1)

    _amistadestotales--;                                                                    // O(1)

    bool amigoComun = false;                                                                // O(1)

    // chequeo si tienen un amigo en común
    for (auto it1 = _amigos[id_A].begin(); it1 != _amigos[id_A].end(); it1++){              // O(a)
        const string& amigoA = *it1;                                                        // O(1)
        for (auto it2 = _amigos[id_B].begin(); it2 != _amigos[id_B].end(); it2++){          // O(b)
            const string& amigoB = *it2;                                                    // O(1)
            if (amigoA == amigoB) amigoComun = true;                                        // O(1)
        }
    }

    if(amigoComun){                                                                         // O(1)
        _conocidos[id_A].insert(aliasB);                                                    // O(log n)
        _conocidos[id_B].insert(aliasA);                                                    // O(log n)
    }

    // actualizar conocidos de amigos de A
    for (auto itAmigosA = _amigos[id_A].begin();  itAmigosA != _amigos[id_A].end(); itAmigosA++) {   // O(a)
        
        const string& amigoA = *itAmigosA;                                                  // O(1)
        int idAmigoA = obtener_id(amigoA);                                                  // O(1) promedio
        bool sigueConocido = false;                                                         // O(1)

        // amigos del amigo de A (máximo c)
        for (auto itAmigosDeAmigo1 = _amigos[idAmigoA].begin();
             itAmigosDeAmigo1 != _amigos[idAmigoA].end(); itAmigosDeAmigo1++) {             // O(c)

            const string& posible = *itAmigosDeAmigo1;                                      // O(1)

            if (_amigos[id_B].find(posible) != _amigos[id_B].end()) {                       // O(log b)
                sigueConocido = true;                                                       // O(1)
            }
        }

        if (!sigueConocido){                                                                // O(1)
            _conocidos[idAmigoA].erase(aliasB);                                             // O(log a)
            _conocidos[id_B].erase(amigoA);                                                 // O(log b)
        }
    }

    // actualizar conocidos de amigos de B
    for (auto itAmigosB = _amigos[id_B].begin(); itAmigosB != _amigos[id_B].end(); itAmigosB++) {   // O(b)
        
        const string& amigoB = *itAmigosB;                                                  // O(1)
        int idAmigoB = obtener_id(amigoB);                                                  // O(1) promedio
        bool sigueConocido = false;                                                         // O(1)

        // amigos del amigo de B (máximo d)
        for (auto itAmigosDeAmigo2 = _amigos[idAmigoB].begin();
             itAmigosDeAmigo2 != _amigos[idAmigoB].end(); itAmigosDeAmigo2++) {             // O(d)

            const string& posible = *itAmigosDeAmigo2;                                      // O(1)

            if (_amigos[id_A].find(posible) != _amigos[id_A].end()) {                       // O(log a)
                sigueConocido = true;                                                       // O(1)
            }
        }

        if (!sigueConocido){                                                                // O(1)
            _conocidos[idAmigoB].erase(aliasA);                                             // O(log b)
            _conocidos[id_A].erase(amigoB);                                                 // O(log a)
        }
    }   

    // recalculo usuario más popular
    int popular = _idMasPopular;                                                            // O(1)
    for (auto itUsuario = _usuarios.begin(); itUsuario != _usuarios.end(); itUsuario++) {   // O(n)
        int usuario = *itUsuario;                                                           // O(1)
        if (_amigos[usuario].size() > _amigos[popular].size()){                             // O(log n)
            _idMasPopular = usuario;                                                        // O(1)
        }
    }
    _conocidos_mas_popular = _conocidos.at(_idMasPopular);                                 // O(log n)
}


// Complejidad: O(1) promedio.
int RedSocial::obtener_id(string alias) const{                      
    return _aliasid.at(alias);                                      // O(1) promedio
}


// Complejidad: O(1).
const set<string> & RedSocial::conocidos_del_usuario_mas_popular() const{
    return _conocidos_mas_popular;                                              // O(1)
}
