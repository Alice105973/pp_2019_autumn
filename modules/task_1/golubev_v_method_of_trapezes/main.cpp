// Copyright 2019 Golubev Vladislav
#include <gtest-mpi-listener.hpp>
#include <gtest/gtest.h>
#include "./method_of_trapezes.h"

double polynom(double x) {
  return 38 * x * x * x - 13 * x * x + 7;
}

TEST(method_of_trapezes, throws_when_n_intervals_is_negative) {
  ASSERT_ANY_THROW(getIntegral(0, sin, 0.0, 3.0));
}

TEST(method_of_trapezes, integral_of_trigonometric_function) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  double res = getIntegral(100000, sin, 0.0, 3.0);
  if (rank == 0) {
    ASSERT_NEAR(res, 1.98999249660045, 1e-5);
  }
}

TEST(method_of_trapezes, integral_of_polinom) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  double res = getIntegral(1000000, polynom, -1.0, 17.0);
  if (rank == 0) {
    ASSERT_NEAR(res, 772272.0, 1e-5);
  }
}

TEST(method_of_trapezes, integral_of_zero_interval_is_zero) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  double res = getIntegral(100000, polynom, 1.0, 1.0);
  if (rank == 0) {
    ASSERT_NEAR(res, 0.0, 1e-5);
  }
}

TEST(method_of_trapezes, integral_of_sin_on_the_symmetric_interval) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  double res = getIntegral(1000000, sin, -3527.0, 3527.0);
  if (rank == 0) {
    ASSERT_NEAR(res, 0.0, 1e-5);
  }
}

TEST(method_of_trapezes, integral_is_negative) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  double res = getIntegral(100000000, polynom, 5.0, 1.0);
  if (rank == 0) {
    ASSERT_NEAR(res, -5418.66666666667, 1e-5);
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
