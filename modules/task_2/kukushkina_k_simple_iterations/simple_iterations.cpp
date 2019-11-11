// Copyright 2019 Kukushkina Ksenia
#include "../../../modules/task_2/kukushkina_k_simple_iterations/simple_iterations.h"
#include <mpi.h>
#include <vector>
#include <ctime>
#include <random>
#include <iostream>

double preprocess(std::vector<double> A, std::vector<double> b) {  // returns modified A norm
  if (A.size() != b.size() * b.size())
    throw "non-equal dimensions";
  int rank, size, start, end;
  MPI_Status stat;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  int n = static_cast<int>(b.size());
  double norm = 0;

  if (rank == 0) {  // strings are divided by processes
    int len = n / size;
    int rem = n % size;
    for (int i = 1; i < size; i++) {
      start = len * i + rem;
      MPI_Send(&start, 1, MPI_INT, i, 5, MPI_COMM_WORLD);
      end = start + len;
      MPI_Send(&end, 1, MPI_INT, i, 6, MPI_COMM_WORLD);
    }
    start = 0;
    end = len + rem;
  } else {
    MPI_Recv(&start, 1, MPI_INT, 0, 5, MPI_COMM_WORLD, &stat);
    MPI_Recv(&end, 1, MPI_INT, 0, 6, MPI_COMM_WORLD, &stat);
  }
  for (int j = start; j < end; j++) {
    if (A[j * n + j] == 0)
      throw "Zero element";
    b[j] /= A[j * n + j];  // division by main diag element
    for (int i = j * n; i < (j + 1) * n; i++)
      if (i == j * (n + 1)) {
        A[i] = 0;  // main diag element
      } else {
        A[i] /= A[j * n + j];
        A[i] = - A[i];
        if (std::abs(A[i]) > norm)
          norm = std::abs(A[i]);
        if (A[i] >= 1 || A[i] <= -1)  // no diag prevalence
          throw "No convergence";
      }
  }
  MPI_Barrier(MPI_COMM_WORLD);
  MPI_Allreduce(&norm, &norm, 1, MPI_DOUBLE, MPI_MAX, MPI_COMM_WORLD);
  return norm;
}

std::vector<double> GenerateVector(int n) {
  if (n <= 0)
    throw "Wrong size";
  std::vector<double> A(n);
  std::cout << A.max_size();
  if (n > static_cast<int>(A.max_size()))
    throw "Overflow";
  std::mt19937 gen(static_cast<int>(time(0)));
  int rank, size, start, end;
  MPI_Status stat;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);

  if (rank == 0) {  // elements are divided by processes
    int len = n / size;
    int rem = n % size;
    for (int i = 1; i < size; i++) {
      start = len * i + rem;
      MPI_Send(&start, 1, MPI_INT, i, 3, MPI_COMM_WORLD);
      end = start + len;
      MPI_Send(&end, 1, MPI_INT, i, 4, MPI_COMM_WORLD);
    }
    start = 0;
    end = len + rem;
  } else {
    MPI_Recv(&start, 1, MPI_INT, 0, 3, MPI_COMM_WORLD, &stat);
    MPI_Recv(&end, 1, MPI_INT, 0, 4, MPI_COMM_WORLD, &stat);
  }
  for (int i = start; i < end; i++) {
    A[i] = gen() % 100;
  }
  MPI_Barrier(MPI_COMM_WORLD);
  return A;
}

std::vector<double> GenerateMatrix(int n) {
  if (n <= 0)
    throw "Wrong size";
  return GenerateVector(n * n);
}

std::vector<double> SimpleIterations(std::vector<double> A, std::vector<double> b, double precision) {
  if (A.size() != b.size() * b.size())
    throw "non-equal dimensions";
  int n = static_cast<int>(b.size());
  double norm = preprocess(A, b);

  if (norm >= 1)
    throw "No convergence";

  int rank, size, start, end;
  MPI_Status stat;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  std::vector<double> xnew(n), xold(n);
  xold = b;
  int iteration = 0;
  int maxiteration = static_cast<int>(floor(log(1 / precision) / log(1 / norm)));
  double vecnorm = 0;  // norm of xold - xnew

  if (rank == 0) {  // strings are divided by processes
    int len = n / size;
    int rem = n % size;
    for (int i = 1; i < size; i++) {
      start = len * i + rem;
      MPI_Send(&start, 1, MPI_INT, i, 7, MPI_COMM_WORLD);
      end = start + len;
      MPI_Send(&end, 1, MPI_INT, i, 8, MPI_COMM_WORLD);
    }
    start = 0;
    end = len + rem;
  } else {
    MPI_Recv(&start, 1, MPI_INT, 0, 7, MPI_COMM_WORLD, &stat);
    MPI_Recv(&end, 1, MPI_INT, 0, 8, MPI_COMM_WORLD, &stat);
  }
  do {
    for (int i = start; i < end; i++)
      xold[i] = xnew[i];  // xold refreshing
    MPI_Barrier(MPI_COMM_WORLD);
    for (int i = start; i < end; i++) {  // strings
      xnew[i] = b[i];
      for (int j = 0; j < n; j++)  // columns
        xnew[i] += A[n * i + j] * xold[j];  // i-str * j-col
      if (std::abs(xnew[i] - xold[i]) > vecnorm)
        vecnorm = std::abs(xnew[i] - xold[i]);
    }
    MPI_Barrier(MPI_COMM_WORLD);
    MPI_Allreduce(&vecnorm, &vecnorm, 1, MPI_DOUBLE, MPI_MAX, MPI_COMM_WORLD);
    iteration++;
  } while (vecnorm > precision && iteration < maxiteration);
  return xnew;
}
