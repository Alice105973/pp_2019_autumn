// Copyright 2019 Lembrikov Stepan
#include <gtest-mpi-listener.hpp>
#include <gtest/gtest.h>
#include <vector>
#include "./gor_mat_on_vec.h"

TEST(Mat_On_Vec_MPI, Test_On_Vector_EQ) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector <int> a(9);
    std::vector <int> b(3);
    std::vector <int> res = { 10, -3, -2 };
    std::vector <int> res_vector(3, 0);
    b[0] = 1;
    b[1] = 2;
    b[2] = -1;
    a[0] = 2;
    a[1] = 4;
    a[2] = 0;
    a[3] = -2;
    a[4] = 1;
    a[5] = 3;
    a[6] = -1;
    a[7] = 0;
    a[8] = 1;
    res_vector = MatrixOnVector(a, 3, 3, b);
    if (rank == 0) {
        EXPECT_EQ(res_vector, res);
    }
}

TEST(Mat_On_Vec_MPI, Test_On_Vector_NE) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector <int> a(9);
    std::vector <int> b(3);
    std::vector <int> res = { 10, -3, -1 };
    std::vector <int> res_vector(3, 0);
    b[0] = 1;
    b[1] = 2;
    b[2] = -1;
    a[0] = 2;
    a[1] = 4;
    a[2] = 0;
    a[3] = -2;
    a[4] = 1;
    a[5] = 3;
    a[6] = -1;
    a[7] = 0;
    a[8] = 1;
    res_vector = MatrixOnVector(a, 3, 3, b);
    if (rank == 0) {
        EXPECT_NE(res_vector, res);
    }
}

TEST(Mat_On_Vec_MPI, Test_On_Identity_Vector_EQ) {
    std::vector <int> a = getIdentityVector(21);
    std::vector <int> b = getIdentityVector(3);
    std::vector <int> res = { 3, 3, 3, 3, 3, 3, 3 };
    std::vector <int> res_vector(7, 0);
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    res_vector = MatrixOnVector(a, 3, 7, b);
    if (rank == 0) {
        EXPECT_EQ(res_vector, res);
    }
}

TEST(Mat_On_Vec_MPI, Test_On_Identity_Vector_NE) {
    std::vector <int> a = getIdentityVector(21);
    std::vector <int> b = getIdentityVector(3);
    std::vector <int> res = { 3, 3, 3, 3, 3, 3, 2 };
    std::vector <int> res_vector(7, 0);
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    res_vector = MatrixOnVector(a, 3, 7, b);
    if (rank == 0) {
        EXPECT_NE(res_vector, res);
    }
}

TEST(Mat_On_Vec_MPI, Test_On_Random_Vector_EQ) {
    std::vector <int> a = getRandomVector(120);
    std::vector <int> b = getRandomVector(3);
    std::vector <int> res_vector_multy(40, 0);
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    res_vector_multy = MatrixOnVector(a, 3, 40, b);
    if (rank == 0) {
        std::vector <int> res_vector_root(40, 0);
        for (int i = 0; i < 120; i += 3) {
            for (int j = 0; j < 3; j++)
                res_vector_root[i / 3] += a[i + j] * b[j];
        }
        EXPECT_EQ(res_vector_multy, res_vector_root);
    }
}

TEST(Mat_On_Vec_MPI, Test_On_Random_Vector_NE) {
    std::vector <int> a = getRandomVector(120);
    std::vector <int> b = getRandomVector(3);
    std::vector <int> res_vector_multy(40, 0);
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    res_vector_multy = MatrixOnVector(a, 3, 40, b);
    res_vector_multy[0] += 1;
    if (rank == 0) {
        std::vector <int> res_vector_root(40, 0);
        for (int i = 0; i < 120; i += 3) {
            for (int j = 0; j < 3; j++)
                res_vector_root[i / 3] += a[i + j] * b[j];
        }
        EXPECT_NE(res_vector_multy, res_vector_root);
    }
}

TEST(Mat_On_Vec_MPI, Test_On_Const_Vector_EQ) {
    std::vector <int> a = getConstVector(12, 5);
    std::vector <int> b = getConstVector(3, 5);
    std::vector <int> res = getConstVector(4, 75);
    std::vector <int> res_vector(4, 0);
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    res_vector = MatrixOnVector(a, 3, 4, b);
    if (rank == 0) {
        EXPECT_EQ(res_vector, res);
    }
}

TEST(Mat_On_Vec_MPI, Test_On_Const_Vector_NE) {
    std::vector <int> a = getConstVector(12, 5);
    std::vector <int> b = getConstVector(3, 5);
    std::vector <int> res = getConstVector(4, 75);
    std::vector <int> res_vector(4, 0);
    int rank;
    res[3] = -10;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    res_vector = MatrixOnVector(a, 3, 4, b);
    if (rank == 0) {
        EXPECT_NE(res_vector, res);
    }
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    MPI_Init(&argc, &argv);

    ::testing::AddGlobalTestEnvironment(new GTestMPIListener::MPIEnvironment);
    ::testing::TestEventListeners& listeners =
        ::testing::UnitTest::GetInstance()->listeners();

    listeners.Release(listeners.default_result_printer());
    listeners.Release(listeners.default_xml_generator());

    listeners.Append(new GTestMPIListener::MPIMinimalistPrinter);

    return RUN_ALL_TESTS();
}
