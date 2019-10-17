// Copyright 2019 Savkin Yuriy
#include <gtest-mpi-listener.hpp>
#include <gtest/gtest.h>
#include <vector>
#include <utility>
#include "./close_values_on_vector.h"

TEST(Close_Values_On_Vector, even_size) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<int> v;

    if (rank == 0)
        v = getRandomVector(40);

    std::pair<int, int> p = getClosestVectorNearbyPositions(v, 40);

    if (rank == 0) {
        std::pair<int, int> q = getClosestVectorNearbyPositionsOneProc(v, 40);
        ASSERT_EQ(p, q);
    }
}

TEST(Close_Values_On_Vector, odd_size) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<int> v;

    if (rank == 0)
        v = getRandomVector(39);

    std::pair<int, int> p = getClosestVectorNearbyPositions(v, 39);

    if (rank == 0) {
        std::pair<int, int> q = getClosestVectorNearbyPositionsOneProc(v, 39);
        ASSERT_EQ(p, q);
    }
}

TEST(Close_Values_On_Vector, small_size) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<int> v;

    if (rank == 0)
        v = getRandomVector(3);

    std::pair<int, int> p = getClosestVectorNearbyPositions(v, 3);

    if (rank == 0) {
        std::pair<int, int> q = getClosestVectorNearbyPositionsOneProc(v, 3);
        ASSERT_EQ(p, q);
    }
}

TEST(Close_Values_On_Vector, very_small_size) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<int> v;

    if (rank == 0)
        v = getRandomVector(1);

    std::pair<int, int> p = getClosestVectorNearbyPositions(v, 1);

    if (rank == 0) {
        std::pair<int, int> q = getClosestVectorNearbyPositionsOneProc(v, 1);
        ASSERT_EQ(p, q);
    }
}

TEST(Close_Values_On_Vector, two_functions_work) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<int> v;

    if (rank == 0)
        v = getRandomVector(40);

    std::pair<int, int> p = getClosestVectorNearbyPositions(v, 40);
    std::pair<int, int> q = getClosestVectorNearbyPositions(v, 40);

    if (rank == 0) {
        ASSERT_EQ(p, q);
    }
}

TEST(Close_Values_On_Vector, zero_size) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<int> v;

    if (rank == 0)
        v = getRandomVector(0);

    std::pair<int, int> p = getClosestVectorNearbyPositions(v, 0);

    if (rank == 0) {
        std::pair<int, int> q = getClosestVectorNearbyPositionsOneProc(v, 0);
        ASSERT_EQ(p, q);
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
