public class ObjetZork {

    private String description;
    private int poids;
    private boolean transportable;
    public ObjetZork(String description, int poids, boolean transportable){
        if((description==null)||description.isBlank()) throw new NullPointerException("la description ne doit pas etre vide ou null");
        if (poids<=0) throw new IllegalArgumentException("argument doit etre sup a 0");
        this.description = description;
        this.poids = poids;
        this.transportable = transportable;
    }
    public String getDescription(){
        return this.description;
    }
    public int getPoids(){
        return this.poids;
    }
    public boolean EstTransportable(){
        return this.transportable;
    }
    public boolean egalité(ObjetZork o){
        if(o==null) throw new NullPointerException("l'objet est null");
        if(description == o.getDescription() && poids == o.getPoids() && transportable == o.EstTransportable() )
            return true; 
        else return false;
    }
    public void affiche(){
        System.out.print(description + " qui pèse : " + poids + " kg");
        if (transportable) System.out.println(", transportable");
        else System.out.println(", non transportable");
    }
    public boolean equals(Object o){
        if(o==null) throw new NullPointerException("l'objet est null");
        if (! (o instanceof ObjetZork))return false;
        //else return true;
        ObjetZork oz = (ObjetZork) o;
        return egalité(oz);
    }
}
    