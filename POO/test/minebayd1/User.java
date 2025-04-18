package minebayd1;

import java.time.Instant;
import java.util.Optional;
import java.util.ArrayList;
import static minebayd1.AdState.*;

/**
 * Un utilisateur de l'application MinEbay.
 * 
 * <p>
 * Un User est caractérisé par:
 * <ul>
 * <li>son nom et son mot de passe</li>
 * <li>le montant disponible sur son compte bancaire</li>
 * <li>sa date d'inscription sur MinEbay (= date de création de l'instance de
 * l'User)</li>
 * <li>la liste des annonces qu'il a posté et pour lesquelles il n'a pas trouvé
 * d'acheteur (open ads)</li>
 * <li>la liste des annonces qu'il a posté et pour lesquelles il a vendu l'objet
 * concerné (closed ads)</li>
 * <li>la liste des annonces (d'autres utilisateurs) pour lesquelles il a acheté
 * l'objet concerné (puchases)</li>
 * </ul>
 * </p>
 * 
 * <p>
 * Chacune des trois listes d'annonces d'un User est représentée en utilisant
 * une instance de CategorizedAdList.
 * </p>
 * 
 * <p>
 * Les méthodes ne possédant pas de paramètre de type AdState ou Category
 * agissent uniquement sur les listes d'annonces dont l'état et, le cas échéant,
 * la catégorie a été sélectionnée.
 * </p>
 * 
 * Un ensemble complet de méthodes permet d'effectuer des itérations
 * bidirectionnelles sur les listes d'annonces de l'User. Ce sont les méthodes:
 * startIteration(), hasNext(), nextIndex(), next(), hasPrevious(),
 * previousIndex(), previous() et lastIndex(). Un exemple d'utilisation de ces
 * méthodes est sonné ci-après:
 * 
 * <pre>{@code
 * aUser.startIteration(); // Initialisation d'une nouvelle itération
 * // Affichage des annonces du plus récent au plus ancien:
 * while (aUser.hasNext()) {
 * 	System.out.println("Annonce suivante (plus ancienne):" + aUser.next());
 * }
 * // Affichage des annonces du plus ancien au plus récent:
 * while (aUser.hasPrevious()) {
 * 	System.out.println("Annonce précédente (plus récente):" + aUser.previous());
 * }
 * }</pre>
 * 
 * @invariant getName() != null && !getName().isBlank();
 * @invariant getPassword() != null && !getPassword().isBlank();
 * @invariant getRegistrationDate() != null;
 * @invariant (\forall AdState state; true; !contains(state, null));
 * @invariant (\forall int i, j; i >= 0 && i < j && j < size();
 *            get(i).isAfter(get(j)));
 * @invariant (\forall AdState state, Category cat;true; <br/>
 *            (\forall int i, j; <br/>
 *            i >= 0 && i < j && j < size(state, Optional<Category>.of(cat));
 *            get(state, Optional<Category>.of(cat), i).isAfter(get(state,
 *            Optional<Category>.of(cat), j))));
 * @invariant size() == asList().size();
 * @invariant previousIndex() >= -1 && previousIndex() < size();
 * @invariant nextIndex() >= 0 && nextIndex() <= size();
 * @invariant nextIndex() == previousIndex() + 1;
 * @invariant lastIndex() >= -1 && lastIndex() < size();
 * @invariant lastIndex() == previousIndex() || lastIndex() == nextIndex();
 * 
 * @author Marc Champesme
 * @since 27/09/2024
 * @version 20/10/2024
 * 
 */
public class User {







	private String name;
	private String password;
	private Instant registrationDate;
	private int availableCash;
	private Optional<Category> selectCategory;
	private AdState selectAdState;
	private int indexcurrent;
	public static final int DEFAULT_CASH_AMMOUNT = 1000;

	private ArrayList<ClassifiedAd> openads;
	private ArrayList<ClassifiedAd> closedads;
	private ArrayList<ClassifiedAd> puchases;


