make clean
make
echo "Par tasks:"
echo "----------"
echo "*** 1 thread ***"
time ./omp_tasks 100 1 1
echo "*** 2 threads ***"
time ./omp_tasks 100 2 1
echo "*** 4 threads ***"
time ./omp_tasks 100 4 1

echo "Par for loop:"
echo "--------------"
echo "*** 1 thread ***"
time ./omp_tasks 100 1 2 
echo "*** 2 threads ***"
time ./omp_tasks 100 2 2 
echo "*** 4 threads ***"
time ./omp_tasks 100 4 2 
