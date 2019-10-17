// Copyright 2019 Maximova Irina
#include <gtest/gtest.h>
#include <gtest-mpi-listener.hpp>
#include <vector>
#include "./sum_val_in_rows_mtx.h"

TEST(Sum_val_in_rows_mtx_MPI, Test_on_Matrix_2x2) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  std::vector<int> arr;
  const size_t rows = 2, cols = 2;

  if (rank == 0) {
    arr = getRandomMatrix(rows, cols);
  }

  std::vector<int64_t> answer(rows, 0);
  answer = getSumValinRows(arr, rows, cols);

  if (rank == 0) {
    std::vector<int64_t> seqAnswer(rows, 0);
    for (size_t i = 0; i < rows; ++i)
      for (size_t j = cols * i; j < cols * (i + 1); ++j)
        seqAnswer[i] += (int64_t)arr[j];

    ASSERT_EQ(seqAnswer, answer);
  }
}

TEST(Sum_val_in_rows_mtx_MPI, Test_On_Matrix_30x30) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  std::vector<int> arr;
  const size_t rows = 30, cols = 30;

  if (rank == 0) {
    arr = getRandomMatrix(rows, cols);
  }

  std::vector<int64_t> answer(rows, 0);
  answer = getSumValinRows(arr, rows, cols);

  if (rank == 0) {
    std::vector<int64_t> seqAnswer(rows, 0);
    for (size_t i = 0; i < rows; ++i)
      for (size_t j = cols * i; j < cols * (i + 1); ++j) {
        seqAnswer[i] += (int64_t)arr[j];
      }
    ASSERT_EQ(seqAnswer, answer);
  }
}

TEST(Sum_val_in_rows_mtx_MPI, Test_On_Matrix_10x15) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  std::vector<int> arr;
  const size_t rows = 10, cols = 15;

  if (rank == 0) {
    arr = getRandomMatrix(rows, cols);
  }

  std::vector<int64_t> answer(rows, 0);
  answer = getSumValinRows(arr, rows, cols);

  if (rank == 0) {
    std::vector<int64_t> seqAnswer(rows, 0);
    for (size_t i = 0; i < rows; ++i)
      for (size_t j = cols * i; j < cols * (i + 1); ++j) {
        seqAnswer[i] += (int64_t)arr[j];
      }
    ASSERT_EQ(seqAnswer, answer);
  }
}

TEST(Sum_val_in_rows_mtx_MPI, Check_zero_number_of_vectors) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  std::vector<int> arr;
  const size_t rows = 0, cols = 15;

  if (rank == 0) {
    arr = getRandomMatrix(rows, cols);
  }

  ASSERT_ANY_THROW(getSumValinRows(arr, rows, cols));
}

TEST(Sum_val_in_rows_mtx_MPI, Check_the_zero_dimension_of_vectors) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  std::vector<int> arr;
  const size_t rows = 10, cols = 0;

  if (rank == 0) {
    arr = getRandomMatrix(rows, cols);
  }

  ASSERT_ANY_THROW(getSumValinRows(arr, rows, cols));
}

TEST(Sum_val_in_rows_mtx_MPI, Check_Specified_Vector_Length) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  std::vector<int> arr;
  const size_t rows = 10, cols = 5;

  if (rank == 0) {
    arr = getRandomMatrix(rows, cols);
  }

  ASSERT_ANY_THROW(getSumValinRows(arr, rows, cols - 1));
}

TEST(Sum_val_in_rows_mtx_MPI, Check_the_given_number_of_vectors) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  std::vector<int> arr;
  const size_t rows = 10, cols = 5;

  if (rank == 0) {
    arr = getRandomMatrix(rows, cols);
  }

  ASSERT_ANY_THROW(getSumValinRows(arr, rows - 1, cols));
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
