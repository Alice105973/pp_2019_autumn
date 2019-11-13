// Copyright 2019 Devlikamov Vladislav

#include <mpi.h>
#include <iostream>
#include <random>
#include <ctime>
#include <numeric>
#include <vector>
#include <algorithm>
#include <functional>
#include "../../../modules/task_1/devlikamov_v_max_matrix/max_matrix.h"

std::vector<std::vector <int>> getRandomMatrix(int n, int m) {
    std::vector <std::vector <int>> Matrix(n, std::vector <int> (m));
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            Matrix[i][j] = 1;
        }
    }
    return Matrix;
}

std::vector<std::vector <int>> getRandomMatrixT(int n, int m) {
    std::vector <std::vector <int>> Matrix(n, std::vector <int> (m));
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            Matrix[i][j] = ((i + j) % 2 == 0) ? 1 : -1;
        }
    }
    return Matrix;
}

std::vector<std::vector <int>> getRandomMatrixU(int n, int m) {
    std::vector <std::vector <int>> Matrix(n, std::vector <int> (m));
    for (int i = 0; i < n; i++) {
        Matrix[i][i] = 1;
    }
    return Matrix;
}

std::vector<std::vector <int>> getRandomMatrixM(int n, int m) {
    std::vector <std::vector <int>> Matrix(n, std::vector <int> (m));
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            Matrix[i][j] = i + j;
        }
    }
    return Matrix;
}

int MaxOfMatrix(const std::vector <std::vector <int>> &a, int n, int m) {
    int answ = 0;
    int size, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    if (rank == 0) {
        for (int i = 0; i < n; i++) {
            if (i % size) {
                MPI_Send(&a[i][0], m, MPI_INT, i % size, 0, MPI_COMM_WORLD);
            }
        }
    }
    MPI_Status status;
    std::vector <int> b(m);
    int ans = 0;
    if (rank == 0) {
        for (int i = 0; i < n; i += size) {
            ans = std::max(*max_element(a[i].begin(), a[i].end()), ans);
        }
    } else {
        for (int i = rank; i < n; i += size) {
            MPI_Recv(&b[0], m, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
            ans = std::max(ans, *max_element(b.begin(), b.end()));
        }
    }
    MPI_Reduce(&ans, &answ, 1, MPI_INT, MPI_MAX, 0, MPI_COMM_WORLD);
    return answ;
}
