#include <iostream>
using namespace std;
#include <cmath>

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

    double stand = ((2.0 / (b - 1)) * (S_self)) + ((4.0 / ((b - 1) * (b - 2))) * (S_neighbor + (2 * S_non_neighbor))) - (average * average);
    cout << "the standard deviation = " << stand << " " << sqrt(stand) << endl;
    double entropy = lgamma(n) - log(2.0);
    cout << "entropy " << entropy << "states " << exp(entropy) << endl;
    int ref1 = 0, ref2 = 0, CE = 0, VCE = 0;
    double ref3; Edge[0][1] = 2;Edge[0][2] = 1;Edge[0][3] = -1; Edge[1][2] = Edge[2][1] = Edge[1][3] + Edge[2][3];
    for (int i = 1; i <= n - 1; i++) {
        if (Edge[0][i] == -1) { continue; }
        for (int j = i + 1; j <= n; j++) {
            if (Edge[0][j] == -1 || Edge[0][i] == j || Edge[0][j] == i) { continue; }
            if (Edge[0][i] != 0 && Edge[0][j] != 0) {
                ref1 = Edge[0][i]; ref2 = Edge[0][j]; ref3 = Edge[ref1][ref2];
                Edge[ref1][ref2] = Edge[ref2][ref1] = Edge[i][j] + Edge[i][ref1] + Edge[j][ref2];
                Edge[0][ref1] = ref2; Edge[0][ref2] = ref1;
                Edge[0][i] = -1; Edge[0][j] = -1;
                VCE++; VCE++; CE--;
                // CCE stuff ref1 and ref2
                cout << "CCE situation "<<ref1<<" "<<ref2 << endl;
                for (int y = 1; y < n; y++) {if (Edge[0][y]==-1) { continue; }
                        for (int z = 1 + y; z <= n; z++) {if (Edge[0][z] == -1) { continue; }
                            if (Edge[0][y] == 0 && Edge[0][z] != 0 || Edge[0][y] != 0 && Edge[0][z] == 0) {
                                cout << " neighbor edge " << y << " " << z << endl;
                            }
                            if (Edge[0][y] != 0 && Edge[0][z] != 0 && Edge[0][y]==z || Edge[0][z]==y) {
                                cout << " connecting chosen neighbor edge " << y << " " << z << endl;
                            }
                            if (Edge[0][y] == 0 && Edge[0][z] == 0) {
                                cout << " independent edge " << y << " " << z << endl;
                            }
                        }
                    }
                // reinitialized here
                Edge[ref1][ref2] = Edge[ref2][ref1] = ref3;
                Edge[0][ref1] = i; Edge[0][ref2] = j;
                Edge[0][i] = ref1; Edge[0][j] = ref2;
                ref1 = 0, ref2 = 0, ref3 = 0;
                VCE--; VCE--; CE++;
            }
            if (Edge[0][i] == 0 && Edge[0][j] != 0) {
                ref2 = Edge[0][j]; ref3 = Edge[ref2][i];
                Edge[i][ref2] = Edge[ref2][i] = Edge[i][j] + Edge[j][ref2];
                Edge[0][i] = ref2; Edge[0][ref2] = i;
                Edge[0][j] = -1;
                VCE++;
                // CCE stuff ref1 and ref2
                cout << "NE situation J " <<i<<" "<<ref2 << endl;
                    for (int y = 1; y < n; y++) {
                        if (Edge[0][y] == -1) { continue; }
                        for (int z = 1 + y; z <= n; z++) {
                            if (Edge[0][z] == -1) { continue; }
                            if (Edge[0][y] == 0 && Edge[0][z] != 0 || Edge[0][y] != 0 && Edge[0][z] == 0) {
                                cout << " neighbor edge " << y << " " << z << endl;
                            }
                            if (Edge[0][y] != 0 && Edge[0][z] != 0 && Edge[0][y]==z || Edge[0][z]==y ) {
                                cout << " connecting chosen neighbor edge " << y << " " << z << endl;
                            }
                            if (Edge[0][y] == 0 && Edge[0][z] == 0) {
                                cout << " independent edge " << y << " " << z << endl;
                            }
                        }
                    }
                // reinitialized here
                Edge[i][ref2] = Edge[ref2][i] = ref3;
                Edge[0][i] = 0; Edge[0][ref2] = j;
                Edge[0][j] = ref2;
                ref2 = 0, ref3 = 0;
                VCE--;
            }
            if (Edge[0][j] == 0 && Edge[0][i] != 0) {
                ref2 = Edge[0][i]; ref3 = Edge[ref2][j];
                Edge[j][ref2] = Edge[ref2][j] = Edge[i][j] + Edge[i][ref2];
                Edge[0][j] = ref2; Edge[0][ref2] = j;
                Edge[0][i] = -1;
                VCE++;
                // CCE stuff ref1 and ref2
                cout << "NE situation I "<<ref2<<" "<<j << endl;
                    for (int y = 1; y < n; y++) {
                        if (Edge[0][y] == -1) { continue; }
                        for (int z = 1 + y; z <= n; z++) {
                            if (Edge[0][z] == -1) { continue; }
                            if (Edge[0][y] == 0 && Edge[0][z] != 0 || Edge[0][y] != 0 && Edge[0][z] == 0) {
                                cout << " neighbor edge " << y << " " << z << endl;
                            }
                            if (Edge[0][y] != 0 && Edge[0][z] != 0 && Edge[0][y]==z || Edge[0][z]==y) {
                                cout << " connecting chosen neighbor edge " << y << " " << z << endl;
                            }
                            if (Edge[0][y] == 0 && Edge[0][z] == 0) {
                                cout << " independent edge " << y << " " << z << endl;
                            }
                        }
                    }
                // reinitialized here 
                Edge[j][ref2] = Edge[ref2][j] = ref3;
                Edge[0][j] = 0; Edge[0][ref2] = i;
                Edge[0][i] = ref2;
                ref2 = 0, ref3 = 0;
                VCE--;
            }
            if (Edge[0][i] == 0 && Edge[0][j] == 0) {
                Edge[0][i] = j; Edge[0][j] = i;
                CE++; VCE++; VCE++;
                // CCE stuff ref1 and ref2
                cout << "IE situation " <<i<<" "<<j << endl;
                    for (int y = 1; y < n; y++) {
                        if (Edge[0][y] == -1) { continue; }
                        for (int z = 1 + y; z <= n; z++) {
                            if (Edge[0][z] == -1) { continue; }
                            if (Edge[0][y] == 0 && Edge[0][z] != 0 || Edge[0][y] != 0 && Edge[0][z] == 0) {
                                cout << " neighbor edge " << y << " " << z << endl;
                            }
                            if (Edge[0][y] != 0 && Edge[0][z] != 0 && Edge[0][y]==z || Edge[0][z]==y) {
                                cout << " connecting chosen neighbor edge " << y << " " << z << endl;
                            }
                            if (Edge[0][y] == 0 && Edge[0][z] == 0) {
                                cout << " independent edge " << y << " " << z << endl;
                            }
                        }
                    }
                // reinitialized here
                Edge[0][i] = 0; Edge[0][j] = 0;
                CE++; VCE--; VCE--;
            }
        }
    }

    return 0;

}
