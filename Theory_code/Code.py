import numpy as np
import matplotlib.pyplot as plt

def calculer_inductance_bobine(point_exterieur, forme, nombre_de_spires, largeur_spire, espacement_spire):
    """
    Calcule l'inductance d'une bobine en utilisant un point d'extrémité spécifique (diamètre extérieur).
    """
    point_exterieur = point_exterieur * 1e-3
    largeur_spire = largeur_spire * 1e-3
    espacement_spire = espacement_spire * 1e-3

    diametre_interieur = point_exterieur - 2 * (nombre_de_spires * largeur_spire + (nombre_de_spires - 1) * espacement_spire)

    if diametre_interieur <= 0:
        raise ValueError("Le diamètre intérieur est négatif ou nul. Vérifiez les paramètres d'entrée.")

    facteur_remplissage = (point_exterieur - diametre_interieur) / (point_exterieur + diametre_interieur)
    diametre_moyen = (point_exterieur + diametre_interieur) / 2
    inductance_bobine = (2.25 * 4 * np.pi * 1e-7 * (nombre_de_spires ** 2) * diametre_moyen) / (1 + 3.55 * facteur_remplissage)

    return {
        "diametre_interieur": round(diametre_interieur * 1e3, 6),
        "facteur_remplissage": round(facteur_remplissage, 6),
        "diametre_moyen": round(diametre_moyen * 1e2, 6),
        "inductance_bobine": round(inductance_bobine * 1e6, 6)
    }

def calcul_resistance(point_exterieur, nombre_de_spires, largeur_spire, espacement_spire, diametre_fil):
    """
    Calcule la résistance d'une bobine en utilisant un point d'extrémité spécifique.
    """
    total_length = calcul_longueur_total(point_exterieur, nombre_de_spires, largeur_spire, espacement_spire)
    resistance = R0 * total_length / largeur_spire
    return round(resistance, 6)

def calcul_longueur_total(point_exterieur, nombre_de_spires, largeur_spire, espacement_spire):
    """
    Calcule la longueur totale du fil d'une bobine en utilisant un point d'extrémité spécifique.
    """
    point_exterieur = point_exterieur * 1e-3
    largeur_spire = largeur_spire * 1e-3
    espacement_spire = espacement_spire * 1e-3

    diametre_interieur = point_exterieur - 2 * (nombre_de_spires * largeur_spire + (nombre_de_spires - 1) * espacement_spire)

    if diametre_interieur <= 0:
        raise ValueError("Le diamètre intérieur est négatif ou nul. Vérifiez les paramètres d'entrée.")

    diametres_spires = [
        diametre_interieur + 2 * i * (largeur_spire + espacement_spire)
        for i in range(nombre_de_spires)
    ]

    longueur_totale = sum(np.pi * d for d in diametres_spires)

    return round(longueur_totale * 1e3, 6)

def calculer_courant(resistance, tension):
    """
    Calcule le courant traversant une bobine en utilisant la loi d'Ohm.
    """
    if resistance <= 0:
        raise ValueError("La résistance doit être strictement positive.")
    courant = tension / resistance
    return round(courant, 6)

def calculer_champ_magnetique(I, n, diametre_moyen):
    """
    Calcule le champ magnétique maximal au centre de la bobine.
    """
    if longueur_totale <= 0:
        raise ValueError("La longueur totale doit être strictement positive.")
    #champ_magnetique = (mu0  * n * I) / (longueur_totale * 1e-3)
    #champ_magnetique = (mu0 * I) / (longueur_totale * 1e-3)
    champ_magnetique = (mu0 * I) / (diametre_moyen*1e-1)
    return round(champ_magnetique, 6)

def champ_magnétique_total(nombre_de_spires , point_exterieur , courant , largeur_spire , espacement_spire):
     # Initialisation du champ magnétique total
    champ_magnétique_total = 0.0
    
    # Calcul du diamètre intérieur de la bobine
    diametre_interieur = point_exterieur - 2 * (nombre_de_spires * largeur_spire + (nombre_de_spires - 1) * espacement_spire)
    
    # Initialisation d'un tableau pour stocker les diamètres des spires
    diametres_spires = np.zeros(nombre_de_spires)
    
    # Calcul du courant dans la bobine
    current = calculer_courant(resistance, tension_appliquee)
    
    # Calculer le champ magnétique pour chaque spire
    for i in range(nombre_de_spires):
        # Calcul du diamètre de la spire actuelle
        diametres_spires[i] = diametre_interieur + 2 * i * (largeur_spire + espacement_spire)
        
        # Ajout du champ magnétique contribué par la spire actuelle
        champ_magnétique_total += calculer_champ_magnetique(current , n , diametres_spires[i]) * 8

    # Retourne le champ magnétique total
    return champ_magnétique_total

 # Création d'un tableau numpy contenant les nombres de spires de 1 à nmax
#nombre_de_spires_values = np.arange(1, nombre_de_spires_range(diametre_exterieur, largeur_spire, espacement_spire), 1)

