/* @author Kamel Seggari 12203821
* Je déclare qu'il s'agit de mon propre travail
*/

package minebay;
import java.util.AbstractCollection;
import java.util.Collection;
import java.util.LinkedList;
import java.util.ListIterator;
import java.util.Set;
import java.util.ArrayList;
/**
 * Collection ordonnée, dont les éléments sont instances de classes implémentant
 * les interfaces Categorized et Comparable. Une MultiEnumList ne peut pas
 * contenir null.
 * 
 * <p>
 * Une MultiEnumList prend en compte les catégories de ses éléments en
 * définissant pour certaines méthodes des versions alternatives opérant
 * uniquement sur un ensemble de catégories donné en argument de ces méthodes.
 * Cependant les méthodes héritées de l'interface Collection s'appliquent
 * toujours sur l'ensemble des éléments quelque soit leur catégorie.
 * </p>
 * 
 * <p>
 * Dans ce but, MultiEnumList utilise une liste distincte (une LinkedList) pour
 * chaque catégorie. Les opérations utilisant un index (i.e. les deux méthodes
 * get), parcourent la liste depuis le début à l'aide d'un itérateur (renvoyé
 * par la méthode iterator) jusqu'à atteindre l'élèment à cet index, en
 * conséquence, pour des raisons d'efficacité, l'usage d'un itérateur doit être
 * préféré à chaque fois qu'un parcours de la liste doit être effecué. Les
 * méthodes iterator et listIterator construisent l'itérateur renvoyé en
 * fusionnant les itérateurs des listes des catégories sélectionnées à l'aide
 * d'un FusionSortedIterator.
 * </p>
 * 
 * @invariant (\forall int i, j; i >= 0 && i < j && j < size();
 *            get(i).compareTo(get(j)) <= 0);
 * @invariant (\forall Set<C> catSet;true; <br/>
 *            (\forall int i, j; i >= 0 && i < j && j < size(catSet); <br/>
 *            get(catSet, i).compareTo(get(catSet, j)) <= 0);
 * @invariant !contains(null);
 * @invariant getCatType() != null;
 * 
 * @author Marc Champesme
 * @since 27/09/2024
 * @version 8/12/2024
 */
