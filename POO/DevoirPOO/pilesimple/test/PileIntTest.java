/**
 *
 */
package pilesimple.test;

import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertNotSame;
import static org.junit.jupiter.api.Assertions.assertFalse;
import static org.junit.jupiter.api.Assertions.assertNotNull;
import static org.junit.jupiter.api.Assertions.assertNull;
import static org.junit.jupiter.api.Assertions.assertTrue;
import static org.junit.jupiter.api.Assertions.fail;
import static org.junit.jupiter.api.Assumptions.assumeTrue;
import static org.junit.jupiter.api.Assumptions.assumeFalse;
import static org.junit.jupiter.api.Assertions.assertThrows;

import java.util.stream.IntStream;
import java.util.stream.Stream;

import org.junit.jupiter.params.ParameterizedTest;
import org.junit.jupiter.params.provider.Arguments;
import org.junit.jupiter.params.provider.MethodSource;

import test.BaseDataProviders;

import pilesimple.PileInt;

/**
 * Tests unitaires pour la classe PileInt.
 *
 * @author Marc Champesme
 * @since 1er juin 2020
 * @version 4 juin 2020
 *
 */
class PileIntTest {
	//////////////////////////////////////////////////
	// Méthodes produisant les données de test:
	//////////////////////////////////////////////////
	public static IntStream intProvider() {
		return BaseDataProviders.intProvider(-5, 30);
	}

	public static Stream<Arguments> tabIntAndIntProvider() {
		return BaseDataProviders.tabIntProvider()
				.map(tab -> Arguments.of(tab, (tab != null) ? BaseDataProviders.randInt(tab.length + 5) : 5));

	}

	public static Stream<PileInt> pileIntProvider() {
		return BaseDataProviders.tabIntProvider().filter(tab -> (tab != null && tab.length > 0))
				.map(tab -> new PileInt(tab, BaseDataProviders.randInt(tab.length)));

	}

	public static Stream<Arguments> pileIntAndIntProvider() {
		return pileIntProvider().map(p -> Arguments.of(p, BaseDataProviders.randInt(50)));
	}

	public static Stream<Arguments> pileIntAndObjectProvider() {
		return pileIntProvider().flatMap(p -> Stream.concat(BaseDataProviders.baseObjectProvider(), pileIntProvider())
				.map(obj -> Arguments.of(p, obj)));
	}
	//////////////////////////////////////////////////
	// Fin des méthodes produisant les données de test
	//////////////////////////////////////////////////

	//////////////////////////////////////////////////
	// Attributs et méthodes pour tester la "pûreté" des méthodes
	//////////////////////////////////////////////////
	// State for purity test:
	private int sommet;
	private int nbElements;
	private int nbMaxElements;
	private int hashCode;
	private boolean estVide;

	private void saveState(PileInt self) {
		nbElements = self.getNbElements();
		nbMaxElements = self.getNbMaxElements();
		hashCode = self.hashCode();
		estVide = self.estVide();
		if (!estVide) {
			sommet = self.getSommet();
		}
	}

	private void assertPurity(PileInt self) {
		assertEquals(estVide, self.estVide());
		if (!self.estVide()) {
			assertEquals(sommet, self.getSommet());
		}
		assertEquals(nbElements, self.getNbElements());
		assertEquals(nbMaxElements, self.getNbMaxElements());
		assertEquals(hashCode, self.hashCode());
	}
	//////////////////////////////////////////////////

	/**
	 * Teste l'invariant de classe pour l'instance spécifiée.
	 *
	 * @param self l'instance à tester
	 */
	public void assertInvariant(PileInt self) {
		assertTrue(self.getNbElements() >= 0);
		assertTrue(self.getNbElements() <= self.getNbMaxElements());
	}

