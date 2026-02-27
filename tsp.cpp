#include <iostream>


using namespace std;


int main()
{
	cout << "THIS IS THE FIRST OPTIMAL TSP SOLVER! \n"
		<< "                    by Michel Seraphin"
		<< "\n\nFirst, how many vertex does your graph have?\n";

	int n, np;
	cin >> n;
	if (n % 2 == 0) {
	np=((n-4)/2)	;
	}
	else
	{
		np=((n-3)/2);
	}
	int e = ((n * (n - 1)) / 2);
	cout << "With a vertex of " << n << " and that the graph is undirected and complete, you should have " << e << " unique edges.\n";
	cout << "Now enter the weights in the following order requested bellow.\n";
	double Edges[50][50];
	for (int h = 1; h <= n - 1; h++)
	{
		for (int t = 2; t <= n; t++)
		{
			if (h >= t)
			{
				continue;
			}
			cout << "E(" << h <<")("<< t << ")   ";
			cin >> Edges[h][t];

		}


	}
	double Euler = 0, sumthree = 0, sumtwo = 0, sumislands = 0; double sumfour = 0, sumfive = 0, sumislands2=0;
	for (int p = 1; p <= n - 1; p++)
	{
		for (int q = 2; q <= n; q++)
		{
			if (p >= q)
			{
				continue;
			}
			Euler += Edges[p][q];
		}
	}
	double b = n;
	double average = ((2 * Euler) / (b - 1));
	cout << "\nThe average hamiltonian tour length is " << average
		<< " and \nthe average tour passing through each vertex is posted bellow.";
	int one = 1, kl = 1, three = 3, two = three - 1, yu, po, ri, mi, ju[50], se[50], gh = 1;;
	double min = average, mini = average;

	do
	{
		if (one > two)
		{
			Edges[one][two] = Edges[two][one];
		}if (one > three)
		{
			Edges[one][three] = Edges[three][one];
		}

		double wap = Edges[one][two] + Edges[one][three];


		for (int neitwo = 1; neitwo <= n; neitwo++)
		{
			if (neitwo < two)
			{
				Edges[two][neitwo] = Edges[neitwo][two];
			}
			if (neitwo == two || neitwo == three || neitwo == one)
			{
				continue;
			}
			sumtwo += Edges[two][neitwo];

		}
		for (int nei = 1; nei <= n; nei++)
		{
			if (nei < three)
			{
				Edges[three][nei] = Edges[nei][three];
			}
			if (nei == three || nei == two || nei == one)
			{
				continue;
			}
			sumthree += Edges[three][nei];
		}
		for (int x = 1; x <= n - 1; x++)
		{
			for (int v = 2; v <= n; v++)
			{
				if (x >= v || x == two || v == three || x == three || v == two || one == x || one == v)
				{
					continue;
				}
				sumislands += Edges[x][v];
			}
		}

		double wapaverage[1176]; 
		wapaverage[gh] = wap + (1 / (b - 3)) * (sumtwo + sumthree) + ((2 / (b - 3)) * sumislands);


		cout << "\n\nAverage of going through vertex V" << one << " via E(" << one << ")(" << two << ") and E(" << one << ")(" << three << ") is ("
			<< wap << ")" << "+(1/(" << (n - 3)
			<< "))*(" << sumtwo + sumthree << ")+(2/(" << n - 3 << ")*(" << sumislands << ")="
			<< wapaverage[gh];

		if (wapaverage[gh] <= mini) {
			mini = wapaverage[gh];
			mi = one; ju[1] = two; se[1] = three;

		}
		int repeat = 1;
			for (int repeated = 1; repeated <= gh - 1; repeated++) {
				if (wapaverage[repeated] == wapaverage[gh]) {
					repeat++;
				}
	}
			if (repeat > 1) {
				cout << "         this value has repeated " << repeat << " times so far.";
			}
		if (min >= wapaverage[gh]) {
			yu = one, po = two, ri = three;
			min = wapaverage[gh];
		}

		gh++;

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

		if (two > n - 1 && three > n)
		{
			cout << "\nThe minimum average tour going through vertex " << yu << " is " << min
				<< " via E(" << yu <<")("<< po << ") and E(" << yu << ")("<<ri << ")\n\n";
			min = average;
			one++; two = 1; three = 2;
			if (one == three) {
				three = 3;
			}
		}


	} while (three <= n && one <= n);
	cout << "\nThe minimum average tour going through vertex " << yu << " is " << min
		<< " via E(" << yu <<")("<< po << ") and E(" << yu <<")("<< ri << ")\n\n";


	cout << "the smallest tour average goes through vertex "<<mi<<" and its " << mini << " via E(" << mi <<")("<< ju[1] << ") and E(" << mi << ")("<<se[1]<<")\n\n";
	if (n == 4) {
		cout << "Therefore the smallest cycle is E(" << ju[1] << ")(" << mi << ")(" << se[1] << ")";

		for (int drake = 1; drake <= n; drake++) {
		
				if (drake != mi && drake != ju[1] && drake != se[1]) {
			
				cout << "(" << drake << ") and its "<< mini<<"\n";
			}
		}
	}
	else {
		int four = 1; int five = 2, lit=1, hg = 1;; double mini2 = mini, wap2, wap2i = 0;

		do {
			do {
				double lit1 = lit;
				int test5 = 2;
				for (int jkl = 1; jkl <= lit; jkl++) {
					if (five == four || five == mi || four == mi || five == ju[jkl] || four == ju[jkl] || five == se[jkl] || four == se[jkl]) {
						test5 = 1;
					}
				}

				int kno = mi, kyes = mi, jg = 1;

				if (kno > ju[jg])
				{
					Edges[kno][ju[jg]] = Edges[ju[jg]][kno];
				}
				if (kyes > se[jg]) {
					Edges[kyes][se[jg]] = Edges[se[jg]][kyes];
				}

				if (ju[lit] > four) {
					Edges[ju[lit]][four] = Edges[four][ju[lit]];
				}
				if (se[lit] > five) {
					Edges[se[lit]][five] = Edges[five][se[lit]];
				}
				if (four > five) {
					Edges[four][five] = Edges[five][four];
				}
				do {
					wap2i += Edges[kno][ju[jg]] + Edges[kyes][se[jg]]; kno = ju[jg]; kyes = se[jg]; kno = ju[jg];
					jg++;
				} while (jg <= lit);
				wap2 = wap2i + Edges[ju[lit]][four] + Edges[se[lit]][five]; wap2i = 0;
				for (int neitwo = 1; neitwo <= n; neitwo++)
				{
					if (neitwo < four)
					{
						Edges[four][neitwo] = Edges[neitwo][four];
					}
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
					sumfour += Edges[four][neitwo];

				}
				for (int nei = 1; nei <= n; nei++)
				{
					if (nei < five)
					{
						Edges[five][nei] = Edges[nei][five];
					}
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
					sumfive += Edges[five][nei];
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
						sumislands2 += Edges[x][v];
					}
				}



				double wapaverage2[1176]; 
				if (sumfour == 0 && sumfive == 0 && sumislands2 == 0) {
					wapaverage2[hg] = wap2 + Edges[four][five];
				}
				else {
					wapaverage2[hg] = wap2 + (1 / (b - ((2 * (lit1 + 1)) + 1))) * (sumfour + sumfive) + ((2 / (b - ((2 * (lit1 + 1)) + 1))) * sumislands2);
				}

				if (test5 == 2) {
					cout << "\n\nGoing through E";
					for (int tyr = lit; tyr >= 1; tyr--) {
						cout << "(" << ju[tyr] << ")";
					}cout << "(" << mi << ")";
					for (int tr = 1; tr <= lit; tr++) {
						cout << "(" << se[tr] << ")";
					}
					cout << " via E(" << ju[lit] << ")(" << four << ") and E(" << se[lit] << ")(" << five << ") is ("
						<< wap2 << ")+(1/" << (b - ((2 * (lit1 + 1)) + 1)) << ")(" << sumfive + sumfour << ")+(2/" << (b - ((2 * (lit1 + 1)) + 1)) << ")(" << sumislands2
						<< ")=" << wapaverage2[hg];

					if (mini2 >= wapaverage2[hg]) {
						mini2 = wapaverage2[hg]; lit++; ju[lit] = four; se[lit] = five; lit--;
					}
					int repeat = 1;
					for (int repeated = 1; repeated <= hg - 1; repeated++) {
						if (wapaverage2[repeated] == wapaverage2[hg]) {
							repeat++;
						}
					}
					if (repeat > 1) {
						cout << "         this value has repeated " << repeat << " times so far.";
					}
				}
			
				hg++;

				sumfour = 0, sumfive = 0, sumislands2 = 0;
				five++;
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
			else {
				cout << "\n\nThe smallest tour average is " << mini2 << " via E";
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
	}string ty;
			cin >> ty;
			
		
	return 0;
}
