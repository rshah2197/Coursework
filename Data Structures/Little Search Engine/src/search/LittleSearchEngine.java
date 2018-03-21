package search;

import java.io.*;
import java.util.*;

/**
* This class encapsulates an occurrence of a keyword in a document. It stores
* the document name, and the frequency of occurrence in that document.
* Occurrences are associated with keywords in an index hash table.
*
*/
class Occurrence {
   /**
   * Document in which a keyword occurs.
   */
   String document;

   /**
   * The frequency (number of times) the keyword occurs in the above document.
   */
   int frequency;

   /**
   * Initializes this occurrence with the given document,frequency pair.
   *
   * @param doc
   * Document name
   * @param freq
   * Frequency
   */
   public Occurrence(String doc, int freq) {
       document = doc;
       frequency = freq;
   }

   /*
   * (non-Javadoc)
   *
   * @see java.lang.Object#toString()
   */
   public String toString() {
       return "(" + document + "," + frequency + ")";
   }
}

/**
* This class builds an index of keywords. Each keyword maps to a set of
* documents in which it occurs, with frequency of occurrence in each document.
* Once the index is built, the documents can searched on for keywords.
*
*/
public class LittleSearchEngine {

   /**
   * This is a hash table of all keywords. The key is the actual keyword, and
   * the associated value is an array list of all occurrences of the keyword
   * in documents. The array list is maintained in descending order of
   * occurrence frequencies.
   */
   HashMap<String, ArrayList<Occurrence>> keywordsIndex;

   /**
   * The hash table of all noise words - mapping is from word to itself.
   */
   HashMap<String, String> noiseWords;

   /**
   * Creates the keyWordsIndex and noiseWords hash tables.
   */
   public LittleSearchEngine() {
       keywordsIndex = new HashMap<String, ArrayList<Occurrence>>(1000, 2.0f);
       noiseWords = new HashMap<String, String>(100, 2.0f);
   }

   /**
   * This method indexes all keywords found in all the input documents. When
   * this method is done, the keywordsIndex hash table will be filled with all
   * keywords, each of which is associated with an array list of Occurrence
   * objects, arranged in decreasing frequencies of occurrence.
   *
   * @param docsFile
   * Name of file that has a list of all the document file names,
   * one name per line
   * @param noiseWordsFile
   * Name of file that has a list of noise words, one noise word
   * per line
   * @throws FileNotFoundException
   * If there is a problem locating any of the input files on disk
   */
   public void makeIndex(String docsFile, String noiseWordsFile)
           throws FileNotFoundException {

       boolean valid = false;

       while (!valid) {
           try {
               Scanner sc = new Scanner(new File(noiseWordsFile));
               valid = true;
           } catch (FileNotFoundException e) {
               return;
           }
       }

       Scanner sc = new Scanner(new File(noiseWordsFile));

       while (sc.hasNext()) {
           String word = sc.next();
           noiseWords.put(word, word);
       }

       valid = false;

       while (!valid) {
           try {
               sc = new Scanner(new File(docsFile));
               valid = true;
           } catch (FileNotFoundException e) {
               return;
           }
       }

       sc = new Scanner(new File(docsFile));
       while (sc.hasNext()) {
           String docFile = sc.next();
           HashMap<String, Occurrence> kws = loadKeyWords(docFile);
           mergeKeyWords(kws);
       }

   }

