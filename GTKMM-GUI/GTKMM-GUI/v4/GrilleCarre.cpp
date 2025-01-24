#include "GrilleCarre.hpp"

GrilleCarre::GrilleCarre(int r, int c, int cellSize)
    : rows(r), cols(c), grid0(r, std::vector<Cell>(c)), grid1(r, std::vector<Cell>(c)), cellSize(cellSize)
{
    set_size_request(cols * cellSize, rows * cellSize);

    // Activer les événements de pression de bouton
    add_events(Gdk::BUTTON_PRESS_MASK);
    signal_button_press_event().connect(sigc::mem_fun(*this, &GrilleCarre::on_button_press_event));
}

void GrilleCarre::setCellActive(int layer, int x, int y, bool active, const Gdk::RGBA &color)
{
    // Vérification des limites
    if (x < 0 || x >= rows || y < 0 || y >= cols)
        return;

    // Sélection de la couche
    if (layer == 0)
    {
        grid0[x][y].active = active;
        grid0[x][y].color = active ? color : Gdk::RGBA("white");
    }
    else if (layer == 1)
    {
        grid1[x][y].active = active;
        grid1[x][y].color = active ? color : Gdk::RGBA("white");
    }

    // Redessiner la grille
    queue_draw();
}

void GrilleCarre::resizeGrid(int newRows, int newCols)
{
    rows = newRows;
    cols = newCols;
    grid0.resize(newRows, std::vector<Cell>(newCols));
    grid1.resize(newRows, std::vector<Cell>(newCols));
    set_size_request(cols * cellSize, rows * cellSize);
    queue_draw();
}

std::pair<int, int> GrilleCarre::getGridSize() const
{
    return {rows, cols};
}

void GrilleCarre::dessinerPiecesSurGrille(const Grille& grille)
{
    auto gridSize = grille.getGridSize();
    for (int i = 0; i < gridSize.first; ++i) {
        for (int j = 0; j < gridSize.second; ++j) {
            const Case& currentCase = grille.getCase(i, j);
            if (!currentCase.estVide()) {
                const Piece& piece = currentCase.getPiece();
                int layer = 1; // Couche par défaut pour les pièces (à adapter selon votre structure)
                Gdk::RGBA pieceColor = Gdk::RGBA("red"); // Couleur par défaut pour les pièces (à adapter selon votre structure)
                setCellActive(layer, i, j, true, pieceColor);
            }
        }
    }
}

bool GrilleCarre::on_draw(const Cairo::RefPtr<Cairo::Context> &cr)
{
    cr->set_source_rgb(0.8, 0.8, 0.8);
    cr->set_line_width(0.1);

    // Dessiner les lignes de la grille
    for (int i = 0; i <= cols; i++)
    {
        cr->move_to(i * cellSize, 0);
        cr->line_to(i * cellSize, rows * cellSize);
        cr->stroke();
    }

    for (int i = 0; i <= rows; i++)
    {
        cr->move_to(0, i * cellSize);
        cr->line_to(cols * cellSize, i * cellSize);
        cr->stroke();
    }

    // Dessiner les cellules actives
    for (int i = 0; i < rows; ++i)
    {
        for (int j = 0; j < cols; ++j)
        {
            const Cell &cell0 = grid0[i][j];
            if (cell0.active)
            {
                cr->set_source_rgba(cell0.color.get_red(), cell0.color.get_green(), cell0.color.get_blue(), cell0.color.get_alpha());
                cr->rectangle(j * cellSize + cellSize / 2, i * cellSize, cellSize, cellSize);
                cr->fill_preserve();
                cr->set_source_rgb(0, 0, 0);
                cr->set_line_width(0.5);
                cr->stroke();
            }

            const Cell &cell1 = grid1[i][j];
            if (cell1.active)
            {
                cr->set_source_rgba(cell1.color.get_red(), cell1.color.get_green(), cell1.color.get_blue(), cell1.color.get_alpha());
                cr->rectangle(j * cellSize, i * cellSize, cellSize, cellSize);
                cr->fill_preserve();
                cr->set_source_rgb(0, 0, 0);
                cr->set_line_width(0.5);
                cr->stroke();
            }
        }
    }

    return true;
}

bool GrilleCarre::on_button_press_event(GdkEventButton *event)
{
    // Récupérer les coordonnées du clic
    double x = event->x;
    double y = event->y;

    // Calculer les indices de ligne et de colonne dans la grille
    int col = static_cast<int>(x) / getCellSize();
    int row = static_cast<int>(y) / getCellSize();

    // Afficher les indices de la case cliquée sur la console
    std::cout << "Case cliquée : Ligne " << row << ", Colonne " << col << std::endl;
    if(row==0)
    std::cout<<col+1<<std::endl;
    // Retourner true pour indiquer que l'événement a été géré
    return true;
}


            