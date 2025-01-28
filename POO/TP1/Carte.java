public class Carte {
	private Famille famille;
	private Rang rang;
	public Carte() {
		famille = Famille.TREFLE;
		rang = Rang.VALET;
	}
	public Carte (Famille famille, Rang rang) {
		this.famille = famille;
		this.rang = rang;
	}
	public Famille getFamille() {
		return famille;
	}
	public Rang getRang() {
		return rang;
	}
	public Couleur getCouleur() {
		if ((famille == Famille.TREFLE) || (famille == Famille.PIQUE))
			return Couleur.NOIR;
		return Couleur.ROUGE;
	} 
} 
