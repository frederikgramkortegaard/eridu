#include <stdio.h>
 
int max(int a, int b) { return (a > b) ? a : b; }
 
int knapSack(int W, int wt[], int val[], int n)
{
    if (n == 0 || W == 0)
        return 0;
    if (wt[n - 1] > W)
        return knapSack(W, wt, val, n - 1);
 
    else
        return max(
            val[n - 1]
                + knapSack(W - wt[n - 1],
                           wt, val, n - 1),
            knapSack(W, wt, val, n - 1));
}
 
int main()
{
    int val[] = { 60, 100, 120, 140, 160, 180, 200, 220, 240, 260, 64, 32, 25, 12, 22, 11, 90, 4, 5, 6};
    int wt[] = { 10, 20, 30, 40, 50, 60, 70, 80, 90, 100, 15, 25, 35, 45, 55, 65, 75, 85, 95, 105};
    int W = 233;
    int n = sizeof(val) / sizeof(val[0]);
    knapSack(W, wt, val, n);
    return 0;
}