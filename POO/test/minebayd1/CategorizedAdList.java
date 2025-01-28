/**
 * 
 */
package minebayd1;

import java.util.ArrayList;
import java.util.List;
import java.util.NoSuchElementException;
import java.util.Optional;

import static minebayd1.Category.*;

/**
 * Gestion d'une liste de ClassifiedAd triée selon la date de création (de la
 * plus récente à la plus ancienne). Une CategorizedAdList ne peut pas contenir
 * d'éléments dupliqués, plus formelement, pour tout couple d'entiers (i, j)
 * tels que: <br/>
 * i >= 0 && j >= 0 && i != j && i < size() && j < size() on a
 * !get(i).equals(get(j)). Une CategorizedAdList prend en compte les catégories
 * des annonces en permettant:
 * <ul>
 * <li>soit un parcours des annonces de toutes les catégories</li>
 * <li>soit un parcours des seules annonces d'une catégorie sélectionnée</li>
 * </ul>
 * Dans ce but, CategorizedAdList utilise une liste distincte (une ArrayList)
 * pour chaque catégorie. Les méthodes selectCategory, getSelectedCategory et
 * clearCatégory permettent de sélectionner ou déselectionner une catégorie
 * particulière. Lorsqu'une catégorie est sélectionnée, toutes les opérations
 * relatives aux itérations (startIteration, next, previous, ...) agissent
 * uniquement sur les annonces de cette catégorie.
 * 
 * Pour les autres méthodes, celles possédant un paramètre de type Category
 * agissent uniquement sur cette catégorie, les autres méthodes agissant sur
 * toutes les annonces.
 * 
 * Les méthodes modifiant le contenu de cette liste (add et remove), entraine
 * une réinitialisation de l'itération en cours (même effet qu'un appel à la
 * méthode startIteration).
 * 
 * @invariant \var Optional&lt;Category&gt; optCat = getSelectedCategory();
 * @invariant getSelectedCategory() != null;
 * @invariant (\forall int i, j; i >= 0 && i < j && j < size();
 *            get(i).isAfter(get(j)));
 * @invariant (\forall Category cat;true; <br/>
 *            (\forall int i, j; i >= 0 && i < j && j < size(cat); <br/>
 *            get(cat, i).isAfter(get(cat, j))));
 * @invariant size() == (\sum Category cat;true;size(cat));
 * @invariant nextIndex() >= 0 && previousIndex() >= -1;
 * @invariant nextIndex() <= size();
 * @invariant previousIndex() < size();
 * @invariant optCat.isPresent() ==> nextIndex() <= size(optCat.get());
 * @invariant optCat.isPresent() ==> previousIndex() < size(optCat.get());
 * @invariant nextIndex() == previousIndex() + 1;
 * @invariant lastIndex() == nextIndex() || lastIndex() == previousIndex();
 * @invariant !hasPrevious() <==> previousIndex() == -1;
 * @invariant optCat.isEmpty() ==> (!hasNext() <==> nextIndex() == size());
 * @invariant optCat.isPresent() ==> <br/>
 *            (!hasNext() <==> nextIndex() == size(optCat.get()));
 * @invariant !contains(null);
 * @invariant (\forall int i, j; i >= 0 && i < j && j < size();
 *            !get(i).equals(get(j)));
 * 
 * @author Marc Champesme
 * @since 27/09/2024
 * @version 20/10/2024
 */
public class CategorizedAdList implements Cloneable {
    private ArrayList<ClassifiedAd> clothesAds; // Liste des annonces de vêtements
    private ArrayList<ClassifiedAd> shoesAds;   // Liste des annonces de chaussures
    private ArrayList<ClassifiedAd> booksAds;    // Liste des annonces de livres
    private ArrayList<ClassifiedAd> gamesAds;    // Liste des annonces de jeux
    private ArrayList<ClassifiedAd> computersAds; // Liste des annonces d'ordinateurs
    private Optional<Category> selectedCategory; // Catégorie actuellement sélectionnée
    private int nextIndex;
    private int previousIndex; 
    private int lastIndex; 
    /**
     * Initialise une nouvelle instance ne contenant aucune annonce.
     * 
     * @ensures size() == 0;
     * @ensures getSelectedCategory() != null;
     * @ensures getSelectedCategory().isEmpty();
     * @ensures !hasPrevious();
     * @ensures !hasNext();
     * @ensures previousIndex() == -1;
     * @ensures nextIndex() == 0;
     * @ensures lastIndex() == -1;
     */

