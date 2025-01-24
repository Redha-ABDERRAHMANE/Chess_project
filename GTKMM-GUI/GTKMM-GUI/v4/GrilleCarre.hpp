#pragma once

#include <gtkmm/drawingarea.h>
#include <vector>
#include <gdkmm/rgba.h>
#include <iostream>
#include "Grille.hpp" // Inclure Grille.hpp pour accéder à la classe Grille

class GrilleCarre : public Gtk::DrawingArea
{
private:
    // Structure de cellule interne
    struct Cell
    {
        bool active;
        Gdk::RGBA color;
        Cell() : active(false), color(Gdk::RGBA("white")) {}
    };

    // Propriétés
    int rows, cols;
    std::vector<std::vector<Cell>> grid0;
    std::vector<std::vector<Cell>> grid1;
    int cellSize;

public:
    // Constructeur
    GrilleCarre(int r, int c, int cellSize = 50);

    // Méthodes publiques
    void setCellActive(int layer, int x, int y, bool active, const Gdk::RGBA &color = Gdk::RGBA("white"));
    void resizeGrid(int newRows, int newCols);
    std::pair<int, int> getGridSize() const;
    void dessinerPiecesSurGrille(const Grille& grille); // Nouvelle méthode pour dessiner les pièces

protected:
    // Méthode de dessin
    bool on_draw(const Cairo::RefPtr<Cairo::Context> &cr) override;

    // Gestionnaire d'événements de clic de souris
    bool on_button_press_event(GdkEventButton *event);

private:
    // Méthode pour obtenir la taille d'une cellule
    int getCellSize() const { return cellSize; }
};