	//////////////////////////////////////////////////
	// Méthodes de test:
	//////////////////////////////////////////////////
	/**
	 * Test method for {@link pilesimple.PileInt#PileInt(int)}.
	 *
	 * Initialise une pile vide avec la capacité maximale spécifiée.
	 *
	 * @param nbMaxElements la capacit&eacute; maximale de la pile
	 *
	 * @requires nbMaxElements >= 0;
	 * @requires nbMaxElements < Integer.MAX_VALUE;
	 * @ensures estVide();
	 * @ensures (getNbMaxElements() == nbMaxElements);
	 * @ensures (getNbElements() == 0);
	 *
	 */
	@ParameterizedTest
	@MethodSource("intProvider")
	public final void testPileIntInt(int nbMaxElements) {
		// préconditions
		assumeTrue(nbMaxElements >= 0);
		assumeTrue(nbMaxElements < Integer.MAX_VALUE);

		// Exécution
		PileInt self = new PileInt(nbMaxElements);

		// Post conditions
		assertTrue(self.estVide());
		assertEquals(self.getNbMaxElements(), nbMaxElements);
		assertEquals(self.getNbElements(), 0);

		// Invariant
		assertInvariant(self);
	}

	/**
	 * Test method for {@link pilesimple.PileInt#PileInt(int[], int)}.
	 *
	 * Initialise une pile avec les nbElements premiers éléments du tableau spécifié
	 * et une capacité maximale correspondant à la taille du tableau spécifié.
	 *
	 * @param elements   le tableau dont les éléments doivent être utilisés pour
	 *                   initialiser la pile.
	 * @param nbElements le nombre d'éléments du tableau à utiliser pour pour
	 *                   initialiser la pile.
	 *
	 * @requires elements != null;
	 * @requires nbElements >= 0;
	 * @requires nbElements <= elements.length;
	 * @ensures (getNbMaxElements() == elements.length);
	 * @ensures (getNbElements() == nbElements);
	 * @ensures (nbElements > 0) ==> getSommet() == elements[nbElements - 1];
	 *
	 * @throws NullPointerException     si le tableau spécifié est null
	 * @throws IllegalArgumentException si nbElements < 0 ou nbElements >
	 *                                  elements.length
	 */
	@ParameterizedTest
	@MethodSource("tabIntAndIntProvider")
	public final void testPileIntIntArrayInt(int[] elements, int nbElements) {
		// préconditions
		assumeTrue(elements != null);
		assumeTrue(nbElements >= 0);
		assumeTrue(nbElements <= elements.length);

		// Exécution
		PileInt self = new PileInt(elements, nbElements);

		// Post conditions
		assertEquals(self.getNbMaxElements(), elements.length);
		assertEquals(self.getNbElements(), nbElements);
		if (nbElements > 0) {
			assertEquals(self.getSommet(), elements[nbElements - 1]);
		} else {
			assertTrue(self.estVide());
		}
		if (nbElements == elements.length) {
			assertTrue(self.estPleine());
		}

		// Invariant
		assertInvariant(self);
	}

	/**
	 * Test method for {@link pilesimple.PileInt#getNbMaxElements()}.
	 *
	 * Consultation du nombre maximal d'&eacute;l&eacute;ment pouvant etre
	 * stock&eacute;s dans la pile.
	 *
	 * @return capacit&eacute; maximale de la pile
	 *
	 * @pure
	 */
	@ParameterizedTest
	@MethodSource("pileIntProvider")
	public final void testGetNbMaxElements(PileInt self) {
		assumeTrue(self != null);
		// Invariant
		assertInvariant(self);

		// préconditions

		// Purity
		saveState(self);

		// Exécution
		self.getNbMaxElements();

		// Post conditions

		// Purity
		assertPurity(self);

		// Invariant
		assertInvariant(self);
	}

	/**
	 * Test method for {@link pilesimple.PileInt#getNbElements()}.
	 *
	 * Consultation du nombre d'&eacute;l&eacute;ments pr&eacute;sents dans la pile.
	 *
	 * @return nombre d'&eacute;l&eacute;ments de la pile
	 *
	 * @pure
	 */
	@ParameterizedTest
	@MethodSource("pileIntProvider")
	public final void testGetNbElements(PileInt self) {
		assumeTrue(self != null);
		// Invariant
		assertInvariant(self);

		// préconditions

		// Purity
		saveState(self);

		// Exécution
		self.getNbElements();

		// Post conditions

		// Purity
		assertPurity(self);

		// Invariant
		assertInvariant(self);
	}

