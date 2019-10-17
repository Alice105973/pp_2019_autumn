// Copyright 2019 Konnov Sergey
#include "../../../modules/task_1/konnov_s_trapez_integr/trapez_integr.h"
#include <mpi.h>
#include <functional>
#include <iostream>
#include <algorithm>

double GetTrapezIntegrParallel(double l, double r, int n, const std::function<double(double)>& f) {
    int size, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (size == 1)
        return GetTrapezIntegrSequential(l, r, n, f);

    const double step = (r - l) / static_cast<double>(n);
    const int delta = n / size;
    double left, right;
    int portion = 0;
    double local_integral = 0;
    double global_integral = 0;

    if (rank == 0) {
        left = l, right = l + step * delta - step, portion = delta-1;
        for (int i = 1; i < size - 1; i++) {
            double mes[] = { left + step * delta*i, left + step * delta*(i + 1) - step};
            MPI_Send(&mes, 2, MPI_DOUBLE, i, 0, MPI_COMM_WORLD);
            int tmp = delta - 1;
            MPI_Send(&tmp, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
        }
        double mes[] = { left + step * delta*(size- 1), r};
        int tmp = n - (size-1)*delta;
        MPI_Send(&mes, 2, MPI_DOUBLE, size-1, 0, MPI_COMM_WORLD);
        MPI_Send(&tmp, 1, MPI_INT, size-1, 0, MPI_COMM_WORLD);
    } else {
        double mes[2];
        MPI_Status status;
        MPI_Recv(&mes, 2, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD, &status);
        MPI_Recv(&portion, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
        left = mes[0], right = mes[1];
    }

    if (left < right) {
        local_integral = GetTrapezIntegrSequential(left, right, portion, f);
        if (l != left)
            local_integral += f(left) / 2 * step;
        if (r != right)
            local_integral += f(right) / 2 * step;
    }

    MPI_Reduce(&local_integral, &global_integral, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
    return global_integral;
}

double GetTrapezIntegrSequential(double l, double r, int n, const std::function<double(double)>& f) {
    double step = (r - l) / static_cast<double>(n);
    double integral = f(l) / 2 + f(r) / 2;
    for (double i = l + step; i < r; i += step)
        integral += f(i);
    integral*=step;
    return integral;
}
