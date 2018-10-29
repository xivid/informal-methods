/* Legi: 17-941-998 */

/** Longest Increasing Subsequence
 *  Find the actual longest upsequence from a sequence of integers 
 *
 *  Input:  some integers separated by space or newline
 *  Output: one of the longest upsequence of the input sequence, each in a line
 *
 *  Example:
 *  [Input]
 *  6 5 1 4 3 2 4 3 2 1
 *  [Output]
 *  1
 *  2
 *  3
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
               size_t& N) 
{
    int x;
    while (scanf("%d", &x) == 1) 
    {
        A.push_back(x);
    }
    N = A.size();
    A.resize(N+1);  // avoids out-of-bound error when n == N-1
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
 *   For all k s.t. 0 <= k <= m, M[k] = min_{0<=i<=n} {A[i]: maxlenPR(i) = k}, 
 *   i.e. M[k] is the minimum A[i] in 0 <= i <= n that has a longest prefix of
 *   length k, and Mi[k] is the index i.
 * Invariant `MSorted':
 *   M is sorted, i.e. for all 0 <= i < j <= m, it holds that M[i] < M[j].
 *   (cannot be M[i] == M[j], otherwise contradict with `MinimalEndpoint')
 * Invariant `Path':
 *   For all i s.t. 0 <= i <= n, P[i] is the index-in-A of the predecessor of
 *   A[i] in the longest upsequence ending with A[i], or P[i] is N if A[i] has
 *   no predecessor.
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
    std::vector<size_t> Mi;  // Mi[k] is the corresponding index of M[k] in A
    std::vector<size_t> P;  // P[i] is the index j of A[j], where A[j] is the
                            // predecessor of A[i] in the longest upsequence 
                            // ending with A[i], or P[i] == N if no predecessor
    get_input(A, N);

    // Assign A[N] with "something", I choose INT_MAX so that it will not have
    // a chance to change M in the statement M[l] = min(M[l], A[n + 1]) of the
    // last round.
    A[N] = INT_MAX;  // The assumption is that all inputs < INT_MAX (2^31 - 1)
    n = m = l = 0;
    // `UpSequence' is true here, because n == 0, and the length of an empty set
    // is 0.
    // `Prefix' is true here, because the prefix of A[0] is empty.

    Mi.resize(N + 1, N);
    Mi[0] = 0;  // Mi = [0, N, ..., N]
    M.resize(N + 1, INT_MAX);
    M[0] = A[0];  // M = [A[0], INT_MAX, ..., INT_MAX] 
    // `MinimalEndpoint' is true here as m == n == 0, M[0] == A[0], Mi[0] = 0.
    // `MSorted' is true here, obviously by the above assignments.
    // `UpSequence' and `Prefix' are still true here as none of m, n, l changed.

    P.resize(N + 1, N);
    // `Path' is true here, as A[0] has no predecessor.

    while (n != N) {
        // `UpSequence', `Prefix', `MinimalEndpoint',
        // `MSorted' and `Path' are all true here, as nothing as changed.

        m = std::max(m, l + 1);
        // `UpSequence' is true up to n+1 here.

        {
            // Precondition: M is sorted.
            // Now use binary search to find the smallest element that is 
            // greater than or equal to A[n + 1] in M[0, m + 1)
            // Invariant `B1': M[0, left) < a <= M[right, m+1) and left <= right
            int a = A[n + 1];
            size_t left = 0, right = m + 1, mid;
            // `B1' is true here, as both sides are just empty, and m > 0.
            while (left != right) {
                // `B1' is true, and left < right
                mid = left + ((right - left) >> 1);  // div 2, avoid overflow
                // left <= mid and mid < right, by properties of arithmetics
                if (M[mid] >= a)
                {
                    // a <= M[mid, m+1)
                    right = mid;
                    // right - left has decreased
                    // a <= M[right, m+1), left <= right
                }
                else
                {
                    // a > M[mid] => M[0, mid+1) < a
                    left = mid + 1;
                    // right - left has decreased
                    // M[0, left) < a, left <= right
                }
                // right - left has decreased
                // `B1' is true here, and left <= right
            }
            // right - left must become 0 at some point -> termination
            // `B1' is true here, and left == right
            l = left;
            // M[0, l) < a <= M[l, m+1) is true here, where a = A[n+1]
        }
        // M[0, l) < A[n+1] <= M[l, m+1), so if l > 0, the longest prefix of 
        // A[n+1] is the length-(l-1) prefix of M[l-1] appended by M[l-1] 
        // itself, thus its length is (l-1)+1 = l. (If l == 0: empty prefix)
        // `Prefix' is true up to n+1 here.

        if (l > 0)  // if A[n + 1] has non-empty prefix
        {
            P[n + 1] = Mi[l - 1];
            // `Path' is true up to n+1 here: M[l-1] is the predecessor of
            // A[n+1], whose index in A is Mi[l-1].
        }
        // otherwise (empty prefix), P[n + 1] = N (already initialized).
        // So `Path' is true up to n+1 here.

        if (A[n + 1] < M[l])
        {
            M[l] = A[n + 1];
            Mi[l] = n + 1;
        }
        // `MinimalEndpoint' is true up to n+1 here.
        // `MSorted' is true up to n+1 here.

        n = n + 1;
        // `UpSequence', `Prefix', `MinimalEndpoint',
        // `MSorted' and `Path' are all true up to n.
    }
    // `UpSequence', `Prefix', `MinimalEndpoint', `MSorted' and `Path'
    // are all true up to N.

    // output actual longest upsequence
    std::vector<int> seq(m);
    size_t idx = P[N];  // traverse backwards from "predecessor of A[N]"
    for (size_t i = m; i > 0; --i)
    {
        seq[i - 1] = A[idx];
        idx = P[idx];
    }
    for (size_t i = 0; i < m; ++i)
    {
        printf("%d\n", seq[i]);
    }
    return 0;
}
