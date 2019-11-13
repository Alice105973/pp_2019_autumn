// Copyright 2019 Boganov Sergey
#include <mpi.h>
#include "../../../modules/task_1/boganov_s_trapezoid_integration/trapezoid_integration.h"

double getSequentialTrapezoidIntegral(double(*f)(double), double a, double b, int N) {
    double h = (b - a) / static_cast<double>(N), I = 0.0;
    for (double x = a; x <= b; x += h) {
        I += ((f(x) + f(x + h)) * h) / 2.0;
    }
    return I;
}

double getParallelTrapezoidIntegral(double(*f)(double), double a, double b, int N) {
    double l_a, l_b, l_I = 0.0, g_I = 0.0;

    int rank, size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (rank == 0) {
        double part = (b - a) / size;
        l_a = a + part;

        for (int proc = 1; proc < size; proc++) {
            l_b = l_a + part;

            MPI_Send(&l_a, 1, MPI_DOUBLE, proc, 0, MPI_COMM_WORLD);
            MPI_Send(&l_b, 1, MPI_DOUBLE, proc, 0, MPI_COMM_WORLD);

            l_a += part;
        }
        l_a = a;
        l_b = l_a + part;
    } else {
        MPI_Status status;
        MPI_Recv(&l_a, 1, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD, &status);
        MPI_Recv(&l_b, 1, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD, &status);
    }

    l_I = getSequentialTrapezoidIntegral(f, l_a, l_b, N);

    MPI_Reduce(&l_I, &g_I, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

    return g_I;
}
