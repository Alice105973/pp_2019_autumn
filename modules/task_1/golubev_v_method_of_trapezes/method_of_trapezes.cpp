// Copyright 2019 Golubev Vladislav
#include "../../../modules/task_1/golubev_v_method_of_trapezes/method_of_trapezes.h"

double getIntegral(int nIntervals, double(*f)(double), double a, double b) {
  if (nIntervals <= 0)
    throw - 1;

  int procRank, procNum;
  double sum, h, x, result;
  MPI_Comm_size(MPI_COMM_WORLD, &procNum);
  MPI_Comm_rank(MPI_COMM_WORLD, &procRank);
  h = (b - a) / static_cast<double>(nIntervals);
  sum = 0.0;
  for (int i = procRank + 1; i < nIntervals; i += procNum) {
    x = a + h * static_cast<double>(i);
    sum += f(x);
    }
  MPI_Reduce(&sum, &result, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
  result = (h / 2.0) * (f(a) + 2 * result + f(b));
  return result;
}
