// Copyright 2019 Boganov Sergey
#include <gtest-mpi-listener.hpp>
#include <gtest/gtest.h>
#include "./trapezoid_integration.h"

double f1(double x) {
    return x * x;
}

double f2(double x) {
    return x * x * x;
}

TEST(getSequentialTrapezoidIntegral, Test_Sequential_Trapezoid_Integral_1) {
    double a = 0.0, b = 3.0;
    int N = 10000;
    double I, answer = 9.0;

    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    I = getSequentialTrapezoidIntegral(f1, a, b, N);

    if (rank == 0) {
        ASSERT_NEAR(answer, I, 1e-2);
    }
}

TEST(getSequentialTrapezoidIntegral, Test_Sequential_Trapezoid_Integral_2) {
    double a = 0.0, b = 2.0;
    int N = 10000;
    double I, answer = 4.0;

    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    I = getSequentialTrapezoidIntegral(f2, a, b, N);

    if (rank == 0) {
        ASSERT_NEAR(answer, I, 1e-2);
    }
}

TEST(getSequentialTrapezoidIntegral, Test_Sequential_Trapezoid_Integral_3) {
    double a = -2.0, b = 2.0;
    int N = 10000;
    double I, answer = 0.0;

    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    I = getSequentialTrapezoidIntegral(f2, a, b, N);

    if (rank == 0) {
        ASSERT_NEAR(answer, I, 1e-2);
    }
}

TEST(getParallelTrapezoidIntegral, Test_Parallel_Trapezoid_Integral_1) {
    double a = -2.0, b = 1.0;
    int N = 10000;
    double I, answer = 3.0;

    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    I = getParallelTrapezoidIntegral(f1, a, b, N);

    if (rank == 0) {
        ASSERT_NEAR(answer, I, 1e-2);
    }
}

TEST(getParallelTrapezoidIntegral, Test_Parallel_Trapezoid_Integral_2) {
    double a = -1.0, b = 1.0;
    int N = 10000;
    double I, answer = 0.0;

    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    I = getParallelTrapezoidIntegral(f2, a, b, N);

    if (rank == 0) {
        ASSERT_NEAR(answer, I, 1e-2);
    }
}

TEST(getParallelTrapezoidIntegral, Test_Parallel_Trapezoid_Integral_3) {
    double a = 0.0, b = 2.0;
    int N = 10000;
    double I, answer = 4.0;

    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    I = getParallelTrapezoidIntegral(f2, a, b, N);

    if (rank == 0) {
        ASSERT_NEAR(answer, I, 1e-2);
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
