public class Soin{
    
    private String nom;
    private int val;

    /**
     * initialise une nouvelle instance de Soin Dont le nom
     * et la valeur sont les valeurs spécifiées
     * @param nom le nom de cc soin
     * @param valeur la valeur du nouveau soin 
     * @requires nom != null;
     * @requires valeur >= 0;
     * @ensures getNom().equals(nom);
     * @ensures getValeur()==valeurs;
     */
     
    public Soin (String nom, int val){
        this.nom = nom;
        this.val = val;
    }

    public String getNom (){
        return this.nom;
    }

    public int getVal() {
        return this.val;
    }

    public boolean equals(Object o){
        if(!(o instanceof Soin)) return false;
        Soins s = (Soin) o;
        if(!(this.nom.equals(s.getNom))) return false;
        if(!(this.val.equals(s.getVal))) return false;
        return true;
    }
}