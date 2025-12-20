- in recursion, always pay attention to branch. If we put if --then>> else if, then two will not get executed. Always ensure branching happens
- for passing string, always use "" instead of '' to avoid \0 removal and pass everytime with string+"a" format
    - for vectors we can do following:
                sub_ans.push_back(nums[nums.size()-index]);
                gen_ans(nums, sub_ans, index-1, fin_ans);
- if we are using non-returning recursive function, always ensure recusrive call and base condition are guraded in if or other condition, otherwise stack overflow will happen
- when there is possibility to base condition with target number, don't try to pass it as increment and compare with target. Instead try to check if we can decrement and compare with 0, so that we can avoid passing two numbers current and target and can rely on single current number as argument
- IMP: if input array has distinct elements, go for this approach or otherwise use next one: check my solution https://leetcode.com/problems/combination-sum/, why here sorting is required and once all current elements gets added repeatedly, then only next element gets stored and this avoid repeaation of all permutations of numbers. Check how this can be converted to loop based solution, preser loop based solution only because keeping multiple types in head will make me overwhelmed
    - Use this only if input array ahs repeated elements: This is too heavy: just understand: https://leetcode.com/problems/combination-sum-ii/submissions/1842620336/
        - I alawys make a mistake of gen_subs(final_ans, sub_ans, nums, i+1); using curr_index instead of i+1

- In this, https://leetcode.com/problems/letter-combinations-of-a-phone-number/submissions/1843336900/, I did  a lot of mistakes of vector, string and vectors of string etc, so before IV ensure that I can do everyuthing fluently. My core logic was okayish

- VVIMP: check and understand why core sudoku solver logic works and why stack overflow will not happen: https://leetcode.com/problems/sudoku-solver/submissions/1846971016/

- IMP: see why I goofed up and unable to create string partitions etc. https://leetcode.com/problems/expression-add-operators/