bitwise operators articles

- bitwise operators: https://www.geeksforgeeks.org/bitwise-operators-in-c-cpp/
    - Sum of two numbers: A|B then add A&B ( this is for carry ). also check https://www.geeksforgeeks.org/add-two-numbers-without-using-arithmetic-operators/
    - XOR:
        - Find single number: XOR all and final result is extra single number
        - Swap two nums: Use ^ in place of + without temp
        - XOR doubly linked list: Stores prev+next in each node. First node is NULL(prev)^first next. Then subsequent are XOR of prev^next. To traverse backward/forward need to have prev or next node's pointer. ( https://www.geeksforgeeks.org/xor-linked-list-a-memory-efficient-doubly-linked-list-set-1/ )
        - 2 non-repeating elements: Find all XOR and result will be XOR of 2 non-repeating elemnts. Take LSB of result and and then XOR all elements having that bit set ( take (XOR & ~(XOR-1)) to find that bit). In else part, XOR others. These XOR will give result. ( https://www.geeksforgeeks.org/find-the-two-numbers-with-odd-occurences-in-an-unsorted-array/ )
        - Add 2 numbers without using arithmetic operators: Clculate carry ( X&Y ) and add to Sum ( X^Y ). Then right shift carry and assign to Y. Repeate till Y is not zero and return X. Do in sequence. ( https://www.geeksforgeeks.org/add-two-numbers-without-using-arithmetic-operators/ )
        - Swap bits in given number ( https://www.geeksforgeeks.org/swap-bits-in-a-given-number/ )
        - Count number of bits to be flipped : Do XOR of both and count number of set bits
        ```C++
        int countSetBits(int n)
        {
            int count = 0;
            while (n > 0) {
                count++;
                n &= (n - 1);
            }
            return count;
        }
        ```
        - To integers have ooposite sign: XOR of both and check MSB set by ( (~((~0)>>1)) & (~0))
        