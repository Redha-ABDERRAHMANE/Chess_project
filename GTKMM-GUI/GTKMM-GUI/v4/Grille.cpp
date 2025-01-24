#include "Grille.hpp"
#include <iostream>
Grille::Grille(int r, int c, int cellSize)
    : rows(r), cols(c), grid(r, std::vector<Case>(c, Case())), cellSize(cellSize)
{
    set_size_request(cols * cellSize, rows * cellSize);

    // Activer les événements de pression de bouton
    add_events(Gdk::BUTTON_PRESS_MASK);
    signal_button_press_event().connect(sigc::mem_fun(*this, &Grille::on_button_press_event));

    // Initialiser la grille avec des cases de couche 1 et une couleur rouge transparente
    Gdk::RGBA red;
    red.set_rgba(1.0, 0.0, 0.0, 0.5); // Rouge transparent
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            grid[i][j].setCouche(1); // Définir la couche à 1
            grid[i][j].setLargeur(cellSize); // Définir la largeur de la cellule
            grid[i][j].setLongueur(cellSize); // Définir la longueur de la cellule
            grid[i][j].setActive(true); // Activer la cellule
            grid[i][j].setColor(getYellowColor()); // Définir la couleur de la cellule
            grid[i][j].setAimant(false); // Exemple : ne pas activer l'aimant
            grid[i][j].setTag(false); // Exemple : ne pas activer le tag
        }
    }

    // Redessiner la grille après l'initialisation
    queue_draw();
}
void Grille::setCellActive(int x, int y, bool active, const Gdk::RGBA &color, const Piece &piece) {
    if (x < 0 || x >= rows || y < 0 || y >= cols)
        return;

    grid[x][y].setActive(active);
    grid[x][y].setColor(color);
    grid[x][y].setPiece(piece);
    queue_draw();
}

void Grille::setPiece(int x, int y, const Piece &piece) {
    if (x < 0 || x >= rows || y < 0 || y >= cols || piece.getNom()=="Vide")
        return;

    grid[x][y].setPiece(piece);
    grid[x][y].setActive(true);
    grid[x][y].setAimant(true); // activer l'aimant

    grid[x][y].setColor(getRedColor()); // Changer la couleur en rouge
    queue_draw();
}

void Grille::removePiece(int x, int y) {
    if (x < 0 || x >= rows || y < 0 || y >= cols)
        return;

    // Remplacer la pièce par une pièce vide
    grid[x][y].setPiece(Piece()); 

    // Définir les autres attributs de la case
    grid[x][y].setAimant(false); // Désactiver l'aimant
    //grid[x][y].setActive(false); // Rendre la case inactive
    grid[x][y].setColor(getYellowColor()); // Changer la couleur en jaune

    queue_draw();
}

Gdk::RGBA Grille::getColor(int x, int y) const {
    if (x < 0 || x >= rows || y < 0 || y >= cols)
        return Gdk::RGBA("white"); // Retourne une couleur par défaut (blanc) si les indices sont hors limites

    return grid[x][y].getColor();
}

Piece Grille::getPiece(int x, int y) const {
    if (x < 0 || x >= rows || y < 0 || y >= cols)
        return Piece();

    return grid[x][y].getPiece();
}

void Grille::deplacerPiece(int x,int y,Piece &piece,int selected_x,int selected_y){
    setPiece(x, y, piece);
    removePiece(selected_x, selected_y);
}

std::pair<int, int> Grille::getGridSize() const
{
    return std::make_pair(rows, cols);
}

void Grille::afficherCouche(bool visible) {
    auto gridSize = getGridSize();

    for (int i = 0; i < gridSize.first; ++i) {
        for (int j = 0; j < gridSize.second; ++j) {
            Piece piece = getPiece(i, j);
            if (piece.getNom() != "Vide") {
                std::cout << "Piece at (" << i << ", " << j << "): " << piece.getNom() << std::endl;
            }
            Gdk::RGBA currentColor = getColor(i, j); // Récupérer la couleur existante de la case
            setCellActive(i, j, visible, currentColor, piece);
        }
    }
}
const Case& Grille::getCase(int row,int col)const{
    return grid[row][col];}
    
bool Grille::selectionnerCase(int x, int y, int& selected_x, int& selected_y, bool& is_selected) {
    if (!is_selected) {
        selected_x = x;
        selected_y = y;
        is_selected = true;
        return true;
    }
    return false;
}

bool Grille::deplacerPieceSiPossible(int x, int y, int selected_x, int selected_y) {
 
        Piece piece = getPiece(selected_x, selected_y);
        Piece destinationPiece = getPiece(x, y);
        if (destinationPiece.getNom() == "Vide") {
            deplacerPiece(x, y, piece, selected_x, selected_y);
            return true;
        }
    
    return false;
}