	/**
	 * Initialise une nouvelle instance ayant les nom et mot de passe spécifiés. La
	 * date d'inscription du nouvel utilisateur est la date au moment de l'exécution
	 * de ce constructeur. Une nouvelle itération est initialisée sur les annonces
	 * OPEN de ce User.
	 * 
	 * @param userName nom de la nouvelle instance de User
	 * @param password mot de passe de la nouvelle instance de User
	 * 
	 * @requires userName != null && !userName.isBlank();
	 * @requires password != null && !password.isBlank();
	 * @ensures getName().equals(userName);
	 * @ensures getPassword().equals(password);
	 * @ensures getRegistrationDate() != null;
	 * @ensures \old(Instant.now()).isBefore(getRegistrationDate());
	 * @ensures getRegistrationDate().isBefore(Instant.now());
	 * @ensures getAvailableCash() = DEFAULT_CASH_AMMOUNT;
	 * @ensures getSelectedAdState().equals(OPEN);
	 * @ensures getSelectedCategory().isEmpty();
	 * @ensures !hasPrevious();
	 * @ensures !hasNext();
	 * @ensures previousIndex() == -1;
	 * @ensures nextIndex() == 0;
	 * @ensures lastIndex() == -1;
	 */
	public User(String userName, String password) {


		if( userName == null || userName.isBlank() || password == null || password.isBlank()){
			throw new IllegalArgumentException("Erreur : userName == null ou password == null\n");
		}

		this.name = userName;
		this.password = password;
		this.registrationDate = Instant.now();
		this.availableCash = DEFAULT_CASH_AMMOUNT;
		this.selectAdState = OPEN;
		this.selectCategory = Optional.empty();
		this.indexcurrent = -1;

		this.openads = new ArrayList<>();
		this.closedads = new ArrayList<>();
		this.puchases = new ArrayList<>();

	}

	/**
	 * Renvoie l'état actuellement sélectionné pour les annonces.
	 * 
	 * @return l'état actuellement sélectionné pour les annonces
	 * 
	 * @ensures \result != null;
	 * 
	 * @pure
	 */
	public AdState getSelectedAdState() {
		return selectAdState;
	}

	/**
	 * Sélectionne l'état spécifié. Une nouvelle itération est initialisée pour les
	 * annonces dans cet état et le cas échéant la catégorie sélectionnée.
	 * 
	 * @param state état à sélectionner pour les annonces
	 * 
	 * @requires state != null;
	 * @ensures getSelectedAdState().equals(state);
	 * @ensures getSelectedCategory().equals(\old(getSelectedCategory()));
	 * @ensures !hasPrevious();
	 * @ensures previousIndex() == -1;
	 * @ensures nextIndex() == 0;
	 * @ensures lastIndex() == -1;
	 */
	public void selectAdState(AdState state) {
		if(state == null){
			throw new IllegalArgumentException("Erreur : le statut ne peut pas etre null\n");
		}
		this.selectAdState = state;
		this.indexcurrent = -1;
	}

	/**
	 * Sélectionne la catégorie spécifiée. Une nouvelle itération est initialisée
	 * pour les annonces de cette catégorie et dans l'état spécifié.
	 * 
	 * @param cat catégorie à sélectionner pour les annonces
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
		if(cat == null){
			throw new IllegalArgumentException("Erreur : la Category est null \n");
		}
		this.selectCategory = Optional.of(cat);
		indexcurrent = -1;
	}

	/**
	 * Renvoie un {@code Optional<Category>} représentant la catégorie actuellement
	 * sélectionnée. Si aucun catégorie n'est sélectionnée, renvoie un
	 * Optional.empty().
	 * 
	 * @return un {@code Optional<Category>} de la catégorie actuellement
	 *         sélectionnée ou Optional.empty()
	 * 
	 * @ensures \result != null;
	 * 
	 * @pure
	 */
	public Optional<Category> getSelectedCategory() {
		return selectCategory;
	}

	/**
	 * Déselectionne la catégorie sélectionnée et effectue les initialisations
	 * nécessaire pour démarrer une nouvelle itération (cf.startIteration()). Si
	 * aucune catégorie n'était sélectionnée l'appel à cette méthode est équivalent
	 * à un appel à startIteration.
	 * 
	 * @ensures getSelectedCategory().isEmpty();
	 * @ensures getSelectedAdState().equals(\old(getSelectedAdState()));
	 * @ensures !hasPrevious();
	 * @ensures previousIndex() == -1;
	 * @ensures nextIndex() == 0;
	 * @ensures lastIndex() == -1;
	 */
	public void clearSelectedCategory() {
		this.selectCategory = Optional.empty();

		startIteration();
	}

