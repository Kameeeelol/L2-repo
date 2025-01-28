/* @author Kamel Seggari 12203821
* Je déclare qu'il s'agit de mon propre travail
*/
package minebay;

import java.util.Collection;
import java.util.Comparator;
import java.util.LinkedList;
import java.util.ListIterator;
import java.util.NoSuchElementException;

/**
 * Un ListIterator fusionnant plusieurs ListIterator ordonnés en interdisant les
 * opérations de modification add et set.
 *
 * Un FusionSortedIterator garantie que, si les ListIterator fusionnés sont
 * ordonnés, alors ce FusionSortedIterator sera également ordonné.
 *
 * Par défaut, l'ordre considéré est l'ordre naturel entre les éléments,
 * cependant un ordre alternatif peut-être spécifié à la création de l'instance.
 * 
 * @param <E> le type des éléments énumérés par cet itérateur
 * @param <I> le type des itérateurs fusionnés
 *
 * @model ListIterObserver<E> iterModel = new ListIterObserverAdapter<E>(this);
 * @invariant nextIndex() == previousIndex() + 1;
 * @invariant previousIndex() >= -1 && previousIndex() < iterModel.size());
 * @invariant nextIndex() >= 0 && nextIndex() <= iterModel.size());
 * @invariant !hasPrevious() <==> previousIndex() == -1;
 * @invariant !hasNext()() <==> nextIndex() == iterModel.size();
 * @invariant lastIndex() == nextIndex() || lastIndex() == previousIndex() ||
 *            lastIndex() == -1;
 * @invariant lastIndex() >= -1 && lastIndex() < iterModel.size());
 * @invariant !iterModel.contains(null);
 * @invariant comparator() != null;
 * @invariant iterModel.isSorted(comparator());
 * 
 * @author Marc Champesme
 * @since 2/08/2023
 * @version 8/12/2024
 *
 */
public class FusionSortedIterator<E extends Comparable<? super E>> implements ListIterator<E> {
	// Variable permettant de savoir quelle methode entre next() et previous() a été appelé en dernier
    // Elle prevent 3 valeurs: 1 pour next() ; -1 pour previous et 0 si aucune des deux
    private int lastCalled;

    // Variable permettant de stocker le dernier iterator sur lequel on a appelé next() ou previous()
    private int lastCalledIter;
	private final Comparator<? super E> comparator;
	private final LinkedList<ListIterator<E>> iterators;
	/**
	 * Initialise une instance permettant d'itérer selon l'ordre "naturel" sur tous
	 * les éléments des ListIterator de la collection spécifiée. Il s'agit donc
	 * d'une fusion de tous les ListIterator contenus dans la collection spécifiée.
	 * Les ListIterator spécifiés sont supposés ordonnés selon l'ordre "naturel" de
	 * leurs éléments.
	 *
	 * @param iters ensemble des ListIterator à fusionner
	 *
	 * @requires iters != null && !iters.contains(null);
	 * @ensures (\forall ListIterator<E> iter; iters.contains(iter);
	 *          iterModel.containsAll(toList(iter)));
	 * @ensures iterModel.size() == (\sum ListIterator<E> iter;
	 *          iters.contains(iter); size(iter));
	 * @ensures (\forall E p; iterModel.contains(p); (\exists ListIterator<E> iter;
	 *          iters.contains(iter); contains(iter, p)));
	 * @ensures !hasPrevious();
	 * @ensures lastIndex() == -1;
	 * @ensures comparator() != null;
	 *
	 * @throws NullPointerException si l'ensemble spécifié est null ou contient null
	 */
	public FusionSortedIterator(Collection<? extends ListIterator<E>> iters) {
		this(iters, Comparator.naturalOrder());
	}

	/**
	 * Initialise une instance permettant d'itérer sur tous les éléments des
	 * ListIterator de la collection spécifiée selon l'ordre spécifié. Il s'agit
	 * donc d'une fusion de tous les ListIterator contenus dans la collection
	 * spécifiée. les ListIterator contenus dans la collection spécifiée sont
	 * supposés ordonnés selon l'ordre induit par le Comparator spécifié.
	 *
	 *
	 * @param iters      collection des ListIterator à fusionner
	 * @param comparator le comparateur à utiliser
	 *
	 * @requires iters != null && !iters.contains(null);
	 * @requires comparator != null;
	 * @ensures comparator() != null;
	 * @ensures comparator().equals(comparator);
	 * @ensures !hasPrevious();
	 * @ensures lastIndex() == -1;
	 *
	 * @throws NullPointerException si l'ensemble spécifié est null ou contient
	 *                              null, ou si le Comparator spécifié est null
	 */
	public FusionSortedIterator(Collection<? extends ListIterator<E>> iters, Comparator<? super E> comparator) {
		if (iters == null || iters.contains(null)) throw new NullPointerException("La collection d'itérateurs ne peut pas être null ni contenir des éléments null.");
		if (comparator == null) throw new NullPointerException("Le comparateur ne peut pas être null.");
		this.iterators = new LinkedList<>(iters);
		this.comparator = comparator;
		this.lastCalledIter = -1;
        this.lastCalled = 0;
	}

