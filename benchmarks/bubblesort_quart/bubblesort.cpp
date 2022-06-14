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
    int arr[] = { 64, 34, 25, 12, 22, 11, 90, 4, 9, 1, 33, 23};
    int N = sizeof(arr) / sizeof(arr[0]);
    bubbleSort(arr, N);
    return 0;
}
// This code is contributed by rathbhupendra

