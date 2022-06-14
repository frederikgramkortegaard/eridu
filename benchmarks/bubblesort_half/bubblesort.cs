// C# program for implementation
// of Bubble Sort
using System;
 
class GFG {
    static void bubbleSort(int[] arr)
    {
        int n = arr.Length;
        for (int i = 0; i < n - 1; i++)
            for (int j = 0; j < n - i - 1; j++)
                if (arr[j] > arr[j + 1]) {
                    // swap temp and arr[i]
                    int temp = arr[j];
                    arr[j] = arr[j + 1];
                    arr[j + 1] = temp;
                }
    }
 
    // Driver method
    public static void Main()
    {
        int[] arr = { 64, 34, 25, 12, 22, 11, 90, 4, 9, 1, 33, 23, 420, 54, 153, 200, 1000, 400, 323, 911, 1000, 999,998,997,996};
        bubbleSort(arr);
    }
}
 
// This code is contributed by Sam007
