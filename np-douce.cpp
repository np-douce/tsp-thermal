#include <iostream>
using namespace std;
#include <cmath>
#include <string>
#include <fstream>
const int MAXN = 100;
int main()
{
    int n=0;
    double beta;
    cout << "Enter beta value: " << endl;
    cin >> beta;
    ifstream file("points.txt");

    if (!file.is_open()) {
        cout << "Error opening file\n";
        return 1;
    }

    double x[MAXN], y[MAXN];
    

    // Read coordinates
    while (file >> x[n+1] >> y[n+1]) {
        n++;
    }

    file.close();

    // Create Edge matrix
    double Edge[MAXN][MAXN];

    // Compute distances
    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= n; j++) {
            double dx = x[i] - x[j];
            double dy = y[i] - y[j];
            Edge[i][j] = sqrt(dx * dx + dy * dy);
        }
    }

    
    double sum = 0.0, average = 0, b = n;

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
cout << "S_self " << S_self << " S_neighbor " << S_neighbor << " S_non_neighbor " << S_non_neighbor << endl;
    double stand = ((2.0 / (b - 1)) * (S_self)) + ((4.0 / ((b - 1) * (b - 2))) * (S_neighbor + (2 * S_non_neighbor))) - (average * average);
    cout << "the standard deviation = " << stand << " " << sqrt(stand) << endl;
    double entropy = lgamma(n) - log(2.0), partition = entropy - (beta * average) + ((beta) * (beta) * (0.5) * stand);
    cout << "entropy " << entropy << "partition " << partition << endl;
    int ref1 = 0, ref2 = 0, CE = 0, VCE = 0;  double min = -1e100; int min1 = 0, min2 = 0;
    double ref3, sumce = 0.0;
    do {
        min = -1e100;
        for (int i = 1; i <= n - 1; i++) {
            if (Edge[0][i] == -1) { continue; }
            for (int j = i + 1; j <= n; j++) { if(Edge[0][j] == -1|| Edge[0][i] == j|| Edge[0][j] == i) { continue; }
            
                double ne = 0.0, cce = 0.0, ie = 0.0, nenen = 0.0, cceccei = 0.0, ieien = 0.0, nenei = 0.0, ieiei = 0.0;
                double nene = 0.0, ccecce = 0.0, ieie = 0.0, ccenei = 0.0, cceiei = 0.0, neien = 0.0, neiei = 0.0;
                double av = 0.0, va = 0.0, ce = 0.0;
                if (Edge[0][j] == -1 || Edge[0][i] == j || Edge[0][j] == i) { continue; }
                if (Edge[0][i] != 0 && Edge[0][j] != 0) {
                    sumce += Edge[i][j];
                    ref1 = Edge[0][i]; ref2 = Edge[0][j];
                    Edge[0][ref1] = ref2; Edge[0][ref2] = ref1;
                    Edge[0][i] = -1; Edge[0][j] = -1; CE--; 
                    // CCE stuff ref1 and ref2
                   
                    for (int y = 1; y < n; y++) {
                        if (Edge[0][y] == -1) { continue; }
                        for (int z = 1 + y; z <= n; z++) {
                            if (Edge[0][z] == -1) { continue; }
                            if (Edge[0][y] == 0 && Edge[0][z] != 0 || Edge[0][y] != 0 && Edge[0][z] == 0) {
                                ne += Edge[y][z]; nene += Edge[y][z] * Edge[y][z];

                            }
                            if (Edge[0][y] != 0 && Edge[0][z] != 0 && Edge[0][y] != z && Edge[0][z] != y) {
                                cce += Edge[y][z]; ccecce += Edge[y][z] * Edge[y][z];

                            }
                            if (Edge[0][y] == 0 && Edge[0][z] == 0) {
                                ie += Edge[y][z]; ieie += Edge[y][z] * Edge[y][z]; }
                        }
                    }
                    if (b-1+CE-VCE == 0) {
                        for(int kl=1; kl<=n; kl++){
                            if (Edge[0][kl] != -1){
                        
                            for(int mn=kl+1; mn<=n; mn++){
                                if (Edge[0][mn] != -1&& Edge[0][mn] == kl){
                                     av=sumce+ Edge[kl][mn]; break;
                                }
                            }
                        }
                    }}
                    else {
                    av = sumce + ((0.5) * (1 / (b - 1 + CE - VCE)) * cce) + ((1 / (b - 1 + CE - VCE)) * ne) + ((2 / (b - 1 + CE - VCE)) * ie);}
                    
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

                                    }
                                    if (Edge[0][c] != 0 && Edge[0][g] == 0 && Edge[0][p] != 0 && Edge[0][k] == 0 && g == k ||
                                        Edge[0][c] != 0 && Edge[0][g] == 0 && Edge[0][p] == 0 && Edge[0][k] != 0 && g == p ||
                                        Edge[0][c] == 0 && Edge[0][g] != 0 && Edge[0][p] != 0 && Edge[0][k] == 0 && c == k ||
                                        Edge[0][c] == 0 && Edge[0][g] != 0 && Edge[0][p] == 0 && Edge[0][k] != 0 && c == p) {
                                        nenen += Edge[c][g] * Edge[p][k];

                                    }
                                    if (Edge[0][c] != 0 && Edge[0][g] == 0 && Edge[0][p] != 0 && Edge[0][k] == 0 && g != k ||
                                        Edge[0][c] != 0 && Edge[0][g] == 0 && Edge[0][p] == 0 && Edge[0][k] != 0 && g != p ||
                                        Edge[0][c] == 0 && Edge[0][g] != 0 && Edge[0][p] != 0 && Edge[0][k] == 0 && c != k ||
                                        Edge[0][c] == 0 && Edge[0][g] != 0 && Edge[0][p] == 0 && Edge[0][k] != 0 && c != p) {
                                        nenei += Edge[c][g] * Edge[p][k];

                                    }
                                    if (Edge[0][c] == 0 && Edge[0][g] == 0 && Edge[0][p] == 0 && Edge[0][k] == 0) {
                                        if (c == k || c == p || g == k || g == p) {
                                            ieien += Edge[c][g] * Edge[p][k];

                                        }
                                        else {
                                            ieiei += Edge[c][g] * Edge[p][k];

                                        }
                                    }
                                    if (Edge[0][c] != 0 && Edge[0][g] != 0 && Edge[0][p] == 0 && Edge[0][k] != 0 ||
                                        Edge[0][c] != 0 && Edge[0][g] != 0 && Edge[0][p] != 0 && Edge[0][k] == 0 ||
                                        Edge[0][c] == 0 && Edge[0][g] != 0 && Edge[0][p] != 0 && Edge[0][k] != 0 ||
                                        Edge[0][c] != 0 && Edge[0][g] == 0 && Edge[0][p] != 0 && Edge[0][k] != 0) {
                                        ccenei += Edge[c][g] * Edge[p][k];

                                    }
                                    if (Edge[0][c] != 0 && Edge[0][g] != 0 && Edge[0][p] == 0 && Edge[0][k] == 0 ||
                                        Edge[0][c] == 0 && Edge[0][g] == 0 && Edge[0][p] != 0 && Edge[0][k] != 0) {
                                        cceiei += Edge[c][g] * Edge[p][k];

                                    }
                                    if (Edge[0][c] != 0 && Edge[0][g] == 0 && Edge[0][p] == 0 && Edge[0][k] == 0 ||
                                        Edge[0][c] == 0 && Edge[0][g] != 0 && Edge[0][p] == 0 && Edge[0][k] == 0 ||
                                        Edge[0][c] == 0 && Edge[0][g] == 0 && Edge[0][p] != 0 && Edge[0][k] == 0 ||
                                        Edge[0][c] == 0 && Edge[0][g] == 0 && Edge[0][p] == 0 && Edge[0][k] != 0) {
                                        if (c == p || c == k || g == k || g == p) {
                                            neien += Edge[c][g] * Edge[p][k];

                                        }
                                        else {
                                            neiei += Edge[c][g] * Edge[p][k];


                                        }
                                    }
                                }

                                // standard deviation stuff end
                            }
                        }
                    }
                    double denom1 = (b - 1 + CE - VCE);
                    double denom2 = (b - 2 + CE - VCE);
