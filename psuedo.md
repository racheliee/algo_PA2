## Longest Common Subsequence

    # Subproblem
        # 
    # Relation
    # Topological order
    # Base case
    # Original problem
    # Time complexity





## Wikipedia Pseudo-code
## Computing length of LCS
    function LCSLength(X[1..m], Y[1..n])
        C = array(0..m, 0..n)
        for i := 0..m
            C[i,0] = 0
        for j := 0..n
            C[0,j] = 0
        for i := 1..m
            for j := 1..n
                if X[i] = Y[j]
                    C[i,j] := C[i-1,j-1] + 1
                else
                    C[i,j] := max(C[i,j-1], C[i-1,j])
        return C[m,n]

## Reading out an LCS
    function backtrack(C[0..m,0..n], X[1..m], Y[1..n], i, j)
        if i = 0 or j = 0
            return ""
        if  X[i] = Y[j]
            return backtrack(C, X, Y, i-1, j-1) + X[i]
        if C[i,j-1] > C[i-1,j]
            return backtrack(C, X, Y, i, j-1)
        return backtrack(C, X, Y, i-1, j)

## Reading out all LCSs
    function backtrackAll(C[0..m,0..n], X[1..m], Y[1..n], i, j)
        if i = 0 or j = 0
            return {""}
        if X[i] = Y[j]
            return {Z + X[i] for all Z in backtrackAll(C, X, Y, i-1, j-1)}
        R := {}
        if C[i,j-1] ≥ C[i-1,j]
            R := backtrackAll(C, X, Y, i, j-1)
        if C[i-1,j] ≥ C[i,j-1]
            R := R ∪ backtrackAll(C, X, Y, i-1, j)
        return R

## Print difference
    function printDiff(C[0..m,0..n], X[1..m], Y[1..n], i, j)
        if i >= 0 and j >= 0 and X[i] = Y[j]
            printDiff(C, X, Y, i-1, j-1)
            print "  " + X[i]
        else if j > 0 and (i = 0 or C[i,j-1] ≥ C[i-1,j])
            printDiff(C, X, Y, i, j-1)
            print "+ " + Y[j]
        else if i > 0 and (j = 0 or C[i,j-1] < C[i-1,j])
            printDiff(C, X, Y, i-1, j)
            print "- " + X[i]
        else
            print ""

## Optimization
    reduce problem set
    reduce comparison time
    strings --> hash


