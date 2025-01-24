#include "Vue.hpp"
#include <iostream>

Vue::Vue(int rows, int cols, int cell_size)
    : grille(rows, cols, cell_size), 
      button_quit("Quitter"), 
      button_afficher("Afficher Couche 1"), 
      button_initialiser("Initialiser"), 
      button_roi("Roi"), 
      button_dame("Dame"),
      button_tour("Tour"), 
      button_cavalier("Cavalier"), 
      button_fou("Fou"), 
      button_pion("Pion"), 
      visible(true), 
      is_selected(false), 
      selected_x(-1), 
      selected_y(-1), 
      selected_piece("")
{
    set_title("Grille 3x3 avec boutons");
    set_default_size(100, 100);

    Gtk::HBox* hbox_main = Gtk::manage(new Gtk::HBox());
    Gtk::VBox* vbox = Gtk::manage(new Gtk::VBox());

    vbox->pack_start(grille);
    vbox->pack_start(button_afficher, Gtk::PACK_SHRINK);
    vbox->pack_start(button_initialiser, Gtk::PACK_SHRINK);

    Gtk::HBox* hbox_pieces = Gtk::manage(new Gtk::HBox());
    hbox_pieces->pack_start(button_roi, Gtk::PACK_SHRINK);
    hbox_pieces->pack_start(button_dame, Gtk::PACK_SHRINK);
    hbox_pieces->pack_start(button_tour, Gtk::PACK_SHRINK);
    hbox_pieces->pack_start(button_cavalier, Gtk::PACK_SHRINK);
    hbox_pieces->pack_start(button_fou, Gtk::PACK_SHRINK);
    hbox_pieces->pack_start(button_pion, Gtk::PACK_SHRINK);

    vbox->pack_start(*hbox_pieces, Gtk::PACK_SHRINK);
    vbox->pack_start(button_quit, Gtk::PACK_SHRINK);

    hbox_main->pack_start(*vbox);

    fixed_container_.set_size_request(0, 0); // Définir la taille du conteneur fixe
    hbox_main->pack_start(fixed_container_, Gtk::PACK_EXPAND_WIDGET);

    add(*hbox_main);

    button_quit.signal_clicked().connect(sigc::mem_fun(*this, &Vue::on_button_quit_clicked));
    button_afficher.signal_clicked().connect(sigc::mem_fun(*this, &Vue::on_button_afficher_clicked));
    button_initialiser.signal_clicked().connect(sigc::mem_fun(*this, &Vue::on_button_initialiser_clicked));
    button_roi.signal_clicked().connect([this]() { on_piece_button_clicked("Roi"); });
    button_dame.signal_clicked().connect([this]() { on_piece_button_clicked("Dame"); });
    button_tour.signal_clicked().connect([this]() { on_piece_button_clicked("Tour"); });
    button_cavalier.signal_clicked().connect([this]() { on_piece_button_clicked("Cavalier"); });
    button_fou.signal_clicked().connect([this]() { on_piece_button_clicked("Fou"); });
    button_pion.signal_clicked().connect([this]() { on_piece_button_clicked("Pion"); });

    grille.signal_case_selected().connect(sigc::mem_fun(*this, &Vue::on_case_selected));
    grille.signal_dessiner_piece.connect(sigc::mem_fun(*this, &Vue::on_dessiner_piece));

    show_all_children();
}

Vue::~Vue() {}

void Vue::on_button_quit_clicked() {
    hide(); // Ferme la fenêtre
}

void Vue::on_button_afficher_clicked() {
    visible = !visible;
    grille.afficherCouche(visible);
}

void Vue::on_case_selected(int x, int y) {
    if (grille.selectionnerCase(x, y, selected_x, selected_y, is_selected)) {
        std::cout << "premier clique: " << std::endl;
    } else {
        if (grille.deplacerPieceSiPossible(x, y, selected_x, selected_y, is_selected)) {
            vueInitialiser();
            grille.dessinerToutesLesPieces(); // Redessiner toutes les pièces après déplacement
        }
        is_selected = false;
        std::cout << "deuxième clique: " << std::endl;
    }

    if (grille.ajouterPieceSiSelectionnee(x, y, selected_piece)) {
        vueInitialiser();
        grille.dessinerToutesLesPieces(); // Redessiner toutes les pièces après ajout d'une nouvelle pièce
        is_selected = false;
        selected_piece = "";
    }
}


void Vue::on_piece_button_clicked(const std::string& piece_type) {
    selected_piece = piece_type; // Enregistrer le type de la pièce sélectionnée
}

void Vue::on_button_initialiser_clicked() {
    vueInitialiser();
    grille.initialiser();
}

Glib::RefPtr<Gdk::Pixbuf> Vue::chargerImage(const std::string& filename, int width, int height) {
    Glib::RefPtr<Gdk::Pixbuf> pixbuf;
    try {
        pixbuf = Gdk::Pixbuf::create_from_file(filename);
    } catch (const Glib::FileError& ex) {
        std::cerr << "FileError: " << ex.what() << std::endl;
    } catch (const Gdk::PixbufError& ex) {
        std::cerr << "PixbufError: " << ex.what() << std::endl;
    }

    if (pixbuf) {
        pixbuf = pixbuf->scale_simple(width, height, Gdk::INTERP_BILINEAR);
    }

    return pixbuf;
}

void Vue::dessinerImage(const std::string& image_path, int y_pos, int x_pos) {
    Glib::RefPtr<Gdk::Pixbuf> pixbuf = chargerImage(image_path, grille.getCellSize()-20, grille.getCellSize()-20); // Taille de l'image
    if (pixbuf) {
        Gtk::Image* piece_image = Gtk::manage(new Gtk::Image(pixbuf));
        piece_images_[{x_pos, y_pos}] = piece_image; // Associer l'image à sa position
        fixed_container_.put(*piece_image, -grille.getCellSize()*(grille.getGridSize().second-x_pos)+10, grille.getCellSize()*y_pos+10); // Positionner l'image à (x_pos, y_pos)
        std::cout << "Case cliquée dans Image: Ligne " << -grille.getCellSize()*(grille.getGridSize().second-x_pos)+10 << ", Colonne " << 100*y_pos+10 << std::endl;
        piece_image->show(); // Assurez-vous que l'image est affichée
    }
}

void Vue::on_dessiner_piece(const std::string& image_path, int x, int y) {
    dessinerImage(image_path, x, y);
}
/*
void Vue::dessinerToutesLesPieces() {
    vueInitialiser();
    grille.dessinerToutesLesPieces();
}
*/
/**
void Vue::dessinerToutesLesPieces() {
    // Effacer toutes les images actuelles
    vueInitialiser();

    auto gridSize = grille.getGridSize();

    for (int i = 0; i < gridSize.first; ++i) {
        for (int j = 0; j < gridSize.second; ++j) {
            Piece piece = grille.getPiece(i, j);
            if (piece.getNom() != "Vide") {
                std::string image_path = piece.getNom() + ".png"; // Déduire le chemin de l'image basé sur le nom de la pièce
                dessinerImage(image_path, i, j);
            }
        }
    }
}
*/
void Vue::vueInitialiser(){
    // Effacer toutes les images actuelles
    for (auto& entry : piece_images_) {
        fixed_container_.remove(*entry.second);
    }
    piece_images_.clear(); // Vider la map des images

}
