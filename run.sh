
#run_algs_undo run_algs_adjlookup run_algs_edgeswap 
for alg in `cat impl.txt`; do

    for filename in `cat graphs.txt`; do
        echo -----------------------
        echo running $alg on $filename
        timeout 1m ./$alg $filename degenopt

    done


done