    public CategorizedAdList() {
        this.clothesAds = new ArrayList<>();
        this.shoesAds = new ArrayList<>();
        this.booksAds = new ArrayList<>();
        this.gamesAds = new ArrayList<>();
        this.computersAds = new ArrayList<>();
        this.selectedCategory = Optional.empty();
        this.nextIndex = 0;
        this.previousIndex = -1;
        this.lastIndex = -1;
    }

    /**
     * Sélectionne la catégorie sur laquelle sera effectuée la prochaine itération.
     * Effectue les initialisations nécessaire pour démarrer une nouvelle itération
     * de la même manière qu'un appel à startIteration().
     * 
     * @param cat la catégorie d'annonce à sélectionner
     * 
     * @requires cat != null;
     * @ensures getSelectedCategory().isPresent();
     * @ensures getSelectedCategory().get().equals(cat);
     * @ensures !hasPrevious();
     * @ensures previousIndex() == -1;
     * @ensures nextIndex() == 0;
     * @ensures lastIndex() == -1;
     */
    public void selectCategory(Category cat) {
        if (cat == null){throw new IllegalArgumentException("Erreur");}
        this.selectedCategory = Optional.of(cat);
        this.nextIndex = 0;
        this.previousIndex = -1;
        this.lastIndex = -1;
    }

    /**
     * Renvoie un Optional pour la catégorie sélectionnée. Si une catégorie a été
     * sélectionnée \result.isPresent() est true; sinon \result.isEmpty() est true.
     * 
     * @return un Optional pour la catégorie sélectionnée
     * 
     * @ensures \result != null;
     * 
     * @pure
     */
    public Optional<Category> getSelectedCategory() {

        return this.selectedCategory;
    }

    /**
     * Déselectionne la catégorie sélectionnée et effectue les initialisations
     * nécessaire pour démarrer une nouvelle itération (cf.startIteration()). Si
     * aucune catégorie n'était sélectionnée l'appel à cette méthode est équivalent
     * à un appel à startIteration.
     * 
     * @ensures getSelectedCategory().isEmpty();
     * @ensures !hasPrevious();
     * @ensures previousIndex() == -1;
     * @ensures nextIndex() == 0;
     * @ensures lastIndex() == -1;
     */
    public void clearSelectedCategory() {
        if (this.selectedCategory.isPresent()){
            this.selectedCategory = Optional.empty();
        }
        startIteration();
    }

    /**
     * Initialise cette liste pour le démarrage d'une nouvelle itération sur les
     * annonces de cette liste. Cette itération s'effectue à partir de l'annonce la
     * plus récente, de sorte que chaque appel à next() renvoie une annonce plus
     * ancienne.
     * 
     * Si une catégorie est sélectionnée, l'itération ne concernera que les annonces
     * de cette catégorie; sinon l'itération concerne toutes les annonces, quelque
     * soit leur catégorie.
     * 
     * @ensures !hasPrevious();
     * @ensures previousIndex() == -1;
     * @ensures nextIndex() == 0;
     * @ensures lastIndex() == -1;
     */
    public void startIteration() {
        nextIndex = 0; 
        lastIndex = -1; 
        previousIndex = -1; 
    }
    

    /**
     * Renvoie true si cette liste possède une annonce plus ancienne pour
     * l'itération en cours.
     * 
     * Si une catégorie est sélectionnée, l'opération ne concernera que les annonces
     * de cette catégorie; sinon l'opération concerne toutes les annonces, quelque
     * soit leur catégorie.
     * 
     * @return true si cette liste possède une annonce plus ancienne pour
     *         l'itération en cours
     * 
     * @ensures \var Optional&lt;Category&gt; optCat = getSelectedCategory();
     * @ensures optCat.isEmpty() ==> (\result <==> nextIndex() < size());
     * @ensures optCat.isEmpty() ==> (!\result <==> nextIndex() == size());
     * @ensures optCat.isPresent() ==> (\result <==> nextIndex() <
     *          size(optCat.get()));
     * @ensures optCat.isPresent() ==> (!\result <==> nextIndex() ==
     *          size(optCat.get()));
     * 
     * @pure
     */
    public boolean hasNext() {
        if (selectedCategory.isPresent()) {
            switch (selectedCategory.get()) {
                case CLOTHES:
                    return nextIndex < clothesAds.size();
                case SHOES:
                    return nextIndex < shoesAds.size();
                case BOOKS:
                    return nextIndex < booksAds.size();
                case GAMES:
                    return nextIndex < gamesAds.size();
                case COMPUTERS:
                    return nextIndex < computersAds.size();
                default:
                    return false;
            }
        } else {
            return nextIndex < (clothesAds.size() + shoesAds.size() + booksAds.size() + gamesAds.size() + computersAds.size());
        }
    }

