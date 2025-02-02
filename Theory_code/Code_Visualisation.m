%Code pour l’affichage du champ magnétique en 3d sur matlab : 
clc;
clear;
close all;
% -------------------------- PARAMETERS ---------------------------------- %
mu_0 = 4 * pi * 1e-7; % Perméabilité du vide (T·m/A)
I = 1;                % Intensité du courant (en Ampères)
n_spires = 75;         % Nombre de spires
diametre_exterieur = 0.05; % Diamètre extérieur des spires (m)
largeur_spire = 0.01; % Largeur d'une spire (m)
espacement_spire = 0.002; % Espacement entre deux spires (m)
points_par_spire = 100; % Nombre de segments par spire
% Domaine spatial des points d'observation
x = -0.025:0.001:0.025; % Axe x de -2.5 cm à 2.5 cm avec un pas de 1 mm
y = -0.025:0.001:0.025; % Axe y de -2.5 cm à 2.5 cm avec un pas de 1 mm
z = -0.025:0.001:0.025; % Axe z de -2.5 cm à 2.5 cm avec un pas de 1 mm
[X, Y, Z] = meshgrid(x, y, z); % Grille des points d'observation
% Initialisation des composantes du champ magnétique
Bx = zeros(size(X));
By = zeros(size(Y));
Bz = zeros(size(Z));
% -------------------- LOOP OVER EACH SPIRE ----------------------------- %
for k = 1:n_spires
   % Calcul du rayon de la spire courante
   rayon_spire = diametre_exterieur / 2 - (k - 1) * (largeur_spire + espacement_spire);
   z_spire = (k - 1) * espacement_spire; % Position en z de la spire courante
   % Coordonnées de la spire (cercle)
   theta = linspace(0, 2 * pi, points_par_spire);
   x_spire = rayon_spire * cos(theta);
   y_spire = rayon_spire * sin(theta);
   z_spire = z_spire * ones(size(theta));
   % Parcourir chaque segment de la spire
   for i = 1:length(x_spire) - 1
       % Position moyenne du segment
       x_seg = (x_spire(i) + x_spire(i + 1)) / 2;
       y_seg = (y_spire(i) + y_spire(i + 1)) / 2;
       z_seg = (z_spire(i) + z_spire(i + 1)) / 2;
       % Vecteur de distance entre le point d'observation et le segment
       Rx = X - x_seg;
       Ry = Y - y_seg;
       Rz = Z - z_seg;
       % Distance entre le point d'observation et le segment
       r_vector = sqrt(Rx.^2 + Ry.^2 + Rz.^2);
       % Vecteur infinitésimal du segment (d\vec{l})
       dlx = x_spire(i + 1) - x_spire(i);
       dly = y_spire(i + 1) - y_spire(i);
       dlz = z_spire(i + 1) - z_spire(i);
       % Calcul des composantes du champ magnétique avec Biot-Savart
       dBx = (mu_0 * I ./ (4 * pi * r_vector.^3)) .* (dly .* Rz - dlz .* Ry);
       dBy = (mu_0 * I ./ (4 * pi * r_vector.^3)) .* (dlz .* Rx - dlx .* Rz);
       dBz = (mu_0 * I ./ (4 * pi * r_vector.^3)) .* (dlx .* Ry - dly .* Rx);
       % Accumulation des contributions des segments
       Bx = Bx + dBx;
       By = By + dBy;
       Bz = Bz + dBz;
   end
