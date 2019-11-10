// Copyright 2019 Kukushkina Ksenia
#include "../../../modules/task_2/kukushkina_k_simple_iterations/simple_iterations.h"
#include <mpi.h>
#include <ctime>
#include <random>

static int offset = 0;

double preprocess(std::vector<double>& A, std::vector<double>& b)  // ���������� ����� ��������������� ������� A
{
  if (A.size() != b.size() * b.size())
    throw "non-equal dimensions";
  int rank, size, start, end;
  MPI_Status stat;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  int n = static_cast<int>(b.size());
  double norm = 0;

  if (rank == 0)  // ����� ������ ������� ����� ����������
  {
    int len = n / size;
    int rem = n % size;
    for (int i = 1; i < size; i++)
    {
      start = len * i + rem;
      MPI_Send(&start, 1, MPI_INT, i, 5, MPI_COMM_WORLD);
      end = start + len;
      MPI_Send(&end, 1, MPI_INT, i, 6, MPI_COMM_WORLD);
    }
    start = 0;
    end = len + rem;
  }
  else {
    MPI_Recv(&start, 1, MPI_INT, 0, 5, MPI_COMM_WORLD, &stat);
    MPI_Recv(&end, 1, MPI_INT, 0, 6, MPI_COMM_WORLD, &stat);
  }
  for (start; start < end; start++)
  {
    if (A[start * n + start] == 0)
      throw "Zero element";
    b[start] /= A[start * n + start];  // ����� �� ������� ������� ���������
    for (int i = start * n; i < (start + 1) * n; i++)
      if (i == start * (n + 1))
        A[i] = 0;  // ������� ������� ��������� ��������
      else
      {
        A[i] /= A[start * n + start];  // ����� �� ������� ������� ���������
        A[i] = - A[i];
        if (abs(A[i]) > norm)
          norm = abs(A[i]);
        if (A[i] >= 1 || A[i] <= -1)  // ��������� ������������� ������������
          throw "No convergence";  // ����� �� �������� 
      }
  }
  MPI_Barrier(MPI_COMM_WORLD);
  MPI_Allreduce(&norm, &norm, 1, MPI_DOUBLE, MPI_MAX, MPI_COMM_WORLD);
  return norm;
}

std::vector<double> GenerateVector(int n)
{
  if (n <= 0)
    throw "Wrong size";
  std::vector<double> A(n);
  std::mt19937 gen;
  int rank, size, start, end;
  MPI_Status stat;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);

  if (rank == 0)  // ����� �������� ����� ����������
  {
    int len = n / size;
    int rem = n % size;
    for (int i = 1; i < size; i++)
    {
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
  for (start; start < end; start++)
  {
    gen.seed(static_cast<int>(time(0)) + ++offset);
    A[start] = gen() % 100;
  }
  MPI_Barrier(MPI_COMM_WORLD);
  return A;
}

std::vector<double> GenerateMatrix(int n)
{
  if (n <= 0)
    throw "Wrong size";
  return GenerateVector(n * n);
}

std::vector<double> SimpleIterations(std::vector<double>& A, std::vector<double>& b, double precision)
{
  if (A.size() != b.size() * b.size())
    throw "non-equal dimensions";
  int n = static_cast<int>(b.size());
  double norm = preprocess(A, b);

  if (norm >= 1)
    throw "No convergence";  // ����� �� ��������

  int rank, size, start, end;
  MPI_Status stat;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  std::vector<double> xnew(n), xold(n);
  xold = b;
  int iteration = 0;
  int maxiteration = static_cast<int>(floor(log(1 / precision) / log(1 / norm)));
  double vecnorm = 0;  // ����� ������� xold � xnew

  if (rank == 0)  // ����� ������ ������� ����� ����������
  {
    int len = n / size;
    int rem = n % size;
    for (int i = 1; i < size; i++)
    {
      start = len * i + rem;
      MPI_Send(&start, 1, MPI_INT, i, 7, MPI_COMM_WORLD);
      end = start + len;
      MPI_Send(&end, 1, MPI_INT, i, 8, MPI_COMM_WORLD);
    }
    start = 0;
    end = len + rem;
  }
  else {
    MPI_Recv(&start, 1, MPI_INT, 0, 7, MPI_COMM_WORLD, &stat);
    MPI_Recv(&end, 1, MPI_INT, 0, 8, MPI_COMM_WORLD, &stat);
  }

  do
  {
    for (int i = start; i < end; i++)
      xold[i] = xnew[i];  // ���������� �������� xold �� ��������
    MPI_Barrier(MPI_COMM_WORLD);
    for (int i = start; i < end; i++)  // ����� �� �������
    {
      xnew[i] = b[i];
      for (int j = 0; j < n; j++)  // ����� �� ��������
        xnew[i] += A[n * i + j] * xold[j];  // ��������� i-��� ������ �� j-��� �������
      if (abs(xnew[i] - xold[i]) > vecnorm)
        vecnorm = abs(xnew[i] - xold[i]);
    }
    MPI_Barrier(MPI_COMM_WORLD);
    MPI_Allreduce(&vecnorm, &vecnorm, 1, MPI_DOUBLE, MPI_MAX, MPI_COMM_WORLD);
    iteration++;
  } while (vecnorm > precision && iteration < maxiteration);
  return xnew;
}