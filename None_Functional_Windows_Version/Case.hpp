#ifndef CASE_HPP
#define CASE_HPP

#include "Piece.hpp"
#include <gdkmm/rgba.h>  // Inclure pour Gdk::RGBA

class Case {
private:
    int couche;
    int largeur;
    int longueur;
    bool aimant;
    bool tag;
    Piece piece;
    bool active;  // État d'activation de la case
    Gdk::RGBA color;  // Couleur de la case

public:
    Case(int c = 0, int l = 0, int lo = 0, const Piece& p = Piece(), bool a = false, const Gdk::RGBA& col = Gdk::RGBA("white"));
    ~Case();
    // Accesseurs
    int getCouche() const;
    int getLargeur() const;
    int getLongueur() const;
    bool getAimant() const;
    bool getTag() const;
    const Piece& getPiece() const;
    bool isActive() const;  // Nouveau
    const Gdk::RGBA& getColor() const;  // Nouveau
    bool estVide() const;
    // Mutateurs
    void setCouche(int c);
    void setLargeur(int l);
    void setLongueur(int lo);
    void setAimant(bool a);
    void setTag(bool t);
    void setPiece(const Piece& p);
    void setActive(bool a);  // Nouveau
    void setColor(const Gdk::RGBA& col);  // Nouveau
    void afficherCaracteristiques() const;
    void deplacerVers(Case& destination);
};

#endif // CASE_HPP