    /**
     * Renvoie l'annonce suivante (plus ancienne) dans l'itération en cours et
     * avance d'un élément dans l'itération.
     * 
     * Si une catégorie est sélectionnée, l'opération ne concernera que les annonces
     * de cette catégorie; sinon l'opération concerne toutes les annonces, quelque
     * soit leur catégorie.
     * 
     * @return l'annonce suivante (plus ancienne) dans l'itération en cours
     * 
     * @requires hasNext();
     * @ensures \var Optional&lt;Category&gt; optCat = getSelectedCategory();
     * @ensures \result != null;
     * @ensures optCat.isEmpty() ==> \result.equals(\old(get(nextIndex())));
     * @ensures optCat.isPresent() ==> \result.equals(\old(get(optCat.get(),
     *          nextIndex())));
     * @ensures optCat.isPresent() ==> \result.getCategory().equals(optCat.get());
     * @ensures nextIndex() == \old(nextIndex()) + 1;
     * @ensures previousIndex() == \old(previousIndex()) + 1;
     * @ensures previousIndex() == \old(nextIndex());
     * @ensures lastIndex() == \old(nextIndex());
     * @ensures lastIndex() == previousIndex();
     */
    public ClassifiedAd next() {
        if(!hasNext()){
            throw new NoSuchElementException("Pas de Next");
        }

        Optional<Category> optCat = getSelectedCategory();
        ClassifiedAd ad;

        if(optCat.isEmpty()){
            ad = get(nextIndex());
        }
        else {
            ad = get(optCat.get(),nextIndex);

        }

        nextIndex++;
        previousIndex++;
        lastIndex = previousIndex;

        return ad;
    }
    /**
     * Renvoie l'index de l'annonce qui sera renvoyée par le prochain appel à
     * next(). Si l'itération est arrivée à la fin la valeur renvoyée est size() (ou
     * size(getSelectedCategory().get()) si une catégorie est sélectionnée).
     * 
     * Si une catégorie est sélectionnée, l'opération ne concernera que les annonces
     * de cette catégorie; sinon l'opération concerne toutes les annonces, quelque
     * soit leur catégorie.
     * 
     * @return l'index de l'annonce qui sera renvoyé par le prochain appel à next();
     *         ou size() s'il n'y a pas d'élément suivant
     * 
     * @ensures \var Optional&lt;Category&gt; optCat = getSelectedCategory();
     * @ensures optCat.isEmpty() ==> (\result == size() <==> !hasNext());
     * @ensures optCat.isPresent() ==> (\result == size(optCat.get()) <==>
     *          !hasNext());
     * @ensures optCat.isEmpty() ==> (hasNext() <==> \result >= 0 && \result <
     *          size());
     * @ensures optCat.isPresent() ==> (hasNext() <==> \result >= 0 && \result <
     *          size(optCat.get()));
     * 
     * @pure
     */
    public int nextIndex() {
        Optional<Category> optCat = getSelectedCategory();

        if (optCat.isEmpty()){
            if (this.nextIndex < size()){
                return this.nextIndex;
            }
            return size();
        }
        if(this.nextIndex < size(optCat.get())){
            return this.nextIndex;
            }
        return size(optCat.get());
        
    }

    /**
     * Renvoie true si cette liste possède une annonce plus récente pour l'itération
     * en cours.
     * 
     * Si une catégorie est sélectionnée, l'opération ne concernera que les annonces
     * de cette catégorie; sinon l'opération concerne toutes les annonces, quelque
     * soit leur catégorie.
     * 
     * @return true si cette liste possède une annonce plus récente pour l'itération
     *         en cours
     * 
     * @ensures \result <==> previousIndex() >= 0;
     * @ensures !\result <==> previousIndex() == -1;
     * 
     * @pure
     */
    public boolean hasPrevious() {
        return previousIndex >= 0;
    }

