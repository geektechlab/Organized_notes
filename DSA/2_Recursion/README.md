```C++
func(n){
	if(n>0) /* base condition */
	{
		return success
	}

	some operation1;
	func(n-1);
	some operation2;
	func(n-2);
	some operation3;
	func(n-3);
	some operation4;
}
```

points:
- base condition is must
- Following parameters determine dimensions ( depth and width ) of tree:
    - all func calls represents branches, so count of func call will decide how muny sub branches any node will have.
    - Depending on amount of decrement/increment in input parameter, height tree will be decided.
- First it will go to right most child node in tree
- operation before recursive call ( function call ) will be on going down path in tree
- operation after recursive call ( function call ) will be on going up path in tree
- If we want to print something in order if recursion is in same sequence as an input then while going down, we can do. It means before recursion call ( function call ), we can do. For example, print number N to 1: we can print number before calling fun(n-1).
- If we want to print something in reverse order if recursion is in same sequence as an input then while going up/coming back, we can do. It means after recursion call ( function call ), we can do. For example, print number N to 1: we can print number before calling fun(n-1).

```C++
tail recursion:
func(n){
	some op1;
	func(n-1);
	// some op2;
}
```
some compiler can optimize generated assembly in a way that call to func(n-1) will be replaced by go to statement with n=n-1. So, caller doesn't have push caller state on stack and it will faster and less space complex. We can modify our code in such a way that op2 can be somehow merged with op1 if we want to make it tail-recursive.
```C++
func(n)
{
	if(base condition)
	{
		return 0;
	}
	return n*func(n-1);
}
```
this is not tail recursive because it needs to save multiplications after coming back from recursion. ( read article )

To solve a recursion problem, first plan some approach using pen and paper. Just plan approach, don't code. Then implement.

Important links:
- Rope Cutting Problem: https://practice.geeksforgeeks.org/batch/dsa-4/track/DSASP-Recursion/video/MjMzMw%3D%3D
- Subsets of string: https://practice.geeksforgeeks.org/batch/dsa-4/track/DSASP-Recursion/video/MjMzMQ%3D%3D
- Tower of hanoi: https://practice.geeksforgeeks.org/batch/dsa-4/track/DSASP-Recursion/video/OTYw
- Subset sum: https://practice.geeksforgeeks.org/batch/dsa-4/track/DSASP-Recursion/video/MTQ2OQ%3D%3D
- Printing all permutations: https://practice.geeksforgeeks.org/batch/dsa-4/track/DSASP-Recursion/video/MjMzNA%3D%3D
- Josephus problem: https://practice.geeksforgeeks.org/batch/dsa-4/track/DSASP-Recursion/video/OTYx ( for getting idea of solution watch from -8:08. However, with little thinking I figured out exact video code without watching video or taking help. )

When somebody ask do it without loop, think if it is meant do it by recursion. For example, print 1 to N without using loop.
