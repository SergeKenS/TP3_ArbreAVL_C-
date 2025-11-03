#include <iostream>
#include "arbreavl.h"
#include <chrono>

// Cette fonction vérifie la présence des fonctionnalités demandées au TP3 (de base + nouvelles).
// dans la classe générique ArbreAVL dans arbreavl.h.
// Si ce test compile, c'est bon signe. Mais, ce test n'est pas exhaustif.
// Le bon fonctionnement n'est pas vérifié.
// L'efficacité temporelle n'est pas mesurée.


int main(){
    int nb_erreurs = 0;
    
    // Test affectation
    ArbreAVL<int> arbre1, arbre2;
    for(int i = 0; i < 100; i++) arbre1.inserer(i);
    arbre2 = arbre1;
    if(!(arbre2 == arbre1)) { 
        std::cout << "Erreur: operator=" << std::endl; 
        nb_erreurs++; 
    }
    
    // Test égalité
    ArbreAVL<int> arbre3, arbre4;
    for(int i = 0; i < 100; i++) { 
        arbre3.inserer(i); 
        arbre4.inserer(i); 
    }
    if(!(arbre3 == arbre4)) { 
        std::cout << "Erreur: operator==" << std::endl; 
        nb_erreurs++; 
    }
    
    // Test union sur place (+=)
    ArbreAVL<int> arbre5, arbre6;
    for(int i = 0; i < 50; i++) { 
        arbre5.inserer(i); 
        arbre6.inserer(50 + i); 
    }
    arbre5 += arbre6;
    if(arbre5.taille() != 100) { 
        std::cout << "Erreur: operator+=" << std::endl; 
        nb_erreurs++; 
    }
    
    // Test union (+ crée nouvel arbre)
    ArbreAVL<int> arbre7, arbre8;
    for(int i = 0; i < 50; i++) { 
        arbre7.inserer(i); 
        arbre8.inserer(50 + i); 
    }
    ArbreAVL<int> arbre9 = arbre7 + arbre8;
    if(arbre9.taille() != 100 || arbre7.taille() != 50) { 
        std::cout << "Erreur: operator+" << std::endl; 
        nb_erreurs++; 
    }
    
    // Test différence sur place (-=)
    ArbreAVL<int> arbre10, arbre11;
    for(int i = 0; i < 100; i++) arbre10.inserer(i);
    for(int i = 50; i < 100; i++) arbre11.inserer(i);
    arbre10 -= arbre11;
    if(arbre10.taille() != 50) { 
        std::cout << "Erreur: operator-=" << std::endl; 
        nb_erreurs++; 
    }
    
    // Test différence (- crée nouvel arbre)
    ArbreAVL<int> arbre12, arbre13;
    for(int i = 0; i < 100; i++) arbre12.inserer(i);
    for(int i = 50; i < 100; i++) arbre13.inserer(i);
    ArbreAVL<int> arbre14 = arbre12 - arbre13;
    if(arbre14.taille() != 50 || arbre12.taille() != 100) { 
        std::cout << "Erreur: operator-" << std::endl; 
        nb_erreurs++; 
    }
    
    // Tests de performance
    std::cout << "Performance:" << std::endl;
    
    // Insertion massive
    auto debut = std::chrono::steady_clock::now();
    ArbreAVL<int> arbrePerf;
    for(int i = 0; i < 100000; i++) arbrePerf.inserer(i);
    auto fin = std::chrono::steady_clock::now();
    std::cout << "  100K insertions: " << std::chrono::duration<double>(fin - debut).count() << "s" << std::endl;
    
    // Union performance
    debut = std::chrono::steady_clock::now();
    ArbreAVL<int> arbreUnion1, arbreUnion2;
    for(int i = 0; i < 50000; i++) {
        arbreUnion1.inserer(i);
        arbreUnion2.inserer(50000 + i);
    }
    arbreUnion1 += arbreUnion2;
    fin = std::chrono::steady_clock::now();
    std::cout << "  Union 100K elements: " << std::chrono::duration<double>(fin - debut).count() << "s" << std::endl;
    
    std::cout << "\nRésultat: " << nb_erreurs << " erreur(s)" << std::endl;
    return nb_erreurs;
}