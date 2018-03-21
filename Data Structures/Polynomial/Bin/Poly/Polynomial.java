package poly;

import java.io.*;
import java.util.StringTokenizer;

/**
 * This class implements a term of a polynomial.
 * 
 * @author runb-cs112
 *
 */
class Term {
	/**
	 * Coefficient of term.
	 */
	public float coeff;
	
	/**
	 * Degree of term.
	 */
	public int degree;
	
	/**
	 * Initializes an instance with given coefficient and degree.
	 * 
	 * @param coeff Coefficient
	 * @param degree Degree
	 */
	public Term(float coeff, int degree) {
		this.coeff = coeff;
		this.degree = degree;
	}
	
	/* (non-Javadoc)
	 * @see java.lang.Object#equals(java.lang.Object)
	 */
	public boolean equals(Object other) {
		return other != null &&
		other instanceof Term &&
		coeff == ((Term)other).coeff &&
		degree == ((Term)other).degree;
	}
	
	/* (non-Javadoc)
	 * @see java.lang.Object#toString()
	 */
	public String toString() {
		if (degree == 0) {
			return coeff + "";
		} else if (degree == 1) {
			return coeff + "x";
		} else {
			return coeff + "x^" + degree;
		}
	}
}

/**
 * This class implements a linked list node that contains a Term instance.
 * 
 * @author runb-cs112
 *
 */
class Node {
	
	/**
	 * Term instance. 
	 */
	Term term;
	
	/**
	 * Next node in linked list. 
	 */
	Node next;
	
	/**
	 * Initializes this node with a term with given coefficient and degree,
	 * pointing to the given next node.
	 * 
	 * @param coeff Coefficient of term
	 * @param degree Degree of term
	 * @param next Next node
	 */
	public Node(float coeff, int degree, Node next) {
		term = new Term(coeff, degree);
		this.next = next;
	}
}

/**
 * This class implements a polynomial.
 * 
 * @author runb-cs112
 *
 */
public class Polynomial {
	
	/**
	 * Pointer to the front of the linked list that stores the polynomial. 
	 */ 
	Node poly;
	
	/** 
	 * Initializes this polynomial to empty, i.e. there are no terms.
	 *
	 */
	public Polynomial() {
		poly = null;
	}
	
