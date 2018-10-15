//======================================================== file = mva.c =====
//=  Mean Value Analysis program for closed queueing networks               =
//===========================================================================
//=  Notes:                                                                 =
//=    1)  Solves for a closed network of tandem queues (i.e., all          =
//=        visit ratios are 1.0)                                            =
//=    2)  Must set M (number of nodes) and N (number of customers)         =
//=        as constant values and service rate (mu[]) for each node         =
//=    3)  Nodes are labled as 1, 2, ... M                                  =
//=    4)  Output is to stdout                                              =
//=    5)  The MVA algorithm is based on the Mean Value Theorem,            =
//=          "For closed queueing networks of the exponential type, the     =
//=           state of the system at the time of arrival of a customer has  =
//=           a probability distribution equal to that of steady-state      =
//=           distribution of the network with one customer less."          =
//=-------------------------------------------------------------------------=
//= Example output (for M = 3, N = 2, mu[1] = mu[2] = mu[3] = 1.0):         =
//=                                                                         =
//=   1 customers --- thruput = 0.333333 cust/sec and delay = 3.000000 sec  =
//=         >>> n[1] = 0.333333 --- t[1] = 1.000000                         =
//=         >>> n[2] = 0.333333 --- t[2] = 1.000000                         =
//=         >>> n[3] = 0.333333 --- t[3] = 1.000000                         =
//=   2 customers --- thruput = 0.500000 cust/sec and delay = 4.000000 sec  =
//=         >>> n[1] = 0.666667 --- t[1] = 1.333333                         =
//=         >>> n[2] = 0.666667 --- t[2] = 1.333333                         =
//=         >>> n[3] = 0.666667 --- t[3] = 1.333333                         =
//=-------------------------------------------------------------------------=
//=  Build:  bcc32 mva.c, cl mva.c, gcc mva.c                               =
//=-------------------------------------------------------------------------=
//=  History:  KJC (01/11/98) - Genesis (adapted from Molloy, 1989)         =
//===========================================================================

//----- Include files -------------------------------------------------------
#include <iostream>
#include <stdio.h>   // Needed for printf()

using namespace std;

//----- Defines -------------------------------------------------------------
#define M   4        // Number of nodes
#define N   5        // Number of customers

//===========================================================================
//=  Main program                                                           =
//===========================================================================
int main()
{
	float mu[M + 1];     // mu[i] = service rate for each node
	int   count;       // Current number of customers in network
	float t[M + 1];      // t[i] = delay at node i w/ count customers in network
	float s[N + 1];      // s[count] = thruput with count customers in network
	float n[M + 1];      // n[i] = num at node i w/ count customers in network
	float total_delay; // Current total delay across all nodes (sum of n[i])
	int   i;           // Counter variable

	// Initialize service rate (cust/sec) for all M nodes
	mu[1] = 0.0771;
	mu[2] = 0.1238;
	mu[3] = 0.0804;
	mu[4] = 0.235;

	// Initialize the number of customers in all M nodes to zero
	for (i = 1; i <= M; i++)
		n[i] = 0.0;

	// Main loop to iterate for customer count from 1 to N
	for (count = 1; count <= N; count++)
	{
		// Calculate the average waiting time in each node with an
		// additional customer based on the number of customers in each
		// queue from the previous loop.  This is the "PASTA step".
		for (i = 1; i <= M; i++)
			t[i] = (1.0 / mu[i]) * (n[i] + 1.0);     // <--- Little's Law

		  // Calculate the average throughput for the network
		total_delay = 0.0;
		for (i = 1; i <= M; i++)
			total_delay = total_delay + t[i];
		s[count] = count / total_delay;            // <--- Little's Law

		// Calculate the average number of customers in each queue
		for (i = 1; i <= M; i++)
			n[i] = s[count] * t[i];                    // <--- Little's Law

		  // Output network statistics (customer count, throughput, and delay)
		printf("%d customers --- thruput = %f cust/sec and delay = %f sec \n",
			count, s[count], total_delay);

		// Output node statistics (length and delay)
		for (i = 1; i <= M; i++)
			printf("    >>> n[%d] = %f --- t[%d] = %f \n", i, n[i], i, t[i]);
	}

	cin.get();

	return 0;
}