   /**
   * Scans a document, and loads all keywords found into a hash table of
   * keyword occurrences in the document. Uses the getKeyWord method to
   * separate keywords from other words.
   *
   * @param docFile
   * Name of the document file to be scanned and loaded
   * @return Hash table of keywords in the given document, each associated
   * with an Occurrence object
   * @throws FileNotFoundException
   * If the document file is not found on disk
   */
   public HashMap<String, Occurrence> loadKeyWords(String docFile)
           throws FileNotFoundException {

       HashMap<String, Occurrence> keywords = new HashMap<String, Occurrence>();

       boolean valid = false;

       while (!valid) {
           try {
               Scanner words = new Scanner(new File(docFile));
               valid = true;
           } catch (FileNotFoundException e) {
               return keywords;
           }
       }

       Scanner words = new Scanner(new File(docFile));
       int freq = 1;

       while (words.hasNext()) {
           String word = words.next();

           if (getKeyWord(word) != null) {
               word = getKeyWord(word);
               if (!keywords.containsKey(word)) {
                   Occurrence occurs = new Occurrence(docFile, freq);
                   keywords.put(word, occurs);
               } else {
                   keywords.get(word).frequency++;
               }
           }
       }

       return keywords;
   }

   /**
   * Merges the keywords for a single document into the master keywordsIndex
   * hash table. For each keyword, its Occurrence in the current document must
   * be inserted in the correct place (according to descending order of
   * frequency) in the same keyword's Occurrence list in the master hash
   * table. This is done by calling the insertLastOccurrence method.
   *
   * @param kws
   * Keywords hash table for a document
   */
   public void mergeKeyWords(HashMap<String, Occurrence> kws) {

       ArrayList<Occurrence> list = new ArrayList<Occurrence>();

       for (String key : kws.keySet()) {
           Occurrence occ = kws.get(key);

           if (!keywordsIndex.containsKey(key)) {
               ArrayList<Occurrence> occurList = new ArrayList<Occurrence>();
               occurList.add(occ);
               keywordsIndex.put(key, occurList);
           } else {
               list = keywordsIndex.get(key);
               list.add(occ);
               insertLastOccurrence(list);
               keywordsIndex.put(key, list);
           }
       }
   }

   /**
   * Given a word, returns it as a keyword if it passes the keyword test,
   * otherwise returns null. A keyword is any word that, after being stripped
   * of any trailing punctuation, consists only of alphabetic letters, and is
   * not a noise word. All words are treated in a case-INsensitive manner.
   *
   * Punctuation characters are the following: '.', ',', '?', ':', ';' and '!'
   *
   * @param word
   * Candidate word
   * @return Keyword (word without trailing punctuation, LOWER CASE)
   */
   public String getKeyWord(String word) {

       word = word.trim();
       char end = word.charAt(word.length() - 1);

       while (end == '.' || end == ',' || end == '?' || end == ':'
               || end == ';' || end == '!') {
           word = word.substring(0, word.length() - 1);

           if (word.length() > 1) {
               end = word.charAt(word.length() - 1);
           } else {
               break;
           }
       }

       word = word.toLowerCase();

       for (String noiseWord : noiseWords.keySet()) {
           if (word.equalsIgnoreCase(noiseWord)) {
               return null;
           }
       }

       for (int j = 0; j < word.length(); j++) {
           if (!Character.isLetter(word.charAt(j))) {
               return null;
           }
       }

       return word;
   }

   /**
   * Inserts the last occurrence in the parameter list in the correct position
   * in the same list, based on ordering occurrences on descending
   * frequencies. The elements 0..n-2 in the list are already in the correct
   * order. Insertion is done by first finding the correct spot using binary
   * search, then inserting at that spot.
   *
   * @param occs
   * List of Occurrences
   * @return Sequence of mid point indexes in the input list checked by the
   * binary search process, null if the size of the input list is 1.
   * This returned array list is only used to test your code - it is
   * not used elsewhere in the program.
   */
   public ArrayList<Integer> insertLastOccurrence(ArrayList<Occurrence> occs) {

       if (occs.size() == 1) {
           return null;
       }

       int lastFreq = occs.get(occs.size() - 1).frequency;
       Occurrence temp = occs.get(occs.size() - 1);
       int lo = 0;
       int hi = occs.size() - 1;
       int mid;
       ArrayList<Integer> midIndex = new ArrayList<Integer>();

       while (lo <= hi) {
           mid = (lo + hi) / 2;
           midIndex.add(mid);

           if (lastFreq > occs.get(mid).frequency) {
               hi = mid - 1;
           } else if (lastFreq < occs.get(mid).frequency) {
               lo = mid + 1;
           } else {
               break;
           }
       }

       if (midIndex.get(midIndex.size() - 1) == 0) {
           if (temp.frequency < occs.get(0).frequency) {
               occs.add(1, temp);
               occs.remove(occs.size() - 1);

               return midIndex;
           }
       }

       occs.add(midIndex.get(midIndex.size() - 1), temp);
       occs.remove(occs.size() - 1);

       /*
       * for(int i = 0; i < midIndex.size(); i++) {
       * System.out.print(midIndex.get(i) + " "); }
       *
       * System.out.println();
       */

       return midIndex;
   }