end
% --------------------- VISUALIZATION ------------------------------------ %
% Norme du champ magnétique
%B_magnitude = sqrt(Bx.^2 + By.^2 + Bz.^2);
B_magnitude = Bz ;
% Représentation en 3D
figure(1);
quiver3(X, Y, Z, Bx, By, Bz, 2, 'b'); % Vecteurs du champ magnétique
title('Champ magnétique (3D)');
xlabel('X (m)');
ylabel('Y (m)');
zlabel('Z (m)');
axis equal;
grid on;
% Coupe dans le plan Z=0
figure(2);
surf(x, y, squeeze(B_magnitude(:, :, floor(end/2))), 'EdgeColor', 'none');
colormap('viridis');
colorbar;
title('Norme du champ magnétique (plan Z=0)');
xlabel('X (m)');
ylabel('Y (m)');
axis equal;
view(2);
function [inductance_bobine, details] = calculer_inductance_bobine(diametre_exterieur, nombre_de_spires, largeur_spire, espacement_spire)
   % Conversion en unités de mètres
   diametre_exterieur = diametre_exterieur * 1e-3;
   largeur_spire = largeur_spire * 1e-3;
   espacement_spire = espacement_spire * 1e-3;
   % Calcul du diamètre intérieur
   diametre_interieur = diametre_exterieur - 2 * (nombre_de_spires * largeur_spire + (nombre_de_spires - 1) * espacement_spire);
   % Calcul du facteur de remplissage
   facteur_remplissage = (diametre_exterieur - diametre_interieur) / (diametre_exterieur + diametre_interieur);
   % Calcul du diamètre moyen
   diametre_moyen = (diametre_exterieur + diametre_interieur) / 2;
   % Calcul de l'inductance
   inductance_bobine = (2.25 * 4 * pi * 1e-7 * (nombre_de_spires^2) * diametre_moyen) / (1 + 3.55 * facteur_remplissage);
   % Conversion pour des unités plus lisibles
   details.diametre_interieur = diametre_interieur * 1e3; % mm
   details.facteur_remplissage = facteur_remplissage;
   details.diametre_moyen = diametre_moyen * 1e2; % cm
   details.inductance_bobine = inductance_bobine * 1e6; % µH
end
function R = calcul_resistance(diametre_exterieur, nombre_de_spires, largeur_spire, espacement_spire, ~)
   % Le code reste identique, mais MATLAB n'attendra plus d'utiliser 'Valim'
   % Calcul de la longueur totale du fil de la bobine
   total_length = calcul_longueur_total(diametre_exterieur, nombre_de_spires, largeur_spire, espacement_spire);
   % Paramètres physiques
   d_ext = 46e-3; % Distance constante (4.6 cm)
   n = 75;        % Nombre de tours
   L = 19e-3;     % Longueur de la ligne initiale (1.9 cm)
   w = 0.127e-3;  % Largeur et espacement (0.127 mm)
   % Calcul de la résistance
   R = 40 * total_length / calcul_longueur_total(d_ext, n, L, w);
end
function total_length = calcul_longueur_total(diametre_exterieur, nombre_de_spires, largeur_spire, espacement_spire)
   % Conversion en mètres
   diametre_exterieur = diametre_exterieur * 1e-3;
   largeur_spire = largeur_spire * 1e-3;
   espacement_spire = espacement_spire * 1e-3;
   % Calcul du diamètre intérieur
   diametre_interieur = diametre_exterieur - 2 * (nombre_de_spires * largeur_spire + (nombre_de_spires - 1) * espacement_spire);
   % Initialisation de la longueur totale
   total_length = 0;
   % Ajout des longueurs pour chaque spire
   for i = 1:nombre_de_spires
       diametre_courant = diametre_interieur + 2 * (i - 1) * (largeur_spire + espacement_spire);
       total_length = total_length + pi * diametre_courant;
   end
end
% ---------------- Calcul de l'inductance ---------------- %
[Inductance, details] = calculer_inductance_bobine(50, 5, 1, 0.2); % Exemple
disp(['Inductance de la bobine : ', num2str(details.inductance_bobine), ' µH']);
% ---------------- Calcul de la résistance ---------------- %
R = calcul_resistance(50, 5, 1, 0.2, 40); % Exemple
disp(['Résistance de la bobine : ', num2str(R), ' ohms']);

