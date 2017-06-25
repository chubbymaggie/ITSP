package info.debatty.java.stringsimilarity;

import info.debatty.java.stringsimilarity.interfaces.StringDistance;
import java.util.HashSet;
import java.util.Map;
import java.util.Set;
import net.jcip.annotations.Immutable;

/**
 * Q-gram distance, as defined by Ukkonen in "Approximate string-matching with
 * q-grams and maximal matches". The distance between two strings is defined as
 * the L1 norm of the difference of their profiles (the number of occurences of
 * each n-gram): SUM( |V1_i - V2_i| ). Q-gram distance is a lower bound on
 * Levenshtein distance, but can be computed in O(m + n), where Levenshtein
 * requires O(m.n).
 *
 * @author Thibault Debatty
 */
@Immutable
public class QGram extends ShingleBased implements StringDistance {

    /**
     * Q-gram similarity and distance. Defined by Ukkonen in "Approximate
     * string-matching with q-grams and maximal matches",
     * http://www.sciencedirect.com/science/article/pii/0304397592901434 The
     * distance between two strings is defined as the L1 norm of the difference
     * of their profiles (the number of occurences of each k-shingle). Q-gram
     * distance is a lower bound on Levenshtein distance, but can be computed in
     * O(|A| + |B|), where Levenshtein requires O(|A|.|B|)
     *
     * @param k
     */
    public QGram(final int k) {
        super(k);
    }

    /**
     * Q-gram similarity and distance. Defined by Ukkonen in "Approximate
     * string-matching with q-grams and maximal matches",
     * http://www.sciencedirect.com/science/article/pii/0304397592901434 The
     * distance between two strings is defined as the L1 norm of the difference
     * of their profiles (the number of occurence of each k-shingle). Q-gram
     * distance is a lower bound on Levenshtein distance, but can be computed in
     * O(|A| + |B|), where Levenshtein requires O(|A|.|B|)
     * Default k is 3.
     */
    public QGram() {
        super();
    }

    /**
     * The distance between two strings is defined as the L1 norm of the
     * difference of their profiles (the number of occurence of each k-shingle).
     * @param s1
     * @param s2
     * @return
     */
    public final double distance(final String s1, final String s2) {
        Map<String, Integer> profile1 = getProfile(s1);
        Map<String, Integer> profile2 = getProfile(s2);

        Set<String> union = new HashSet<String>();
        union.addAll(profile1.keySet());
        union.addAll(profile2.keySet());

        int agg = 0;
        for (String key : union) {
            int v1 = 0;
            int v2 = 0;
            if (profile1.containsKey(key)) {
                v1 = profile1.get(key);
            }


            if (profile2.containsKey(key)) {
                v2 = profile2.get(key);
            }
            agg += Math.abs(v1 - v2);
        }
        return agg;
    }
}
