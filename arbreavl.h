/* Participant 1 : Kenfack Guessopn Junior Serges        KENJ22299603
 * Participant 2 : Celestine Djeukam Tchouyomgou         DJEC12328708
 * TP3 -- Arbres AVL
 * UQAM / Département d'informatique
 * INF3105 - Structures de données et algorithmes
 * http://ericbeaudry.uqam.ca/INF3105/tp3/
 *
 * Recommandation:
 *  - Réutilisez votre solution des Lab6 et Lab7.
 *  - Ajoutez-y les fonctions demandées ci-dessous.
 *
 */
#if !defined(_ARBREAVL_INF3105_H_)
#define _ARBREAVL_INF3105_H_

#include <algorithm>

template <class T>
class ArbreAVL {
  public:
// Constructeurs
    ArbreAVL();
    ArbreAVL(const ArbreAVL&);
    ~ArbreAVL();

    // Lab6 / Tâche 2 - Fonctions de base
    bool vide() const;
    bool contient(const T&) const;
    void inserer(const T&);
    void vider();

    // Nouvelles fonctionnalités du TP3:
    int             taille() const; // retourne le nombre d'éléments
    
    ArbreAVL<T>&    operator = (const ArbreAVL<T>& autre);

    bool            operator == (const ArbreAVL<T>& autre) const;
    
    ArbreAVL<T>&    operator += (const ArbreAVL<T>& autre); // ajoute tous les éléments dans autre
    ArbreAVL<T>     operator +  (const ArbreAVL<T>& autre) const; // retourne l'union

    ArbreAVL<T>&    operator -= (const ArbreAVL<T>& autre); // retire tous les éléments dans autre
    ArbreAVL<T>     operator -  (const ArbreAVL<T>& autre) const; // retourne la différence
    

  private:
    struct Noeud {
        T element;
        int hauteur;
        Noeud* gauche;
        Noeud* droite;
    
        Noeud(const T& e) : element(e), hauteur(1), gauche(nullptr), droite(nullptr) {}
    };
    
    Noeud* racine;
    int nbElements;
    
    // Fonctions auxiliaires privées
    int hauteur(Noeud* n) const;
    int balance(Noeud* n) const;
    Noeud* rotationGauche(Noeud* y);
    Noeud* rotationDroite(Noeud* x);
    Noeud* insererAux(Noeud* n, const T& e);
    Noeud* copier(Noeud* n) const;
    void detruire(Noeud* n);
    bool contientAux(Noeud* n, const T& e) const;
    bool egalAux(Noeud* n1, Noeud* n2) const;
    void parcourirIns(Noeud* n, ArbreAVL<T>& result) const;
    void parcourirRet(Noeud* n1, Noeud* n2, ArbreAVL<T>& result) const;
};


// Constructeur par défaut : initialise un arbre vide
template <class T>
ArbreAVL<T>::ArbreAVL() 
{
    racine = nullptr;
    nbElements = 0;
}

template <class T>
// Constructeur par copie, cre une copie profonde de l'arbre
ArbreAVL<T>::ArbreAVL(const ArbreAVL<T>& autre)
{
    racine = copier(autre.racine);
    nbElements = autre.nbElements;
}

template <class T>
// Destructeur, libere toute la memoire de l'arbre
ArbreAVL<T>::~ArbreAVL()
{
    detruire(racine);
}

template <class T>
// Verifie si un element est present dans larbre
bool ArbreAVL<T>::contient(const T& element) const
{
    return contientAux(racine, element);
}

template <class T>
// Insere un element dans l'arbre
void ArbreAVL<T>::inserer(const T& element)
{
    racine = insererAux(racine, element);
}

template <class T>
// Verifie si l'arbre est vide
bool ArbreAVL<T>::vide() const
{
    return racine == nullptr;
}

template <class T>
// Vide l'arbre et libere toute la memoire
void ArbreAVL<T>::vider(){
    detruire(racine);
    racine = nullptr;
    nbElements = 0;
}

template <class T>
// Retourne le nombre d'elements dans l'arbre 
int  ArbreAVL<T>::taille() const{
    return nbElements;
}

template <class T>
// Operateur d'assignatio, copie profonde avec gestion auto assignation
ArbreAVL<T>& ArbreAVL<T>::operator=(const ArbreAVL& autre) {
    if (this != &autre) {
        detruire(racine);
        racine = copier(autre.racine);
        nbElements = autre.nbElements;
    }
    return *this;
}

template <class T>
// Operateur d'egalite, compare le contenu des deux arbre
bool ArbreAVL<T>::operator == (const ArbreAVL<T>& autre) const{
    if (nbElements != autre.nbElements) return false;
    return egalAux(racine, autre.racine);
}
    
template <class T>
// Operateur +=, ajoute tous les elements de l'autre arbre
ArbreAVL<T>& ArbreAVL<T>::operator += (const ArbreAVL<T>& autre){
    parcourirIns(autre.racine, *this);
    return *this;
}

template <class T>
// Operateur +, retourne l'union de deux arbres en crayant une copie
ArbreAVL<T>  ArbreAVL<T>::operator +  (const ArbreAVL<T>& autre) const{
    ArbreAVL<T> result = *this;
    result += autre;
    return result;
}
template <class T>
// Opérateur -=, retire tous les elements de lautre arbre
ArbreAVL<T>& ArbreAVL<T>::operator -= (const ArbreAVL<T>& autre) {
    ArbreAVL<T> resultat;
    // Parcourir en in-order et construire directement sans copie
    parcourirRet(racine, autre.racine, resultat);
    detruire(racine);
    racine = resultat.racine;
    nbElements = resultat.nbElements;
    resultat.racine = nullptr; // On eviter double destruction
    return *this;
}

