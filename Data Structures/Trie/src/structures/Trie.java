package structures;

import java.util.ArrayList;

/**
 * This class implements a compressed trie. Each node of the tree is a CompressedTrieNode, with fields for
 * indexes, first child and sibling.
 * 
 * @author Sesh Venugopal
 *
 */
public class Trie {
	
	/**
	 * Words indexed by this trie.
	 */
	ArrayList<String> words;
	
	/**
	 * Root node of this trie.
	 */
	TrieNode root;
	
	/**
	 * Initializes a compressed trie with words to be indexed, and root node set to
	 * null fields.
	 * 
	 * @param words
	 */
	public Trie() {
		root = new TrieNode(null, null, null);
		words = new ArrayList<String>();
	}
	
	/**
	 * Inserts a word into this trie. Converts to lower case before adding.
	 * The word is first added to the words array list, then inserted into the trie.
	 * 
	 * @param word Word to be inserted.
	 */
	public void insertWord(String word) {
		
	    if(word == null || word.equals("")) 
	    returnÍ¾
	    HashMap<Character, TrieNode> children = root.childrenÍ¾
	    for(int i=0Í¾ i<word.length()Í¾ i++){
	        char tmp = word.charAt(i)Í¾
	        TrieNode tÍ¾
	        if(children.containsKey(tmp)){
	          t = children.get(tmp)Í¾
	        }else{
	          t = new TrieNode(tmp)Í¾
	        children.put(tmp, t)Í¾
	        }
	        children = t.childrenÍ¾
	        if(i == word.length() 1)
	          t.isLeaf = trueÍ¾
	}
	
	/**
	 * Given a string prefix, returns its "completion list", i.e. all the words in the trie
	 * that start with this prefix. For instance, if the tree had the words bear, bull, stock, and bell,
	 * the completion list for prefix "b" would be bear, bull, and bell; for prefix "be" would be
	 * bear and bell; and for prefix "bell" would be bell. (The last example shows that a prefix can be
	 * an entire word.) The order of returned words DOES NOT MATTER. So, if the list contains bear and
	 * bell, the returned list can be either [bear,bell] or [bell,bear]
	 * 
	 * @param prefix Prefix to be completed with words in trie
	 * @return List of all words in tree that start with the prefix, order of words in list does not matter.
	 *         If there is no word in the tree that has this prefix, null is returned.
	 */
	public ArrayList<String> completionList(String prefix) {
	    ArrayList<String> cList = new ArrayList<String>();
	    completionList(root, cList, prefix);
	}
	
	private ArrayList<String> completionList(TrieNode root, ArrayList<String> cList, String prefix) {
		if (root == null) {
			return null;
		}/
		if(root.substr != null) {
			boolean x = false;
			if(words.get(root.substr.wordIndex).startsWith(prefix)){
				x = true;
			}
			if(x && !cList.contains(words.get(root.substr.wordIndex))) {
				set.add(words.get(root.substr.wordIndex));
			}
		}
		
		
		for (TrieNode ptr=root.firstChild; ptr != null; ptr=ptr.sibling) {
			completionList(ptr, cList, prefix);
		}
		return CList;
	}
	
	
	public void print() {
		print(root, 1, words);
	}
	
	private static void print(TrieNode root, int indent, ArrayList<String> words) {
		if (root == null) {
			return;
		}
		for (int i=0; i < indent-1; i++) {
			System.out.print("    ");
		}
		
		if (root.substr != null) {
			System.out.println("      " + words.get(root.substr.wordIndex));
		}
		
		for (int i=0; i < indent-1; i++) {
			System.out.print("    ");
		}
		System.out.print(" ---");
		System.out.println("(" + root.substr + ")");
		
		for (TrieNode ptr=root.firstChild; ptr != null; ptr=ptr.sibling) {
			for (int i=0; i < indent-1; i++) {
				System.out.print("    ");
			}
			System.out.println("     |");
			print(ptr, indent+1, words);
		}
	}
 }