	/**
	 * Renvoie le nom de cet utilisateur.
	 * 
	 * @return le nom de cet utilisateur
	 * 
	 * @pure
	 */
	public String getName() {
		return name;
	}

	/**
	 * Renvoie le mot de passe de cet utilisateur.
	 * 
	 * @return le mot de passe de cet utilisateur
	 * 
	 * @pure
	 */
	public String getPassword() {
		return password;
	}

	/**
	 * Renvoie l'Instant représentant la date d'inscription de cet utilisateur.
	 * 
	 * @return la date d'inscription de cet utilisateur
	 * 
	 * @pure
	 */
	public Instant getRegistrationDate() {
		return registrationDate;
	}

	/**
	 * Renvoie la somme d'argent disponible pour cet utilisateur pour acheter des
	 * objets.
	 * 
	 * @return la somme d'argent disponible pour cet utilisateur
	 * 
	 * @ensures \result >= 0;
	 * 
	 * @pure
	 */
	public int getAvailableCash() {
		return availableCash;
	}

	/**
	 * Achète à l'utilisateur spécifié l'objet présenté dans l'annonce spécifiée.
	 * Pour le vendeur l'annonce passe de la liste des annonces OPEN à la liste des
	 * annonces CLOSED et il reçoit le montant du prix de l'objet présenté dans
	 * l'annonce. Pour ce User, l'annonce est ajoutée à la liste des annonces
	 * PURCHASE et sa somme d'argent disponible est diminuée du prix de l'objet.
	 * Pour l'acheteur et le vendeur les itérations en cours sont réinitialisées
	 * comme par un appel à startIteration.
	 * 
	 * @param vendor l'utilisateur auteur de l'annonce spécifiée
	 * @param ad     l'annonce de l'objet à acheter
	 * 
	 * 
	 * @requires vendor != null;
	 * @requires !this.equals(vendor);
	 * @requires ad != null;
	 * @requires vendor.containsInState(OPEN, ad);
	 * @requires getAvailableCash() >= ad.getPrice();
	 * @ensures containsInState(PURCHASE, ad);
	 * @ensures size(PURCHASE, Optional.of(ad.getCategory()))<br/>
	 *          == \old(size(PURCHASE, Optional.of(ad.getCategory()))) + 1;
	 * @ensures !vendor.containsInState(OPEN, ad);
	 * @ensures vendor.size(OPEN, Optional.of(ad.getCategory())) ==
	 *          \old(vendor.size(OPEN, Optional.of(ad.getCategory()))) - 1;
	 * @ensures vendor.containsInState(CLOSED, ad);
	 * @ensures vendor.size(CLOSED, Optional.of(ad.getCategory())) ==
	 *          \old(vendor.size(CLOSED, Optional.of(ad.getCategory()))) + 1;
	 * @ensures getAvailableCash() == \old(getAvailableCash()) - ad.getPrice();
	 * @ensures vendor.getAvailableCash() == \old(vendor.getAvailableCash()) +
	 *          ad.getPrice();
	 * @ensures !hasPrevious();
	 * @ensures previousIndex() == -1;
	 * @ensures nextIndex() == 0;
	 * @ensures lastIndex() == -1;
	 * @ensures vendor.!hasPrevious();
	 * @ensures vendor.previousIndex() == -1;
	 * @ensures vendor.nextIndex() == 0;
	 * @ensures vendor.lastIndex() == -1; }
	 */
	public void buy(User vendor, ClassifiedAd ad) {
		if(vendor == null || ad == null || this.equals(vendor) || getAvailableCash() < ad.getPrice()){
			throw new IllegalArgumentException("Erreur !!");
		}

		this.puchases.add(ad);

		vendor.openads.remove(ad);
		vendor.closedads.add(ad);

		this.availableCash -= ad.getPrice();
		vendor.availableCash += ad.getPrice();
	}

