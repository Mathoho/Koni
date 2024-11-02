echo "1. serial; for (sum values 0 to 50m on 1 thread)"
time ./omp_nth_tri 50000000 4 1
echo "2. critical; parallel for (sum values 0 to 50m on 1 thread)"
time ./omp_nth_tri 50000000 4 2
echo "3. manual private; parallel for (sum values 0 to 50m on 4 threads)"
time ./omp_nth_tri 50000000 4 3
echo "4. reduction; parallel for (sum values 0 to 50m on 4 threads)"
time ./omp_nth_tri 50000000 4 4
