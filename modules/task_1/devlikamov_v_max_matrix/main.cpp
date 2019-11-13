// Copyright 2019 Devlikamov Vladislav

#include <gtest-mpi-listener.hpp>
#include <gtest/gtest.h>
#include <vector>
#include "../../../modules/task_1/devlikamov_v_max_matrix/max_matrix.h"

TEST(Matrix_max_MPI, test1_on_matrix) {
    std::vector <std::vector<int>> a(2, std::vector <int> (2));
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    a[0][0] = 2;
    a[0][1] = 1;
    a[1][0] = 1;
    a[1][1] = -100;
    int maxi = MaxOfMatrix(a, 2, 2);
    if (rank == 0) {
        EXPECT_EQ(2, maxi);
    }
}

TEST(Matrix_max_MPI, test2_on_matrix) {
    std::vector <std::vector<int>> a(3, std::vector <int> (3));
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    a[0][0] = 2;
    a[0][1] = 1;
    a[1][0] = 1;
    a[1][1] = -100;
    int maxi = MaxOfMatrix(a, 3, 3);
    if (rank == 0) {
        EXPECT_EQ(2, maxi);
    }
}

TEST(Matrix_max_MPI, test3_on_matrix) {
    std::vector <std::vector<int>> a(3, std::vector <int> (3));
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    a[0][0] = 2;
    a[0][1] = 1;
    a[0][2] = 2;
    a[1][0] = 1;
    a[1][2] = 100;
    a[1][1] = -100;
    a[2][0] = -2;
    a[2][1] = -2;
    int maxi = MaxOfMatrix(a, 3, 3);
    if (rank == 0) {
        EXPECT_EQ(100, maxi);
    }
}

TEST(Matrix_max_MPI, test_on_random_matrix1) {
    std::vector <std::vector<int>> a = getRandomMatrixT(100, 100);
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int maxi = MaxOfMatrix(a, 100, 100);
    if (rank == 0) {
        EXPECT_EQ(1, maxi);
    }
}

TEST(Matrix_max_MPI, test_on_random_matrix2) {
    std::vector <std::vector<int>> a = getRandomMatrixT(100, 100);
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int maxi = MaxOfMatrix(a, 100, 100);
    if (rank == 0) {
        EXPECT_EQ(1, maxi);
    }
}

TEST(Matrix_max_MPI, test_on_random_matrix3) {
    std::vector <std::vector<int>> a = getRandomMatrixT(500, 500);
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int maxi = MaxOfMatrix(a, 500, 500);
    if (rank == 0) {
        EXPECT_EQ(1, maxi);
    }
}

TEST(Matrix_max_MPI, test_on_random_matrix4) {
    std::vector <std::vector<int>> a = getRandomMatrixT(100, 1);
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int maxi = MaxOfMatrix(a, 100, 1);
    if (rank == 0) {
        EXPECT_EQ(1, maxi);
    }
}

TEST(Matrix_max_MPI, test_on_matrix_of_units_1) {
    std::vector <std::vector<int>> a = getRandomMatrix(100, 100);
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int maxi = MaxOfMatrix(a, 100, 100);
    if (rank == 0) {
        EXPECT_EQ(1, maxi);
    }
}

TEST(Matrix_max_MPI, test_on_matrix_of_units_2) {
    std::vector <std::vector<int>> a = getRandomMatrix(50, 50);
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int maxi = MaxOfMatrix(a, 50, 50);
    if (rank == 0) {
        EXPECT_EQ(1, maxi);
    }
}

TEST(Matrix_max_MPI, test_on_Identity_matrix_1) {
    std::vector <std::vector<int>> a = getRandomMatrixU(100, 100);
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int maxi = MaxOfMatrix(a, 100, 100);
    if (rank == 0) {
        EXPECT_EQ(1, maxi);
    }
}

TEST(Matrix_max_MPI, test_on_Identity_matrix_2) {
    std::vector <std::vector<int>> a = getRandomMatrixU(50, 60);
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int maxi = MaxOfMatrix(a, 50, 60);
    if (rank == 0) {
        EXPECT_EQ(1, maxi);
    }
}

TEST(Matrix_max_MPI, test_on_zero_matrix) {
    std::vector <std::vector<int>> a(101, std::vector <int> (101));
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int maxi = MaxOfMatrix(a, 101, 101);
    if (rank == 0) {
        EXPECT_EQ(0, maxi);
    }
}

TEST(Matrix_max_MPI, test_on_max_matrix1) {
    std::vector <std::vector<int>> a = getRandomMatrixM(50, 60);
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int maxi = MaxOfMatrix(a, 50, 60);
    if (rank == 0) {
        EXPECT_EQ(108, maxi);
    }
}

TEST(Matrix_max_MPI, test_on_max_matrix2) {
    std::vector <std::vector<int>> a = getRandomMatrixM(5, 60);
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int maxi = MaxOfMatrix(a, 5, 60);
    if (rank == 0) {
        EXPECT_EQ(63, maxi);
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