# Paramètres de la bobine
point_exterieur = 46  # Diamètre extérieur en millimètres
largeur_spire = 0.127  # En millimètres
espacement_spire = 0.127  # En millimètres
diametre_fil = 0.5e-3  # Diamètre du fil en mètres
R0 = 6.6e-4  # Résistivité du matériau (Ω*m)
tension_appliquee = 12  # Tension appliquée en Volts
mu0 = 4 * np.pi * 1e-7  # Perméabilité du vide (H/m)

# Définir une plage de nombres de spires
nombre_de_spires_range = np.arange(2, 100, 1)

# Initialiser les listes de résultats
resistances = []
courants = []
inductances = []
longueurs_totales = []
champs_magnetiques = []

# Parcourir les nombres de spires et effectuer les calculs
for n in nombre_de_spires_range:
    try:
        # Calcul de l'inductance
        inductance_data = calculer_inductance_bobine(point_exterieur, "cylindrique", n, largeur_spire, espacement_spire)
        inductances.append(inductance_data["inductance_bobine"])

        # Calcul de la résistance
        resistance = calcul_resistance(point_exterieur, n, largeur_spire, espacement_spire, diametre_fil)
        resistances.append(resistance)

        # Calcul du courant
        courant = calculer_courant(resistance, tension_appliquee)
        courants.append(courant)

        # Calcul de la longueur totale
        longueur_totale = calcul_longueur_total(point_exterieur, n, largeur_spire, espacement_spire)
        longueurs_totales.append(longueur_totale)

        # Calcul du champ magnétique
        champ_magnetique = calculer_champ_magnetique(courant, n, inductance_data["diametre_moyen"])
        champs_magnetiques.append(champ_magnetique)

        # Affichage des valeurs calculées
        print(f"Nombre de spires : {n}")
        print(f"Longueur totale : {longueur_totale:.6f} mm")
        print(f"Inductance : {inductance_data['inductance_bobine']:.6f} µH")
        print(f"Résistance : {resistance:.6f} Ω")
        print(f"Courant : {courant:.6f} A")
        print(f"Champ magnétique : {champ_magnetique:.6f} T\n")

    except ValueError as e:
        print(f"Arrêt à {n} spires : {e}")
        break

# Tracer les résultats
plt.figure(figsize=(12, 8))

# Calcul du champ magnétique total pour chaque valeur de nombre de spires
magnetic_field_values = [champ_magnétique_total(n, point_exterieur, courant , largeur_spire, espacement_spire) for n in nombre_de_spires_range]


nombre_de_spires = 75
I = calculer_courant(resistance, tension_appliquee)
result = champ_magnétique_total(nombre_de_spires, point_exterieur, I, largeur_spire, espacement_spire)
print(f"Champ magnétique total pour {nombre_de_spires} spires : {result * 1e6:.6f} uT")


# Tracé de la résistance
plt.subplot(2, 2, 1)
plt.plot(nombre_de_spires_range[:len(resistances)], resistances, label="Résistance (Ω)", color="blue")
plt.title("Résistance en fonction du nombre de spires")
plt.xlabel("Nombre de spires")
plt.ylabel("Résistance (Ω)")
plt.legend()
plt.grid(True)

# Tracé du courant
plt.subplot(2, 2, 2)
plt.plot(nombre_de_spires_range[:len(courants)], courants, label="Courant (A)", color="red")
plt.title("Courant en fonction du nombre de spires")
plt.xlabel("Nombre de spires")
plt.ylabel("Courant (A)")
plt.legend()
plt.grid(True)

# Tracé du champ magnétique
#plt.subplot(2, 2, 3)
#plt.plot(nombre_de_spires_range[:len(champs_magnetiques)], champs_magnetiques, label="Champ magnétique (T)", color="green")
#plt.title("Champ magnétique en fonction du nombre de spires")
#plt.xlabel("Nombre de spires")
#plt.ylabel("Champ magnétique (T)")
#plt.legend()
#plt.grid(True) 

plt.subplot(2, 2, 3)
# Tracé des résultats
plt.plot(nombre_de_spires_range) #, magnetic_field_values)  # Tracé du graphique avec les valeurs calculées
plt.title('Champ magnétique total en fonction du nombre de spires')  # Titre du graphique
plt.xlabel('Nombre de spires')  # Étiquette de l'axe des abscisses
plt.ylabel('Champ magnétique total')  # Étiquette de l'axe des ordonnées
plt.legend()
plt.grid(True)  # Affichage de la grille



# Tracé de l'inductance
plt.subplot(2, 2, 4)
plt.plot(nombre_de_spires_range[:len(inductances)], inductances, label="Inductance (µH)", color="purple")
plt.title("Inductance en fonction du nombre de spires")
plt.xlabel("Nombre de spires")
plt.ylabel("Inductance (µH)")
plt.legend()
plt.grid(True)

# Afficher le graphique
plt.tight_layout()
plt.show()

