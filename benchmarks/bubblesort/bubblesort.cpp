// C++ program for implementation
// of Bubble sort
#include <bits/stdc++.h>
using namespace std;
 
// A function to implement bubble sort
void bubbleSort(int arr[], int n)
{
    int i, j;
    for (i = 0; i < n - 1; i++)
 
        // Last i elements are already
        // in place
        for (j = 0; j < n - i - 1; j++)
            if (arr[j] > arr[j + 1])
                swap(arr[j], arr[j + 1]);
}
 
 
// Driver code
int main()
{
    int arr[] = { 64, 34, 25, 12, 22, 11, 90, 4, 9, 1, 33, 23, 420, 54, 153, 200, 1000, 400, 323, 911, 1000, 999,998,997,996,995,994,993,992,991,990,989,988,987,986,985,984,983,982,981,980,979,978,977,976,975,974,973,972,971};
    int N = sizeof(arr) / sizeof(arr[0]);
    bubbleSort(arr, N);
    return 0;
}
// This code is contributed by rathbhupendra

