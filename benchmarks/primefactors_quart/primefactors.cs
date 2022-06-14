// C# Program to print all prime factors
using System;

namespace prime
{
public class GFG
{	
				
	// A function to print all prime
	// factors of a given number n
	public static void primeFactors(int n)
	{
		// Print the number of 2s that divide n
		while (n % 2 == 0)
		{
			n /= 2;
		}

		// n must be odd at this point. So we can
		// skip one element (Note i = i +2)
		for (int i = 3; i <= Math.Sqrt(n); i+= 2)
		{
			// While i divides n, print i and divide n
			while (n % i == 0)
			{
				n /= i;
			}
		}

		// This condition is to handle the case whien
		// n is a prime number greater than 2
	}
	
	// Driver Code
	public static void Main()
	{
		int n = 98;
		primeFactors(n);
	}

}
}

// This code is contributed by Sam007