template <class T>
// Opérateur -  retourne la difference ensembliste en crayant un nouvel arbre
ArbreAVL<T> ArbreAVL<T>::operator -  (const ArbreAVL<T>& autre) const{
    ArbreAVL<T> result;
    parcourirRet(racine, autre.racine, result);
    return result;
}

// Fonctions auxiliaires
template <class T>
// Destruction recursive de tous les nœuds de l'arbre
void ArbreAVL<T>::detruire(Noeud* n) {
    if (n) {
        detruire(n->gauche);
        detruire(n->droite);
        delete n;
    }
}

template <class T>
// Copie recursive profonde de tous les nœuds
typename ArbreAVL<T>::Noeud* ArbreAVL<T>::copier(Noeud* n) const {
    if (!n) return nullptr;
    Noeud* nouveau = new Noeud(n->element);
    nouveau->hauteur = n->hauteur;
    nouveau->gauche = copier(n->gauche);
    nouveau->droite = copier(n->droite);
    return nouveau;
}

template <class T>
// Recherche récursive d'un element dans l'arbre
bool ArbreAVL<T>::contientAux(Noeud* n, const T& e) const {
    if (!n) return false;
    if (e < n->element) return contientAux(n->gauche, e);
    if (n->element < e) return contientAux(n->droite, e);
    return true;
}

template <class T>
// Retourne la hauteur d'un nœud 
int ArbreAVL<T>::hauteur(Noeud* n) const {
    if (n != nullptr) {
        return n->hauteur;
    } else {
        return 0;
    }
}

template <class T>
// Calcule le facteur dequilibre d'un nœud 
int ArbreAVL<T>::balance(Noeud* n) const {
    return n ? hauteur(n->gauche) - hauteur(n->droite) : 0;
}

template <class T>
// Rotation simple droite pour requilibrer l'arbre AVL
typename ArbreAVL<T>::Noeud* ArbreAVL<T>::rotationDroite(Noeud* y) {
    Noeud* x = y->gauche;
    y->gauche = x->droite;
    x->droite = y;
    
    y->hauteur = 1 + std::max(hauteur(y->gauche), hauteur(y->droite));
    x->hauteur = 1 + std::max(hauteur(x->gauche), hauteur(x->droite));
    
    return x;
}

template <class T>
// Rotation simple gauche pour reequilibrer l'arbre AVL
typename ArbreAVL<T>::Noeud* ArbreAVL<T>::rotationGauche(Noeud* x) {
    Noeud* y = x->droite;
    x->droite = y->gauche;
    y->gauche = x;
    
    x->hauteur = 1 + std::max(hauteur(x->gauche), hauteur(x->droite));
    y->hauteur = 1 + std::max(hauteur(y->gauche), hauteur(y->droite));
    
    return y;
}

template <class T>
// Insertion recursive avec reequilibrage AVL
typename ArbreAVL<T>::Noeud* ArbreAVL<T>::insererAux(Noeud* n, const T& e) {
    if (!n) {
        nbElements++;
        return new Noeud(e);
    }
    
    if (e < n->element) {
        n->gauche = insererAux(n->gauche, e);
    } else if (n->element < e) {
        n->droite = insererAux(n->droite, e);
    } else {
        return n; // element deja present
    }
    
    n->hauteur = 1 + std::max(hauteur(n->gauche), hauteur(n->droite));
    
    int bal = balance(n);
    
    // Rotation simple droite
    if (bal > 1 && e < n->gauche->element) {
        return rotationDroite(n);
    }
    
    // Rotation simple gauche
    if (bal < -1 && n->droite->element < e) {
        return rotationGauche(n);
    }
    
    // Rotation double gauche-droite
    if (bal > 1 && n->gauche->element < e) {
        n->gauche = rotationGauche(n->gauche);
        return rotationDroite(n);
    }
    
    // Rotation double droite-gauche
    if (bal < -1 && e < n->droite->element) {
        n->droite = rotationDroite(n->droite);
        return rotationGauche(n);
    }
    
    return n;
}

template <class T>
// Fonction auxiliaire pour comparer le contenu de deux arbres
// Verifie que tous les éléments de n1 sont présents dans n2
bool ArbreAVL<T>::egalAux(Noeud* n1, Noeud* n2) const {
    if (!n1) return true; // Si n1 est vide, tous ses éléments (aucun) sont dans n2
    // Vérifier que l'element courant de n1 est dans n2, puis verifier recursivement
    return contientAux(n2, n1->element) &&
           egalAux(n1->gauche, n2) &&
           egalAux(n1->droite, n2);
}

template <class T>
// Parcourt un arbre et insere tous les elements dans result
void ArbreAVL<T>::parcourirIns(Noeud* n, ArbreAVL<T>& result) const {
    if (n) {
        result.inserer(n->element);
        parcourirIns(n->gauche, result);
        parcourirIns(n->droite, result);
    }
}

template <class T>
// Parcourt n1 et insere dans result les elements absents de n2
void ArbreAVL<T>::parcourirRet(Noeud* n1, Noeud* n2, ArbreAVL<T>& result) const {
    if (n1 && !contientAux(n2, n1->element)) {
        result.inserer(n1->element);
    }
    if (n1) {
        parcourirRet(n1->gauche, n2, result);
        parcourirRet(n1->droite, n2, result);
    }
}

#endif // _ARBREAVL_INF3105_H_