    /**
     * Renvoie l'annonce précedente (plus récente) dans l'itération en cours et
     * recule d'un élément dans l'itération.
     * 
     * Si une catégorie est sélectionnée, l'opération ne concernera que les annonces
     * de cette catégorie; sinon l'opération concerne toutes les annonces, quelque
     * soit leur catégorie.
     * 
     * @return l'annonce précedente (plus récente) dans l'itération en cours
     * 
     * 
     * @requires hasPrevious();
     * @ensures \var Optional&lt;Category&gt; optCat = getSelectedCategory();
     * @ensures optCat.isEmpty() ==> \result.equals(\old(get(previousIndex())));
     * @ensures optCat.isPresent() ==> \result.equals(\old(get(optCat.get(),
     *          previousIndex())));
     * @ensures optCat.isPresent() ==> \result.getCategory().equals(optCat.get());
     * @ensures nextIndex() == \old(nextIndex()) - 1;
     * @ensures previousIndex() == \old(previousIndex()) - 1;
     * @ensures nextIndex() == \old(previousIndex());
     * @ensures lastIndex() == \old(previousIndex());
     * @ensures lastIndex() == nextIndex();
     * 
     */
    public ClassifiedAd previous() {
        Optional<Category> optCat = getSelectedCategory();
        ClassifiedAd ad;

        if(optCat.isPresent()){
            ad = get(optCat.get(),previousIndex);
        }
        else{
            ad = get(previousIndex);
        }
        previousIndex--;
        nextIndex--;
        lastIndex = nextIndex;

        return ad;
        
    }
    

    /**
     * Renvoie l'index de l'annonce qui sera renvoyée par le prochain appel à
     * previous(). Si l'itération est arrivée au début la valeur renvoyée est -1.
     * 
     * Si une catégorie est sélectionnée, l'opération ne concernera que les annonces
     * de cette catégorie; sinon l'opération concerne toutes les annonces, quelque
     * soit leur catégorie.
     * 
     * @return l'index de l'annonce qui sera renvoyé par le prochain appel à
     *         previous(); ou -1
     * 
     * @ensures \var Optional&lt;Category&gt; optCat = getSelectedCategory();
     * @ensures \result == -1 <==> !hasPrevious();
     * @ensures optCat.isEmpty() ==> (hasPrevious() <==> \result >= 0 && \result <
     *          size());
     * @ensures optCat.isPresent() ==> (hasPrevious() <==> \result >= 0 && \result <
     *          size(optCat.get()));
     * 
     * @pure
     */
    public int previousIndex() {
        Optional<Category> optCat = getSelectedCategory();
        if (!hasPrevious()){
            return -1;
        }
        if (optCat.isEmpty()){
            if(previousIndex >= 0 && previousIndex < size()){
                return previousIndex;
            }
            else {
                return -1;
            }
        }
        else{
            if(previousIndex >= 0 && previousIndex < size(optCat.get())){
                return previousIndex;
            }
            else{
                return -1;
            }
        }
    }
    

    /**
     * Renvoie l'index de l'annonce qui a été renvoyée par le dernier appel à
     * previous() ou next(). Si next() ou previous() n'ont pas été appelé depuis le
     * dernier appel à startIteration() la valeur renvoyée est -1.
     * 
     * Si une catégorie est sélectionnée, l'opération ne concernera que les annonces
     * de cette catégorie; sinon l'opération concerne toutes les annonces, quelque
     * soit leur catégorie.
     * 
     * @return l'index de l'annonce qui a été renvoyé par le dernier appel à
     *         previous() ou next(); ou -1
     * 
     * @ensures \result == nextIndex() || \result == previousIndex();
     * 
     * @pure
     */
    public int lastIndex() {
        if(nextIndex > 0 || previousIndex >= 0){
            return lastIndex;
        }
        return -1;

    }
    

