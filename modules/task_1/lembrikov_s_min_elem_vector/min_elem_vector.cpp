// Copyright 2019 Lembrikov Stepan
#include <../../../modules/task_1/lembrikov_s_min_elem_vector/min_elem_vector.h>
#include <algorithm>
#include <vector>

std::vector<int> getIdentityVector(int n) {
    std::vector <int> a(n);
    for (int i = 0; i < n; i++) {
        a[i] = 1;
    }
    return a;
}

std::vector<int> getRandomVector(int n) {
    std::mt19937 engine;
    std::vector<int> a(n);
    engine.seed(n);
    for (int i = 0; i < n; i++) {
        a[i] = engine() % 100;
    }
    return a;
}

std::vector<int> getConstVector(int n, int c) {
    std::vector <int> a(n);
    for (int i = 0; i < n; i++) {
        a[i] = c;
    }
    return a;
}

std::vector<int> getNegativeVector(int n) {
    std::vector <int> a(n);
    for (int i = 0; i < n; i++) {
        a[i] = -1;
    }
    return a;
}

int MinOfVector(const std::vector <int> a, int n) {
    int res = 0;
    int size;
    int rank;
    int ost;
    int k;
    int flag = 0;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    ost = n % size;
    if (n < size) {
        size = 1;
        ost = 0;
        flag = -1;
    }
    k = n / size;

    if (ost == 0) {
        if (rank == 0) {
            for (int i = k; i <= n - k; i += k) {
                MPI_Send(&a[i], k, MPI_INT, i / k, 0, MPI_COMM_WORLD);
            }
        }
    } else {
        if (rank == 0) {
            for (int i = k + ost; i <= n - k; i += k) {
                MPI_Send(&a[i], k, MPI_INT, (i - ost) / k, 0, MPI_COMM_WORLD);
            }
        }
    }

    MPI_Status status;
    std::vector <int> b(k, 0);
    int prom_res;
    if (rank == 0) {
        prom_res = a[0];
        for (int i = 0; i < k + ost; i++) {
            prom_res = std::min(prom_res, a[i]);
        }
    } else {
        if (flag == 0) {
            MPI_Recv(&b[0], k, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
            prom_res = b[0];
            for (int i = 0; i < k; i++) {
                prom_res = std::min(prom_res, b[i]);
            }
        }
    }
    if (flag == 0) {
        MPI_Reduce(&prom_res, &res, 1, MPI_INT, MPI_MIN, 0, MPI_COMM_WORLD);
        return res;
    } else {
        if (rank == 0) {
            return prom_res;
        } else {
            return -1;
        }
    }
}
