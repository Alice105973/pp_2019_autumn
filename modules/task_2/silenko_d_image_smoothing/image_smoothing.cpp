// Copyright 2019 Silenko Dmitrii

#include <mpi.h>
#include <iostream>
#include <random>
#include <ctime>
#include <numeric>
#include <vector>
#include <stdexcept>
#include "../../../modules/task_2/silenko_d_image_smoothing/image_smoothing.h"

double** getRandomMas(const int n, const int m) {
  if (n <= 0) {
    throw "Wrong rows";
  } else if (m <= 0) {
    throw "wrong columns";
  }
  double** mas = new double*[n];
  std::mt19937 seed;
  for (int i = 0; i < n; i++)
    mas[i] = new double[m];
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < m; j++)
      mas[i][j] = seed() / RAND_MAX;
  }
  return mas;
}

double** ImageSmoothing(double** mas, const int n, const int m) {
  int size, rank;
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Status status;
  int delta;
  int ost;
  int f = 0;
  int extra;
  ost = n % size;

  if (n < size) {
    ost = 0;
    f = -1;
    size = 1;
  }

  delta = n / size;

  if (size == 1) {
    extra = 0;
  } else {
    extra = 1;
  }

  if (rank == 0) {
    double* ssend = new double[n*m];
    int tmp = 0;
    for (int i=0; i < n; i++)
      for (int j = 0; j < m; j++) {
        ssend[tmp] = mas[i][j];
        tmp++;
      }
    for (int i = 1; i < size; i++) {
      if (i == size - 1)
        extra = 0;
      MPI_Send(&ssend[m*(delta + ost - 1)] + (i-1) * delta * m,
        m*(delta + extra + 1), MPI_DOUBLE, i, 1, MPI_COMM_WORLD);
    }
    delete[] ssend;
  }

  double** workmas = NULL;
  double** prom_res = NULL;

  if (rank == 0) {
    if (size > 1)
      extra = 1;
    workmas = new double*[delta + ost + extra];
    for (int i = 0; i < delta + ost + extra; i++)
      workmas[i] = new double[m];
    for (int i = 0; i < delta + ost + extra; i++)
      for (int j = 0; j < m; j++)
        workmas[i][j] = mas[i][j];

    prom_res = new double*[delta + ost];
    for (int i = 0; i < delta + ost; i++)
      prom_res[i] = new double[m];

    for (int i = 0; i < delta + ost; i++)
      for (int j = 0; j < m; j++) {
        int count = 0;
        double middle = 0;
        for (int l = -1; l < 2; l++)
          for (int ll = -1; ll < 2; ll++) {
            int tmp1 = j + l;
            if (tmp1 < 0)
              continue;
            if (tmp1 >= m)
              continue;
            int tmp2 = i + ll;
            if (tmp2 < 0)
              continue;
            if (tmp2 >= (delta + ost + extra))
              continue;
            if ((tmp1 == j) && (tmp2 == i))
              continue;
            middle += workmas[tmp2][tmp1];
            count++;
          }
        prom_res[i][j] = middle / count;
      }
    delete[] workmas;
  } else {
    if (f == 0) {
      double* b = new double[m*(delta + 2)];
      if (rank == size - 1)
        extra = 1;
      else
        extra = 2;
      MPI_Recv(&b[0], m*(delta + extra), MPI_DOUBLE, 0, 1, MPI_COMM_WORLD, &status);
      workmas = new double*[delta + extra];
      for (int i = 0; i < delta + extra; i++)
        workmas[i] = new double[m];
      int tmp = 0;
      for (int i = 0; i < delta + extra; i++)
        for (int j = 0; j < m; j++) {
          workmas[i][j] = b[tmp];
          tmp++;
        }
      delete[] b;
      prom_res = new double*[delta];
      for (int i = 0; i < delta; i++)
        prom_res[i] = new double[m];
      for (int i = 1; i <= delta; i++)
        for (int j = 0; j < m; j++) {
          int count = 0;
          double middle = 0;
          for (int l = -1; l < 2; l++)
            for (int ll = -1; ll < 2; ll++) {
              int tmp1 = j + l;
              if (tmp1 < 0)
                continue;
              if (tmp1 >= m)
                continue;
              int tmp2 = i + ll;
              if (tmp2 < 0)
                continue;
              if (tmp2 > (delta + extra - 1))
                continue;
              if ((tmp1 == j) && (tmp2 == i))
                continue;
              middle += workmas[tmp2][tmp1];
              count++;
            }
          prom_res[i-1][j] = middle / count;
        }
      delete[] workmas;
    }
  }

  if (f == 0) {
    double** res = new double*[n];
    for (int i = 0; i < n; i++)
      res[i] = new double[m];
    if (rank == 0) {
      for (int i = 0; i < delta + ost; i++)
        for (int j = 0; j < m; j++)
          res[i][j] = prom_res[i][j];
      if (size >= 2) {
        double* b = new double[m*delta];
        for (int i = 1; i < size; i++) {
          MPI_Recv(&b[0], m*delta, MPI_DOUBLE, i, 9, MPI_COMM_WORLD, &status);
          int tmp = 0;
          for (int k = delta * i + ost; k < delta*i + ost + delta; k++)
            for (int j = 0; j < m; j++) {
              res[k][j] = b[tmp];
              tmp++;
            }
        }
        delete[] b;
      }
    } else {
      double* send = new double[delta*m];
      int tmp = 0;
      for (int i = 0; i < delta; i++)
        for (int j = 0; j < m; j++) {
          send[tmp] = prom_res[i][j];
          tmp++;
        }
      MPI_Send(&send[0], m*delta, MPI_DOUBLE, 0, 9, MPI_COMM_WORLD);
      delete[] send;
    }
    delete[] prom_res;
    return res;
  } else {
    if (rank == 0) {
      return prom_res;
    } else {
      return NULL;
    }
  }
}
