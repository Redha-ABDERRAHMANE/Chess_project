#include "Case.hpp"
#include <iostream>

// Constructeur
Case::Case(int c, int l, int lo, const Piece& p, bool a, const Gdk::RGBA& col)
    : couche(c), largeur(l), longueur(lo), piece(p), aimant(true), tag(false), active(a), color(col) {
    if (c != 1) {
        piece.setNom("Vide");
    }
    if (c == 1) {
        tag = true;
    }
}

Case::~Case() {}

int Case::getCouche() const {
    return couche;
}

int Case::getLargeur() const {
    return largeur;
}

int Case::getLongueur() const {
    return longueur;
}

bool Case::getAimant() const {
    return aimant;
}

bool Case::getTag() const {
    return tag;
}
bool Case::estVide() const{
    return piece.getNom() =="Vide";}
    
const Piece& Case::getPiece() const {
    return piece;
}

bool Case::isActive() const { return active; }  // Retourne l'état d'activation

const Gdk::RGBA& Case::getColor() const { return color; }  // Retourne la couleur

void Case::setCouche(int c) {
    couche = c;
    if (couche == 1) {
        tag = true;
    } else {
        tag = false;
    }
}

void Case::setLargeur(int l) {
    largeur = l;
}

void Case::setLongueur(int lo) {
    longueur = lo;
}

void Case::setAimant(bool a) {
    aimant = a;
}

void Case::setTag(bool t) {
    tag = t;
}

void Case::setPiece(const Piece& p) {
    piece = p;
}

void Case::setActive(bool a) { active = a; }  // Définit l'état d'activation

void Case::setColor(const Gdk::RGBA& col) { color = col; }  // Définit la couleur

void Case::afficherCaracteristiques() const {
    std::cout << "Case: Couche " << couche;
    std::cout << ", Aimant: " << (aimant ? "true" : "false");
    std::cout << ", Tag: " << (tag ? "true" : "false");
    std::cout << ", Pièce posée: " << piece.getNom() << std::endl;
}

void Case::deplacerVers(Case& destination) {
    if (couche != destination.getCouche()) {
        throw std::invalid_argument("Erreur: Les cases ne sont pas sur la même couche.");
    }
    if (destination.getPiece().getNom() == "Vide") {
        destination.setPiece(piece);
        piece.setNom("Vide");
    } else {
        throw std::invalid_argument("Erreur: La case destinataire n'est pas vide.");
    }
}

