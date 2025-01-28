import java.util.ArrayList;
import java.util.Collection;

public abstract class ArrayListConteners {
    
    private ArrayList<ObjetZork> lesObjets;

    public ArrayListConteners(){
       this.lesObjets = new ArrayList<ObjetZork>(); 
    }

    public ArrayListConteners(Collection <ObjetZork> l){
        this();
        if(l==null) throw new NullPointerException("la collection passé est null");
        for (ObjetZork o : l) ajouterObjet(o);
    }

    public ArrayList<ObjetZork> getLesObjets(){
        return lesObjets;
    }

    public int getNbObjet(){
        return lesObjets.size();
    }

    public boolean contient(ObjetZork o){
        if(o==null) throw new NullPointerException("l'objet est null");
        return lesObjets.contains(o);
    }

    public int contientCbDe(ObjetZork obj){
        if(obj==null) throw new NullPointerException("l'objet est null");
        int i =0;
        for(ObjetZork o : lesObjets){
            if (o.equals(obj)) i++;
        }
        return i;
    }

    public ObjetZork[] getTabObjet(){
        return lesObjets.toArray(new ObjetZork[0]);
    }

    public boolean retirerObjet(ObjetZork obj){
        if(obj==null) throw new NullPointerException("l'objet est null");
        return lesObjets.remove(obj);
    }

    public abstract boolean ajoutPossible(ObjetZork obj);

    public void ajouterObjet(ObjetZork o){
        if(o==null) throw new NullPointerException("l'objet est null");
        if (ajoutPossible(o)) lesObjets.add(o);
        if(!ajoutPossible(o)) throw new IllegalArgumentException("l'objet est trop lourd pur l'ajouter au joueur");
    }

}