	/**
	 * Reads a polynomial from an input stream (file or keyboard). The storage format
	 * of the polynomial is:
	 * <pre>
	 *     <coeff> <degree>
	 *     <coeff> <degree>
	 *     ...
	 *     <coeff> <degree>
	 * </pre>
	 * with the guarantee that degrees will be in descending order. For example:
	 * <pre>
	 *      4 5
	 *     -2 3
	 *      2 1
	 *      3 0
	 * </pre>
	 * which represents the polynomial:
	 * <pre>
	 *      4*x^5 - 2*x^3 + 2*x + 3 
	 * </pre>
	 * 
	 * @param br BufferedReader from which a polynomial is to be read
	 * @throws IOException If there is any input error in reading the polynomial
	 */
	public Polynomial(BufferedReader br) throws IOException {
		String line;
		StringTokenizer tokenizer;
		float coeff;
		int degree;
		
		poly = null;
		
		while ((line = br.readLine()) != null) {
			tokenizer = new StringTokenizer(line);
			coeff = Float.parseFloat(tokenizer.nextToken());
			degree = Integer.parseInt(tokenizer.nextToken());
			poly = new Node(coeff, degree, poly);
		}
	}
	
	
	/**
	 * Returns the polynomial obtained by adding the given polynomial p
	 * to this polynomial - DOES NOT change this polynomial
	 * 
	 * @param p Polynomial to be added
	 * @return A new polynomial which is the sum of this polynomial and p.
	 */
	public Polynomial add(Polynomial p) {
		    Node curre = poly;
			Node pcurre = p.poly;
			
			// The sum and the new third polynomial
			Polynomial polySum = new Polynomial();
			Node temp = null;
			float curreSumcoeff;
			
			
			// This avoids NullPointerException in case if they run out of terms
			while (curre != null) {
				
				if (pcurre == null){
					temp = new Node(curre.term.coeff, curre.term.degree, temp);
					curre = curre.next;
					continue;
				}
				
				else if (curre.term.degree == pcurre.term.degree) {
					curreSumcoeff = curre.term.coeff + pcurre.term.coeff;
					temp = new Node(curreSumcoeff, curre.term.degree, temp);
					
					curre = curre.next;
					pcurre = pcurre.next;
					
				}
				
				else if (curre.term.degree < pcurre.term.degree)
				{
					temp = new Node(curre.term.coeff, curre.term.degree, temp);
					curre = curre.next;
				}
				
				else if (curre.term.degree > pcurre.term.degree)
				{
					temp = new Node(pcurre.term.coeff, pcurre.term.degree, temp);
					pcurre = pcurre.next;
				}
			}
				
			
			
			// This loop assures the second polynomial avoids the absence terms
			while (pcurre != null) {
				
				if (curre == null){
					temp = new Node(pcurre.term.coeff, pcurre.term.degree, temp);
					pcurre = pcurre.next;
					continue;
				}
				
				
				else if (curre.term.degree == pcurre.term.degree) {
					curreSumcoeff = curre.term.coeff + pcurre.term.coeff;
					temp = new Node(curreSumcoeff, curre.term.degree, temp);
					
					curre = curre.next;
					pcurre = pcurre.next;
					
				}
				
				else if (curre.term.degree < pcurre.term.degree)
				{
					temp = new Node(curre.term.coeff, curre.term.degree, temp);
					curre = curre.next;
				}
				
				else if (curre.term.degree > pcurre.term.degree)
				{
					temp = new Node(pcurre.term.coeff, pcurre.term.degree, temp);
					pcurre = pcurre.next;
				}
			}
			
			// Polynomial is reversed to the right sequence in this method
			Node reve = temp;
			Node result = null;
			while (reve != null)
			{
				if (reve.term.coeff == 0)
				{
					reve = reve.next;
					continue;
				}
				else 
				{
					result = new Node(reve.term.coeff, reve.term.degree, result);
					reve = reve.next;
				}
			}
			
			if (result == null)
				return null;
			else
			{
				polySum.poly = result;
				return polySum;
			}	
	}


	
	/**
	 * Returns the polynomial obtained by multiplying the given polynomial p
	 * with this polynomial - DOES NOT change this polynomial
	 * 
	 * @param p Polynomial with which this polynomial is to be multiplied
	 * @return A new polynomial which is the product of this polynomial and p.
	 */
	public Polynomial multiply(Polynomial p) {
	    Node curre = poly;
		Node pcurre = p.poly;
		
		// The result will be zero if polynomial is empty.
		if (poly == null || p.poly == null)
		{
			Polynomial returnzero = new Polynomial();
			Node zeromult = null;
			zeromult = new Node(0, 0, zeromult);
			returnzero.poly = zeromult;
			return returnzero;
		}
		
		float curreCoeffMult;
		int curreMultDegree;
		
		// Used for linked list
		Node multtemp = null;
		Polynomial polytemp = new Polynomial();
		Polynomial polynetsum = new Polynomial();

		// Terms are added in the right sequence.
		while (curre != null)
		{
			while (pcurre != null)
			{
				curreCoeffMult = curre.term.coeff * pcurre.term.coeff;
				curreMultDegree = curre.term.degree + pcurre.term.degree;
				multtemp = new Node(curreCoeffMult, curreMultDegree, multtemp);				
				pcurre = pcurre.next;
			}
			
			// Reverses into right order
			Node reve = multtemp;
			Node multtempreversed = null;
			while (reve != null)
			{
				if (reve.term.coeff == 0)
				{
					reve = reve.next;
					continue;
				}
				else 
				{
					multtempreversed = new Node(reve.term.coeff, reve.term.degree, multtempreversed);
					reve = reve.next;
				}
			} 
			
			polytemp.poly = multtempreversed;
			
			polynetsum = polynetsum.add(polytemp);	//Add method.  It is adding the mutltiplied polynomials that were previously created
																		
			curre = curre.next;											
			pcurre = p.poly;
			multtemp = null;											
		}
		return polynetsum;
	}
	
	/**
	 * Evaluates this polynomial at the given value of x
	 * 
	 * @param x Value at which this polynomial is to be evaluated
	 * @return Value of this polynomial at x
	 */
	public float evaluate(float x) {
		double sum = 0;
		float curreCoef;
		int curredegree;
		Node curre = poly;
		
		while (curre != null) {
			curreCoef = curre.term.coeff;
			curredegree = curre.term.degree;
			
			sum = sum + curreCoef * Math.pow(x, curredegree);
			curre = curre.next;
		}
		
		return (float) sum;
	}
	
	public String toString() {
		String retval;
		
		if (poly == null) {
			return "0";
		} else {
			retval = poly.term.toString();
			for (Node current = poly.next ;
			current != null ;
			current = current.next) {
				retval = current.term.toString() + " + " + retval;
			}
			return retval;
		}
	}
}
