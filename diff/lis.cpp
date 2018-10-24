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
    A.resize(N+1);  // avoids out-of-bound error when n == N-1
    M.resize(N+1);
}

/** Invariants
 * 
 * Invariant `UpSequence': 
 *   The variable m is the maximum length of the upsequences in A[0, n) 
 *   (i.e. ending before A[n]).
 * Invariant `Prefix': 
 *   The variable l is the length of the longest prefix of A[n].
 *   (i.e. longest upsequence ending exactly at A[i] where 0 <= i < n and 
 *   A[i] < A[n]).
 *   In other words, l is the smallest k satisfying M[k] >= A[n], thus it 
 *   gives a length-((l-1)+1) prefix ending with M[l-1] for A[n].
 * Invariant `MinimalEndpoint':
 *   For all k s.t. 0 <= k <= m, M[k] is the minimum A[i] in 0 <= i <= n
 *   that has a longest prefix of length k.
 * Invariant `MSorted':
 *   M is sorted, i.e. for all 0 <= i < j <= m, it holds that M[i] < M[j].
 *   (cannot be M[i] == M[j], otherwise contradict with `MinimalEndpoint')
 *
 */
int main() 
{
    std::vector<int> A;  // input sequence
    size_t N;  // length of A
    size_t n;  // loop variable
    size_t m;  // length of longest upsequence seen so far (i.e. in A[0, n))
    size_t l;  // length of longest prefix of A[n]
    std::vector<int> M;  // M[k] is the minimal A[i] seen so far 
                         // whose longest prefix's length is k
    get_input(A, N, M);

    // Assign A[N] with "something", I choose INT_MAX so that it will not have
    // a chance to change M in the statement M[l] = min(M[l], A[n + 1]) of the
    // last round.
    A[N] = INT_MAX;  // The assumption is that all inputs < INT_MAX (2^31 - 1)
    n = m = l = 0;

    // `UpSequence' is true here, because n == 0, and the length of an empty set
    // is 0.
    // `Prefix' is true here, because the prefix of A[0] is empty.
    M[0] = A[0]; 
    for (size_t i = 1; i < N; ++i)
    {
        M[i] = INT_MAX;
    }
    // `MinimalEndpoint' is true here as m == n == 0, and M[0] == A[0].
    // `MSorted' is true here, obviously by the above assignments.
    // `UpSequence' and `Prefix' are still true here as none of m, n, l changed.

    while (n != N) {
        // `UpSequence', `Prefix', `MinimalEndpoint' 
        // and `MSorted' are all true here, as nothing as changed.

        m = std::max(m, l + 1);
        // `UpSequence' is true up to n+1 here.

        l = binary_search(M, 0, m + 1, A[n + 1]);
        // M[0, l) < A[n+1] <= M[l, m+1), so the longest prefix of A[n+1] is
        // the length-(l-1) prefix of M[l-1] appended by M[l-1] itself, thus
        // its length is (l-1)+1 = l.
        // `Prefix' is true up to n+1 here.

        M[l] = std::min(M[l], A[n + 1]);
        // `MinimalEndpoint' is true up to n+1 here.
        // `MSorted' is true up to n+1 here.

        n = n + 1;
        // `UpSequence', `Prefix', `MinimalEndpoint' 
        // and `MSorted' are all true up to n.
    }
    // `UpSequence' is true here.

    printf("%lu\n", m);
    return 0;
}
