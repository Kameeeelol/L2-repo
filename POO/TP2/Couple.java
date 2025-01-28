

public class Couple<E, F>{
    
    private E first;
    private F second;

    public Couple(E first, F second){
        if (first == null || second == null ) throw new NullPointerException("les deux argumets sont nulls");
        this.first = first;
        this.second = second;
    }

    public E getFirst(){
        return this.first;
    }


    public F getSecond(){
        return this.second;
    }

    public boolean equals(Object o){
        if(!(o instanceof Couple<?,?>)) return false;
        Couple<?,?> cpl = (Couple<?,?>) o;
        return getFirst().equals(cpl.getFirst()) && getSecond().equals(cpl.getSecond());
    }

    public int hashcode(){
        int code = getFirst().hashCode() + 17 ;
        code *= getSecond().hashCode();
        return code;
    }

    public String toString(){
        return "le couple ("+getFirst() +","+ getSecond()+")";
    }
}