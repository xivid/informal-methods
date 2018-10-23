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
#include <algorithm>

#define max(a, b) ((a) > (b) ? (a) : (b))
#define min(a, b) ((a) < (b) ? (a) : (b))

// temporary
size_t binary_search(const std::vector<int>& M,
                           size_t low,
                           size_t high,
                           int key)
{
    size_t position = std::lower_bound(M.begin() + low, M.begin() + high, key) - M.begin();
    return position;
}

/** Read input from stdin
 *
 *  Stores the input numbers in A and the length in N,
 *  and reserves enough space for M
 */
void get_input(std::vector<int>& A, 
               size_t& N, 
               std::vector<int>& M) 
{
    int x;
    while (scanf("%d", &x) == 1) 
    {
        A.push_back(x);
    }
    N = A.size();
    A.resize(N+1);  // for holding A[N]
    M.resize(N+1);
}

int main() 
{
    std::vector<int> A;  // input sequence
    size_t N;  // length of A
    size_t n;  // loop variable
    size_t m;  // TBA
    size_t l;  // TBA
    std::vector<int> M;  // TBA auxiliary array
    get_input(A, N, M);

    // Assign A[N] with "something", I choose INT_MAX so that it will not have
    // a chance to change M in the statement M[l] = min(M[l], A[n + 1]) of the
    // last round.
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
    M[0] = A[0];  // why 0? why isn't it INT_MAX? counterexample: -100 -98 -99
    for (size_t i = 1; i < N; ++i)
    {
        M[i] = INT_MAX;
    }
    // Invariant `MinimalEndpoint':
    //   for all k s.t. 0 <= k <= m, M[k] is the minimum A[i] in 0 <= i <= n
    //   that has a longest prefix of length k.
    // Invariant `MSorted':
    //   M is sorted, i.e. if i < j then M[i] < M[j] (cannot be M[i] == M[j])

    // `MinimalEndpoint' is true here as m == n == 0, and M[0] == A[0].
    // `MSorted' is true here, obviously by the above assignments.
    while (n != N) {
        // `UpSequence', `Prefix', `MinimalEndpoint' 
        // and `MSorted' are all true here, as nothing as changed.
        m = max(m, l + 1);
        // `UpSequence' is true up to n+1 here.
        l = binary_search(M, 0, m + 1, A[n + 1]);
        // M[0, l) < A[n+1] <= M[l, m+1), so `Prefix' is true up to n+1 here.
        M[l] = min(M[l], A[n + 1]);
        // `MinimalEndpoint' is true up to n+1 here.
        // `MSorted' is true here.
        n = n + 1;
        // `UpSequence', `Prefix', `MinimalEndpoint' 
        // and `MSorted' are all true up to n.
    }

    printf("%lu\n", m);
    return 0;
}
