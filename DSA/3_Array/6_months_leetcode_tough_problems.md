Videos: 

Trapping rain water - 
- Stock buy and sell problem: https://practice.geeksforgeeks.org/batch/dsa-4/track/DSASP-Arrays/video/MTE2Nw%3D%3D ( just follow slop of day and add to profit if positive )
- Maximum difference problem with order: https://practice.geeksforgeeks.org/batch/dsa-4/track/DSASP-Arrays/video/MTE2NQ%3D%3D ( just keep track of minimum visited so far and subtract with current and keep updating maximum, no need to track maximum element )
- Left rotate array by D amount: https://practice.geeksforgeeks.org/batch/dsa-4/track/DSASP-Arrays/video/MTMwMA%3D%3D ( reverse first D subgroup, then reverse remaining subgroup and then reverse entire resulting array )
- Leaders in an array: https://practice.geeksforgeeks.org/batch/dsa-4/track/DSASP-Arrays/video/MTE2NA%3D%3D more efficient https://practice.geeksforgeeks.org/batch/dsa-4/track/DSASP-Arrays/article/NzE5Ng%3D%3D ( scan from left, track maximum and print whenever maximum changes )
- Trapping rain water ( IMP, never went into head ): https://practice.geeksforgeeks.org/batch/dsa-4/track/DSASP-Arrays/video/MTE2OA%3D%3D ( time complexity: O(n), space complexity O(n)) ( store maximum element array from right and left and at each position, find subtraction of right left max diff and then subtract current height )
- Maximum sum subarray ( IMP ) ( Kadane's algorithm ): https://practice.geeksforgeeks.org/batch/dsa-4/track/DSASP-Arrays/video/MTQwMg%3D%3D ( time complexity: O(n), space complexity O(n))
- Maximum circular sum ( IMP ):https://practice.geeksforgeeks.org/batch/dsa-4/track/DSASP-Arrays/video/MTQ2NDU%3D
- Majority element :https://practice.geeksforgeeks.org/batch/dsa-4/track/DSASP-Arrays/video/MTQyMg%3D%3D
- Minimum consecutive flip: https://practice.geeksforgeeks.org/batch/dsa-4/track/DSASP-Arrays/video/MTU4Ng%3D%3D
- Subarray with given sum: https://practice.geeksforgeeks.org/batch/dsa-4/track/DSASP-Arrays/video/MTQ2NTk%3D
- Prefix sum: https://practice.geeksforgeeks.org/batch/dsa-4/track/DSASP-Arrays/video/NzU4Nw%3D%3D
- Equilibrium point: https://practice.geeksforgeeks.org/batch/dsa-4/track/DSASP-Arrays/video/NzU4OA%3D%3D
- Maximum appearing element: https://practice.geeksforgeeks.org/batch/dsa-4/track/DSASP-Arrays/video/NzU4OQ%3D%3D

Other than video:
- Kadane's algorithm
    ( while doing it in practice, I though to make subarray sub zero whenever negative. It will work in cases where elements can be non-negative. But where all elements will be negative, it will fail. )

Forgot while coding:
- Maximum difference problem: https://practice.geeksforgeeks.org/batch/dsa-4/track/DSASP-Arrays/video/MTE2NQ%3D%3D
- Remainder problem ( if A picks least ( 1 ) in first round then for B remaining matchsticks should be grater than he can pick maximum ). ( So here we need to check N%5 is not equal to 0. If 0, then it is not possible for A to win ): https://practice.geeksforgeeks.org/batch/dsa-4/track/DSASP-Arrays/problem/-matchsticks-game4906
- Check if array is sorted ( not a complex, just refresh ): https://practice.geeksforgeeks.org/batch/dsa-4/track/DSASP-Arrays/problem/check-if-array-is-sorted-and-rotated-clockwise-1587115620
- Reverse array in groups: https://practice.geeksforgeeks.org/batch/dsa-4/track/DSASP-Arrays/problem/reverse-array-in-groups0255
- Rearrange an array with O(1) extra space: https://practice.geeksforgeeks.org/batch/dsa-4/track/DSASP-Arrays/problem/rearrange-an-array-with-o1-extra-space3142, related similar problem: https://www.geeksforgeeks.org/batch/dsa-4/track/DSASP-Arrays/problem/-rearrange-array-alternately-1587115620    
```C++
class Solution{
public:
// arr: input array
// n: size of array
//Function to rearrange an array so that arr[i] becomes arr[arr[i]]
//with O(1) extra space.
void arrange(long long arr[], int n) {
    // Your code here
    /* Embed arr[i] and *(arr+arr[i]) in single elememt */
    for(long long int i=0;i<n;i++)
    {
        arr[i]=arr[i]+(((*(arr+arr[i]))%n)*n);
    }
    
    /* Retreive original arr[i] */
    for(long long int i=0;i<n;i++)
    {
        arr[i]=arr[i]/n;
    }
    
    /* Retreive *(arr+arr[i]) */
    /*for(long long int i=0;i<n;i++)
    {
        arr[i]=arr[i]%n;
    }*/
}
};
```
- Smallest positive missing number: https://www.geeksforgeeks.org/batch/dsa-4/track/DSASP-Arrays/problem/smallest-positive-missing-number-1587115621
- Frequency of limited range array elements: https://www.geeksforgeeks.org/batch/dsa-4/track/DSASP-Arrays/problem/frequency-of-array-elements-1587115620
- Maximum occured integer: https://www.geeksforgeeks.org/batch/dsa-4/track/DSASP-Arrays/problem/maximum-occured-integer4602
- Fraction trouble: https://www.geeksforgeeks.org/batch/dsa-4/track/DSASP-Arrays/problem/fraction-trouble3803
- Longest subarray of even and odd: https://www.geeksforgeeks.org/batch/dsa-4/track/DSASP-Arrays/problem/longest-subarray-of-evens-and-odds
- Betting game: https://www.geeksforgeeks.org/batch/dsa-4/track/DSASP-Arrays/problem/betting-game0634
- Equilibrium point: https://www.geeksforgeeks.org/batch/dsa-4/track/DSASP-Arrays/problem/equilibrium-point-1587115620
- Strongest neighbor: https://www.geeksforgeeks.org/batch/dsa-4/track/DSASP-Arrays/problem/strongest-neighbour