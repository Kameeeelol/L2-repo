public class Jeu {
	public static void main (String[] args) {
		Carte carte, carte2;
	}
	carte = new Carte();
	carte2 = new Carte(Famille.PIQUE, RANG.DIX);
	System.out.println(carte2.getRang() + "," + carte2.getFamille());
	System.out.println(carte.getRang() + "+" + carte.getFamille());
}