    /**
     * Renvoie l'élément d'index spécifié dans la liste des annonces de la catégorie
     * spécifiée. À la différence de la méthode get(int i), cette implémentation
     * effectue un accès direct au ième élément et peut donc être utilisée sans
     * pénalité pour effectuer une itération sur les éléments de la catégorie
     * spécifiée.
     * 
     * @param cat catégorie dans la quelle l'élement est recherché
     * @param i   rang de l'élément cherché dans la liste des éléments de la
     *            catégorie spécifiée
     * @return l'élément d'index spécifié dans la liste des annonces de la catégorie
     *         spécifiée
     * 
     * @requires cat != null;
     * @requires i >= 0 && i < size(cat);
     * @ensures contains(\result);
     * @ensures \result.getCategory().equals(cat);
     * 
     * @pure
     */
    public ClassifiedAd get(Category cat, int i) {
        if (cat == null) {
            throw new IllegalArgumentException("La catégorie ne peut pas être nulle.");
        }
        if (i < 0 || i >= size(cat)) {
            throw new IndexOutOfBoundsException("Index en dehors des limites de la catégorie.");
        }
    
        switch (cat) {
            case CLOTHES:
                return clothesAds.get(i);
            case SHOES:
                return shoesAds.get(i);
            case BOOKS:
                return booksAds.get(i);
            case GAMES:
                return gamesAds.get(i);
            case COMPUTERS:
                return computersAds.get(i);
            default:
                throw new IllegalArgumentException("Catégorie inconnue.");
        }
    }
    

    /**
     * Renvoie l'élément d'index spécifié dans la liste des annonces. Cette
     * implémentation n'est pas adaptée à une utilisation pour effectuer des
     * itérations sur l'ensemble de cette liste. Pour effectuer une itération, il
     * est fortement recommandé d'utiliser les méthodes prévues à cet effet
     * (startIteration, hasNext, next, hasPrevious, previous, ...).
     * 
     * @param i rang de l'élément cherché dans la liste des éléments
     * @return l'élément d'index spécifié dans la liste des annonces
     * 
     * @requires i >= 0 && i < size();
     * @ensures contains(\result);
     * @ensures (\exists int i; i >= 0 && i < size(\result.getCategory());
     *          \result.equals(get(\result.getCategory(), i)));
     * 
     * @pure
     */
    public ClassifiedAd get(int i) {
        if (i < 0 || i >= size()) {
            throw new IndexOutOfBoundsException("Index en dehors des limites de la liste.");
        }
    
        startIteration(); 
        int count = 0;
    
        while (hasNext()) {
            ClassifiedAd ad = next(); 
            if (count == i) {
                return ad; 
            }
            count++;
        }
    
        throw new NoSuchElementException("Aucune annonce trouvée à l'index spécifié.");
    }

    /**
     * Ajoute l'élement spécifié dans cette liste. L'élement est inséré dans la
     * liste de manière à ce que la liste reste triée. L'itération en cours est
     * réinitialisée.
     * 
     * @param elt l'élement à ajouter
     * 
     * @requires elt != null;
     * @requires !contains(elt);
     * @ensures contains(elt);
     * @ensures size() == \old(size()) + 1;
     * @ensures size(elt.getCategory()) == \old(size(elt.getCategory())) + 1;
     * @ensures (\forall Category cat; !cat.equals(elt.getCategory()); size(cat) ==
     *          \old(size(cat)));
     * @ensures !hasPrevious();
     * @ensures previousIndex() == -1;
     * @ensures nextIndex() == 0;
     * @ensures lastIndex() == -1;
     */
    public void add(ClassifiedAd elt) {
        if (elt == null) {
            throw new IllegalArgumentException("Erreur : l'annonce ne peut pas être nulle.");
        }
    
        ArrayList<ClassifiedAd> targetList = null;
    
        switch (elt.getCategory()) {
            case BOOKS:
                targetList = booksAds;
                break;
            case CLOTHES:
                targetList = clothesAds;
                break;
            case GAMES:
                targetList = gamesAds;
                break;
            case SHOES:
                targetList = shoesAds;
                break;
            case COMPUTERS:
                targetList = computersAds;
                break;
            default:
                throw new IllegalArgumentException("Erreur : catégorie non reconnue.");
        }
    
        if (targetList.contains(elt)) {
            throw new IllegalArgumentException("Erreur : l'annonce existe déjà dans la catégorie.");
        }
    
        int index = 0;
        while (index < targetList.size() && targetList.get(index).getDate().isBefore(elt.getDate())) {
            index++;
        }
    
        targetList.add(index, elt);
    
        startIteration();
    }

    
    public boolean remove(Object o) {
        if( o == null || !(o instanceof ClassifiedAd)){
            return false;
            
        }
        ArrayList<ClassifiedAd> targetList = new ArrayList<>();
        ClassifiedAd adToRemove = (ClassifiedAd) o;
        switch (adToRemove.getCategory()) {
            case BOOKS:
                targetList = booksAds;
                break;
            case COMPUTERS:
                targetList = computersAds;
                break;
            case CLOTHES:
                targetList = clothesAds;
                break;
            case SHOES:
                targetList = shoesAds;
                break;
            case GAMES:
                targetList = gamesAds;
                break;
            default:
                return false;
            }
        return targetList.remove(adToRemove);
    }

