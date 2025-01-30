
#pragma once
#include <gtkmm.h>
#include <gtkmm/window.h>
#include <gtkmm/button.h>
#include <gtkmm/box.h>
#include "Grille.hpp"
#include <map>


class Vue : public Gtk::Window
{
private:
    Gtk::VBox vbox;
    Grille grille;
    Gtk::Button button_quit;
    Gtk::Button button_afficher;
    Gtk::Button button_initialiser;
    Gtk::Button button_roi;   // Bouton pour le Roi
    Gtk::Button button_dame;  // Bouton pour la Dame
    Gtk::Button button_tour;
    Gtk::Button button_cavalier;
    Gtk::Button button_fou;
    Gtk::Button button_pion;
    bool visible;
    // Variables pour stocker la sélection
    bool is_selected;
    int selected_x, selected_y;
    std::string selected_piece; // Type de pièce sélectionnée
    std::map<std::pair<int, int>, Gtk::Image*> piece_images_; // Map pour stocker les images des pièces par position
public:
    Vue(int rows,int cols,int cell_size);
    virtual ~Vue();
    unsigned char* movement(GdkEventButton *event,int x, int y,int selected_x, int selected_y, bool is_selected);

protected:
    void on_button_quit_clicked();
    void on_button_afficher_clicked();
    void on_button_initialiser_clicked();  // Nouveau gestionnaire
    //bool on_draw(const Cairo::RefPtr<Cairo::Context> &cr) override;
    void on_case_selected(int x, int y);  // Nouveau gestionnaire d'événement pour la sélection de cases
    void on_piece_button_clicked(const std::string& piece_type); // Nouveau gestionnaire d'événement pour les boutons des pièces
    // Méthode pour charger l'image
    Glib::RefPtr<Gdk::Pixbuf> chargerImage(const std::string& filename, int width, int height);
    // Membre pour l'image
    Gtk::Image image_;
    Glib::RefPtr<Gdk::Pixbuf> piece_pixbuf_;
    Gtk::Fixed fixed_container_; // Conteneur fixe pour positionner l'image
    void dessinerImage(const std::string& image_path, int x_pos, int y_pos);
    void on_dessiner_piece(const std::string& image_path, int x, int y);
    //void dessinerToutesLesPieces(); // Nouvelle méthode pour dessiner toutes les pièces
    void vueInitialiser();
};
