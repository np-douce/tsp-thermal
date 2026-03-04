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
                if (i!=j&&i!=k) {
                    S_neighbor += Edge[i][j] * Edge[i][k];
                }
            }
        }
    }

    double S_non_neighbor =0.0;
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
    cout << "the standard deviation = " << stand <<" " <<sqrt(stand)<< endl;
    double entropy = lgamma(n) - log(2.0);
    cout << "entropy " << entropy << "states " << exp(entropy)<<endl;
    int ref1=0, ref2=0, CE=0, VCE=0;
    double ref3;
    for (int i = 1; i <= n - 1; i++)
{
    for (int j = i + 1; j <= n; j++)
    {
        if(Edge[0][i]!=0&&Edge[0][j]!=0){
            ref1=Edge[0][i]; ref2=Edge[0][j]; ref3=Edge[ref1][ref2];
            Edge[ref1][ref2]=Edge[ref2][ref1]=Edge[i][j]+Edge[i][ref1]+Edge[j][ref2];
            Edge[0][ref1]=ref2; Edge[0][ref2]=ref1;
            Edge[0][i]=-1; Edge[0][j]=-1;
            VCE++; VCE++; CE--;
            // CCE stuff ref1 and ref2
            // reinitialized here
             Edge[ref1][ref2]=Edge[ref2][ref1]=ref3;
            Edge[0][ref1]=i; Edge[0][ref2]=j;
            Edge[0][i]=ref1; Edge[0][j]=ref2;
            ref1=0, ref2=0, ref3=0;
            VCE--; VCE--; CE++;
        }
      if(Edge[0][i]==0&&Edge[0][j]!=0){
            ref2=Edge[0][j]; ref3=Edge[ref2][i];
            Edge[i][ref2]=Edge[ref2][i]=Edge[i][j]+Edge[j][ref2];
            Edge[0][i]=ref2; Edge[0][ref2]=i; 
            Edge[0][j]=-1;
            VCE++; 
           // CCE stuff ref1 and ref2
            // reinitialized here
            Edge[i][ref2]=Edge[ref2][i]=ref3;
            Edge[0][i]=0; Edge[0][ref2]=j; 
            Edge[0][j]=ref2; 
            ref2=0, ref3=0;
            VCE--; 
      }
        if(Edge[0][j]==0&&Edge[0][i]!=0){
             ref2=Edge[0][i]; ref3=Edge[ref2][j];
            Edge[j][ref2]=Edge[ref2][j]=Edge[i][j]+Edge[i][ref2];
            Edge[0][j]=ref2; Edge[0][ref2]=j; 
            Edge[0][i]=-1;
            VCE++; 
           // CCE stuff ref1 and ref2
            // reinitialized here 
            Edge[j][ref2]=Edge[ref2][j]=ref3;
            Edge[0][j]=0; Edge[0][ref2]=i; 
            Edge[0][i]=ref2; 
            ref2=0, ref3=0;
            VCE--; 
        }
        if(Edge[0][i]==0&&Edge[0][j]==0){
            Edge[0][i]=j; Edge[0][j]=i;
            CE++; VCE++; VCE++;
            // CCE stuff ref1 and ref2
            // reinitialized here
              Edge[0][i]=0; Edge[0][j]=0;
            CE++; VCE--; VCE--;
        }
    }
}

    return 0;

}
