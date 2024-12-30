Always check NULL pointer

Remember difference between doubly/single, circular/non-circular linkedlist

- Insert at the end of circular list: https://practice.geeksforgeeks.org/batch/dsa-4/track/DSASP-LinkedList/video/MTUxOA%3D%3D
- Circular linked list advantages and disadvantages: https://practice.geeksforgeeks.org/batch/dsa-4/track/DSASP-LinkedList/video/MjA1NQ%3D%3D
- Applications of linked list: https://practice.geeksforgeeks.org/batch/dsa-4/track/DSASP-LinkedList/video/MjAzMA%3D%3D
- Problems with array data structure: https://practice.geeksforgeeks.org/batch/dsa-4/track/DSASP-LinkedList/video/MTEyMA%3D%3D
- Delete head of circular linkedlist: https://practice.geeksforgeeks.org/batch/dsa-4/track/DSASP-LinkedList/video/MTU1NQ%3D%3D
- Circular linkedlist advantages and disadvantages: https://practice.geeksforgeeks.org/batch/dsa-4/track/DSASP-LinkedList/video/MjA1NQ%3D%3D
- Middle of linkedlist: https://practice.geeksforgeeks.org/batch/dsa-4/track/DSASP-LinkedList/video/NjI1
- Nth node from end of linkedlist: https://practice.geeksforgeeks.org/batch/dsa-4/track/DSASP-LinkedList/video/Nzk2Nw%3D%3D
- Reverse linkedlist iterative: https://practice.geeksforgeeks.org/batch/dsa-4/track/DSASP-LinkedList/video/NjI3
- Recusrive reverse linkedlist ( didn't understand ): https://practice.geeksforgeeks.org/batch/dsa-4/track/DSASP-LinkedList/video/NjI4
https://practice.geeksforgeeks.org/batch/dsa-4/track/DSASP-LinkedList/video/MTQ1MQ%3D%3D
- Reverse linkedlist in group of size k: https://practice.geeksforgeeks.org/batch/dsa-4/track/DSASP-LinkedList/video/MTQ2OA%3D%3D
- Remove duplicates from sorted linkedlist: https://practice.geeksforgeeks.org/batch/dsa-4/track/DSASP-LinkedList/video/MTQ0MA%3D%3D
- Detect loop in linkedlist without Floyd's algo: https://practice.geeksforgeeks.org/batch/dsa-4/track/DSASP-LinkedList/video/NjI5
- Detect loop using floyd's cycle detection: https://practice.geeksforgeeks.org/batch/dsa-4/track/DSASP-LinkedList/video/NjMw https://practice.geeksforgeeks.org/batch/dsa-4/track/DSASP-LinkedList/video/NjMx
- Delete node with only pointer given to it: https://practice.geeksforgeeks.org/batch/dsa-4/track/DSASP-LinkedList/video/NjMy
- Segregate even and odd nodes: https://practice.geeksforgeeks.org/batch/dsa-4/track/DSASP-LinkedList/video/NzAyNg%3D%3D
- Intersection point of two linkedlist: https://practice.geeksforgeeks.org/batch/dsa-4/track/DSASP-LinkedList/video/NjM0
- Pairwise swap nodes in linkedlist: https://practice.geeksforgeeks.org/batch/dsa-4/track/DSASP-LinkedList/video/NzAyOA%3D%3D
- Clone a linkedlist with random pointer: https://practice.geeksforgeeks.org/batch/dsa-4/track/DSASP-LinkedList/video/MTU5MzY%3D

- LRU Cache design IMP: ( was asked to NK in Nvidia test ): https://practice.geeksforgeeks.org/batch/dsa-4/track/DSASP-LinkedList/video/MTQyMQ%3D%3D

- Merge two sorted linkedlist: https://practice.geeksforgeeks.org/batch/dsa-4/track/DSASP-LinkedList/video/MTQ0NA%3D%3D
- Palindrome check in linkedlist: https://practice.geeksforgeeks.org/batch/dsa-4/track/DSASP-LinkedList/video/MTg0MA%3D%3D

Coding issues:
- https://practice.geeksforgeeks.org/batch/dsa-4/track/DSASP-LinkedList/problem/find-length-of-loop ( Check how first both ptrs are assigned to head and then those are traversed. Otherwise, while loop will not break. Basically, in while just check if ptrs are valid. ptrs next can be checked inside. Also, while assigning next I did a mistake of assigning slow_ptr=head->next causing infinite loop, asn't able to debug even after Dhaval's hint. I should have done slow_ptr=slow_ptr->next).