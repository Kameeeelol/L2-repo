

public class Joueur extends ArrayListConteners{
    private String nom;
    private Piece pieceAct;
    private int capaciteTrans;

    // constrcuteurs
    public Joueur(String nom, Piece pieceAct, int capaciteTrans){
        if(pieceAct==null) throw new NullPointerException("la pièce est null");
        if((nom==null)||nom.isBlank()) throw new NullPointerException("la description ne doit pas etre vide ou null");
        if(capaciteTrans <= 0) throw new IllegalArgumentException("capacité de transport doit etre sup a 0");
        this.nom = nom;
        this.pieceAct = pieceAct;
        this.capaciteTrans = capaciteTrans;
    }

    public String getNom() {
        return nom;
    }
    
    public String getPiece() {
        return pieceAct.getDescription();
    }

    public void ChangerPiece(Piece NouvellePiece){
        if(NouvellePiece==null) throw new NullPointerException("la pièce est null");
        this.pieceAct = NouvellePiece;
    }

    public int getCapaciteTrans(){
        return capaciteTrans;
    }


    public int PoidsCourant(){
        int res = 0;
        for(ObjetZork o : getLesObjets()){
            res += o.getPoids();
        }
        return res;
    }

    

    public void affiche(){
        System.out.println("Nom du joueur : " + getNom());
        System.out.println("Pièce actuelle : " + getPiece());
        System.out.println("capacité de transport du joueur : " + getCapaciteTrans());
        System.out.println("Poids total transporté par le joueur : " + PoidsCourant());
        System.out.println("Le joueur transport : ");
        for (ObjetZork e : getLesObjets() ) e.affiche();
    }

    public boolean ajoutPossible(ObjetZork o ){
        if(o==null) throw new NullPointerException("l'objet est null");
        return PoidsCourant()+o.getPoids() <= getCapaciteTrans();
    }

}
