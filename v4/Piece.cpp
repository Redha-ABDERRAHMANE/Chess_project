#include "Piece.hpp"

Piece::Piece(float r, const std::string& id, const std::string& path)
    : rayon(r), identifiant(id), nom(trouverNomParIdentifiant(id)), imagePath(path) {}

float Piece::getRayon() const {
    return rayon;
}

std::string Piece::getIdentifiant() const {
    return identifiant;
}

std::string Piece::getNom() const {
    return nom;
}

bool Piece::getEmpty() const{
    return 1;
}

std::string Piece::getImagePath() const {
    return imagePath;
}

void Piece::setRayon(float r) {
    rayon = r;
}

void Piece::setIdentifiant(const std::string& s) {
    identifiant = s;
    nom = trouverNomParIdentifiant(s);
}

void Piece::setNom(const std::string& s) {
    nom = s;
}

void Piece::setImagePath(const std::string& path) {
    imagePath = path;
}

std::string Piece::trouverNomParIdentifiant(const std::string& id) const {
    if (id == "Roi")
        return "Roi";
    if (id == "Dame")
        return "Dame";
    if (id == "Tour")
        return "Tour";
    if (id == "Cavalier")
        return "Cavalier";
    if (id == "Pion")
        return "Pion";
    // Ajouter des correspondances supplémentaires pour d'autres pièces si nécessaire
    return "Vide";
}
