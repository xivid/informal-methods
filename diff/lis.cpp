/* Legi: 17-941-998 */

/** Longest Increasing Subsequence
 *  Find the length of a longest upsequence from a sequence of integers 
 *
 *  Input:  some integers separated by space or newline
 *  Output: the length of the longest upsequence(s) of the input sequence
 *
 *  Example:
 *  [Input]
 *  6 5 1 4 3 2 4 3 2 1
 *  [Output]
 *  3
 *  [Explanation]
 *  The longest upsequences are [1, 3, 4] and [1, 2, 4], of which the length 
 *  is 3.
 */

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <vector>

int main() {
    std::vector<int> A;
    int x;
    while (scanf("%d", &x) == 1) {
        A.push_back(x);
    }

    printf("length is %d\n", A.size());
    for (auto a: A) printf("%d\n", a);
    return 0;
}
