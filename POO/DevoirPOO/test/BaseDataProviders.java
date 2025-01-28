/**
 * 
 */
package test;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.Collection;
import java.util.Collections;
import java.util.HashSet;
import java.util.List;
import java.util.Random;
import java.util.stream.IntStream;
import java.util.stream.Stream;

/**
 * Méthodes statiques pour simplifier la génération de données de test.
 * 
 * @author Marc Champesme
 * 
 * @since 1 novembre 2019
 * @version 31 mai 2020
 *
 */
public class BaseDataProviders {
	private static Random randGen = new Random();
	private static List<Object> objectInstances = Arrays.asList(null, new Object(), "abc", 1);
	private static List<Collection<Object>> lCollObj = new ArrayList<Collection<Object>>();
	private static List<String> stringInstances = Arrays.asList(null, "", "abc", "Abjh45", "ch1", "5662", "ch2", "ch3",
			"ch4", "ch5", "chaine6", "chaine7", "chaine8", "chaine10", "chaine11", "chaine12");
	private static List<Collection<String>> lCollStr = new ArrayList<Collection<String>>(30);
	private static List<int[]> lTabInt = new ArrayList<int[]>(30);
	private static int intStreamSize = 10;
	private static int collStrStreamSize = 10;
	
	private static void initTabInt() {
		if (!lTabInt.isEmpty()) {
			return;
		}
		lTabInt.add(null);
		lTabInt.add(new int[0]);
		for (int i = 0; i < (intStreamSize*2); i++) {
			int size = randInt(30);
			int[] tab = new int[size];
			int nbElements = (size > 0 ? randInt(size) : 0);
			for (int j = 0; j < nbElements; j++) {
				tab[j] = randGen.nextInt();
			}
			lTabInt.add(tab);
		}
	}

	private static void initCollObj() {
		if (!lCollObj.isEmpty()) {
			return;
		}
		lCollObj.add(null);
		lCollObj.add(Collections.emptyList());
		for (int i = 0; i < objectInstances.size(); i++) {
			List<Object> lobj = randomSubList(objectInstances);
			if (randBool()) {
				lCollObj.add(lobj);
			} else {
				lCollObj.add(new HashSet<Object>(lobj));
			}
		}
	}
	/**
	 * @ensures lCollStr != null;
	 * @ensures lCollStr.size() >= nbColl;
	 * 
	 * @param nbColl
	 */
	private static void initLCollStr(int nbColl) {
		if (nbColl < lCollStr.size()) {
			return;
		}
		int nbCreated = lCollStr.size();
		if (lCollStr.isEmpty()) {
			lCollStr.add(Collections.emptyList());
			lCollStr.add(null);
			nbCreated += 2;
		}
		while (nbCreated < nbColl) {
			List<String> list = randomSubList(stringInstances);
			if (randBool()) {
				lCollStr.add(list);
			} else {
				lCollStr.add(new HashSet<String>(list));
			}
			nbCreated++;
		}
	}

	public static Stream<int[]> tabIntProvider() {
		initTabInt();
		return lTabInt.stream();
	}
	public static Stream<Collection<String>> collStringProvider(int streamSize) {
		initLCollStr(streamSize);
		if (streamSize < lCollStr.size()) {
			return lCollStr.subList(0, streamSize).stream();
		}
		return lCollStr.stream();
	}

	public static Stream<Collection<String>> collStringProvider() {
		initLCollStr(collStrStreamSize);
		if (collStrStreamSize < lCollStr.size()) {
			return lCollStr.subList(0, collStrStreamSize).stream();
		}
		return lCollStr.stream();
	}

	public static Stream<Collection<? extends Object>> collObjProvider() {
		initCollObj();
		return Stream.concat(lCollObj.stream(), collStringProvider()) ;
	}
	
	public static boolean randBool() {
		return randGen.nextBoolean();
	}

	/**
	 * 
	 * @requires max > 0;
	 * @ensures \result >= 0;
	 * @ensures \result < max;
	 * 
	 * @param max
	 * @return un entier >= 0 et < max
	 * 
	 * @throws IllegalArgumentException si max <= 0
	 */
	public static int randInt(int max) {
		return randGen.nextInt(max);
	}

	/**
	 * @requires l != null;
	 * @ensures \result != null;
	 * @ensures l.containsAll(\result);
	 * @ensures \result.size() <= l.size();
	 * @ensures l.isEmpty() => \result.isEmpty();
	 * 
	 * @param <T>
	 * @param l
	 * @return
	 */
	public static <T> List<T> randomSubList(List<T> l) {
		if (l.isEmpty()) {
			return Collections.emptyList();
		}
		int upper = randGen.nextInt(l.size());
		int lower = randGen.nextInt(upper + 1);
		return l.subList(lower, upper);
	}

	/**
	 * 
	 * @requires l != null;
	 * @requires !l.isEmpty();
	 * @ensures l.contains(\result);
	 * 
	 * @param <T>
	 * @param l
	 * @return
	 */
	public static <T> T getRandomElt(List<T> l) {
		int index = randGen.nextInt(l.size());
		return l.get(index);
	}

	/**
	 * 
	 * @requires c != null;
	 * @requires !c.isEmpty();
	 * @ensures c.contains(\result);
	 * 
	 * @param <T>
	 * @param c
	 * @return
	 */
	public static <T> T getRandomElt(Collection<T> c) {
		int index = randGen.nextInt(c.size());
		int i = 0;
		for (T elt : c) {
			if (i == index) {
				return elt;
			}
			i++;
		}
		throw new IllegalArgumentException();
	}

	/**
	 * 
	 * @requires tab != null;
	 * @requires tab.length > 0;
	 * @ensures (\exists int i; i >= 0 && i < tab.length; tab[i] == \result);
	 * 
	 * @param <T>
	 * @param tab
	 * @return
	 */
	public static <T> T getRandomElt(T[] tab) {
		int index = randGen.nextInt(tab.length);
		return tab[index];
	}

	public static Stream<String> stringProvider() {
		return stringInstances.stream();
	}

	public static Stream<String> stringProvider(int maxStreamSize) {
		if (stringInstances.size() > maxStreamSize) {
			return stringInstances.subList(0, maxStreamSize).stream();
		}
		return stringInstances.stream();
	}

	public static Stream<Object> baseObjectProvider() {
		return Stream.concat(stringProvider(), objectInstances.stream());
	}

	public static IntStream intProvider() {
		return randGen.ints(intStreamSize);
	}

	public static IntStream intProvider(int min, int max) {
		return randGen.ints(intStreamSize, min, max);
	}

	public static Stream<Integer> intergerProvider() {
		return intProvider().boxed();
	}

}
