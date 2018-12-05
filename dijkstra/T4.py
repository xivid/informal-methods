import sys; INFTY= 1000

def heappop(hp, idx):
    '''
    Pop the minimum element and re-establish the heap invariant, in O(logN) time.
    Restore the heap invariant by first swapping the last element to the first and deleting the last element, then repeatedly moving the first element down until its m-value <= its children's. Meanwhile, `idx` is updated.
    '''
    # update top and remove redundancy
    hp[0] = hp[-1]; del hp[-1]
    if not hp:  # hp becomes empty
        return
    idx[hp[0][1]] = 0
    # re-heapify
    i, n = 0, len(hp)
    while 2*i+1 < n:  # while node i has at least the left child
        smallest = i
        if hp[2*i+1] < hp[i]:
            smallest = 2*i+1
        elif 2*i+2 < n and hp[2*i+2] < hp[i]:
            smallest = 2*i+2
        if smallest == i:  # heap invariant restored, adjusting finished
            break
        # otherwise do the swap and continue testing with new children
        idx[hp[smallest][1]] = i
        idx[hp[i][1]] = smallest
        hp[smallest], hp[i] = hp[i], hp[smallest]
        i = smallest


def heapupdate(hp, idx, (newD, nTo)):
    '''
    Decrease the m-value of hp[idx[nTo]] to newD and re-establish the heap invariant, in O(logN) time.
    Restore the heap invariant by repeatedly moving the updated element upwards (until its m-value >= its parent's), and update `idx` meanwhile.
    '''
    hp[idx[nTo]] = (newD, nTo)
    # Since it's decreased, it could only be moved upwards but not downwards
    i = idx[nTo]
    parent = (i-1)//2
    while i > 0 and hp[i] < hp[parent]:
        idx[hp[i][1]] = parent
        idx[hp[parent][1]] = i
        hp[i], hp[parent] = hp[parent], hp[i]
        i = parent
        parent = (i-1)//2  # becomes negative when i == 0 but doesn't matter because it will never be accessed (we have loop guard i>0)

print "How many nodes? ",
N= int(sys.stdin.readline()) # N= number of nodes.
print "Nodes are 0..%d, with initial node %d." % (N-1, 0)

# Three white-space separated integers representing (from,dist,to) per line.
print "Now enter the edges:"
G2 = [[] for i in range(N)]
for line in sys.stdin.readlines():
    nFrom, dist, nTo = map(int, line.split())
    G2[nFrom].append((dist, nTo))  # put every arc in the respective list
m= [INFTY]*N; m[0]= 0   # m:=  initially INFTY except for initial node
#>> un1 = [1]*N; nun1 = N
hp3 = [(m[i], i) for i in range(N)]
idx3 = list(range(N))
#<<
tn= 0                   # tn:= initial node

# Initialisation has established the invariants:
#   I1--- For all marked nodes, m has the least distance from 0. // A.
#   I2--- For all unmarked nodes, m has least all-marked distance from 0. // B.
#   I3--- Node tn is unmarked, and is m-least among the unmarked nodes. // C.

while 1:
#>> un1[tn] = 0; nun1 -= 1
#>> if nun1 == 0: break
    heappop(hp3, idx3)  # mark tn, which is at the heap top
    if len(hp3) == 0: break
#<<

    # Re-establish I2. // G.
    for (dist, nTo) in G2[tn]:
#>>     if un1[nTo]:
        if idx3[nTo] != N:
#<<
            newD= m[tn]+dist
            if newD<m[nTo]:
                m[nTo]= newD
#>>
                heapupdate(hp3, idx3, (newD, nTo))
#<<

    # Re-establish I3. // H.
    minD= INFTY
#>> for n in range(N):
#>>   if un1[n]:
#>>     if m[n]<=minD: tn= n; minD= m[n]
    if hp3:  # if there are remaining unmarked nodes
        minD, tn = hp3[0]
#<<
    if minD==INFTY: break # All remaining nodes unreachable. // I.
###
#   I1,I2 and m is INFTY for all nodes in un1. // J,K,L,M.

print "Least distances from Node 0 are:"
for n in range(N):
    if m[n]!=INFTY: print "Distance to Node %d is %d." % (n,m[n])
    else:           print "Node %d is unreachable." % n
