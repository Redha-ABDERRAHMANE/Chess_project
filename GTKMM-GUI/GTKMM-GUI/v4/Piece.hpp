#ifndef PIECE_HPP
#define PIECE_HPP

#include <string>

class Piece {
private:
    float rayon;
    std::string identifiant;
    std::string nom;
    std::string imagePath; // Chemin de l'image

public:
    Piece(float r = 0.0f, const std::string& id = "", const std::string& path = "");

    float getRayon() const;
    std::string getIdentifiant() const;
    std::string getNom() const;
    std::string getImagePath() const; // Ajouter cette méthode
    void setRayon(float r);
    void setIdentifiant(const std::string& s);
    void setNom(const std::string& s);
    void setImagePath(const std::string& path); // Ajouter cette méthode
    bool getEmpty() const;

private:
    std::string trouverNomParIdentifiant(const std::string& id) const;
};

#endif // PIECE_HPP
