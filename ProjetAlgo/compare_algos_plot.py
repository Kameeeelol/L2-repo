#!/usr/bin/env python3

"""
compare_algos_plot_fr.py

Usage:
  python compare_algos_plot_fr.py resultats_backtracking.txt resultats_dp.txt ...

Description:
  Lit plusieurs fichiers de résultats contenant des lignes de la forme:
    n temps coût parenthésage
  où 'n' est le nombre de matrices, 'temps' est le temps de résolution,
  'coût' est le coût optimal, et 'parenthésage' est la solution.

  Pour chaque fichier:
    - Parse les couples (n, temps)
    - Regroupe les temps par n
    - Calcule le temps moyen pour chaque n
    - Trace les courbes sur un même graphique pour comparaison.

  Le premier fichier tracé aura sa courbe en bleu, les autres conservent
  la coloration par défaut.
"""

import sys
import statistics
import matplotlib.pyplot as plt
import numpy as np
from collections import defaultdict
import os


def main():
    if len(sys.argv) < 2:
        print("Usage: python compare_algos_plot_fr.py <fichierResultats1> [<fichierResultats2> ...]")
        sys.exit(1)

    # Appliquer le style ggplot
    plt.style.use('ggplot')
    plt.figure(figsize=(10, 6))

    # Parcourir tous les fichiers passés en argument
    for idx, results_file in enumerate(sys.argv[1:]):
        nom_algo = os.path.splitext(os.path.basename(results_file))[0]
        # Transformer le nom pour la légende
        legende = nom_algo.replace('_', ' ').capitalize()

        # Lecture des données
        temps_par_n = defaultdict(list)
        with open(results_file, 'r') as f:
            for ligne in f:
                parts = ligne.strip().split(maxsplit=3)
                if len(parts) < 2:
                    continue
                try:
                    n = int(parts[0])
                    t = float(parts[1])
                except ValueError:
                    continue
                temps_par_n[n].append(t)

        if not temps_par_n:
            print(f"Aucune donnée valide trouvée dans {results_file}, omission.")
            continue

        # Tri et calcul des moyennes
        Ns = sorted(temps_par_n.keys())
        moyennes = [statistics.mean(temps_par_n[n]) for n in Ns]

        # Tracé de la courbe
        couleur = 'blue' if idx == 0 else None
        plt.plot(Ns, moyennes, marker='o', label=legende, color=couleur)

    # Personnalisation du graphique
    plt.title("Comparaison des temps de résolution par algorithme", fontsize=16)
    plt.xlabel("Nombre de matrices (n)", fontsize=14)
    plt.ylabel("Temps moyen (secondes)", fontsize=14)
    plt.xticks(np.arange(0, 21, 3))
    plt.xlim(0, 20)
    plt.grid(True, which='both', linestyle='--', linewidth=0.5)
    plt.legend(fontsize=12)
    plt.tight_layout()

    # Sauvegarde et affichage
    plt.savefig("comparaison_algos.png", dpi=300)
    print("Le graphique comparatif a été enregistré sous 'comparaison_algos.png'.")
    plt.show()


if __name__ == "__main__":
    main()
