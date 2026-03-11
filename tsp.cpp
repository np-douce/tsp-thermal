#include <iostream>
using namespace std;
#include <cmath>
#include <string>
int main()
{
    int n;
    cout << "Enter number of vertices (max 50): ";
    cin >> n;

    if (n < 1 || n > 50)
    {
        cout << "Invalid n.\n";
        return 0;
    }

    double Edge[50][50];

    // Initialize matrix to 0
    for (int i = 0; i < 50; i++)
        for (int j = 0; j < 50; j++)
            Edge[i][j] = 0.0;

    cout << "Enter weights for each edge (i < j):\n";

    for (int i = 1; i <= n; i++)
    {
        for (int j = i + 1; j <= n; j++)
        {
            cout << "Weight of edge (" << i << "," << j << "): ";
            cin >> Edge[i][j];

            // Make symmetric
            Edge[j][i] = Edge[i][j];
        }
    }

    // Optional: print matrix to verify
    cout << "\nAdjacency Matrix:\n";
    for (int i = 1; i <= n; i++)
    {
        for (int j = 1; j <= n; j++)
        {
            cout << Edge[i][j] << "\t";
        }
        cout << endl;
    }
    double sum = 0.0, average = 0, b = n, beta=0.05;

    for (int i = 1; i < n; i++)
    {
        for (int j = i + 1; j <= n; j++)
        {
            sum += Edge[i][j];
        }
    }
    average = (2.0 / (b - 1)) * sum;
    cout << "the average  = " << average << endl;

    double S_self = 0.0;
    for (int i = 1; i <= n; i++)
    {
        for (int j = i + 1; j <= n; j++)
        {
            S_self += Edge[i][j] * Edge[i][j];
        }
    }

    double S_neighbor = 0.0;


    for (int i = 1; i <= n; i++)
    {
        for (int j = 1; j <= n; j++)
        {
            for (int k = j + 1; k <= n; k++)
            {
                if (i != j && i != k) {
                    S_neighbor += Edge[i][j] * Edge[i][k];
                }
            }
        }
    }

    double S_non_neighbor = 0.0;
    for (int i = 1; i <= n; i++)
    {
        for (int j = i + 1; j <= n; j++)
        {
            for (int k = i + 1; k <= n; k++)
            {
                for (int l = k + 1; l <= n; l++)
                {
                    if (i == k || j == l || i == l || j == k) { continue; }
                    else {
                        S_non_neighbor += Edge[i][j] * Edge[k][l];
                    }
                }
            }
        }
    }

    double stand = ((2.0 / (b - 1)) * (S_self)) + ((4.0 / ((b - 1) * (b - 2))) * (S_neighbor + (2 * S_non_neighbor))) - (average * average);
    cout << "the standard deviation = " << stand << " " << sqrt(stand) << endl;
    double entropy = lgamma(n) - log(2.0), partition=exp(entropy-(beta*average)+((beta)*(beta)*(0.5)*stand));
    cout << "entropy " << entropy << "partition " << partition; << endl;
    int ref1 = 0, ref2 = 0, CE = 0, VCE = 0, sumce = 0.0;
    double ref3; Edge[0][1] = 2;Edge[0][2] = 1; CE = 1; VCE = 2; sumce = Edge[1][2];
    for (int i = 1; i <= n - 1; i++) {
        if (Edge[0][i] == -1) { continue; }
        for (int j = i + 1; j <= n; j++) {
            double ne = 0.0, cce = 0.0, ie = 0.0, nenen = 0.0, cceccei = 0.0, ieien = 0.0, nenei = 0.0, ieiei = 0.0;
            double nene = 0.0, ccecce = 0.0, ieie = 0.0, ccenei = 0.0, cceiei = 0.0, neien = 0.0, neiei = 0.0;
            double av = 0.0, va = 0.0, ce = 0.0;
            if (Edge[0][j] == -1 || Edge[0][i] == j || Edge[0][j] == i) { continue; }
            if (Edge[0][i] != 0 && Edge[0][j] != 0) {
                sumce += Edge[i][j];
                ref1 = Edge[0][i]; ref2 = Edge[0][j];
                Edge[0][ref1] = ref2; Edge[0][ref2] = ref1;
                Edge[0][i] = -1; Edge[0][j] = -1;
                VCE++; VCE++; CE--; sumce += Edge[i][j];
                // CCE stuff ref1 and ref2
                cout << "CCE situation " << ref1 << " " << ref2 << endl;
                for (int y = 1; y < n; y++) {
                    if (Edge[0][y] == -1) { continue; }
                    for (int z = 1 + y; z <= n; z++) {
                        if (Edge[0][z] == -1) { continue; }
                        if (Edge[0][y] == 0 && Edge[0][z] != 0 || Edge[0][y] != 0 && Edge[0][z] == 0) {
                            ne += Edge[y][z]; nene += Edge[y][z] * Edge[y][z];
                            cout << " neighbor edge " << y << " " << z << " and ne is " << ne << " and nene is " << nene << endl;
                        }
                        if (Edge[0][y] != 0 && Edge[0][z] != 0 && Edge[0][y] != z && Edge[0][z] != y) {
                            cce += Edge[y][z]; ccecce += Edge[y][z] * Edge[y][z];
                            cout << " connecting chosen edge " << y << " " << z << " and cce is " << cce << " and ccecce is " << ccecce << endl;
                        }
                        if (Edge[0][y] == 0 && Edge[0][z] == 0) {
                            ie += Edge[y][z]; ieie += Edge[y][z] * Edge[y][z];
                            cout << " independent edge " << y << " " << z << " and ie is " << ie << " and ieie is " << ieie << endl;
                        }
                    }
                }
                av = sumce + ((0.5) * (1 / (b - 1 + CE - VCE)) * cce) + ((1 / (b - 1 + CE - VCE)) * ne) + ((2 / (b - 1 + CE - VCE)) * ie);
                cout << " the average for this situation is " << av << endl;
                // reinitialized here
                ce = 0.0, ne = 0.0, cce = 0.0, ie = 0.0, nenen = 0.0, cceccei = 0.0, ieien = 0.0, nenei = 0.0, ieiei = 0.0, av = 0.0, va = 0.0;
                nene = 0.0, ccecce = 0.0, ieie = 0.0, ccenei = 0.0, cceiei = 0.0, neien = 0.0, neiei = 0.0;
                Edge[0][ref1] = i; Edge[0][ref2] = j;
                Edge[0][i] = ref1; Edge[0][j] = ref2;
                ref1 = 0, ref2 = 0, ref3 = 0;
                VCE--; VCE--; CE++; sumce -= Edge[i][j];
            }
            if (Edge[0][i] == 0 && Edge[0][j] != 0) {
                sumce += Edge[i][j];
                ref2 = Edge[0][j];
                Edge[0][i] = ref2; Edge[0][ref2] = i;
                Edge[0][j] = -1;
                VCE++;
                // CCE stuff ref1 and ref2
                cout << "NE situation J " << i << " " << ref2 << endl;
                for (int y = 1; y < n; y++) {
                    if (Edge[0][y] == -1) { continue; }
                    for (int z = 1 + y; z <= n; z++) {
                        if (Edge[0][z] == -1) { continue; }
                        if (Edge[0][y] == 0 && Edge[0][z] != 0 || Edge[0][y] != 0 && Edge[0][z] == 0) {
                            ne += Edge[y][z]; nene += Edge[y][z] * Edge[y][z];
                            cout << " neighbor edge " << y << " " << z << " and ne is " << ne << " and nene is " << nene << endl;
                        }
                        if (Edge[0][y] != 0 && Edge[0][z] != 0 && Edge[0][y] != z && Edge[0][z] != y) {
                            cce += Edge[y][z]; ccecce += Edge[y][z] * Edge[y][z];
                            cout << " connecting chosen edge " << y << " " << z << " and cce is " << cce << " and ccecce is " << ccecce << endl;
                        }
                        if (Edge[0][y] == 0 && Edge[0][z] == 0) {
                            ie += Edge[y][z]; ieie += Edge[y][z] * Edge[y][z];
                            cout << " independent edge " << y << " " << z << " and ie is " << ie << " and ieie is " << ieie << endl;
                        }
                    }
                }
                av = sumce + ((0.5) * (1 / (b - 1 + CE - VCE)) * cce) + ((1 / (b - 1 + CE - VCE)) * ne) + ((2 / (b - 1 + CE - VCE)) * ie);
                cout << " the average for this situation is " << av << endl;
                // reinitialized here
                ne = 0.0, cce = 0.0, ie = 0.0, nenen = 0.0, cceccei = 0.0, ieien = 0.0, nenei = 0.0, ieiei = 0.0, av = 0.0, va = 0.0;
                nene = 0.0, ccecce = 0.0, ieie = 0.0, ccenei = 0.0, cceiei = 0.0, neien = 0.0, neiei = 0.0;
                Edge[0][i] = 0; Edge[0][ref2] = j;
                Edge[0][j] = ref2;
                ref2 = 0, ref3 = 0;
                VCE--; sumce -= Edge[i][j];
            }
            if (Edge[0][j] == 0 && Edge[0][i] != 0) {
                sumce += Edge[i][j];
                ref2 = Edge[0][i];

                Edge[0][j] = ref2; Edge[0][ref2] = j;
                Edge[0][i] = -1;
                VCE++;
                // CCE stuff ref1 and ref2
                cout << "NE situation I " << ref2 << " " << j << endl;
                for (int y = 1; y < n; y++) {
                    if (Edge[0][y] == -1) { continue; }
                    for (int z = 1 + y; z <= n; z++) {
                        if (Edge[0][z] == -1) { continue; }
                        if (Edge[0][y] == 0 && Edge[0][z] != 0 || Edge[0][y] != 0 && Edge[0][z] == 0) {
                            ne += Edge[y][z]; nene += Edge[y][z] * Edge[y][z];
                            cout << " neighbor edge " << y << " " << z << " and ne is " << ne << " and nene is " << nene << endl;
                        }
                        if (Edge[0][y] != 0 && Edge[0][z] != 0 && Edge[0][y] != z && Edge[0][z] != y) {
                            cce += Edge[y][z]; ccecce += Edge[y][z] * Edge[y][z];
                            cout << " connecting chosen edge " << y << " " << z << " and cce is " << cce << " and ccecce is " << ccecce << endl;
                        }
                        if (Edge[0][y] == 0 && Edge[0][z] == 0) {
                            ie += Edge[y][z]; ieie += Edge[y][z] * Edge[y][z];
                            cout << " independent edge " << y << " " << z << " and ie is " << ie << " and ieie is " << ieie << endl;
                        }
                    }
                }
                av = sumce + ((0.5) * (1 / (b - 1 + CE - VCE)) * cce) + ((1 / (b - 1 + CE - VCE)) * ne) + ((2 / (b - 1 + CE - VCE)) * ie);
                cout << " the average for this situation is " << av << endl;
                // reinitialized here
                ne = 0.0, ie = 0.0, nenen = 0.0, cceccei = 0.0, ieien = 0.0, nenei = 0.0, ieiei = 0.0, av = 0.0, va = 0.0;
                nene = 0.0, ccecce = 0.0, ieie = 0.0, ccenei = 0.0, cceiei = 0.0, neien = 0.0, neiei = 0.0;
                Edge[0][j] = 0; Edge[0][ref2] = i;
                Edge[0][i] = ref2;
                ref2 = 0,
                    VCE--; sumce -= Edge[i][j];
            }
            if (Edge[0][i] == 0 && Edge[0][j] == 0) {
                sumce += Edge[i][j];
                Edge[0][i] = j; Edge[0][j] = i;
                CE++; VCE++; VCE++;
                // CCE stuff ref1 and ref2
                cout << "IE situation " << i << " " << j << endl;
                for (int y = 1; y < n; y++) {
                    if (Edge[0][y] == -1) { continue; }
                    for (int z = 1 + y; z <= n; z++) {
                        if (Edge[0][z] == -1) { continue; }
                        if (Edge[0][y] == 0 && Edge[0][z] != 0 || Edge[0][y] != 0 && Edge[0][z] == 0) {
                            ne += Edge[y][z]; nene += Edge[y][z] * Edge[y][z];
                            cout << " neighbor edge " << y << " " << z << " and ne is " << ne << " and nene is " << nene << endl;
                        }
                        if (Edge[0][y] != 0 && Edge[0][z] != 0 && Edge[0][y] != z && Edge[0][z] != y) {
                            cce += Edge[y][z]; ccecce += Edge[y][z] * Edge[y][z];
                            cout << " connecting chosen edge " << y << " " << z << " and cce is " << cce << " and ccecce is " << ccecce << endl;
                        }
                        if (Edge[0][y] == 0 && Edge[0][z] == 0) {
                            ie += Edge[y][z]; ieie += Edge[y][z] * Edge[y][z];
                            cout << " independent edge " << y << " " << z << " and ie is " << ie << " and ieie is " << ieie << endl;
                        }
                    }
                }
                av = sumce + ((0.5) * (1 / (b - 1 + CE - VCE)) * cce) + ((1 / (b - 1 + CE - VCE)) * ne) + ((2 / (b - 1 + CE - VCE)) * ie);
                cout << " the average for this situation is " << av << " and sumce " << sumce << " ce " << CE << " vce " << VCE << " cce " << cce << " ne " << ne << " ie " << ie << endl;
                for (int c = 1; c < n; c++) {
                    if (Edge[0][c] == -1) { continue; }
                    for (int g = c + 1; g <= n; g++) {
                        if (Edge[0][g] == -1 || Edge[0][g] == c || Edge[0][c] == g) { continue; }
                        for (int p = c; p < n; p++) {
                            if (Edge[0][p] == -1) { continue; }
                            for (int k = 1 + p; k <= n; k++) {
                                if (Edge[0][k] == -1 || Edge[0][k] == p || Edge[0][p] == k) { continue; }
                                if (Edge[0][c] != 0 && k == c || Edge[0][c] != 0 && p == c || Edge[0][g] != 0 && k == g || Edge[0][g] != 0 && p == g) { continue; }
                                if (Edge[0][k] != 0 && k == c || Edge[0][k] != 0 && k == g || Edge[0][p] != 0 && p == c || Edge[0][p] != 0 && p == g) { continue; }
                                if (Edge[0][c] == p && g == k || Edge[0][c] == k && g == p || Edge[0][g] == p && c == k || Edge[0][g] == k && c == p) { continue; }
                                if (Edge[0][k] == c && g == p || Edge[0][k] == g && c == p || Edge[0][p] == c && g == k || Edge[0][p] == g && c == k) { continue; }
                                if (Edge[0][c] == p && Edge[0][g] == k || Edge[0][c] == k && Edge[0][g] == p) { continue; }
                                if (p == c && k <= g) { continue; }
                                // standadiation stuff begin
                                if (Edge[0][k] != 0 && Edge[0][p] != 0 && Edge[0][c] != 0 && Edge[0][g] != 0) {
                                    cceccei += Edge[c][g] * Edge[p][k];
                                    cout << "cceccei Edge[" << c << "][" << g << "]" << " Edge[" << p << "][" << k << "]" << endl;
                                }
                                if (Edge[0][c] != 0 && Edge[0][g] == 0 && Edge[0][p] != 0 && Edge[0][k] == 0 && g == k ||
                                    Edge[0][c] != 0 && Edge[0][g] == 0 && Edge[0][p] == 0 && Edge[0][k] != 0 && g == p ||
                                    Edge[0][c] == 0 && Edge[0][g] != 0 && Edge[0][p] != 0 && Edge[0][k] == 0 && c == k ||
                                    Edge[0][c] == 0 && Edge[0][g] != 0 && Edge[0][p] == 0 && Edge[0][k] != 0 && c == p) {
                                    nenen += Edge[c][g] * Edge[p][k];
                                    cout << "nenen Edge[" << c << "][" << g << "]" << " Edge[" << p << "][" << k << "]" << endl;
                                }
                                if (Edge[0][c] != 0 && Edge[0][g] == 0 && Edge[0][p] != 0 && Edge[0][k] == 0 && g != k ||
                                    Edge[0][c] != 0 && Edge[0][g] == 0 && Edge[0][p] == 0 && Edge[0][k] != 0 && g != p ||
                                    Edge[0][c] == 0 && Edge[0][g] != 0 && Edge[0][p] != 0 && Edge[0][k] == 0 && c != k ||
                                    Edge[0][c] == 0 && Edge[0][g] != 0 && Edge[0][p] == 0 && Edge[0][k] != 0 && c != p) {
                                    nenei += Edge[c][g] * Edge[p][k];
                                    cout << "nenei Edge[" << c << "][" << g << "]" << " Edge[" << p << "][" << k << "]" << endl;
                                }
                                if (Edge[0][c] == 0 && Edge[0][g] == 0 && Edge[0][p] == 0 && Edge[0][k] == 0) {
                                    if (c == k || c == p || g == k || g == p) {
                                        ieien += Edge[c][g] * Edge[p][k];
                                        cout << "ieien Edge[" << c << "][" << g << "]" << " Edge[" << p << "][" << k << "]" << endl;
                                    }
                                    else {
                                        ieiei += Edge[c][g] * Edge[p][k];
                                        cout << "ieiei Edge[" << c << "][" << g << "]" << " Edge[" << p << "][" << k << "]" << endl;
                                    }
                                }
                                if (Edge[0][c] != 0 && Edge[0][g] != 0 && Edge[0][p] == 0 && Edge[0][k] != 0 ||
                                    Edge[0][c] != 0 && Edge[0][g] != 0 && Edge[0][p] != 0 && Edge[0][k] == 0 ||
                                    Edge[0][c] == 0 && Edge[0][g] != 0 && Edge[0][p] != 0 && Edge[0][k] != 0 ||
                                    Edge[0][c] != 0 && Edge[0][g] == 0 && Edge[0][p] != 0 && Edge[0][k] != 0) {
                                    ccenei += Edge[c][g] * Edge[p][k];
                                    cout << "ccenei Edge[" << c << "][" << g << "]" << " Edge[" << p << "][" << k << "]" << endl;
                                }
                                if (Edge[0][c] != 0 && Edge[0][g] != 0 && Edge[0][p] == 0 && Edge[0][k] == 0 ||
                                    Edge[0][c] == 0 && Edge[0][g] == 0 && Edge[0][p] != 0 && Edge[0][k] != 0) {
                                    cceiei += Edge[c][g] * Edge[p][k];
                                    cout << "cceiei Edge[" << c << "][" << g << "]" << " Edge[" << p << "][" << k << "]" << endl;
                                }
                                if (Edge[0][c] != 0 && Edge[0][g] == 0 && Edge[0][p] == 0 && Edge[0][k] == 0 ||
                                    Edge[0][c] == 0 && Edge[0][g] != 0 && Edge[0][p] == 0 && Edge[0][k] == 0 ||
                                    Edge[0][c] == 0 && Edge[0][g] == 0 && Edge[0][p] != 0 && Edge[0][k] == 0 ||
                                    Edge[0][c] == 0 && Edge[0][g] == 0 && Edge[0][p] == 0 && Edge[0][k] != 0) {
                                    if (c == p || c == k || g == k || g == p) {
                                        neien += Edge[c][g] * Edge[p][k];
                                        cout << "neien Edge[" << c << "][" << g << "]" << " Edge[" << p << "][" << k << "]" << endl;
                                    }
                                    else {
                                        neiei += Edge[c][g] * Edge[p][k];
                                        cout << "neiei Edge[" << c << "][" << g << "]" << " Edge[" << p << "][" << k << "]" << endl;
                                            
                                    }
                                }
                            }

                            // standard deviation stuff end
                        }
                    }
                }
                double denom1 = (b - 1 + CE - VCE);
                double denom2 = (b - 2 + CE - VCE);

                va =
                    (0.5 / denom1) * ccecce +
                    (1.0 / denom1) * nene +
                    (2.0 / denom1) * ieie +

                    (1.0 / (denom1 * denom2)) *
                    (
                        0.5 * cceccei +
                        nenen +
                        2 * nenei +
                        4 * ieien +
                        8 * ieiei +
                        ccenei +
                        2 * cceiei +
                        2 * neien +
                        4 * neiei
                        )

                    - ((av - sumce) * (av - sumce));
                cout << "The standard deviation is " << sqrt(va) << endl;
                 double entro = lgamma(denom1) + (log(2.0) * (CE-1)); double min=0.0; int min1=0, min2=0;
                double z = exp(entro-(beta*av)+((beta)*(beta)*(0.5)*va));
                if(z>=min){
                   min 
                }
                // reinitialized here
                ne = 0.0, cce = 0.0, ie = 0.0, nenen = 0.0, cceccei = 0.0, ieien = 0.0, nenei = 0.0, ieiei = 0.0, av = 0.0, va = 0.0;
                nene = 0.0, ccecce = 0.0, ieie = 0.0, ccenei = 0.0, cceiei = 0.0, neien = 0.0, neiei = 0.0;
                Edge[0][i] = 0; Edge[0][j] = 0;
                CE--; VCE--; VCE--; sumce -= Edge[i][j];
            }
        }
    }

    return 0;

}



