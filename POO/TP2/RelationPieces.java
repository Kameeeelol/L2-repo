import java.time.Period;
import java.util.HashSet;

public class RelationPieces implements Cloneable{
    
    private HashSet<Couple<Piece,Piece>> relations;
    
    public RelationPieces(){
        this.relations = new HashSet<Couple<Piece,Piece>>();
    }

    public boolean add(Piece p1, Piece p2){
        Couple<Piece,Piece> c = new Couple<Piece,Piece>(p1, p2);
        return relations.add(c);
    }

    public boolean contient(Piece p1, Piece p2){
        Couple<Piece,Piece> c = new Couple<Piece,Piece>(p1, p2);
        Couple<Piece,Piece> c2 = new Couple<Piece,Piece>(p2, p1);
        return relations.contains(c) || relations.contains(c2);
    }

    public HashSet<Piece> Voisins(Piece p){
        HashSet<Piece> res = new HashSet<Piece>();
        for( Couple<Piece,Piece> c : relations){
            if(c.getFirst() == p) res.add(c.getSecond());
            if(c.getSecond()==p) res.add(c.getFirst());
        }
        return res;
    }

    public boolean remove(Piece p1, Piece p2){
        Couple<Piece,Piece> c = new Couple<Piece,Piece>(p1, p2);
        return relations.remove(c);
    }

    public RelationPieces clone(){
        RelationPieces leClone = null;
        try {
            leClone = (RelationPieces) super.clone();
        } catch (CloneNotSupportedException e) {
            throw new InternalError();
        }
        leClone.relations = (HashSet<Piece,Piece>) relations.clone();
        return leClone;
    }
}
