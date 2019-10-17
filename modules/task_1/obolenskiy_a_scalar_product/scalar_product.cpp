// Copyright 2019 Obolenskiy Arseniy
#include <mpi.h>
#include <random>
#include <ctime>
#include <vector>
#include <stdexcept>
#include "../../../modules/task_1/obolenskiy_a_scalar_product/scalar_product.h"

static int offset = 0;

std::vector<int> getRandomVector(int vectorSize) {
    std::mt19937 gen;
    gen.seed((unsigned)time(0) + ++offset);
    std::vector<int> vec(vectorSize);
    for (int i = 0; i < vectorSize; ++i)
        vec[i] = gen() % 100;
    return vec;
}

int64_t getScalarProduct(const std::vector <int> &a, const std::vector <int> &b, size_t vectorSize) {
    int size, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    const int delta = vectorSize / size;
    const int rem = vectorSize % size;

    int errorCode;

    if (rank == 0) {
        if (a.size() != b.size()) {
            errorCode = 1;
        } else if (a.size() != vectorSize) {
            errorCode = 2;
        } else {
            errorCode = 0;
        }
        for (int proc = 1; proc < size; ++proc)
            MPI_Send(&errorCode, 1, MPI_INT, proc, 9, MPI_COMM_WORLD);
    } else {
        MPI_Status status;
        MPI_Recv(&errorCode, 1, MPI_INT, 0, 9, MPI_COMM_WORLD, &status);
    }
    switch (errorCode) {
    case 0:
        break;
    case 1:
        throw std::runtime_error("Vector sizes do not match");
    case 2:
        throw std::runtime_error("Specified vector size and real sizes do not match");
    }

    if (rank == 0) {
        for (int proc = 1; proc < size; ++proc) {
            if (delta > 0) {
                MPI_Send(&a[rem] + proc * delta, delta, MPI_INT, proc, 1, MPI_COMM_WORLD);
                MPI_Send(&b[rem] + proc * delta, delta, MPI_INT, proc, 2, MPI_COMM_WORLD);
            }
        }
    }

    std::vector <int> c(delta, 0), d(delta, 0);

    if (rank == 0) {
        c = std::vector<int>(a.begin(), a.begin() + rem + delta);
        d = std::vector<int>(b.begin(), b.begin() + rem + delta);
    } else {
        MPI_Status status;
        if (delta > 0) {
            MPI_Recv(&c[0], delta, MPI_INT, 0, 1, MPI_COMM_WORLD, &status);
            MPI_Recv(&d[0], delta, MPI_INT, 0, 2, MPI_COMM_WORLD, &status);
        }
    }

    int64_t ans = 0;
    for (size_t i = 0; i < c.size(); ++i) {
        ans += (int64_t)c[i] * d[i];
    }

    if (rank == 0) {
        for (int proc = 1; proc < size; ++proc) {
            int64_t temp;
            MPI_Status status;
            MPI_Recv(&temp, 1, MPI_INT64_T, MPI_ANY_SOURCE, 3, MPI_COMM_WORLD, &status);
            ans += temp;
        }
    } else {
        MPI_Send(&ans, 1, MPI_INT64_T, 0, 3, MPI_COMM_WORLD);
    }

    return ans;
}