	/**
	 * Test method for {@link pilesimple.PileInt#getSommet()}.
	 *
	 * Consultation du sommet de pile
	 *
	 * @return le sommet de pile
	 *
	 * @requires !estVide();
	 *
	 * @pure
	 */
	@ParameterizedTest
	@MethodSource("pileIntProvider")
	public final void testGetSommet(PileInt self) {
		assumeTrue(self != null);
		// Invariant
		assertInvariant(self);

		// préconditions
		assumeFalse(self.estVide());

		// Purity
		saveState(self);

		// Exécution
		self.getSommet();

		// Post conditions

		// Purity
		assertPurity(self);

		// Invariant
		assertInvariant(self);
	}

	/**
	 * Test method for {@link pilesimple.PileInt#empiler(int)}.
	 *
	 * Ajout au sommet de la pile (i.e. push).
	 *
	 * @param e L'&eacute;lement &agrave; ajouter
	 *
	 * @requires !estPleine();
	 * @ensures !estVide();
	 * @ensures getSommet() == e;
	 * @ensures (getNbElements() == \old(getNbElements()) + 1);
	 *
	 */
	@ParameterizedTest
	@MethodSource("pileIntAndIntProvider")
	public final void testEmpiler(PileInt self, int e) {
		assumeTrue(self != null);
		// Invariant
		assertInvariant(self);

		// préconditions
		assumeFalse(self.estPleine());

		// Old
		int oldNbElements = self.getNbElements();

		// Exécution
		self.empiler(e);

		// Post conditions
		assertFalse(self.estVide());
		assertEquals(self.getSommet(), e);
		assertEquals(self.getNbElements(), oldNbElements + 1);

		// Invariant
		assertInvariant(self);
	}

	/**
	 * Test method for {@link pilesimple.PileInt#depiler()}.
	 *
	 * Retrait de l'&eacute;lement au sommet de la pile (i.e. pop)
	 *
	 * @requires !estVide();
	 * @ensures !estPleine();
	 * @ensures (getNbElements() == \old(getNbElements()) - 1);
	 *
	 */
	@ParameterizedTest
	@MethodSource("pileIntProvider")
	public final void testDepiler(PileInt self) {
		assumeTrue(self != null);
		// Invariant
		assertInvariant(self);

		// préconditions
		assumeFalse(self.estVide());

		// Old
		int oldNbElements = self.getNbElements();

		// Exécution
		self.depiler();

		// Post conditions
		assertFalse(self.estPleine());
		assertEquals(self.getNbElements(), oldNbElements - 1);

		// Invariant
		assertInvariant(self);
	}

	/**
	 * Test method for {@link pilesimple.PileInt#estPleine()}.
	 *
	 * La pile est-elle pleine ? M&eacute;thode n&eacute;cessaire &agrave;
	 * l'impl&eacute;mentation par tableau de taille fixe. M&eacute;thode &agrave;
	 * utiliser avant tout ajout.
	 *
	 * @return true si la pile est pleine (capacit&eacute; atteinte), false sinon.
	 *
	 * @ensures (\result <==> (getNbElements() == getNbMaxElements()));
	 *
	 * @pure
	 */
	@ParameterizedTest
	@MethodSource("pileIntProvider")
	public final void testEstPleine(PileInt self) {
		assumeTrue(self != null);
		// Invariant
		assertInvariant(self);

		// préconditions

		// Purity
		saveState(self);

		// Exécution
		boolean result = self.estPleine();

		// Post conditions
		assertTrue(result == (self.getNbElements() == self.getNbMaxElements()));

		// Purity
		assertPurity(self);

		// Invariant
		assertInvariant(self);
	}

	/**
	 * Test method for {@link pilesimple.PileInt#estVide()}.
	 *
	 * La pile est-elle vide ? M&eacute;thode &agrave; utiliser avant toute
	 * suppression ou consultation.
	 *
	 * @return true si la pile est vide, false sinon.
	 *
	 * @ensures (\result == (getNbElements() == 0));
	 *
	 * @pure
	 */
	@ParameterizedTest
	@MethodSource("pileIntProvider")
	public final void testEstVide(PileInt self) {
		assumeTrue(self != null);
		// Invariant
		assertInvariant(self);

		// préconditions

		// Purity
		saveState(self);

		// Exécution
		boolean result = self.estVide();

		// Post conditions
		assertTrue(result == (self.getNbElements() == 0));

		// Purity
		assertPurity(self);

		// Invariant
		assertInvariant(self);
	}