   /**
   * Search result for "kw1 or kw2". A document is in the result set if kw1 or
   * kw2 occurs in that document. Result set is arranged in descending order
   * of occurrence frequencies. (Note that a matching document will only
   * appear once in the result.) Ties in frequency values are broken in favor
   * of the first keyword. (That is, if kw1 is in doc1 with frequency f1, and
   * kw2 is in doc2 also with the same frequency f1, then doc1 will appear
   * before doc2 in the result. The result set is limited to 5 entries. If
   * there are no matching documents, the result is null.
   *
   * @param kw1
   * First keyword
   * @param kw1
   * Second keyword
   * @return List of NAMES of documents in which either kw1 or kw2 occurs,
   * arranged in descending order of frequencies. The result size is
   * limited to 5 documents. If there are no matching documents, the
   * result is null.
   */
   public ArrayList<String> top5search(String kw1, String kw2) {

       ArrayList<String> resultList = new ArrayList<String>();
       ArrayList<Occurrence> listOne = new ArrayList<Occurrence>();

       kw1 = kw1.toLowerCase();

       if (keywordsIndex.get(kw1) != null) {
           listOne = keywordsIndex.get(kw1);
       }

       ArrayList<Occurrence> listTwo = new ArrayList<Occurrence>();

       kw2 = kw2.toLowerCase();

       if (keywordsIndex.get(kw2) != null) {
           listTwo = keywordsIndex.get(kw2);
       }

       /*
       * System.out.println(); System.out.print(kw1 + ": "); for(int i = 0; i
       * < listOne.size(); i++) { //print out the first list if(i+1 ==
       * listOne.size()) { System.out.print(listOne.get(i)); } else {
       * System.out.print(listOne.get(i) + ", "); } }
       *
       * System.out.println(); System.out.print(kw2 + ": "); for(int i = 0; i
       * < listTwo.size(); i++) { //print out the second list if(i+1 ==
       * listTwo.size()) { System.out.print(listTwo.get(i)); } else {
       * System.out.print(listTwo.get(i) + ", "); } }
       */

       for (int p = 0; p < listOne.size(); p++) {
           if (resultList.size() <= 4) {
               int L1 = listOne.get(p).frequency;
               String docNameOne = listOne.get(p).document;

               for (int w = 0; w < listTwo.size(); w++) {
                   String docNameTwo = listTwo.get(w).document;
                   int L2 = listTwo.get(w).frequency;

                   if (L2 <= L1) {
                       if (!resultList.contains(docNameOne)
                               && resultList.size() <= 4) {
                           resultList.add(docNameOne);
                       }
                   } else if (L2 > L1) {
                       if (!resultList.contains(docNameTwo)
                               && resultList.size() <= 4) {
                           resultList.add(docNameTwo);
                       }
                   }
               }
           }
       }

       for (int k = 0; k < resultList.size(); k++) {
           if (k + 1 == resultList.size()) {
               System.out.print(resultList.get(k));
           } else {
               System.out.print(resultList.get(k) + ", ");
           }
       }

       if (resultList.size() == 0) {
           return null;
       }

return resultList;
   }
}