	/**
	 * Crée et renvoie une nouvelle instance de ClassifiedAd dont les
	 * caractéristiques sont les arguments spécifiés et l'ajoute à la liste des
	 * annonces ouvertes (open ads) de cet utilisateur. La date de cette annonce est
	 * la date d'exécution de cette méthode. Si l'état sélectionné des annonces est
	 * OPEN, l'itération en cours est réinitialisée, comme par un appel à
	 * startIteration.
	 * 
	 * @param cat   la catégorie de la nouvelle annonce
	 * @param msg   la description de la nouvelle annonce
	 * @param price le prix de l'objet décrit dans la nouvelle annonce
	 * 
	 * @return la nouvelle annonce de cet utilisateur
	 * 
	 * @old oldDate = Instant.now();
	 * @requires cat != null;
	 * @requires msg != null;
	 * @requires price > 0;
	 * @ensures get(OPEN, Optional.of(cat), 0).equals(\result);
	 * @ensures \result.getCategory().equals(cat);
	 * @ensures \result.getDescription().equals(msg);
	 * @ensures \result.getPrice() == price;
	 * @ensures size(OPEN, Optional.of(cat)) == \old(size(OPEN, Optional.of(cat))) +
	 *          1;
	 * @ensures oldDate.isBefore(\result.getDate());
	 * @ensures \result.getDate().isBefore(Instant.now());
	 * @ensures getSelectedAdState().equals(OPEN) ==> !hasPrevious();
	 * @ensures getSelectedAdState().equals(OPEN) ==> previousIndex() == -1;
	 * @ensures getSelectedAdState().equals(OPEN) ==> nextIndex() == 0;
	 * @ensures getSelectedAdState().equals(OPEN) ==> lastIndex() == -1;
	 */
	public ClassifiedAd add(Category cat, String msg, int price) {
		return null;
	}

	/**
	 * Renvoie le nombre d'annonces de cet utilisateur dans l'état sélectionné
	 * (open, closed ou purchase). Si une catégorie est sélectionnée seules les
	 * annonces de cette catégorie sont comptées.
	 * 
	 * @return le nombre d'annonces de cet utilisateur dans l'état et la catégorie
	 *         sélectionnés.
	 * 
	 * @ensures \result == size(getSelectedAdState(), getSelectedCategory());
	 * 
	 * @pure
	 */
	public int size() {
		return -1;
	}

	/**
	 * Renvoie le nombre d'annonces de cet utilisateur dans l'état spécifié et
	 * appartenant à la catégorie spécifiée.
	 * 
	 * @param state état des annonces
	 * @param cat   catégorie des annonces
	 * 
	 * @requires state != null;
	 * @requires cat != null;
	 * @ensures \result >= 0;
	 * @ensures getSelectedAdState().equals(state) &&
	 *          getSelectedCategory().equals(cat) <br/>
	 *          ==> \result == size();
	 * 
	 * @return le nombre d'annonces de cet utilisateur dans l'état spécifié et
	 *         appartenant à la catégorie spécifiée
	 * @pure
	 */
	public int size(AdState state, Optional<Category> cat) {
		return -1;
	}

	/**
	 * Renvoie la ième plus récente annonce de ce User dans l'état et la catégorie
	 * sélectionnée.
	 * 
	 * Cette opération ne concerne les annonces dans l'état sélectionné (open,
	 * closed ou purchases).
	 * 
	 * Si une catégorie est sélectionnée, cette opération ne concerne que les
	 * annonces de cette catégorie, pour les annonces dans l'état sélectionné.
	 * 
	 * @param i index de l'annonce cherchée
	 * 
	 * @return la ième plus récente annonce de ce User
	 * 
	 * @requires i >= 0 && i < size();
	 * @ensures \result != null;
	 * @ensures \result.equals(get(getSelectedAdState(), getSelectedCategory(), i);
	 * 
	 * @pure
	 */
	public ClassifiedAd get(int i) {
		return null;
	}

	/**
	 * Renvoie le ième élément de la liste des annonces étant dans l'état spécifié
	 * et appartenant à la catégorie spécifiée.
	 * 
	 * @param state l'état des annonces
	 * @param cat   catégorie des annonces
	 * @param i     index de l'élément dans la liste
	 * 
	 * @return le ième élément de la liste des annonces étant dans l'état spécifié
	 *         et appartenant à la catégorie spécifiée
	 * 
	 * @requires state != null;
	 * @requires cat != null;
	 * @requires i >= 0 && i < size(state, cat);
	 * @ensures \result != null;
	 * @ensures containsInState(state, \result);
	 * @ensures cat.isPresent() ==> \result.getCategory().equals(cat.get());
	 * 
	 * @pure
	 */
	public ClassifiedAd get(AdState state, Optional<Category> cat, int i) {
		return null;
	}

