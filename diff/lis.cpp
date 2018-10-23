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
#include <climits>
#include <vector>

/** Read input from stdin
 *
 *  Stores the input numbers in A and the length in N,
 *  and reserves enough space for M
 */
void get_input(std::vector<int>& A, 
               unsigned int& N, 
               std::vector<unsigned int>& M) 
{
    int x;
    while (scanf("%d", &x) == 1) 
    {
        A.push_back(x);
    }
    N = (unsigned int) A.size();
    A.resize(N+1);  // for holding A[N]
}

int main() 
{
    std::vector<int> A;  // input sequence
    unsigned int N;  // length of A
    unsigned int n;  // loop variable
    unsigned int m;  // TBA
    unsigned int l;  // TBA
    std::vector<unsigned int> M;  // TBA auxiliary array
    get_input(A, N, M);
    // for (auto i : A) printf("%d\n", i);

    // Assign A[N] with "something"
    A[N] = INT_MAX;
    n = m = l = 0;
    // Invariant `UpSequence': 
    //   the variable m is the maximum length of the upsequences in A[0, n) 
    //   (i.e. ending before A[n]).
    // Invariant `Prefix': 
    //   the variable l is the length of the longest prefix of A[n] 
    //   (i.e. some upsequence ending exactly at A[i] where 0 <= i < n and 
    //   A[i] < A[n]).

    // `UpSequence' is true here, because n == 0, and the length of an empty set
    // is 0.
    // `Prefix' is true here, because the prefix of A[0] is empty.
    M[0] = 0;  // why 0? why isn't it INT_MAX? counterexample: -100 -98 -99
    for (int i = 0; i < N; ++i)
    {
        M[i] = INT_MAX;
    }
    // Invariant `MinimalEndpoint':
    //   for all k s.t. 0 <= k <= m, M[k] is the minimum A[i] in 0 <= i <= n
    //   that has a longest prefix of length k.
    // Invariant `MSorted':
    //   M is sorted, i.e. if i < j then M[i] < M[j] (cannot be M[i] == M[j])

    return 0;
}