if (denom2 <= 0) {
    va = 0.0;
}
else {
    // your full variance formula

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

                        - ((av - sumce) * (av - sumce));}
                    double entro = lgamma(denom1+1) + (log(2.0) * (CE - 1));
                    double z = entro - (beta * av) + ((beta) * (beta) * (0.5) * va);
                    
                    if (z >= min) {
                        min = z; min1 = i; min2 = j;
                    }
                    // reinitialized here
                    ce = 0.0, ne = 0.0, cce = 0.0, ie = 0.0, nenen = 0.0, cceccei = 0.0, ieien = 0.0, nenei = 0.0, ieiei = 0.0, av = 0.0, va = 0.0;
                    nene = 0.0, ccecce = 0.0, ieie = 0.0, ccenei = 0.0, cceiei = 0.0, neien = 0.0, neiei = 0.0;
                    Edge[0][ref1] = i; Edge[0][ref2] = j;
                    Edge[0][i] = ref1; Edge[0][j] = ref2;
                    ref1 = 0, ref2 = 0, ref3 = 0;
                    CE++; sumce -= Edge[i][j];
                }
                if (Edge[0][i] == 0 && Edge[0][j] != 0) {
                    sumce += Edge[i][j];
                    ref2 = Edge[0][j];
                    Edge[0][i] = ref2; Edge[0][ref2] = i;
                    Edge[0][j] = -1;
                    VCE++;
                    // CCE stuff ref1 and ref2
                    
                    for (int y = 1; y < n; y++) {
                        if (Edge[0][y] == -1) { continue; }
                        for (int z = 1 + y; z <= n; z++) {
                            if (Edge[0][z] == -1) { continue; }
                            if (Edge[0][y] == 0 && Edge[0][z] != 0 || Edge[0][y] != 0 && Edge[0][z] == 0) {
                                ne += Edge[y][z]; nene += Edge[y][z] * Edge[y][z];

                            }
                            if (Edge[0][y] != 0 && Edge[0][z] != 0 && Edge[0][y] != z && Edge[0][z] != y) {
                                cce += Edge[y][z]; ccecce += Edge[y][z] * Edge[y][z];

                            }
                            if (Edge[0][y] == 0 && Edge[0][z] == 0) {
                                ie += Edge[y][z]; ieie += Edge[y][z] * Edge[y][z];

                            }
                        }
                    }
                    if (b-1+CE-VCE == 0) {
                        for(int kl=1; kl<=n; kl++){
                            if (Edge[0][kl] != -1){
                        
                            for(int mn=kl+1; mn<=n; mn++){
                                if (Edge[0][mn] != -1&& Edge[0][mn] == kl){
                                     av=sumce+ Edge[kl][mn]; break;
                                }
                            }
                        }
                    }}
                    else {
                    av = sumce + ((0.5) * (1 / (b - 1 + CE - VCE)) * cce) + ((1 / (b - 1 + CE - VCE)) * ne) + ((2 / (b - 1 + CE - VCE)) * ie);}
                    
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

                                    }
                                    if (Edge[0][c] != 0 && Edge[0][g] == 0 && Edge[0][p] != 0 && Edge[0][k] == 0 && g == k ||
                                        Edge[0][c] != 0 && Edge[0][g] == 0 && Edge[0][p] == 0 && Edge[0][k] != 0 && g == p ||
                                        Edge[0][c] == 0 && Edge[0][g] != 0 && Edge[0][p] != 0 && Edge[0][k] == 0 && c == k ||
                                        Edge[0][c] == 0 && Edge[0][g] != 0 && Edge[0][p] == 0 && Edge[0][k] != 0 && c == p) {
                                        nenen += Edge[c][g] * Edge[p][k];

                                    }
                                    if (Edge[0][c] != 0 && Edge[0][g] == 0 && Edge[0][p] != 0 && Edge[0][k] == 0 && g != k ||
                                        Edge[0][c] != 0 && Edge[0][g] == 0 && Edge[0][p] == 0 && Edge[0][k] != 0 && g != p ||
                                        Edge[0][c] == 0 && Edge[0][g] != 0 && Edge[0][p] != 0 && Edge[0][k] == 0 && c != k ||
                                        Edge[0][c] == 0 && Edge[0][g] != 0 && Edge[0][p] == 0 && Edge[0][k] != 0 && c != p) {
                                        nenei += Edge[c][g] * Edge[p][k];

                                    }
                                    if (Edge[0][c] == 0 && Edge[0][g] == 0 && Edge[0][p] == 0 && Edge[0][k] == 0) {
                                        if (c == k || c == p || g == k || g == p) {
                                            ieien += Edge[c][g] * Edge[p][k];

                                        }
                                        else {
                                            ieiei += Edge[c][g] * Edge[p][k];

                                        }
                                    }
                                    if (Edge[0][c] != 0 && Edge[0][g] != 0 && Edge[0][p] == 0 && Edge[0][k] != 0 ||
                                        Edge[0][c] != 0 && Edge[0][g] != 0 && Edge[0][p] != 0 && Edge[0][k] == 0 ||
                                        Edge[0][c] == 0 && Edge[0][g] != 0 && Edge[0][p] != 0 && Edge[0][k] != 0 ||
                                        Edge[0][c] != 0 && Edge[0][g] == 0 && Edge[0][p] != 0 && Edge[0][k] != 0) {
                                        ccenei += Edge[c][g] * Edge[p][k];

                                    }
                                    if (Edge[0][c] != 0 && Edge[0][g] != 0 && Edge[0][p] == 0 && Edge[0][k] == 0 ||
                                        Edge[0][c] == 0 && Edge[0][g] == 0 && Edge[0][p] != 0 && Edge[0][k] != 0) {
                                        cceiei += Edge[c][g] * Edge[p][k];

                                    }
                                    if (Edge[0][c] != 0 && Edge[0][g] == 0 && Edge[0][p] == 0 && Edge[0][k] == 0 ||
                                        Edge[0][c] == 0 && Edge[0][g] != 0 && Edge[0][p] == 0 && Edge[0][k] == 0 ||
                                        Edge[0][c] == 0 && Edge[0][g] == 0 && Edge[0][p] != 0 && Edge[0][k] == 0 ||
                                        Edge[0][c] == 0 && Edge[0][g] == 0 && Edge[0][p] == 0 && Edge[0][k] != 0) {
                                        if (c == p || c == k || g == k || g == p) {
                                            neien += Edge[c][g] * Edge[p][k];

                                        }
                                        else {
                                            neiei += Edge[c][g] * Edge[p][k];


                                        }
                                    }
                                }

                                // standard deviation stuff end
                            }
                        }
                    }
                    double denom1 = (b - 1 + CE - VCE);
                    double denom2 = (b - 2 + CE - VCE);
                    if (denom2 <= 0) {
                       va = 0.0;
}
else {
    // your full variance formula


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

                        - ((av - sumce) * (av - sumce));}
                    double entro = lgamma(denom1+1) + (log(2.0) * (CE - 1));
                    double z = entro - (beta * av) + ((beta) * (beta) * (0.5) * va);
                    
                    if (z >= min) {
                        min = z; min1 = i; min2 = j;
                    }
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
                   
                    for (int y = 1; y < n; y++) {
                        if (Edge[0][y] == -1) { continue; }
                        for (int z = 1 + y; z <= n; z++) {
                            if (Edge[0][z] == -1) { continue; }
                            if (Edge[0][y] == 0 && Edge[0][z] != 0 || Edge[0][y] != 0 && Edge[0][z] == 0) {
                                ne += Edge[y][z]; nene += Edge[y][z] * Edge[y][z];

                            }
                            if (Edge[0][y] != 0 && Edge[0][z] != 0 && Edge[0][y] != z && Edge[0][z] != y) {
                                cce += Edge[y][z]; ccecce += Edge[y][z] * Edge[y][z];

                            }
                            if (Edge[0][y] == 0 && Edge[0][z] == 0) {
                                ie += Edge[y][z]; ieie += Edge[y][z] * Edge[y][z];

                            }
                        }
                    }
                    if (b-1+CE-VCE == 0) {
                        for(int kl=1; kl<=n; kl++){
                            if (Edge[0][kl] != -1){
                         
                            for(int mn=kl+1; mn<=n; mn++){
                                if (Edge[0][mn] != -1&& Edge[0][mn] == kl){
                                     av=sumce+ Edge[kl][mn]; break;
                                }
                            }
                        }
                    }}
                    else {
                    av = sumce + ((0.5) * (1 / (b - 1 + CE - VCE)) * cce) + ((1 / (b - 1 + CE - VCE)) * ne) + ((2 / (b - 1 + CE - VCE)) * ie);}
                   
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

                                    }
                                    if (Edge[0][c] != 0 && Edge[0][g] == 0 && Edge[0][p] != 0 && Edge[0][k] == 0 && g == k ||
                                        Edge[0][c] != 0 && Edge[0][g] == 0 && Edge[0][p] == 0 && Edge[0][k] != 0 && g == p ||
                                        Edge[0][c] == 0 && Edge[0][g] != 0 && Edge[0][p] != 0 && Edge[0][k] == 0 && c == k ||
                                        Edge[0][c] == 0 && Edge[0][g] != 0 && Edge[0][p] == 0 && Edge[0][k] != 0 && c == p) {
                                        nenen += Edge[c][g] * Edge[p][k];

                                    }
                                    if (Edge[0][c] != 0 && Edge[0][g] == 0 && Edge[0][p] != 0 && Edge[0][k] == 0 && g != k ||
                                        Edge[0][c] != 0 && Edge[0][g] == 0 && Edge[0][p] == 0 && Edge[0][k] != 0 && g != p ||
                                        Edge[0][c] == 0 && Edge[0][g] != 0 && Edge[0][p] != 0 && Edge[0][k] == 0 && c != k ||
                                        Edge[0][c] == 0 && Edge[0][g] != 0 && Edge[0][p] == 0 && Edge[0][k] != 0 && c != p) {
                                        nenei += Edge[c][g] * Edge[p][k];

                                    }
                                    if (Edge[0][c] == 0 && Edge[0][g] == 0 && Edge[0][p] == 0 && Edge[0][k] == 0) {
                                        if (c == k || c == p || g == k || g == p) {
                                            ieien += Edge[c][g] * Edge[p][k];

                                        }
                                        else {
                                            ieiei += Edge[c][g] * Edge[p][k];

                                        }
                                    }
                                    if (Edge[0][c] != 0 && Edge[0][g] != 0 && Edge[0][p] == 0 && Edge[0][k] != 0 ||
                                        Edge[0][c] != 0 && Edge[0][g] != 0 && Edge[0][p] != 0 && Edge[0][k] == 0 ||
                                        Edge[0][c] == 0 && Edge[0][g] != 0 && Edge[0][p] != 0 && Edge[0][k] != 0 ||
                                        Edge[0][c] != 0 && Edge[0][g] == 0 && Edge[0][p] != 0 && Edge[0][k] != 0) {
                                        ccenei += Edge[c][g] * Edge[p][k];

                                    }
                                    if (Edge[0][c] != 0 && Edge[0][g] != 0 && Edge[0][p] == 0 && Edge[0][k] == 0 ||
                                        Edge[0][c] == 0 && Edge[0][g] == 0 && Edge[0][p] != 0 && Edge[0][k] != 0) {
                                        cceiei += Edge[c][g] * Edge[p][k];

                                    }
                                    if (Edge[0][c] != 0 && Edge[0][g] == 0 && Edge[0][p] == 0 && Edge[0][k] == 0 ||
                                        Edge[0][c] == 0 && Edge[0][g] != 0 && Edge[0][p] == 0 && Edge[0][k] == 0 ||
                                        Edge[0][c] == 0 && Edge[0][g] == 0 && Edge[0][p] != 0 && Edge[0][k] == 0 ||
                                        Edge[0][c] == 0 && Edge[0][g] == 0 && Edge[0][p] == 0 && Edge[0][k] != 0) {
                                        if (c == p || c == k || g == k || g == p) {
                                            neien += Edge[c][g] * Edge[p][k];

                                        }
                                        else {
                                            neiei += Edge[c][g] * Edge[p][k];


                                        }
                                    }
                                }

                                // standard deviation stuff end
                            }
                        }
                    }
                    double denom1 = (b - 1 + CE - VCE);
                    double denom2 = (b - 2 + CE - VCE);
                    if (denom2 <= 0) {
    va = 0.0;
}
else {
    // your full variance formula


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

                        - ((av - sumce) * (av - sumce));}
                    double entro = lgamma(denom1+1) + (log(2.0) * (CE - 1));
                    double z = entro - (beta * av) + ((beta) * (beta) * (0.5) * va);
                    
                    if (z >= min) {
                        min = z; min1 = i; min2 = j;
                    }
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
                   
                    for (int y = 1; y < n; y++) {
                        if (Edge[0][y] == -1) { continue; }
                        for (int z = 1 + y; z <= n; z++) {
                            if (Edge[0][z] == -1) { continue; }
                            if (Edge[0][y] == 0 && Edge[0][z] != 0 || Edge[0][y] != 0 && Edge[0][z] == 0) {
                                ne += Edge[y][z]; nene += Edge[y][z] * Edge[y][z];

                            }
                            if (Edge[0][y] != 0 && Edge[0][z] != 0 && Edge[0][y] != z && Edge[0][z] != y) {
                                cce += Edge[y][z]; ccecce += Edge[y][z] * Edge[y][z];

                            }
                            if (Edge[0][y] == 0 && Edge[0][z] == 0) {
                                ie += Edge[y][z]; ieie += Edge[y][z] * Edge[y][z];

                            }
                        }
                    }
                    if (b-1+CE-VCE == 0) {
                        for(int kl=1; kl<=n; kl++){
                            if (Edge[0][kl] != -1){
                            for(int mn=kl+1; mn<=n; mn++){
                                if (Edge[0][mn] != -1&& Edge[0][mn] == kl){
                                     av=sumce+ Edge[kl][mn]; break;
                                }
                            }
                        }
                    }}
                    else {
                    av = sumce + ((0.5) * (1 / (b - 1 + CE - VCE)) * cce) + ((1 / (b - 1 + CE - VCE)) * ne) + ((2 / (b - 1 + CE - VCE)) * ie);}
                    
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

                                    }
                                    if (Edge[0][c] != 0 && Edge[0][g] == 0 && Edge[0][p] != 0 && Edge[0][k] == 0 && g == k ||
                                        Edge[0][c] != 0 && Edge[0][g] == 0 && Edge[0][p] == 0 && Edge[0][k] != 0 && g == p ||
                                        Edge[0][c] == 0 && Edge[0][g] != 0 && Edge[0][p] != 0 && Edge[0][k] == 0 && c == k ||
                                        Edge[0][c] == 0 && Edge[0][g] != 0 && Edge[0][p] == 0 && Edge[0][k] != 0 && c == p) {
                                        nenen += Edge[c][g] * Edge[p][k];

                                    }
                                    if (Edge[0][c] != 0 && Edge[0][g] == 0 && Edge[0][p] != 0 && Edge[0][k] == 0 && g != k ||
                                        Edge[0][c] != 0 && Edge[0][g] == 0 && Edge[0][p] == 0 && Edge[0][k] != 0 && g != p ||
                                        Edge[0][c] == 0 && Edge[0][g] != 0 && Edge[0][p] != 0 && Edge[0][k] == 0 && c != k ||
                                        Edge[0][c] == 0 && Edge[0][g] != 0 && Edge[0][p] == 0 && Edge[0][k] != 0 && c != p) {
                                        nenei += Edge[c][g] * Edge[p][k];

                                    }
                                    if (Edge[0][c] == 0 && Edge[0][g] == 0 && Edge[0][p] == 0 && Edge[0][k] == 0) {
                                        if (c == k || c == p || g == k || g == p) {
                                            ieien += Edge[c][g] * Edge[p][k];

                                        }
                                        else {
                                            ieiei += Edge[c][g] * Edge[p][k];

                                        }
                                    }
                                    if (Edge[0][c] != 0 && Edge[0][g] != 0 && Edge[0][p] == 0 && Edge[0][k] != 0 ||
                                        Edge[0][c] != 0 && Edge[0][g] != 0 && Edge[0][p] != 0 && Edge[0][k] == 0 ||
                                        Edge[0][c] == 0 && Edge[0][g] != 0 && Edge[0][p] != 0 && Edge[0][k] != 0 ||
                                        Edge[0][c] != 0 && Edge[0][g] == 0 && Edge[0][p] != 0 && Edge[0][k] != 0) {
                                        ccenei += Edge[c][g] * Edge[p][k];

                                    }
                                    if (Edge[0][c] != 0 && Edge[0][g] != 0 && Edge[0][p] == 0 && Edge[0][k] == 0 ||
                                        Edge[0][c] == 0 && Edge[0][g] == 0 && Edge[0][p] != 0 && Edge[0][k] != 0) {
                                        cceiei += Edge[c][g] * Edge[p][k];

                                    }
                                    if (Edge[0][c] != 0 && Edge[0][g] == 0 && Edge[0][p] == 0 && Edge[0][k] == 0 ||
                                        Edge[0][c] == 0 && Edge[0][g] != 0 && Edge[0][p] == 0 && Edge[0][k] == 0 ||
                                        Edge[0][c] == 0 && Edge[0][g] == 0 && Edge[0][p] != 0 && Edge[0][k] == 0 ||
                                        Edge[0][c] == 0 && Edge[0][g] == 0 && Edge[0][p] == 0 && Edge[0][k] != 0) {
                                        if (c == p || c == k || g == k || g == p) {
                                            neien += Edge[c][g] * Edge[p][k];

                                        }
                                        else {
                                            neiei += Edge[c][g] * Edge[p][k];


                                        }
                                    }
                                }

                                // standard deviation stuff end
                            }
                        }
                    }
                    double denom1 = (b - 1 + CE - VCE);
                    double denom2 = (b - 2 + CE - VCE);
                    if (denom2 <= 0) {
    va = 0.0;
}
else {
    // your full variance formula


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

                        - ((av - sumce) * (av - sumce));}
                    double entro = lgamma(denom1+1) + (log(2.0) * (CE - 1));
                    double z = entro - (beta * av) + ((beta) * (beta) * (0.5) * va);
                   
                    if (z >= min) {
                        min = z; min1 = i; min2 = j;
                    }

                    // reinitialized here
                    ne = 0.0, cce = 0.0, ie = 0.0, nenen = 0.0, cceccei = 0.0, ieien = 0.0, nenei = 0.0, ieiei = 0.0, av = 0.0, va = 0.0;
                    nene = 0.0, ccecce = 0.0, ieie = 0.0, ccenei = 0.0, cceiei = 0.0, neien = 0.0, neiei = 0.0;
                    Edge[0][i] = 0; Edge[0][j] = 0;
                    CE--; VCE--; VCE--; sumce -= Edge[i][j];
                }
            }
        }

        cout << "The biggetst probability is " << min << " at  Edge[" << min1 << "][" << min2 << "]. *************************************" << endl;
        if (Edge[0][min1] != 0 && Edge[0][min2] != 0) {
            ref1 = Edge[0][min1]; ref2 = Edge[0][min2];
            Edge[0][ref1] = ref2; Edge[0][ref2] = ref1;
            Edge[0][min1] = -1; Edge[0][min2] = -1; CE--; sumce += Edge[min1][min2]; ref1 = 0.0, ref2 = 0.0;
        }
        if (Edge[0][min1] != 0 && Edge[0][min2] == 0) {
            ref1 =Edge[0][min1];
            Edge[0][min2] = ref1; Edge[0][ref1] = min2;
            Edge[0][min1] = -1; VCE++; sumce += Edge[min1][min2]; ref1 = 0.0;
        }
        if (Edge[0][min1] == 0 && Edge[0][min2] != 0) {
            ref2 =Edge[0][min2];
            Edge[0][min1] = ref2; Edge[0][ref2] = min1;
            Edge[0][min2] = -1; VCE++; sumce += Edge[min1][min2]; ref2 = 0.0;
        }
        if (Edge[0][min1] == 0 && Edge[0][min2] == 0) {
            Edge[0][min1] = min2; Edge[0][min2] = min1; VCE++; VCE++; CE++; sumce += Edge[min1][min2];
        }
    } while (b-VCE+CE-1 > 0);
    return 0;

}