	/**
	 * Renvoie true si ce User possède parmi les annonces dans l'état spécifié,
	 * l'objet spécifié.
	 * 
	 * @param state état des annonces parmi lesquelles l'objet est recherché
	 * @param obj   annonce cherchée
	 * @return true si ce User possède parmi les annonces dans l'état spécifié,
	 *         l'objet spécifié; false sinon
	 * 
	 * @requires state != null;
	 * @ensures !(obj instanceof ClassifiedAd) ==> !\result;
	 * @ensures \result <==> (\exists int i; <br/>
	 *          i >= 0 && i < size(state, Optional.empty()); <br/>
	 *          get(state, Optional.empty(), i).equals(obj));
	 * @pure
	 */
	public boolean containsInState(AdState state, Object obj) {
		return false;
	}

	/**
	 * Initialise ce User pour le démarrage d'une nouvelle itération sur les
	 * annonces de ce User. Cette itération s'effectue à partir des annonces les
	 * plus récentes, de sorte que chaque appel à next() renvoie une annonce plus
	 * ancienne.
	 * 
	 * Cette itération ne concerne que les annonces dans l'état sélectionné (open,
	 * closed ou purchase).
	 * 
	 * Si une catégorie est sélectionnée, cette itération ne concerne que les
	 * annonces de cette catégorie.
	 * 
	 * @ensures !hasPrevious();
	 * @ensures previousIndex() == -1;
	 * @ensures nextIndex() == 0;
	 * @ensures lastIndex() == -1;
	 */
	public void startIteration() {
	}

	/**
	 * Renvoie true si ce User possède une annonce plus ancienne pour l'itération en
	 * cours.
	 * 
	 * Cette itération ne concerne que le type d'annonces sélectionné (open ads,
	 * closed ads ou purchases).
	 * 
	 * Si une catégorie est sélectionnée, cette itération ne concerne que les
	 * annonces de cette catégorie.
	 * 
	 * 
	 * 
	 * @return true si ce User possède une annonce plus ancienne pour l'itération en
	 *         cours
	 * 
	 * @ensures \result <==> nextIndex() < size();
	 * 
	 * @pure
	 */
	public boolean hasNext() {
		return false;
	}

	/**
	 * Renvoie l'annonce suivante (plus ancienne) dans l'itération en cours et
	 * avance d'un élément dans l'itération.
	 * 
	 * Cette itération ne concerne que le type d'annonces sélectionné (open ads,
	 * closed ads ou purchases).
	 * 
	 * Si une catégorie est sélectionnée, cette itération ne concerne que les
	 * annonces de cette catégorie.
	 * 
	 * @return l'annonce suivante (plus ancienne) dans l'itération en cours
	 * 
	 * @requires hasNext();
	 * @ensures \result != null;
	 * @ensures \result.equals(\old(get(nextIndex())));
	 * @ensures \result.equals(get(lastIndex()));
	 * @ensures getSelectedCategory().isPresent() ==><br/>
	 *          \result.getCategory().equals(getSelectedCategory().get());
	 * @ensures containsInState(getSelectedAdState(), \result);
	 * @ensures nextIndex() == \old(nextIndex()) + 1;
	 * @ensures previousIndex() == \old(previousIndex()) + 1;
	 * @ensures previousIndex() == \old(nextIndex());
	 * @ensures lastIndex() == \old(nextIndex());
	 * @ensures lastIndex() == previousIndex();
	 */
	public ClassifiedAd next() {
		return null;
	}

	/**
	 * Renvoie l'index de l'annonce qui sera renvoyée par le prochain appel à
	 * next(). Si l'itération est arrivée à la fin la valeur renvoyée est size().
	 * 
	 * Cette itération ne concerne que le type d'annonces sélectionné (open ads,
	 * closed ads ou purchases).
	 * 
	 * Si une catégorie est sélectionnée, cette itération ne concerne que les
	 * annonces de cette catégorie.
	 * 
	 * @return l'index de l'annonce qui sera renvoyée par le prochain appel à
	 *         next(); ou size()
	 * 
	 * @ensures \result == size() <==> !hasNext();
	 * @ensures hasNext() <==> \result >= 0 && \result < size();
	 * 
	 * @pure
	 */
	public int nextIndex() {
		return -1;
	}

