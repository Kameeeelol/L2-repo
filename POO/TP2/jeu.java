public class jeu {

    public static void main(String[] args) {
        ObjetZork lampe = new ObjetZork("lampe", 1, true);
        ObjetZork epée = new ObjetZork("Epée", 3, true);
        ObjetZork roche = new ObjetZork("Roche", 50, false);
        ObjetZork roche2 = new ObjetZork("Roche", 50, false);
        lampe.affiche();
        epée.affiche();
        roche.affiche();
        roche2.affiche();
        Piece cuisine = new Piece("cuisine"); 
        Piece salledebain = new Piece("salle de bain");
        Piece salon = new Piece("salon"); 
        Piece chambre_1 = new Piece("chambre1");
        Piece chambre_2 = new Piece("chambre2");
        cuisine.setEst(chambre_2);
        cuisine.setNord(salon);
        cuisine.setOuest(chambre_1);
        cuisine.setSud(salledebain);
        salledebain.setNord(cuisine);
        salon.setSud(cuisine);
        chambre_1.setEst(cuisine);
        chambre_2.setOuest(cuisine);
        cuisine.affiche();
        cuisine.ajouterObjet(roche2);
        cuisine.ajouterObjet(lampe);
        cuisine.affiche();
        cuisine.retirerObjet(roche2);
        cuisine.affiche();
        System.out.println("La lampe et la roche sont égales (egalité) ? " + lampe.egalité(roche));
        System.out.println("La roche et la roche2 sont égales (egalité) ? " + roche2.egalité(roche));
        System.out.println("La lampe et la roche sont égales (equals) ? " + lampe.equals(roche));
        System.out.println("La roche2 et la roche sont égales (equals) ? " + roche2.equals(roche));
        Joueur joueur1 = new Joueur("Kamel", cuisine, 100);
        joueur1.affiche();
        joueur1.ajouterObjet(roche2);
        joueur1.ajouterObjet(roche2);
        joueur1.affiche();
        joueur1.ajouterObjet(epée);
        joueur1.retirerObjet(roche2);
        joueur1.ajouterObjet(epée);
        joueur1.affiche();
    }
}