    /**
     * Renvoie true si et seulement si l'argument spécifié est présent dans cette
     * liste.
     * 
     * @param o l'objet dont on veut tester la présence dans cette liste
     * 
     * @return true si l'argument spécifié est présent dans cette liste; false sinon
     * 
     * @ensures \result <==> (\exists int i; i >= 0 && i < size();
     *          get(i).equals(o));
     * @ensures \result <==> (o instanceof ClassifiedAd) <br/>
     *          && (\exists int i; i >= 0 && i < size((ClassifiedAd)
     *          o).getCategory()); <br/>
     *          get(((ClassifiedAd) o).getCategory(), i).equals(o));
     * 
     * @pure
     */
    public boolean contains(Object o) {
        if (!(o instanceof ClassifiedAd) ) {
            throw new IllegalArgumentException("Erreur : mauvaise instance \n");
        }
        ClassifiedAd ad = (ClassifiedAd) o;
        ArrayList<ClassifiedAd> targetList = null;

        switch(ad.getCategory()){

        case BOOKS:
            targetList = booksAds;
            break;

        case SHOES:
            targetList = shoesAds;
            break;

        case GAMES:
            targetList = gamesAds;
            break;

        case CLOTHES:
            targetList = clothesAds;
            break;

        case COMPUTERS:
            targetList = computersAds;
            break;

        }

        return targetList.contains(ad);
        
    }

    /**
     * Renvoie le nombre total d'éléments de cette liste.
     * 
     * @return le nombre total d'éléments de cette liste
     * 
     * @ensures \result >= 0;
     * @ensures \result == (\sum Category cat;true;size(cat));
     * 
     * @pure
     */
    public int size() {
        

        return this.booksAds.size() + this.shoesAds.size() + this.computersAds.size() + this.gamesAds.size() + this.clothesAds.size();
    }

    /**
     * Renvoie le nombre d'annonces de cette liste appartenant à la catégorie
     * spécifiée.
     * 
     * @param cat catégorie dont on souhaite connaître le nombre d'éléments
     * 
     * @return le nombre d'annonces de cette liste appartenant à la catégorie
     *         spécifiée
     * 
     * @requires cat != null;
     * @ensures \result >= 0 && \result <= size();
     * @ensures \result == (\sum int i; i >= 0 && i < size() &&
     *          get(i).getCategory().equals(cat); 1);
     * 
     * @pure
     */
    public int size(Category cat) {

        ArrayList<ClassifiedAd> targetList = null;

        switch(cat){

        case BOOKS:
            targetList = booksAds;
            break;

        case SHOES:
            targetList = shoesAds;
            break;

        case GAMES:
            targetList = gamesAds;
            break;

        case CLOTHES:
            targetList = clothesAds;
            break;

        case COMPUTERS:
            targetList = computersAds;
            break;

        }

        return targetList.size();

    }

