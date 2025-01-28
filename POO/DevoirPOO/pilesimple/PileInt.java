package pilesimple;


/**
 * Une pile d'entiers de taille bornée representée par un tableau.
 *
 * @author Marc Champesme
 * @version 1.8.0
 * @invariant (0 <= getNbElements()) && (getNbElements() <= getNbMaxElements());
 */

public class PileInt implements Cloneable {
	private int[] contenu;
	private int nbMaxElements;
	private int nbElements;

	/**
	 * Initialise une pile vide avec la capacité maximale spécifiée.
	 *
	 * @param nbMaxElements la capacité maximale de la pile
	 *
	 * @requires nbMaxElements >= 0;
	 * @requires nbMaxElements < Integer.MAX_VALUE;
	 * @ensures estVide();
	 * @ensures (getNbMaxElements() == nbMaxElements);
	 * @ensures (getNbElements() == 0);
	 *
	 */
	public PileInt(int nbMaxElements) {
		contenu = new int[nbMaxElements];
		this.nbMaxElements = nbMaxElements;
	}

	/**
	 * Initialise une pile avec les nbElements premiers éléments du tableau spécifié
	 * et une capacité maximale correspondant à la taille du tableau spécifié.
	 *
	 * @param elements   le tableau dont les éléments doivent être utilisés pour
	 *                   initialiser la pile.
	 * @param nbElements le nombre d'éléments du tableau à utiliser pour pour
	 *                   initialiser la pile.
	 *
	 * @requires elements != null;
	 * @requires nbElements >= 0;
	 * @requires nbElements <= elements.length;
	 * @ensures (getNbMaxElements() == elements.length);
	 * @ensures (getNbElements() == nbElements);
	 * @ensures (nbElements > 0) ==> getSommet() == elements[nbElements - 1];
	 *
	 */
	public PileInt(int[] elements, int nbElements) {
		contenu = elements.clone();
		this.nbMaxElements = contenu.length;
		this.nbElements = nbElements;
	}

	/**
	 * Consultation du nombre maximal d'élément pouvant etre
	 * stockés dans la pile.
	 *
	 * @return capacité maximale de la pile
	 *
	 * @pure
	 */
	public int getNbMaxElements() {
		return nbMaxElements;
	}

	/**
	 * Consultation du nombre d'éléments présents dans la pile.
	 *
	 * @return nombre d'éléments de la pile
	 *
	 * @pure
	 */
	public int getNbElements() {
		return nbElements;
	}

	/**
	 * Consultation du sommet de pile
	 *
	 * @return le sommet de pile
	 *
	 * @requires !estVide();
	 *
	 * @pure
	 */
	public int getSommet() {
		return contenu[nbElements - 1];
	}

	/**
	 * Ajout au sommet de la pile (i.e. push).
	 *
	 * @param e L'élement à ajouter
	 *
	 * @requires !estPleine();
	 * @ensures !estVide();
	 * @ensures getSommet() == e;
	 * @ensures (getNbElements() == \old(getNbElements()) + 1);
	 *
	 */
	public void empiler(int e) {
		contenu[nbElements] = e;
		nbElements = nbElements + 1;
	}

	/**
	 * Retrait de l'élement au sommet de la pile (i.e. pop)
	 *
	 * @requires !estVide();
	 * @ensures !estPleine();
	 * @ensures (getNbElements() == \old(getNbElements()) - 1);
	 *
	 */
	public void depiler() {
		nbElements = nbElements - 1;
	}

	/**
	 * La pile est-elle pleine ? Méthode nécessaire à
	 * l'implémentation par tableau de taille fixe. Méthode à
	 * utiliser avant tout ajout.
	 *
	 * @return true si la pile est pleine (capacité atteinte), false sinon.
	 *
	 * @ensures (\result <==> (getNbElements() == getNbMaxElements()));
	 *
	 * @pure
	 *
	 */
	public boolean estPleine() {
		return (nbElements == nbMaxElements);
	}

	/**
	 * La pile est-elle vide ? Méthode à utiliser avant toute
	 * suppression ou consultation.
	 *
	 * @return true si la pile est vide, false sinon.
	 *
	 * @ensures (\result == (getNbElements() == 0));
	 *
	 * @pure
	 */
	public boolean estVide() {
		return (nbElements == 0);
	}

	/**
	 * Compare cette <code>PileInt</code> avec l'objet spécifié. Le résultat est
	 * <code>true</code> si et seulement si l'objet spécifié est une instance de
	 * <code>PileInt</code> contenant les mêmes éléments que cette
	 * <code>PileInt</code> et dont le nombre maximal d'éléments est le même que
	 * cette <code>PileInt</code>.
	 *
	 * @param o l'objet à comparer avec cette <code>PileInt</code>.
	 *
	 * @return <code>true</code> si les deux objets contiennent les mêmes éléments,
	 *         <code>false</code> sinon.
	 *
	 * @also
	 * @ensures !(o instanceof PileInt) ==> !\result;
	 * @ensures \result ==> ((o instanceof PileInt)
	 *  && (this.getNbElements() == ((PileInt) o).getNbElements())
	 *  && (this.getNbMaxElements() == ((PileInt) o).getNbMaxElements())
	 *  && (this.getSommet() == ((PileInt) o).getSommet()));
	 * @ensures \result ==> (this.hashCode() == o.hashCode());
	 * @ensures \result ==> (this.toString().equals(o.toString()));
	 *
	 *
	 * @pure
	 */
	public boolean equals(Object o) {
		if (!(o instanceof PileInt)) {
			return false;
		}
		PileInt p = (PileInt) o;
		if (getNbElements() != p.getNbElements()) {
			return false;
		}
		if (getNbMaxElements() != p.getNbMaxElements()) {
			return false;
		}
		for (int i = 0; i < getNbElements(); i++) {
			if (contenu[i] != p.contenu[i]) {
				return false;
			}
		}
		return true;
	}

	/**
	 * Renvoie une copie conforme de cette <code>PileInt</code>.
	 *
	 * @return un clone de cette instance.
	 *
	 * @also
	 * @ensures \result != null;
	 * @ensures \result != this;
	 * @ensures \result.getClass() == \result.getClass();
	 * @ensures this.equals(\result);
	 *
	 * @pure
	 */
	public Object clone() {
		Object o = null;
		try {
			o = super.clone();
		} catch (CloneNotSupportedException e) {
			throw new InternalError("Erreur lors du clonage");
		}
		PileInt p = (PileInt) o;
		p.contenu = contenu.clone();
		return p;
	}

	/**
	 * Renvoie un code de hashage pour cette instance.
	 *
	 * @return un code de hashage pour cette instance.
	 *
	 * @pure
	 */
	public int hashCode() {
		int code = (getNbElements() * 31) + getNbMaxElements();
		for (int i = 0; i < getNbElements(); i++) {
			code = (31 * code) + contenu[i];
		}
		return code;
	}

	/**
	 * Renvoie une représentation de cette instance sous forme de chaîne de
	 * caractère.
	 *
	 * @return une chaîne de caractères représentant cette instance.
	 *
	 * @also
	 * @ensures \result != null;
	 *
	 * @pure
	 */
	public String toString() {
		String str = "PileInt(" + getNbElements() + "/" + getNbMaxElements() + "):[";
		for (int i = 0; i < getNbElements(); i++) {
			str = str + contenu[i] + " ";
		}
		return str + "]";
	}
}
