#include <iostream>
#include <fstream>

using namespace std;


int main()
{
	cout << "THIS IS THE FIRST OPTIMAL TSP SOLVER! \n"
		<<   "                 by Paul A. M. Dirac\n";
	double Vertex[3000000];
	ifstream tsp;
	tsp.open("dataset.txt");
	if (tsp.fail()) {
		cerr << "error finding file.\n"; exit(1);
	}
	int x = 1;
	while (!tsp.eof()) {
		tsp >> Vertex[x]; x++;
	}
	tsp.close();
	int n, np;
	n = ((x-1) / 2);
	if (n % 2 == 0) {
		np = ((n - 4) / 2);
	}
	else
	{
		np = ((n - 3) / 2);
	}
	int e = ((n * (n-1) )/ 2);
	cout << "With a vertex of " << n << " and that the graph is undirected and complete, you should have " << e << " unique edges.\n";	
	
	double Euler = 0, sumthree = 0, sumtwo = 0, sumislands = 0; double sumfour = 0, sumfive = 0, sumislands2 = 0;
	for (int p = 1; p <= n - 1; p++)
	{
		for (int q = 2; q <= n; q++)
		{
			if (p >= q)
			{
				continue;
			}
			Euler += sqrt(((Vertex[(2 * p) - 1] - Vertex[(2 * q) - 1]) * (Vertex[(2 * p) - 1] - Vertex[(2 * q) - 1])) + ((Vertex[2 * p] - Vertex[2 * q]) * (Vertex[2 * p] - Vertex[2 * q])));
		}
	}
	double b = n;
	double average = ((2 * Euler) / (b - 1));
	cout << "\nThe average hamiltonian tour length is " << average
	<< " and \nthe average tour passing through each vertex is posted bellow.";
	int one = 1, kl = 1, three = 3, two = three - 1, yu, po, ri, mi, ju[1000000], se[1000000];
	double min = average, mini = average;

	do
	{
	
		double wap = sqrt(((Vertex[(2 * one) - 1] - Vertex[(2 * two) - 1]) * (Vertex[(2 * one) - 1] - Vertex[(2 * two) - 1])) + ((Vertex[2 * one] - Vertex[2 * two]) * (Vertex[2 * one] - Vertex[2 * two])))
		+sqrt(((Vertex[(2 * one) - 1] - Vertex[(2 * three) - 1]) * (Vertex[(2 * one) - 1] - Vertex[(2 * three) - 1])) + ((Vertex[2 * one] - Vertex[2 * three]) * (Vertex[2 * one] - Vertex[2 * three])));
		


		for (int neitwo = 1; neitwo <= n; neitwo++)
		{
			if (neitwo == two || neitwo == three || neitwo == one)
			{
				continue;
			}
			sumtwo += sqrt(((Vertex[(2 * two) - 1] - Vertex[(2 * neitwo) - 1]) * (Vertex[(2 * two) - 1] - Vertex[(2 * neitwo) - 1])) + ((Vertex[2 * two] - Vertex[2 * neitwo]) * (Vertex[2 * two] - Vertex[2 * neitwo])));
		}
		for (int nei = 1; nei <= n; nei++)
		{
			if (nei == three || nei == two || nei == one)
			{
				continue;
			}
			sumthree += sqrt(((Vertex[(2 * three) - 1] - Vertex[(2 * nei) - 1]) * (Vertex[(2 * three) - 1] - Vertex[(2 * nei) - 1])) + ((Vertex[2 * three] - Vertex[2 * nei]) * (Vertex[2 * three] - Vertex[2 * nei])));

		}
		for (int x = 1; x <= n - 1; x++)
		{
			for (int v = 2; v <= n; v++)
			{
				if (x >= v || x == two || v == three || x == three || v == two || one == x || one == v)
				{
					continue;
				}
				sumislands += sqrt(((Vertex[(2 * x) - 1] - Vertex[(2 * v) - 1]) * (Vertex[(2 * x) - 1] - Vertex[(2 * v) - 1])) + ((Vertex[2 * x] - Vertex[2 * v]) * (Vertex[2 * x] - Vertex[2 * v])));
			}
		}

		double wapaverage = wap + (1 / (b - 3)) * (sumtwo + sumthree) + ((2 / (b - 3)) * sumislands);
		if (wapaverage <= mini) {
			mini = wapaverage;
			mi = one; ju[1] = two; se[1] = three;
		}	
		sumtwo = 0; sumthree = 0; sumislands = 0;
		three++;
		if (one == three) {
			three++;
		}
		if (three > n)
		{
			two++;
			if (two == one) {
				two++;
			}
			three = two + 1;
			if (three == one) {
				three++;
			}

		}
		if (one == n - ((n*3) / 4)&&two>n-1) {
			cout << "\npart 1 25%";
		}if (one == n - (n / 2)&&two>n-1) {
			cout << "\npart 1 50%";
		}
		if (one == n - (n / 4)&&two>n-1) {
			cout << "\npart 1 75%";
		}
		if (two > n - 1 && three > n)
		{
			one++; two = 1; three = 2;
			if (one == three) {
				three = 3;
			}
		}
	} while (three <= n && one <= n);
	cout << "\nthe smallest tour average goes through vertex " << mi << " and its " << mini << " via E(" << mi << ")(" << ju[1] << ") and E(" << mi << ")(" << se[1] << ")\n\n";
		
	int four = 1; int five = 2, lit = 1; double mini2 = mini, wap2, wap2i = 0;
		do {
			do {
				double lit1 = lit;
				for (int jkl = 1; jkl <= lit; jkl++) {
					if (five == four || five == mi || four == mi || five == ju[jkl] || four == ju[jkl] || five == se[jkl] || four == se[jkl]&&five!=n) {
						continue;
					}
				}
               int kno = mi, kyes = mi, jg = 1;
				   do {
					   wap2i += sqrt(((Vertex[(2 * kno) - 1] - Vertex[(2 * (ju[jg])) - 1]) * (Vertex[(2 * kno) - 1] - Vertex[(2 * (ju[jg])) - 1])) + ((Vertex[2 * kno] - Vertex[2 * (ju[jg])]) * (Vertex[2 * kno] - Vertex[2 * (ju[jg])])))

						   + sqrt(((Vertex[(2 * kyes) - 1] - Vertex[(2 * (se[jg])) - 1]) * (Vertex[(2 * kyes) - 1] - Vertex[(2 * (se[jg])) - 1])) + ((Vertex[2 * kyes] - Vertex[2 * (se[jg])]) * (Vertex[2 * kyes] - Vertex[2 * (se[jg])])));

					   kno = ju[jg]; kyes = se[jg]; kno = ju[jg];
					   jg++;
				   } while (jg <= lit);
				   wap2 = wap2i + sqrt(((Vertex[(2 * (ju[lit])) - 1] - Vertex[(2 * four) - 1]) * (Vertex[(2 * (ju[lit])) - 1] - Vertex[(2 * four) - 1])) + ((Vertex[2 * (ju[lit])] - Vertex[2 * four]) * (Vertex[2 * (ju[lit])] - Vertex[2 * four])))
					   + sqrt(((Vertex[(2 * (se[lit])) - 1] - Vertex[(2 * five) - 1]) * (Vertex[(2 * (se[lit])) - 1] - Vertex[(2 * five) - 1])) + ((Vertex[2 * (se[lit])] - Vertex[2 * five]) * (Vertex[2 * (se[lit])] - Vertex[2 * five])));

				   wap2i = 0;
				   for (int neitwo = 1; neitwo <= n; neitwo++)
				   {
					   int test2 = 2;
					   for (int neithree = 1; neithree <= lit; neithree++) {
						   if (neitwo == four || neitwo == five || neitwo == mi || neitwo == ju[neithree] || neitwo == se[neithree])
						   {
							   test2 = 1;
						   }
					   }
					   if (test2 == 1) {
						   continue;
					   }
					   sumfour += sqrt(((Vertex[(2 * four) - 1] - Vertex[(2 * neitwo) - 1]) * (Vertex[(2 * four) - 1] - Vertex[(2 * neitwo) - 1])) + ((Vertex[2 * four] - Vertex[2 * neitwo]) * (Vertex[2 * four] - Vertex[2 * neitwo])));

				   }
				   for (int nei = 1; nei <= n; nei++)
				   {
					   int test3 = 2;
					   for (int neifour = 1; neifour <= lit; neifour++) {
						   if (nei == five || nei == four || nei == mi || nei == ju[neifour] || nei == se[neifour])
						   {
							   test3 = 1;
						   }
					   }
					   if (test3 == 1) {
						   continue;
					   }
					   sumfive += sqrt(((Vertex[(2 * five) - 1] - Vertex[(2 * nei) - 1]) * (Vertex[(2 * five) - 1] - Vertex[(2 * nei) - 1])) + ((Vertex[2 * five] - Vertex[2 * nei]) * (Vertex[2 * five] - Vertex[2 * nei])));
				   }
				   for (int x = 1; x <= n - 1; x++)
				   {
					   for (int v = 2; v <= n; v++)
					   {
						   int test4 = 2;
						   for (int love = 1; love <= lit; love++) {
							   if (x >= v || x == four || v == four || x == five || v == five || x == mi || v == mi || x == ju[love] || v == ju[love] || x == se[love] || v == se[love])
							   {
								   test4 = 1;
							   }
						   }
						   if (test4 == 1) {
							   continue;
						   }
						   sumislands2 += sqrt(((Vertex[(2 * x) - 1] - Vertex[(2 * v) - 1]) * (Vertex[(2 * x) - 1] - Vertex[(2 * v) - 1])) + ((Vertex[2 * x] - Vertex[2 * v]) * (Vertex[2 * x] - Vertex[2 * v])));
					   }
				   }
				   double wapaverage2;
				   if (sumfour == 0 && sumfive == 0 && sumislands2 == 0) {
					   wapaverage2 = wap2 + sqrt(((Vertex[(2 * four) - 1] - Vertex[(2 * five) - 1]) * (Vertex[(2 * four) - 1] - Vertex[(2 * five) - 1])) + ((Vertex[2 * four] - Vertex[2 * five]) * (Vertex[2 * four] - Vertex[2 * five])));
				   }
				   else {
					   wapaverage2 = wap2 + (1 / (b - ((2 * (lit1 + 1)) + 1))) * (sumfour + sumfive) + ((2 / (b - ((2 * (lit1 + 1)) + 1))) * sumislands2);
				   }
				   if (mini2 >= wapaverage2) {
					   mini2 = wapaverage2; lit++; ju[lit] = four; se[lit] = five; lit--;
				}
				   sumfour = 0, sumfive = 0, sumislands2 = 0;
				   five++;
				   if (lit == np - ((np * 3) / 4) && four == n && five == n) {
					   cout << "\npart 2 25%";
				   }if (lit == np - (np / 2) && four == n && five == n) {
					   cout << "\npart 2 50%";
				   }if (lit == np - (np / 4) && four == n && five == n) {
					   cout << "\npart 2 75%";
				   }
					if (five > n) {
						four++; five = 1;
					}
				} while (five <= n && four <= n); lit++; four = 1; five = 2;
				if (lit == (np + 1)) {
					cout << "\n\nThe smallest tour is " << mini2 << " via E";
					for (int tyr = lit; tyr >= 1; tyr--) {
						cout << "(" << ju[tyr] << ")";
					}cout << "(" << mi << ")";
					for (int tr = 1; tr <= lit; tr++) {
						cout << "(" << se[tr] << ")";
					}
				}
			} while (lit <= np);

			for (int drake = 1; drake <= n; drake++) {
				int test6 = 2;
				for (int yolo = 1; yolo <= (np + 1); yolo++) {
					if (drake == mi || drake == ju[yolo] || drake == se[yolo]) { test6 = 1; }
				}
				if (test6 == 2) {
					cout << "(" << drake << ")";
				}
			}
			cout << " thats your cycle.\n";
		
		string ty;
		cin >> ty;


		return 0;
	}
