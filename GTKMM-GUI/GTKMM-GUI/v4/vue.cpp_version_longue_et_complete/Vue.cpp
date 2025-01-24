#include "Vue.hpp"
#include <iostream>


Vue::Vue()
    : grille(8, 8, 100), 
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
    set_title("Grille 2x2 avec boutons");
    set_default_size(1000, 1000);

    // Créer un conteneur principal (par exemple, une boîte horizontale)
    Gtk::HBox* hbox_main = Gtk::manage(new Gtk::HBox());

    // Créer une boîte verticale pour la grille et les boutons
    Gtk::VBox* vbox = Gtk::manage(new Gtk::VBox());

    // Ajouter la grille et les boutons à la boîte verticale
    vbox->pack_start(grille);
    vbox->pack_start(button_afficher, Gtk::PACK_SHRINK);
    vbox->pack_start(button_initialiser, Gtk::PACK_SHRINK);  // Ajouter le bouton Initialiser
    
    // Créer une boîte horizontale pour les boutons des pièces
    Gtk::HBox* hbox_pieces = Gtk::manage(new Gtk::HBox());
    hbox_pieces->pack_start(button_roi, Gtk::PACK_SHRINK);
    hbox_pieces->pack_start(button_dame, Gtk::PACK_SHRINK);
    hbox_pieces->pack_start(button_tour, Gtk::PACK_SHRINK);
    hbox_pieces->pack_start(button_cavalier, Gtk::PACK_SHRINK);
    hbox_pieces->pack_start(button_fou, Gtk::PACK_SHRINK);
    hbox_pieces->pack_start(button_pion, Gtk::PACK_SHRINK);

    // Ajouter la boîte horizontale à la boîte verticale
    vbox->pack_start(*hbox_pieces, Gtk::PACK_SHRINK);

    vbox->pack_start(button_quit, Gtk::PACK_SHRINK);

    // Ajouter la boîte verticale au conteneur principal
    hbox_main->pack_start(*vbox);

    // Ajouter un conteneur pour l'image
    Gtk::VBox* vbox_image = Gtk::manage(new Gtk::VBox());
    vbox_image->pack_start(image_);

    // Ajouter le conteneur pour l'image au conteneur principal
    hbox_main->pack_start(*vbox_image, Gtk::PACK_SHRINK);

    // Ajouter le conteneur principal à la fenêtre
    add(*hbox_main);

    // Connecter les signaux des boutons
    button_quit.signal_clicked().connect(sigc::mem_fun(*this, &Vue::on_button_quit_clicked));
    button_afficher.signal_clicked().connect(sigc::mem_fun(*this, &Vue::on_button_afficher_clicked));
    button_initialiser.signal_clicked().connect(sigc::mem_fun(*this, &Vue::on_button_initialiser_clicked));  // Connecter le bouton Initialiser
    button_roi.signal_clicked().connect([this]() { on_piece_button_clicked("roi"); });  // Connecter le bouton Roi
    button_dame.signal_clicked().connect([this]() { on_piece_button_clicked("dame"); });  // Connecter le bouton Dame
    button_tour.signal_clicked().connect([this]() { on_piece_button_clicked("tour"); });  // Connecter le bouton Tour
    button_cavalier.signal_clicked().connect([this]() { on_piece_button_clicked("cavalier"); });  // Connecter le bouton Cavalier
    button_fou.signal_clicked().connect([this]() { on_piece_button_clicked("fou"); });  // Connecter le bouton Fou
    button_pion.signal_clicked().connect([this]() { on_piece_button_clicked("pion"); });  // Connecter le bouton Pion

    // Connecter le signal de sélection de case
    grille.signal_case_selected().connect(sigc::mem_fun(*this, &Vue::on_case_selected));

    // Chemin vers le fichier d'image (remplacez avec votre propre chemin d'image)
    std::string image_path = "roi.png";

    // Charger et afficher l'image
    Glib::RefPtr<Gdk::Pixbuf> pixbuf = chargerImage(image_path, 100, 100); // Taille de case ici
    if (pixbuf) {
        image_.set(pixbuf);
    }

        // Chemin vers le fichier d'image (remplacez avec votre propre chemin d'image)
    std::string image_path1 = "queen.png";

    // Charger et afficher l'image
    Glib::RefPtr<Gdk::Pixbuf> pixbuf1 = chargerImage(image_path1, 100, 100); // Taille de case ici
    if (pixbuf1) {
        //image_.set(pixbuf1);
    }

    // Afficher tous les enfants
    show_all_children();
}

Vue::~Vue() {}

void Vue::on_button_quit_clicked()
{
    hide(); // Ferme la fenêtre
}

void Vue::on_button_afficher_clicked() {
    visible = !visible;
    auto gridSize = grille.getGridSize();

    for (int i = 0; i < gridSize.first; ++i) {
        for (int j = 0; j < gridSize.second; ++j) {
            Piece piece = grille.getPiece(i, j);
            if (piece.getNom() != "Vide") {
                std::cout << "Piece at (" << i << ", " << j << "): " << piece.getNom() << std::endl;
            }
            Gdk::RGBA currentColor = grille.getColor(i, j); // Récupérer la couleur existante de la case
            grille.setCellActive(i, j, visible, currentColor, piece);
        }
    }
}


void Vue::on_case_selected(int x, int y)
{

    if (!is_selected) {
        std::cout<<"premier clique "<<std::endl;
        // Première sélection
        selected_x = x;
        selected_y = y;
        is_selected = true;
    } else {
        std::cout<<"deuxième clique "<<std::endl;
        // Deuxième sélection : déplacement de la pièce
        Piece piece = grille.getPiece(selected_x, selected_y);

        // Vérifier si la case de destination contient déjà une pièce
        Piece destinationPiece = grille.getPiece(x, y);
        if (destinationPiece.getNom() != "Vide") {
            std::cout << "La case de destination (" << x << ", " << y << ") contient déjà une pièce : " << destinationPiece.getNom() << std::endl;
        } else {
            grille.setPiece(x, y, piece);
            grille.removePiece(selected_x, selected_y);
        }

        is_selected = false;
    }

    if (!selected_piece.empty()) {
        // Placer la pièce sélectionnée à la case cliquée
        if (selected_piece == "roi") {
            Piece roi(5.0f, "roi");
            roi.setNom("Roi");
            grille.setPiece(x, y, roi);
        } else if (selected_piece == "dame") {
            Piece dame(5.0f, "dame");
            dame.setNom("Dame");
            grille.setPiece(x, y, dame);
        } else if (selected_piece == "tour") {
            Piece tour(5.0f, "tour");
            tour.setNom("Tour");
            grille.setPiece(x, y, tour);
        } else if (selected_piece == "cavalier") {
            Piece cavalier(5.0f, "cavalier");
            cavalier.setNom("Cavalier");
            grille.setPiece(x, y, cavalier);
        } else if (selected_piece == "fou") {
            Piece fou(5.0f, "fou");
            fou.setNom("Fou");
            grille.setPiece(x, y, fou);
        } else if (selected_piece == "pion") {
            Piece pion(5.0f, "pion");
            pion.setNom("Pion");
            grille.setPiece(x, y, pion);
        }
        is_selected = false;
        selected_piece = ""; // Réinitialiser la sélection après avoir placé la pièce
    } else {
        std::cout << "Aucune pièce sélectionnée." << std::endl;
    }
}

void Vue::on_piece_button_clicked(const std::string& piece_type) {
    selected_piece = piece_type; // Enregistrer le type de la pièce sélectionnée
}

void Vue::on_button_initialiser_clicked()
{
    // Réinitialiser la grille à son état de départ
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


