// Copyright 2019 Lembrikov Stepan
#include <../../../modules/task_2/lembrikov_s_gor_mat_on_vec/gor_mat_on_vec.h>
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
    int k = 1;
    for (int i = 0; i < n; i++) {
        k = -k;
        a[i] = engine() % 100;
        a[i] = a[i] * k;
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

std::vector <int> MatrixOnVector(const std::vector <int> &matr, int cols,
                                 int rows, const std::vector <int> &vect) {
    int size;
    int rank;
    int ost;
    int k;
    int flag = 0;
    int per = vect.size();
    if ((per != cols) || (cols < 0) || (rows < 0))
        throw "wrong size of matrix a or vector b";
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    ost = rows % size;
    if (rows < size) {
        size = 1;
        ost = 0;
        flag = -1;
    }
    k = rows / size;

    if (rank == 0) {
        int buf1;
        for (int i = cols * (k + ost); i <= cols * (rows - k); i += cols * k) {
            buf1 = (i / cols - ost) / k;
            MPI_Send(&matr[i], cols * k, MPI_INT, buf1, 0, MPI_COMM_WORLD);
        }
    }

    MPI_Status status;
    std::vector <int> res_vec_null_proc(k + ost, 0);
    std::vector <int> res_vec_other_proc(k, 0);
    std::vector <int> res_vec(rows, 0);
    std::vector <int> part_matr(cols * k, 0);

    if (rank == 0) {
        for (int i = 0; i < cols * (k + ost); i += cols) {
            for (int j = 0; j < cols; j++)
                res_vec_null_proc[i / cols] += matr[i + j] * vect[j];
        }
    } else {
        if (flag == 0) {
            int q = cols * k;
            MPI_Recv(&part_matr[0], q, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
            for (int i = 0; i < cols * k; i += cols) {
                for (int j = 0; j < cols; j++)
                    res_vec_other_proc[i / cols] += part_matr[i + j] * vect[j];
            }
        }
    }

    if (flag == 0) {
        MPI_Gather(&res_vec_other_proc[0], k, MPI_INT,
            &res_vec[ost], k, MPI_INT, 0, MPI_COMM_WORLD);
        if (rank == 0) {
            for (int i = 0; i < k + ost; i++)
                res_vec[i] = res_vec_null_proc[i];
        }
        if (rank == 0) {
            for (int i = 0; i < k + ost; i++)
                res_vec[i] = res_vec_null_proc[i];
        }
        return res_vec;
    } else {
        if (rank == 0) {
            return res_vec_null_proc;
        } else {
            std::vector <int> excep_vec = { -1 };
            return excep_vec;
        }
    }
}