	/**
	 * Renvoie le comparateur selon lequel les éléments de cet itérateur sont
	 * ordonnés.
	 * 
	 * @return le comparateur selon lequel les éléments de cet itérateur sont
	 *         ordonnés
	 * 
	 * @ensures \result != null;
	 * 
	 * @pure
	 */
	public Comparator<? super E> comparator() {
		return this.comparator;
	}

	/**
	 * Renvoie true s'il reste un élément après dans l'itération.
	 * 
	 * @return true s'il reste un élément après dans l'itération; false sinon
	 * 
	 * @ensures !\result <==> nextIndex() == iterModel.size();
	 * 
	 * @pure
	 */
	@Override
	public boolean hasNext() {
		for (ListIterator<E> iterator : this.iterators) {
			if (iterator.hasNext()) {
				return true;				
			}
		}	
		return false;
	}

	/**
	 * Renvoie l'élèment qui sera renvoyé par le prochain appel à next().
	 * 
	 * @return l'élèment qui sera renvoyé par le prochain appel à next()
	 * 
	 * @throws NoSuchElementException si l'itérateur n'a pas d'élément suivant
	 * 
	 * @requires hasNext();
	 * @ensures \result.equals(iterModel.get(nextIndex()));
	 * 
	 * @pure
	 */
	public E getNext() {
		if(! hasNext()) throw new NoSuchElementException("L'iterateur n'as pas d'éléments suivant");
		int tmpC = lastCalled;
        int tmpIter = lastCalledIter;
        E res = next();
	    previous();
	    lastCalled = tmpC;
        lastCalledIter = tmpIter;
		return res;
	}

	/**
	 * Renvoie l'élément suivant et avance le curseur.
	 *
	 * @return l'élément suivant
	 *
	 * @throws NoSuchElementException si l'itérateur n'a pas d'élément suivant
	 *
	 * @requires hasNext();
	 * @ensures \result != null;
	 * @ensures \result.equals(\old(getNext()));
	 * @ensures \result.equals(getPrevious());
	 * @ensures \result.equals(iterModel.get(previousIndex()))
	 * @ensures \old(hasPrevious()) ==> comparator().compare(\old(getPrevious()),
	 *          \result) <= 0;
	 * @ensures hasNext() ==> comparator().compare(\result, getNext()) <= 0;
	 * @ensures hasPrevious();
	 * @ensures previousIndex() == \old(nextIndex());
	 * @ensures nextIndex() == \old(nextIndex() + 1);
	 * @ensures lastIndex() == \old(nextIndex());
	 * @ensures lastIndex() == previousIndex();
	 */
	@Override
	public E next() {
		if(! hasNext()) throw new NoSuchElementException("L'iterateur n'as pas d'éléments suivant");
		E nextElement = null;
		int minIteratorIndex = -1;
    	for ( int i = 0; i < iterators.size(); i++){
			ListIterator<E> iter = iterators.get(i);
			if (iter.hasNext()){
				E tmp = iter.next();
				if (nextElement == null || comparator.compare(tmp, nextElement) < 0) {
					if (minIteratorIndex != -1) {
							iterators.get(minIteratorIndex).previous(); // Annuler le déplacement pour l'ancien minimum
					}
					nextElement = tmp;
					minIteratorIndex = i;
				}else{
					iter.previous();
				}
			}
		}
		lastCalled = 1;
		lastCalledIter = minIteratorIndex;
		return nextElement;
	}

	/**
	 * Renvoie true s'il y a un élément précédent dans l'itération.
	 * 
	 * @return true s'il y a un élément précédent dans l'itération; false sinon
	 * 
	 * @ensures !\result <==> previousIndex() == -1;
	 *
	 * @pure
	 */
	@Override
	public boolean hasPrevious() {
		for (ListIterator<E> l : iterators) {
			if (l.hasPrevious()) {
					return true;
			}
		}
	return false;
	}

	/**
	 * Renvoie l'élèment qui sera renvoyé par le prochain appel à previous().
	 * 
	 * @return l'élèment qui sera renvoyé par le prochain appel à previous()
	 * 
	 * @throws NoSuchElementException si l'itérateur n'a pas d'élément précédent
	 * 
	 * @requires hasPrevious();
	 * @ensures \result.equals(iterModel.get(previousIndex()));
	 * 
	 * @pure
	 */
	public E getPrevious() {
		if (! hasPrevious() ) throw new NoSuchElementException("L'iterateur n'as pas d'éléments précédent");
		int tmpC = lastCalled;
        int tmpIter = lastCalledIter;
		E res = previous();
        next();
		lastCalled = tmpC;
        lastCalledIter = tmpIter;
		return res;
	}