bool Grille::ajouterPieceSiSelectionnee(int x, int y, const std::string& selected_piece) {
    if (!selected_piece.empty()) {
        Piece new_piece(5.0f, selected_piece);
        setPiece(x, y, new_piece);
        return true;
    }
    return false;
}

void Grille::dessinerToutesLesPieces() {
    auto gridSize = getGridSize();
    for (int i = 0; i < gridSize.first; ++i) {
        for (int j = 0; j < gridSize.second; ++j) {
            Piece piece = getPiece(i, j);
            if (piece.getNom() != "Vide") {
                std::string image_path = piece.getNom() + ".png";
                signal_dessiner_piece.emit(image_path, i, j);
            }
        }
    }
}

bool Grille::on_draw(const Cairo::RefPtr<Cairo::Context> &cr) {
    cr->set_source_rgb(0.8, 0.8, 0.8);
    cr->set_line_width(0.1);

    // Dessiner la grille
    for (int i = 0; i <= cols; ++i) {
        cr->move_to(i * cellSize, 0);
        cr->line_to(i * cellSize, rows * cellSize);
        cr->stroke();
    }

    for (int i = 0; i <= rows; ++i) {
        cr->move_to(0, i * cellSize);
        cr->line_to(cols * cellSize, i * cellSize);
        cr->stroke();
    }

    // Dessiner les cases
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            const Case &cell = grid[i][j];
            if (cell.isActive()) {
                cr->set_source_rgba(cell.getColor().get_red(), cell.getColor().get_green(), cell.getColor().get_blue(), cell.getColor().get_alpha());
                cr->rectangle(j * cellSize, i * cellSize, cellSize, cellSize);
                cr->fill_preserve();
                cr->stroke();
            }

            // Dessiner les pièces
            const Piece &piece = cell.getPiece();
            if (piece.getNom() != "Vide") {
                cr->set_source_rgba(0.0, 0.0, 0.0, 1.0); // Couleur noire pour les pièces
                cr->arc(j * cellSize + cellSize / 2.0, i * cellSize + cellSize / 2.0, cellSize / 4.0, 0, 2 * M_PI);
                cr->fill_preserve();
                cr->stroke();
            }
        }
    }

    return true;
}

bool Grille::on_button_press_event(GdkEventButton *event)
{
    // Récupérer les coordonnées du clic
    double x = event->x;
    double y = event->y;

    // Calculer les indices de ligne et de colonne dans la grille
    int col = static_cast<int>(x) / getCellSize();
    int row = static_cast<int>(y) / getCellSize();

    // Afficher les indices de la case cliquée sur la console
    std::cout << "Case cliquée : Ligne  " << row << ", Colonne " << col << std::endl;
    
   
    // Émettre le signal de sélection de case
    m_signal_case_selected.emit(row, col);

    // Retourner true pour indiquer que l'événement a été géré
    return true;
}

unsigned int Grille::position(unsigned int row, unsigned int col){
    
    unsigned int p=1;
    if((row==1)||(row==2)||(row==4)||(row==5))
    p=(row/3+1)*4+row*3+(row%3?col/3+1:col+1);
    else
    p=(row/3)*4+row*3+(row%3?col/3+1:col+1);
    return p;
 }
   
 unsigned char* Grille::movement(unsigned int fromPos, unsigned int toPos){

   static unsigned char result[2];
   
   result[0] = fromPos;
   
   result[1] = toPos;
      
       
       
   // TODO: call the UART library function to actually move the piece 
   return result;
        
}

        
        
        
Grille::type_signal_case_selected Grille::signal_case_selected()
{
    return m_signal_case_selected;
}

void Grille::initialiser()
{

    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            grid[i][j].setActive(true);  // Activer la cellule
            //if (grid[i][j].getPiece().getNom() == "Vide") {
            grid[i][j].setPiece(Piece());
            grid[i][j].setColor(getYellowColor());  // Couleur jaune

            //} else {
                // Garder la couleur rouge pour les cellules avec des pièces
              //  grid[i][j].setColor(getRedColor());
            //}
        }
    }

    queue_draw(); // Redessiner la grille
}

Gdk::RGBA Grille::getRedColor() const {
    Gdk::RGBA red;
    red.set_rgba(1.0, 0.0, 0.0, 0.5); // Rouge transparent
    return red;
}

Gdk::RGBA Grille::getYellowColor() const {
    Gdk::RGBA yellow;
    yellow.set_rgba(1.0, 1.0, 0.0, 0.5); // Jaune transparent
    return yellow;
}





