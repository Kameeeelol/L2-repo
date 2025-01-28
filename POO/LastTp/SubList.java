import java.util.List;

public class SubList<E> implements List<E>{

    private List<E> supportList;
    private int fromIndex;
    private int toIndex;

    public SubList(List<E> list, int i, int j){
        if (i < 0 && j > supportList.size() || i > j ){
            throw new IndexOutOfBoundsException();
        }
        supportList = list;
        fromIndex = i;
        toIndex = j;
    }

    public int size(){
        return toIndex - fromIndex;
    }

    public List<E> getSupportList(){
        return supportList;
    }

    public int startInSupportList(){
        return this.fromIndex;
    }

    public int endInSupportList(){
        return this.toIndex;
    }

    public boolean add(E elt){
        supportList.add(toIndex,elt);
        toIndex++;
        return true;
    }

    public void add(int index , E elt){
        if(index < 0 || index > size()){
            throw new IndexOutOfBoundsException();
        }
        supportList.add(fromIndex+index, elt);
        toIndex++;
    }






    // toutes les méthodes de List (y compris méthodes de collection)
    // sauf les méthodes statiques 

}