	/**
	 * Test method for {@link pilesimple.PileInt#equals(java.lang.Object)}.
	 *
	 * Compare cette <code>PileInt</code> avec l'objet spécifié. Le résultat est
	 * <code>true</code> si et seulement si l'objet spécifié est une instance de
	 * <code>PileInt</code> contenant les mêmes éléments que cette
	 * <code>PileInt</code> et dont le nombre maximal d'éléments est le même que
	 * cette <code>PileInt</code>.
	 *
	 * @param o l'objet à comparer avec cette <code>PileInt</code>.
	 *
	 * @return <code>true</code> si les deux objets contiennent les mêmes éléments,
	 *         <code>false</code> sinon.
	 *
	 * @also
	 * @ensures !(o instanceof PileInt) ==> !\result;
	 * @ensures \result ==> ((o instanceof PileInt) && (this.getNbElements() ==
	 *          ((PileInt) o).getNbElements()) && (this.getNbMaxElements() ==
	 *          ((PileInt) o).getNbMaxElements()));
	 * @ensures (\result && !estVide()) ==> (this.getSommet() == ((PileInt)
	 *          o).getSommet());
	 * @ensures \result ==> (this.hashCode() == o.hashCode());
	 * @ensures \result ==> (this.toString().equals(o.toString()));
	 *
	 *
	 * @pure
	 */
	@ParameterizedTest
	@MethodSource("pileIntAndObjectProvider")
	public final void testEqualsObject(PileInt self, Object o) {
		assumeTrue(self != null);

		// Exécution
		boolean result = self.equals(o);

	}

	/**
	 * Test method for {@link pilesimple.PileInt#clone()}.
	 *
	 * Renvoie une copie conforme de cette <code>PileInt</code>.
	 *
	 * @return un clone de cette instance.
	 *
	 * @also
	 * @ensures \result != null;
	 * @ensures \result != this;
	 * @ensures \result.getClass() == \result.getClass();
	 * @ensures this.equals(\result);
	 *
	 * @pure
	 */
	@ParameterizedTest
	@MethodSource("pileIntProvider")
	public final void testClone(PileInt self) {
		assumeTrue(self != null);
		// Invariant
		assertInvariant(self);

		// préconditions

		// Purity
		saveState(self);

		// Exécution
		Object result = self.clone();

		// Post conditions
		assertNotNull(result);
		assertNotSame(self, result);
		assertEquals(self.getClass(), result.getClass());
		assertEquals(self, result);

		// Purity
		assertPurity(self);

		// Invariant
		assertInvariant(self);
	}

	/**
	 * Test method for {@link pilesimple.PileInt#hashCode()}.
	 *
	 * Renvoie un code de hashage pour cette instance.
	 *
	 * @return un code de hashage pour cette instance.
	 *
	 * @pure
	 */
	@ParameterizedTest
	@MethodSource("pileIntProvider")
	public final void testHashCode(PileInt self) {
		assumeTrue(self != null);
		// Invariant
		assertInvariant(self);

		// préconditions

		// Purity
		saveState(self);

		// Exécution
		self.hashCode();

		// Post conditions

		// Purity
		assertPurity(self);

		// Invariant
		assertInvariant(self);
	}

	/**
	 * Test method for {@link pilesimple.PileInt#toString()}.
	 *
	 * Renvoie une représentation de cette instance sous forme de chaîne de
	 * caractère.
	 *
	 * @return une chaîne de caractères représentant cette instance.
	 *
	 * @also
	 * @ensures \result != null;
	 *
	 * @pure
	 */
	@ParameterizedTest
	@MethodSource("pileIntProvider")
	public final void testToString(PileInt self) {
		assumeTrue(self != null);
		// Invariant
		assertInvariant(self);

		// préconditions

		// Purity
		saveState(self);

		// Exécution
		String result = self.toString();

		// Post conditions
		assertNotNull(result);

		// Purity
		assertPurity(self);

		// Invariant
		assertInvariant(self);
	}

}