	/**
	 * Renvoie true si ce User possède une annonce plus récente pour l'itération en
	 * cours.
	 * 
	 * Cette itération ne concerne que le type d'annonces sélectionné (open ads,
	 * closed ads ou purchases).
	 * 
	 * Si une catégorie est sélectionnée, cette itération ne concerne que les
	 * annonces de cette catégorie.
	 * 
	 * @return true si ce User possède une annonce plus récente pour l'itération en
	 *         cours
	 * 
	 * @ensures \result <==> previousIndex() >= 0;
	 * @ensures !\result <==> previousIndex() == -1;
	 * 
	 * @pure
	 */
	public boolean hasPrevious() {
		return false;
	}

	/**
	 * Renvoie l'annonce précedente (plus récente) dans l'itération en cours et
	 * recule d'un élément dans l'itération.
	 * 
	 * Cette itération ne concerne que le type d'annonces sélectionné (open ads,
	 * closed ads ou purchases).
	 * 
	 * Si une catégorie est sélectionnée, cette itération ne concerne que les
	 * annonces de cette catégorie.
	 * 
	 * @return l'annonce précedente (plus récente) dans l'itération en cours
	 * 
	 * @requires hasPrevious();
	 * @ensures \result.equals(\old(get(previousIndex())));
	 * @ensures \result.equals(get(lastIndex()));
	 * @ensures getSelectedCategory().isPresent() ==><br/>
	 *          \result.getCategory().equals(getSelectedCategory().get());
	 * @ensures containsInState(getSelectedAdState(), \result);
	 * @ensures nextIndex() == \old(nextIndex()) - 1;
	 * @ensures previousIndex() == \old(previousIndex()) - 1;
	 * @ensures nextIndex() == \old(previousIndex());
	 * @ensures lastIndex() == \old(previousIndex());
	 * @ensures lastIndex() == nextIndex();
	 */
	public ClassifiedAd previous() {
		return null;
	}

	/**
	 * Renvoie l'index de l'annonce qui sera renvoyée par le prochain appel à
	 * previous(). Si l'itération est arrivée au début la valeur renvoyée est -1.
	 * 
	 * Cette itération ne concerne que le type d'annonces sélectionné (open ads,
	 * closed ads ou purchases).
	 * 
	 * Si une catégorie est sélectionnée, cette itération ne concerne que les
	 * annonces de cette catégorie.
	 * 
	 * @return l'index de l'annonce qui sera renvoyé par le prochain appel à previous();
	 *         ou -1
	 * 
	 * @ensures \result == -1 <==> !hasPrevious();
	 * @ensures hasPrevious() <==> \result >= 0 && \result < size();
	 * 
	 * @pure
	 */
	public int previousIndex() {
		return -1;
	}
	
	/**
	 * Renvoie l'index de l'annonce qui a été renvoyée par le dernier appel à
	 * previous() ou next(). Si next() ou previous() n'ont pas été appelé depuis le
	 * dernier appel à startIteration() la valeur renvoyée est -1.
	 * 
	 * Cette itération ne concerne que le type d'annonces sélectionné (open ads,
	 * closed ads ou purchases).
	 * 
	 * Si une catégorie est sélectionnée, cette itération ne concerne que les
	 * annonces de cette catégorie.
	 *  
	 * @return l'index de l'annonce qui a été renvoyé par le dernier appel à
	 *         previous() ou next(); ou -1
	 * 
	 * @ensures \result == nextIndex() || \result == previousIndex();
	 * 
	 * @pure
	 */
	public int lastIndex() {
		return -1;
	}
	/**
	 * Renvoie une chaîne de caractères contenant le nom de ce User ainsi que le
	 * nombre d'annonces de cet utilisateur dans les toirs états possibles (OPEN,
	 * CLOSED, PURCHASE).
	 * 
	 * @return une chaîne de caractères contenant le nom de ce User ainsi que le
	 *         nombre d'annonces de cet utilisateur
	 * 
	 * @ensures \result != null;
	 * @ensures \result.contains(getName());
	 * @ensures \result.contains("" + size(OPEN, Optional.empty()));
	 * @ensures \result.contains("" + size(CLOSED, Optional.empty()));
	 * @ensures \result.contains("" + size(PURCHASE, Optional.empty()));
	 * 
	 * @pure
	 */
	@Override
	public String toString() {
		return null;
	}
}
