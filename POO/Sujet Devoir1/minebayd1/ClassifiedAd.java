package minebayd1;

import java.time.Instant;

/**
 * Une petite annonce d'un utilisateur de l'application Minebay. Chaque annonce
 * est caractérisée par sa description, sa date de création, sa catégorie et son
 * prix.
 * 
 * Cette classe est non modifiable.
 * 
 * @invariant getDescription() != null;
 * @invariant !getDescription().isBlank();
 * @invariant getCategory() != null;
 * @invariant getPrice() > 0;
 * @invariant getDate() != null;
 * 
 * @author Marc Champesme
 * @since 27/09/2024
 * @version 20/10/2024
 */
public class ClassifiedAd {
	private Category cat;
	private String desc;
	private int price;
	private Instant date;
	/**
	 * Initialise une nouvelle annonce. La date de cette nouvelle annonce est la
	 * date courante au moment de l'exécution de ce constructeur.
	 * 
	 * @param cat   la catégorie de l'objet mis en vente
	 * @param desc  la description de l'objet mis en vente
	 * @param price le prix de l'objet mis en vente par cette annonce
	 * 
	 * @requires cat != null;
	 * @requires desc != null;
	 * @requires !desc.isBlank();
	 * @requires price > 0;
	 * @ensures getCategory().equals(cat);
	 * @ensures getDescription().equals(desc);
	 * @ensures getPrice() == price;
	 * @ensures getDate() != null;
	 * @ensures \old(Instant.now()).isBefore(getDate());
	 * @ensures getDate().isBefore(Instant.now());
	 */
	public ClassifiedAd(Category cat, String desc, int price) {
		this.cat = cat;
		this.desc = desc;
		this.price = price;
		this.date = Instant.now();
	}

	/**
	 * Renvoie la date de création de cette annonce.
	 * 
	 * @return la date de création de cette annonce
	 * 
	 * @pure
	 */
	public Instant getDate() {
		return this.date;
	}

	/**
	 * Renvoie la description de cette annonce.
	 * 
	 * @return la description de cette annonce
	 * 
	 * @pure
	 */
	public String getDescription() {
		return this.desc;
	}

	/**
	 * Teste si cette annonce a été publiée avant l'annonce spécifiée.
	 * 
	 * @param ad l'annonce à comparer avec cette annonce
	 * 
	 * @return true si et seulement si la date de cette annonce est strictement antérieure
	 *         à celle de l'annonce spécifiée
	 * 
	 * @requires ad != null;
	 * @ensures \result <==> this.getDate().isBefore(ad.getDate());
	 * @ensures !(\result && this.isAfter(ad));
	 * 
	 * @pure
	 */
	public boolean isBefore(ClassifiedAd ad) {
		return this.getDate().isBefore(ad.getDate());

	}

	/**
	 * Teste si cette annonce a été publié après l'annonce spécifiée.
	 * 
	 * @param ad l'annonce à comparer avec cette annonce
	 * 
	 * @return true si et seulement si la date de cette annonce est strictement
	 *         postérieure à celle de l'annonce spécifiée
	 * 
	 * @requires ad != null;
	 * @ensures \result <==> this.getDate().isAfter(ad.getDate());
	 * @ensures !(\result && this.isBefore(ad));
	 * 
	 * @pure
	 */
	public boolean isAfter(ClassifiedAd ad) {
		return this.getDate().isAfter(ad.getDate());
	}

	/**
	 * Renvoie true si l'objet spécifié est une ClassifiedAd ayant les mêmes
	 * caractéristiques que cette ClassifiedAd.
	 * 
	 * @param obj l'objet à comparer avec cette ClassifiedAd
	 * 
	 * @return true si l'objet spécifié est une ClassifiedAd ayant les mêmes
	 *         caractéristiques que cette ClassifiedAd; false sinon
	 * 
	 * @ensures !(obj instanceof ClassifiedAd) ==> !\result;
	 * @ensures \result <==> (obj instanceof ClassifiedAd) &&
	 *          getDate().equals(((ClassifiedAd) obj).getDate()) &&
	 *          getDescription().equals(((ClassifiedAd) obj).getDescription()) &&
	 *          getCategory().equals(((ClassifiedAd) obj).getCategory()) &&
	 *          getPrice() == ((ClassifiedAd) obj).getPrice());
	 * @ensures \result ==> hashCode() == obj.hashCode());
	 * 
	 * @pure
	 */
	@Override
	public boolean equals(Object obj) {
		if(! (obj instanceof ClassifiedAd)) return false;
		ClassifiedAd ad = (ClassifiedAd) obj;
		if(! (getDate().equals(ad.getDate()))) return false;
		if(! (getDescription().equals(ad.getDescription()))) return false;
		if(! (getCategory().equals(ad.getCategory()))) return false;
		if(! (getPrice() == (ad.getPrice()))) return false;
		return true;
	}

	/**
	 * Renvoie un code de hashage pour cette ClassifiedAd.
	 * 
	 * @return un code de hashage pour cette ClassifiedAd
	 * 
	 * @pure
	 */
	@Override
	public int hashCode() {
        int code = cat.hashCode();
        code = 15 * code + desc.hashCode();
        code = 15 * code + price;
        code = 15 * code + date.hashCode();
        return code;
    }
	

	/**
	 * Renvoie une chaîne de caractères contenant les caractéristiques de cette ClassifiedAd.
	 * 
	 * @return une chaîne de caractères contenant les caractéristiques de cette ClassifiedAd
	 * 
	 * @ensures \result != null;
	 * @ensures \result.contains(getCategory().toString());
	 * @ensures \result.contains(getDate().toString());
	 * @ensures \result.contains(getDescription().toString());
	 * @ensures \result.contains("" + getPrice());
	 * @pure
	 */
	@Override
	public String toString() {
		String str = "L'annonce porte sur" + getCategory() + ":" + getDescription() + "mise le :" + getDate() + ", coute :" + getPrice();
		return str;
	}

	/**
	 * Renvoie la catégorie de cette annonce.
	 * 
	 * @return la catégorie de cette annonce
	 * 
	 * @pure
	 */
	public Category getCategory() {
		return this.cat;
	}

	/**
	 * Renvoie le prix de l'objet mis en vente par cette annonce.
	 * 
	 * @return le prix de l'objet à vendre
	 * 
	 * @pure
	 */
	public int getPrice() {
		return this.price;
	}

}
