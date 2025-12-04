- 560. Subarray Sum Equals K: https://leetcode.com/problems/subarray-sum-equals-k/submissions/1817795228/
- ( Remember each steps carefully ) Lexicographically greater permutation: https://takeuforward.org/data-structure/next_permutation-find-next-lexicographically-greater-permutation/
- IMP: https://takeuforward.org/arrays/count-subarray-sum-equals-k/
    - https://takeuforward.org/data-structure/count-the-number-of-subarrays-with-given-xor-k/
- check my leetcode solution and find why particular condition is added: https://leetcode.com/problems/majority-element-ii/submissions/1834423742 also check why adding numcount as 1/or incremneting immediately after reset is required

- check why I failed in first solution: https://leetcode.com/problems/rotate-image/
- similarly check this also https://leetcode.com/problems/spiral-matrix/
- check why extra bit is needed in matrix https://takeuforward.org/data-structure/set-matrix-zero/

- check xor approach: https://takeuforward.org/data-structure/find-the-repeating-and-missing-numbers/

- check why and how cnt is just getting added based on distance in sorted array instead of getting multiplied https://takeuforward.org/data-structure/count-inversions-in-an-array/
    - also check and apply deep thinking https://takeuforward.org/data-structure/count-reverse-pairs/

- IMP, check how max j-i works here instead of elemts but index https://leetcode.com/problems/maximum-width-ramp/description/

always prefer to use index from array instead of storing array element, this will help keep track of both index and element

whenever time limit exceeds, mostly it comes from while loop and check if i or j etc are properly incremented or decremented
whenever we have to compare any element with previous, try to compare with start+1 or end-1, this will avoid out of bound access
whenver sum or subtraction overflow happens take diff like stuff mid = l + (r-l)/2 or use long long

use:
no duplicates
    sorted
        single value -> set
        pair like combo -> map

    unsorted
        single value -> unordered_set
        pair like combo -> unordered_map

duplicates
    single value -> set
    pair like combo -> map
