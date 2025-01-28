import java.util.ArrayList;

public class Medecin{
    private String nom;
    private ArrayList<Soin> LesSoins;
    private int nbSoins;

    public Medecin(String nom, ArrayList<Soin> LesSoins, int nbSoins){
        this.nom = nom;
        this.LesSoins = LesSoins;
        this.nbSoins = nbSoins;
    }

    public String getNom(){
        return this.nom;
    }

    public boolean reserveEstVide(){
        if ( this.nbSoins == 0) return true;
        return false;
    }

    public int getNbSoins(){
        return this.nbSoins;
    }

    public Soin prochainSoin(){
        if(lesSoins.reserveEstVide()) return null;
        return LesSoins.get(0);
    }

    public ArrayList<Soin> getTabSoins(){
        return this.LesSoins;
    }

    public boolean soigne(Joueur unJoueur){
        if (!(unJoueur.estVivant())) return false;
        if (LesSoins.reserveEstVide()) return false;
        Soin unSoin = prochainSoin();
        unJoueur.recoitSoin(unSoin);
        LesSoins.remove(unSoin);
        return true;
    }

    public boolean possede(Soin soin){
        if( LesSoins.contains(soin)) return true;
        return false;
    }

    public int possedeCombienDe(Soin soin){
        if (soin == null) return 0;
        int nb_soins=0;
        for ( Soin e : LesSoins){
            if ( e.equals(soin)) nb_soins += 1;
        }
        return nb_soins;
    }

    public void ajoute(Soin unSoin){
        LesSoins.add(unSoin);
    }

    public int getPuissance(){
        if(lesSoins.reserveEstVide()) return 0;
        int puissance = 0;
        for( Soins e : lesSoins){
            puissance += e.getVal();
        }
        return puissance;
    }

    public boolean equals(Object obj){
        if(!(obj instanceof Medecin)) return false;
        Medecin med = (Medecin) obj;
        if (!(this.getNom().equals(med.getNom()))) return false;
        int i=0;
        for( Soin e : lesSoins){
            if(!(e.equals(med.LesSoins(i)))){
                return false;
            }else{
                i++;
            }
        }
        return true; 
    }

    public Medecin clone(){
        Medecin m = null;
        try{
            m = (Medecin) super.clone();
        } catch (CloneNotSupportedException e){
            throw new internalError();
        }
        m.mesSoins = new ArrayList<Soin>(mesSoins);
        return m;
    }

    public int hashcode(){
        return nom.hashCode() * getNbSoins();
    }

    public String toString(){
        return "Medecin: "+ getNom() + getTabSoins();
    }

}