	/**
	 * Renvoie l'élément précédent et recule le curseur.
	 *
	 * @return l'élément précédent
	 *
	 * @throws NoSuchElementException si l'itérateur n'a pas d'élément précédent
	 *
	 * @requires hasPrevious();
	 * @ensures hasNext();
	 * @ensures \result != null;
	 * @ensures \result.equals(\old(getPrevious()));
	 * @ensures \result.equals(getNext());
	 * @ensures \result.equals(\old(iterModel.get(previousIndex())));
	 * @ensures \result.equals(iterModel.get(nextIndex()));
	 * @ensures \old(hasNext()) ==> comparator().compare(\result,
	 *          iterModel.get(\old(nextIndex())) <= 0;
	 * @ensures previousIndex() == \old(previousIndex()) - 1;
	 * @ensures nextIndex() == \old(previousIndex());
	 * @ensures lastIndex() == \old(previousIndex());
	 * @ensures lastIndex() == nextIndex();
	 */
	@Override
	public E previous() {
		if (! hasPrevious() ) throw new NoSuchElementException("L'iterateur n'as pas d'éléments précédent");
		E previousElement = null;
		int maxIteratorIndex = -1;
		for ( int i = 0; i<iterators.size(); i++){
			ListIterator<E> iter = iterators.get(i);
			if (iter.hasPrevious()){
				E tmp = iter.previous();
				if (previousElement == null || comparator.compare(tmp, previousElement) > 0) {
					if (maxIteratorIndex != -1) {
							iterators.get(maxIteratorIndex).next(); // Annuler le déplacement pour l'ancien maximum
					}
					previousElement = tmp;
					maxIteratorIndex = i;
				}else{
					iter.next();
				}
			}
		}
		lastCalled = -1;
		lastCalledIter = maxIteratorIndex;
		return previousElement;
	}

	/**
	 * Renvoie l'index de l'élément suivant dans l'itération. Renvoie le nombre
	 * total d'élément dans l'itération s'il n'y a pas d'élément suivant.
	 * 
	 * @return l'index de l'élément suivant dans l'itération
	 * 
	 * @ensures hasNext() <==> \result >= 0 && \result < iterModel.size();
	 * @ensures !hasNext() <==> \result == iterModel.size();
	 * 
	 * @pure
	 */
	@Override
	public int nextIndex() {
		int tmp = 0;
		for (ListIterator<E> l : iterators) {
            tmp += l.nextIndex();
        }
		return tmp;
	}

	/**
	 * Renvoie l'index de l'élément précédent dans l'itération. Renvoie -1 s'il n'y
	 * a pas d'élément précédent.
	 * 
	 * @return l'index de l'élément précédent dans l'itération
	 * 
	 * @ensures hasPrevious() ==> \result >= 0;
	 * @ensures !hasPrevious() <==> \result == -1;
	 *
	 * @pure
	 */
	@Override
	public int previousIndex() {
		return nextIndex() -1;
	}

	/**
	 * Renvoie l'index de l'élément renvoyé par le dernier appel à next() ou
	 * previous(). Renvoie -1 si next() ou previous() n'ont jamais été appelés
	 * depuis la création de cet itérateur ou bien si remove a été appelée depuis le
	 * dernier appel à next ou previous.
	 * 
	 * @return l'index de l'élément renvoyé par le dernier appel à next() ou
	 *         previous()
	 * 
	 * @ensures \result >= -1 && \result < iterModel.size();
	 * 
	 * @pure
	 */
	public int lastIndex() {
		if (lastCalled == 1) {
			return previousIndex();
		} else if (lastCalled == -1) {
			return nextIndex();
		}
		return -1;
	}

	/**
	 * Retire de l'itération le dernier élèment renvoyé par next() ou previous().
	 * L'élément retiré est l'élèment renvoyé par le dernier appel à next() ou
	 * previous(). Ne peut être appelé qu'une fois par appel à next ou previous.
	 * 
	 * @throws IllegalStateException si next ou previous n'ont jamais été appelés,
	 *                               ou bien si remove a déjà été appelé depuis le
	 *                               dernier appel a next ou remove
	 * 
	 * @requires lastIndex() != -1;
	 * @ensures iterModel.size() == \old(iterModel.size()) - 1;
	 * @ensures (* if last move is previous: *) <br />
	 *          \old(lastIndex() == nextIndex()) ==> (previousIndex() ==
	 *          \old(previousIndex()));
	 * @ensures (* if last move is next: *) <br />
	 *          \old(lastIndex() == previousIndex()) ==> (previousIndex() ==
	 *          \old(previousIndex()) - 1);
	 * @ensures lastIndex() == -1;
	 */
	@Override
	public void remove() {
		if ( lastCalled == 0 || lastCalledIter == -1 ) throw new IllegalStateException("next() ou previous() n'ont pas été déja appelés");
		iterators.get(lastCalledIter).remove();
        lastCalled = 0;
        lastCalledIter = -1;
	}

	/**
	 * Opération non supportée.
	 * 
	 * @throws UnsupportedOperationException toujours
	 */
	@Override
	public void set(E e) {
		throw new UnsupportedOperationException("L'opération set n'est pas supportée par cet itérateur.");
	}

	/**
	 * Opération non supportée.
	 * 
	 * @throws UnsupportedOperationException toujours
	 */
	@Override
	public void add(E e) {
		throw new UnsupportedOperationException("L'opération add n'est pas supportée par cet itérateur.");
	}

}