    /**
     * Renvoie true si et seulement si l'argument spécifié est une CategorizedAdList
     * contenant les mêmes éléments dans le même ordre que cette CategorizedAdList,
     * et que la CategorizedAdList spécifiée est dans le même état que cette liste
     * concernant l'itération en cours et la catégorie sélectionnée.
     * 
     * @param obj l'objet à comparer à cette liste
     * 
     * @return true si l'argument spécifié contient les mêmes éléments et est dans
     *         le même état que cette liste
     * 
     * @ensures !(obj instanceof CategorizedAdList) ==> !\result;
     * @ensures \result ==> ((CategorizedAdList) obj).size() == size();
     * @ensures \result ==> ((CategorizedAdList) obj).previousIndex() ==
     *          previousIndex();
     * @ensures \result ==> ((CategorizedAdList) obj).nextIndex() == nextIndex();
     * @ensures \result ==> ((CategorizedAdList) obj).lastIndex() == lastIndex();
     * @ensures \result ==> ((CategorizedAdList)
     *          obj).getSelectedCategory().equals(getSelectedCategory());
     * @ensures \result ==> (\forall int i; i >= 0 && i < size();
     *          ((CategorizedAdList) obj).get(i).equals(get(i)));
     * @ensures \result ==> ((CategorizedAdList) obj).hashCode() == hashCode();
     * 
     * @pure
     */
    @Override
    public boolean equals(Object obj) {
        if (!(obj instanceof CategorizedAdList)) {
            return false;
        }
        CategorizedAdList list = (CategorizedAdList) obj;
    
        if(this.nextIndex == list.nextIndex && this.previousIndex == list.previousIndex && this.lastIndex == list.lastIndex && this.selectedCategory.equals(list.selectedCategory)){
            if(this.clothesAds.equals(list.clothesAds) && this.gamesAds.equals(list.gamesAds) && this.shoesAds.equals(list.shoesAds) && this.computersAds.equals(list.computersAds) && this.booksAds.equals(list.booksAds)){
                return true;
            }
        }
        return false;
    }

    /**
     * Renvoie un clone de cette liste.
     * 
     * @return un clone de cette liste
     * 
     * @ensures \result != null;
     * @ensures \result != this;
     * @ensures \result.equals(this);
     * @ensures \result.getClasse() == this.getClass();
     * 
     * @pure
     */
    @Override
    public CategorizedAdList clone() {
        try {
            // Clone superficiel de l'objet
            CategorizedAdList cloned = (CategorizedAdList) super.clone();
            
            // Clonage profond des listes d'annonces par catégorie
            cloned.clothesAds = new ArrayList<>(this.clothesAds);
            cloned.shoesAds = new ArrayList<>(this.shoesAds);
            cloned.booksAds = new ArrayList<>(this.booksAds);
            cloned.gamesAds = new ArrayList<>(this.gamesAds);
            cloned.computersAds = new ArrayList<>(this.computersAds);

            // Clonage des attributs d'itération et de catégorie sélectionnée
            cloned.selectedCategory = this.selectedCategory;
            cloned.nextIndex = this.nextIndex;
            cloned.previousIndex = this.previousIndex;
            cloned.lastIndex = this.lastIndex;

            return cloned;
        } catch (CloneNotSupportedException e) {
            throw new AssertionError("Le clonage n'est pas supporté", e);
        }
    }


    /**
     * Renvoie un code de hashage pour cette liste.
     * 
     * @return un code de hashage pour cette liste
     * 
     * @pure
     */
    @Override
    public int hashCode() {
        int result = clothesAds.hashCode();
        result = 31 * result + shoesAds.hashCode();
        result = 31 * result + booksAds.hashCode();
        result = 31 * result + gamesAds.hashCode();
        result = 31 * result + computersAds.hashCode();
        result = 31 * result + selectedCategory.hashCode();
        result = 31 * result + nextIndex;
        result = 31 * result + previousIndex;
        result = 31 * result + lastIndex;
        return result;
    }

    /**
     * Renvoie une représentation de cette liste sous forme d'une chaîne de caractères.
     * 
     * @return une représentation de cette liste sous forme d'une chaîne de caractères
     * 
     * @ensures \result != null;
     * @ensures \result.contains("" + lastIndex());
     * @ensures \result.contains("" + nextIndex());
     * @ensures \result.contains("" + previousIndex());
     * @ensures \result.contains(getSelectedCategory().toString());
     * @ensures (\forall ClassifiedAd ad; contains(ad); \result.contains(ad.toString())); 
     * 
     * @pure
     */
    @Override
    public String toString() {
        StringBuilder sb = new StringBuilder("CategorizedAdList {");

        sb.append("lastIndex=").append(lastIndex).append(", ");
        sb.append("nextIndex=").append(nextIndex).append(", ");
        sb.append("previousIndex=").append(previousIndex).append(", ");
        sb.append("selectedCategory=").append(selectedCategory.orElse(null)).append(", ");

        sb.append("clothesAds=").append(clothesAds).append(", ");
        sb.append("shoesAds=").append(shoesAds).append(", ");
        sb.append("booksAds=").append(booksAds).append(", ");
        sb.append("gamesAds=").append(gamesAds).append(", ");
        sb.append("computersAds=").append(computersAds);

        sb.append("}");

        return sb.toString();
    }


}








