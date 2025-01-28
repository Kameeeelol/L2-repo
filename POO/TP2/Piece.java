


public class Piece extends ArrayListConteners{

    private String description;
    private Piece Sud;
    private Piece Nord;
    private Piece Est;
    private Piece Ouest;
    public Piece (String description){
        if((description==null)||description.isBlank()) throw new NullPointerException("la description ne doit pas etre vide ou null");
        this.description = description;
    }

    public void affiche(){
        System.out.println(getDescription()+","+getNbObjet()+", contenant les objets :");
        for (ObjetZork e : getLesObjets()) e.affiche();
    }

    
    public String getDescription(){
        return this.description;
    }

    public Piece getpieceSud(){
        return Sud;
    }
    public Piece getpieceNord(){
        return Nord;
    }
    public Piece getpieceEst(){
        return Est;
    }
    public Piece getpieceOuest(){
        return Ouest;
    }
    
    public boolean ajoutPossible(ObjetZork o){
        if(o==null) throw new NullPointerException("l'objet est null");
        return true;
    }
   
    public void setNord(Piece p){
        if(p==null) throw new NullPointerException("la pièce est null");
        Nord=p;
    }
    public void setSud(Piece p){
        if(p==null) throw new NullPointerException("la pièce est null");
        Sud=p;
    }
    public void setEst(Piece p){
        if(p==null) throw new NullPointerException("la pièce est null");
        Est=p;
    }
    public void setOuest(Piece p){
        if(p==null) throw new NullPointerException("la pièce est null");
        Ouest=p;
    }
}