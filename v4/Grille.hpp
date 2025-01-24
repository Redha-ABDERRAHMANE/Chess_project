#pragma once

#include <gtkmm/drawingarea.h>
#include <vector>
#include <gdkmm/rgba.h>
#include <sigc++/sigc++.h>
#include "Piece.hpp"
#include "Case.hpp" // Inclure la définition de Case

class Grille : public Gtk::DrawingArea
{
private:
    // Propriétés
    int rows, cols;
    std::vector<std::vector<Case>> grid;  // Utiliser Case au lieu de la structure Cell
    int cellSize;

public:
    // Constructeur
    Grille(int r, int c, int cellSize = 100);

    // Méthodes publiques
    void setCellActive(int x, int y, bool active, const Gdk::RGBA &color = Gdk::RGBA("white"), const Piece &piece = Piece());
    void setPiece(int x, int y, const Piece &piece);
    void removePiece(int x, int y);
    Piece getPiece(int x, int y) const;
    //void resizeGrid(int newRows, int newCols);
    //void redessinerCase(int x, int y);  // Nouvelle fonction
    void deplacerPiece(int x,int y,Piece &piece,int selected_x,int selected_y);
    Gdk::RGBA getColor(int x, int y) const;
    // Méthode pour obtenir la taille d'une cellule
    int getCellSize() const { return cellSize; }
    void afficherCouche(bool visible);
    void initialiser(void);
    std::pair<int, int> getGridSize() const;
    const Case& getCase(int row,int col) const;
    // Signal de sélection de case
    typedef sigc::signal<void, int, int> type_signal_case_selected;
    type_signal_case_selected signal_case_selected();
    bool selectionnerCase(int x, int y, int& selected_x, int& selected_y, bool& is_selected);
    bool deplacerPieceSiPossible(int x, int y, int selected_x, int selected_y, bool is_selected);
    bool ajouterPieceSiSelectionnee(int x, int y, const std::string& selected_piece);
    void dessinerToutesLesPieces();
    int position(GdkEventButton *event);
    
    // Signaux
    sigc::signal<void, const std::string&, int, int> signal_dessiner_piece;


protected:
    // Méthode de dessin
    bool on_draw(const Cairo::RefPtr<Cairo::Context> &cr) override;

    // Gestionnaire d'événements de clic de souris
    bool on_button_press_event(GdkEventButton *event);

private:

    // Signal
    type_signal_case_selected m_signal_case_selected;

    Gdk::RGBA getRedColor() const;
    Gdk::RGBA getYellowColor() const;
    Gdk::RGBA nogetYellowColor() const; 
};