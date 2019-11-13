// Copyright 2019 Savkin Yuriy
#include <mpi.h>
#include <climits>
#include <cstdlib>
#include <ctime>
#include <random>
#include <utility>
#include <vector>
#include "../../../modules/task_1/savkin_y_close_values_on_vector/close_values_on_vector.h"

std::vector<int> getRandomVector(int size) {
    std::mt19937 gen(static_cast<unsigned int>(time(0)));
    std::uniform_int_distribution<int> uid(-10000, 10000);
    std::vector<int> v(size);
    for (int i = 0; i < size; ++i)
        v[i] = uid(gen);
    return v;
}

int getStartPos(int size, int comm_size, int rank) {
    int buf_size = size / comm_size;
    int res = size % comm_size;
    if (rank < res)
        return rank * (buf_size + 1);
    else
        return res * (buf_size + 1) + (rank - res) * buf_size;
}

std::vector<int> parseArray(int size, int comm_size) {
    std::vector<int> v(comm_size, 0);
    if (size < 2)
        return v;
    int buf_size = size / comm_size;
    int res = size % comm_size;
    for (int i = 0; i < res; ++i)
        v[i] = buf_size + 2;
    for (int i = res; i < comm_size - 1; ++i)
        v[i] = buf_size + 1;
    v.back() = buf_size;
    return v;
}

std::pair<int, int> getClosestVectorNearbyPositions(
    const std::vector<int>& v, int count) {
    int comm_size, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &comm_size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Status status;

    std::vector<int> buf_sz = parseArray(count, comm_size);
    int start = getStartPos(count, comm_size, rank);
    int pos = -1, diff = INT_MAX;

    if (rank == 0) {
        const int* p = v.data() + buf_sz[0] - 1;
        for (int i = 1; i < comm_size; ++i) {
            MPI_Send(p, buf_sz[i], MPI_INT, i, 0, MPI_COMM_WORLD);
            p += buf_sz[i] - 1;
        }

        for (int i = 1; i < buf_sz[0]; ++i)
            if (abs(v[i] - v[i - 1]) < diff) {
                diff = abs(v[i] - v[i - 1]);
                pos = i;
            }

        int q[2];
        for (int i = 1; i < comm_size; ++i) {
            MPI_Recv(q, 2, MPI_INT, i, 0, MPI_COMM_WORLD, &status);
            if (q[0] < diff) {
                diff = q[0];
                pos = q[1];
            }
        }

    } else {
        std::vector<int> w(buf_sz[rank]);
        MPI_Recv(w.data(), buf_sz[rank], MPI_INT, 0, 0,
            MPI_COMM_WORLD, &status);

        for (int i = 1; i < buf_sz[rank]; ++i)
            if (abs(w[i] - w[i - 1]) < diff) {
                diff = abs(w[i] - w[i - 1]);
                pos = i;
            }

        int q[2] = {diff, pos + start};
        MPI_Send(q, 2, MPI_INT, 0, 0, MPI_COMM_WORLD);
    }

    return std::make_pair(pos - 1, pos);
}

std::pair<int, int> getClosestVectorNearbyPositionsOneProc(
    const std::vector<int>& v, int count) {
    int pos = -1, diff = INT_MAX;
    for (int i = 1; i < count; ++i)
        if (abs(v[i] - v[i - 1]) < diff) {
            diff = abs(v[i] - v[i - 1]);
            pos = i;
        }
    return std::make_pair(pos - 1, pos);
}
