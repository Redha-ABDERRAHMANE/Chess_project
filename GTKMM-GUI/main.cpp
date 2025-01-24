//v1: on peut déplacer une pièce  sur la grille 
//traivail à faire:redéfinir la grille 
//v2: la grille est bien définie avec les couleurs rouges et jaunes en fonction des pièces
//pour v3 travail à faire: initialiser une grille vide et faire glisser les pièces selon la préférence de l'utilisateur

//v3 qui reprend v2 et essaie alors de faire glisser le pièces

//travail à faire pour V4:ajouter une image pour les pièces

//pb à résoudre:si la grille est plus petite que la largeur des boutons=>décalage des pièces déssinées

#include <gtkmm/application.h>
#include "Vue.hpp"

int main(int argc, char* argv[]) {
    auto app = Gtk::Application::create(argc, argv, "org.gtkmm.example");

    // Définir les paramètres de la grille
    int rows = 7;         // Nombre de lignes
    int cols = 7;         // Nombre de colonnes
    int cell_size = 100;  // Taille de chaque case

    Vue vue(rows, cols, cell_size);

    // Démarre l'application, montre la fenêtre principale
    return app->run(vue);
}