public class MultiEnumList<C extends Enum<C>, E extends Categorized<C> & Comparable<E>> extends AbstractCollection<E>
		implements Cloneable {
	private LinkedList<E>[] listes;
	private Class<C> cats;
	/**
	 * Initialise une MultiEnumList vide dont les éléments sont catégorisés à l'aide
	 * du type spécifié.
	 * 
	 * @param catType le type enum permettant de catégoriser les éléments de cette
	 *                MultiEnumList
	 * 
	 * @requires catType != null;
	 * @ensures isEmpty();
	 * @ensures getCatType().equals(catType);
	 * 
	 * @throws NullPointerException si l'argument spécifié est null
	 */
	public MultiEnumList(Class<C> catType) {
		super();
		if ( catType == null ) throw new NullPointerException("Catégorie passé en argument est null");
		cats = catType;
		listes = (LinkedList<E>[]) new LinkedList[cats.getEnumConstants().length];
		for (int i = 0; i < listes.length; i++) {
            listes[i] = new LinkedList<>();
        }
	}

	/**
	 * Initialise une MultiEnumList contenant les éléments de la collection
	 * spécifiée dont les éléments sont catégorisés à l'aide du type spécifié.
	 * 
	 * @param catType le type enum permettant de catégoriser les éléments de cette
	 *                MultiEnumList
	 * @param c       la collection dont les élèments doivent être placés dans cette
	 *                nouvelle MultiEnumList
	 * 
	 * @requires catType != null;
	 * @requires c != null;
	 * @requires !c.contains(null);
	 * @ensures containsAll(c);
	 * @ensures size() == c.size();
	 * @ensures getCatType().equals(catType);
	 * 
	 * @throws NullPointerException si un des arguments spécifiés est null ou si la
	 *                              collection spécifié contient null
	 */
	public MultiEnumList(Class<C> catType, Collection<? extends E> c) {
		this(catType);
		if ( c == null || c.contains(null)) throw new NullPointerException("La collection est null ou contient null");
		for ( E elem : c){
			this.add(elem);
		}
	}

	/**
	 * Renvoie le type enum catégorisant les élèments de cette collection.
	 * 
	 * @return le type enum catégorisant les élèments de cette collection
	 * 
	 * @pure
	 */
	public Class<C> getCatType() {
		return this.cats;
	}

	@Override
	public int size() {
		int tmp = 0;
		for (LinkedList<E> l : listes) {
            tmp += l.size();
        }
        return tmp;
	}

	/**
	 * Renvoie le nombre d'élèments de cette collection appartenant à une des
	 * catégories de l'ensemble spécifié.
	 * 
	 * @param catSet ensemble de catégories
	 * 
	 * @return le nombre d'élèments de cette collection appartenant à une des
	 *         catégories de l'ensemble spécifié
	 * 
	 * @requires catSet != null;
	 * @ensures \result >= 0 && \result <= size();
	 * @ensures catSet.isEmpty() ==> \result == 0;
	 * @ensures catSet.equals(EnumSet.allOf(getCatType())) ==> \result == size();
	 * 
	 * @throws NullPointerException si l'ensemble spécifié est null ou contient null
	 * 
	 * @pure
	 */
	public int size(Set<? extends C> catSet) {
		if ( catSet == null || catSet.contains(null)) throw new NullPointerException("L'argument est null ou contient null");
		int i = 0;
		for ( C cat : catSet){
			i += listes[cat.ordinal()].size();
		}
		return i;
	}

	@Override
	public boolean remove(Object obj) {
		return super.remove(obj);
	}

	@Override
	public boolean contains(Object obj) {
		return super.contains(obj);
	}

	@Override
	public void clear() {
		super.clear();
	}

	/**
	 * Retire tous les élèments de cette collection dont la catégorie appartient à
	 * une des catégories de l'ensemble spécifié. Si l'ensemble spécifié est vide
	 * cette collection n'est pas modifiée.
	 * 
	 * @param catSet ensemble de catégories auxquelles appartiennent les éléments à
	 *               retirer de cette collection
	 * 
	 * @requires catSet != null;
	 * @ensures size(catSet) == 0;
	 * @ensures catSet.isEmpty() ==> size() == \old(size());
	 * @ensures catSet.equals(EnumSet.allOf(getCatType())) ==> isEmpty();
	 * @ensures (\forall E elt; catSet.contains(elt.getCategory()) &&
	 *          \old(contains(elt)); !contains(elt));
	 * @ensures (\forall E elt; !catSet.contains(elt.getCategory()) &&
	 *          \old(contains(elt)); contains(elt));
	 * 
	 * @throws NullPointerException si l'ensemble spécifié est null ou contient null
	 */
	public void clear(Set<? extends C> catSet) {
		if ( catSet == null || catSet.contains(null)) throw new NullPointerException("L'argument est null ou contient null");
		if ( catSet.isEmpty()) return;
		for ( C cat : catSet){
			listes[cat.ordinal()].clear();
		}
	}

	/**
	 * Renvoie l'élèment situé à la position spécifiée dans cette collection.
	 * 
	 * @param i index de l'élèmet à renvoyer
	 * @return l'élèment situé à la position spécifiée dans cette collection
	 * 
	 * @requires i >= 0 && i < size();
	 * @ensures contains(\result);
	 * @ensures (\forall int j; j >= 0 && j < i; get(j).compareTo(\result) <= 0);
	 * @ensures (\forall int j; j > i && j < size(); get(j).compareTo(\result) >=
	 *          0);
	 * 
	 * @throws IndexOutOfBoundsException si l'index spécifié est strictement
	 *                                   inférieur à 0 ou supérieur ou égal à size()
	 * 
	 * @pure
	 */
	public E get(int i) {
		if (i < 0 || i >= size()) throw new IndexOutOfBoundsException("Index hors des limites de la collection."); 
    	int currentIndex = 0;
		ListIterator<E> iter = iterator();
		while(currentIndex<i){
			iter.next();
			currentIndex++;
		}
		return iter.next();
	}

	/**
	 * Renvoie l'élèment situé à la position spécifiée parmi les élèments de cette
	 * collection dont la catégorie appartient à l'ensemble spécifié.
	 * 
	 * @param i      index de l'élèmet à renvoyer
	 * @param catSet ensemble des catégories
	 * 
	 * @return l'élèment situé à la position spécifiée parmi les élèments dont la
	 *         catégorie appartient à l'ensemble spécifié
	 * 
	 * @requires i >= 0 && i < size(catSet);
	 * @requires !catSet.contains(null);
	 * @ensures contains(\result);
	 * @ensures catSet.contains(\result.getCategory());
	 * @ensures (\forall int j; j >= 0 && j < i; get(catSet, j).compareTo(\result)
	 *          <= 0);
	 * @ensures (\forall int j; j > i && j < size(catSet); get(catSet,
	 *          j).compareTo(\result) >= 0);
	 * 
	 * @throws IndexOutOfBoundsException si l'index spécifié est strictement
	 *                                   inférieur à 0 ou supérieur ou égal à size()
	 * @throws NullPointerException      si l'ensemble spécifié est null ou contient
	 *                                   null
	 * 
	 * @pure
	 */
	public E get(Set<? extends C> catSet, int i) {
		if (catSet == null || catSet.contains(null)) throw new NullPointerException("L'ensemble des catégories ne doit pas être null ou contenir null.");
		if (i < 0 || i >= size(catSet)) throw new IndexOutOfBoundsException("Index hors des limites.");
		ListIterator<E> iter = listIterator(catSet);
        int currentIndex = 0;
		while (currentIndex < i) {
			iter.next();
			currentIndex++;
		}
		return iter.next();
	}

    

	/**
	 * Ajoute l'élèment spécifié à cette collection en préservant l'ordre des
	 * élèments.
	 * 
	 * @param elt l'élèment à ajouter à cette collection
	 * 
	 * @return toujours true
	 * 
	 * @requires elt != null;
	 * @ensures \result == true;
	 * @ensures contains(elt);
	 * @ensures size() == \old(size()) + 1;
	 * @ensures size(EnumSet.of(elt.getCategory())) ==
	 *          \old(size(EnumSet.of(elt.getCategory()))) + 1;
	 * 
	 * @throws NullPointerException si l'élèment spécifié est null
	 */
	@Override
	public boolean add(E elt) {
		if (elt == null) throw new NullPointerException("Le paramètre passé est null");
		LinkedList<E> list = listes[elt.getCategory().ordinal()];
        ListIterator<E> iter = list.listIterator();
        while (iter.hasNext()) {
            if (elt.compareTo(iter.next()) <= 0) {
                iter.previous();
                break;
            }
        }
    	iter.add(elt);
        return true;
	}

	/**
	 * Renvoie un ListIterator sur les éléments de cette collection dont la
	 * catégorie appartient à l'ensemble spécifié. Cet itérateur respect l'ordre
	 * naturel des élèments.
	 * 
	 * @implSpec L'itérateur renvoyé est construit en fusionnant des itérateurs de
	 *           chacune des listes des catégories sélectionnées. Aucune nouvelle
	 *           liste n'est crée.
	 * 
	 * @param catSet ensemble de catégories
	 * 
	 * @return un ListIterator sur les éléments de cette collection dont la
	 *         catégorie appartient à l'ensemble spécifié
	 * 
	 * @requires catSet != null;
	 * @ensures \result != null;
	 * @ensures ListIterObserverAdapter.containsAll(\result, this);
	 * @ensures ListIterObserverAdapter.size(\result) == size(catSet);
	 * 
	 * @throws NullPointerException si l'ensemble spécifié est null ou contient null
	 * 
	 * @pure
	 */
	public ListIterator<E> listIterator(Set<? extends C> catSet) {
		if ((catSet == null) || (catSet.contains(null))) throw new NullPointerException("catSet est null ou contient null");
		ArrayList<ListIterator<E>> myList = new ArrayList<>();
		for (C cat : catSet) {
            myList.add(listes[cat.ordinal()].listIterator(0));
        }
		return new FusionSortedIterator<>(myList);
	}

	/**
	 * Renvoie un ListIterator sur tous les éléments de cette collection. Cet
	 * itérateur respect l'ordre naturel des élèments.
	 * 
	 * @implSpec L'itérateur renvoyé est construit en fusionnant des itérateurs de
	 *           chacune des catégories. Aucune nouvelle liste n'est crée.
	 * 
	 * @return un ListIterator sur les éléments de cette collection dont la
	 *         catégorie appartient à l'ensemble spécifié
	 * 
	 * @requires catSet != null;
	 * @ensures \result != null;
	 * @ensures containsAll(ListIterObserverAdapter.toList(\result));
	 * @ensures ListIterObserverAdapter.size(\result) == size();
	 * @ensures ListIterObserverAdapter.isSorted(\result);
	 * 
	 * @pure
	 */
	@Override
	public ListIterator<E> iterator() {
		ArrayList<ListIterator<E>> myList = new ArrayList<>();
		for (LinkedList<E> l : listes) {
            myList.add(l.listIterator(0));
        }
		return new FusionSortedIterator<>(myList);
	}

	/**
	 * Compare l'objet spécifié avec cette collection en terme d'égalité. Renvoie
	 * true si l'objet spécifié est une MultiEnumList contenant les mêmes éléments
	 * dans le même ordre que cette collection.
	 * 
	 * @param obj l'objet à comparer avec cette collection en terme d'égalité
	 * 
	 * @return true si l'objet spécifié est une MultiEnumList contenant les mêmes
	 *         éléments que cette collection
	 * 
	 * @ensures !(obj instanceof MultiEnumList<?,?>) ==> !\result;
	 * @ensures !getCatType().equals(((MultiEnumList<?,?>) obj).getCatType()) ==>
	 *          !\result;
	 * @ensures \result ==> size() == ((MultiEnumList<?,?>) obj).size();
	 * @ensures \result ==> (\forall int i; i >= 0 && i < size();
	 *          get(i).equals(((MultiEnumList<?,?>) obj).get(i)));
	 * 
	 * @pure
	 */
	@Override
	public boolean equals(Object obj) {
		if (!(obj instanceof MultiEnumList<?, ?> tmp)) return false;
		if (!(cats.equals(tmp.cats))) return false;
		if (size() != tmp.size()) return false;
		for (int i = 0; i < size(); i++) {
			if (!get(i).equals(tmp.get(i))) {
					return false;
			}
		}
		return true;
	}

	/**
	 * Returns the hash code value for this MultiEnumList. The hash code of a
	 * MultiEnumList is defined to be the result of the following calculation:
	 * 
	 * <pre>{@code
	 * int hashCode = 1;
	 * for (E e : list)
	 * 	hashCode = 31 * hashCode + (e == null ? 0 : e.hashCode());
	 * }</pre>
	 * 
	 * @return the hash code value for this list
	 * 
	 * @pure
	 */
	@Override
	public int hashCode() {
		int hashCode = 99;
		for (E e : this) hashCode = 15 * hashCode + (e == null ? 0 : e.hashCode());
		return hashCode;
	}

	/**
	 * Renvoie un clone de cette MultiEnumList. Chacune des listes composant cette
	 * MultiEnumList est clonée.
	 * 
	 * @return un clone de cette MultiEnumList
	 * 
	 * @ensures \result.getClass().equals(getClass());
	 * @ensures \result != this;
	 * @ensures \result.equals(this);
	 * 
	 * @pure
	 */
	@Override
	public MultiEnumList<C, E> clone() {
		MultiEnumList<C, E> leClone;
		try {
           leClone = (MultiEnumList<C, E>) super.clone();
        } catch (CloneNotSupportedException e) {
            throw new InternalError();
        }
		leClone.listes = listes.clone();
		for (int i = 0; i < listes.length; i++) {
            leClone.listes[i] = (LinkedList<E>) this.listes[i].clone();
        }
	return leClone;
	}
}
