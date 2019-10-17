// Copyright 2019 Maximova Irina
#include <mpi.h>
#include <random>
#include <ctime>
#include <stdexcept>
#include <vector>
#include "../../../modules/task_1/maximova_i_sum_val_in_rows_mtx/sum_val_in_rows_mtx.h"

static int offset = 0;

std::vector<int> getRandomMatrix(const int rows, const int cols) {
  std::mt19937 gen;
  gen.seed((unsigned)time(0) + ++offset);

  std::vector<int> matrix(rows * cols);
  for (int i = 0; i < rows * cols; ++i) {
    matrix[i] = gen() % 10;
  }

  return matrix;
}

std::vector<int64_t> getSumValinRows(const std::vector<int>& a,
  const size_t rows, const size_t cols) {
  int size, rank;
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  const int delta = rows / size;
  const int rem = rows % size;

  int errorCode;

  if (rank == 0) {
    if (a.size() != rows * cols) {
      errorCode = 3;
    } else if (rows == 0) {
      errorCode = 2;
    } else if (cols == 0) {
      errorCode = 1;
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
    throw std::runtime_error("The size of the vectors is zero.");
  case 2:
    throw std::runtime_error("The number of vectors is zero.");
  case 3:
    throw std::runtime_error("Matrix size does not match description.");
  }

  if (rank == 0 && delta > 0) {
    for (int proc = 1; proc < size; ++proc) {
      MPI_Send(&a[rem * cols] + proc * delta * cols, delta * cols, MPI_INT,
        proc, 1, MPI_COMM_WORLD);
    }
  }

  std::vector<int> recv(delta * cols, 0);

  if (rank == 0) {
    recv = std::vector<int>(a.begin(), a.begin() + rem * cols + delta * cols);
  } else {
    MPI_Status status;
    if (delta > 0) {
      MPI_Recv(&recv[0], delta * cols, MPI_INT, 0, 1, MPI_COMM_WORLD, &status);
    }
  }

  size_t vec = recv.size() / cols;
  std::vector<int64_t> sum(vec, 0);
  for (size_t i = 0; i < vec; ++i)
    for (size_t j = cols * i; j < cols * (i + 1); ++j)
      sum[i] += (int64_t)recv[j];

  std::vector<int64_t> rez(rows, 0);

  if (rank == 0) {
    for (size_t i = 0; i < vec; ++i) {
      rez[i] = sum[i];
    }

    if (delta > 0) {
      for (int proc = 1; proc < size; ++proc) {
        MPI_Status status;
        MPI_Recv(&rez[vec] + delta * (proc - 1), delta, MPI_INT64_T, proc, 3,
          MPI_COMM_WORLD, &status);
      }
    }
  } else {
    if (delta > 0) MPI_Send(&sum[0], delta, MPI_INT64_T, 0, 3, MPI_COMM_WORLD);
  }

  return rez;
  }
