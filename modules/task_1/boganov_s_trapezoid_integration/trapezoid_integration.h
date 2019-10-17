// Copyright 2019 Boganov Sergey
#ifndef MODULES_TASK_1_BOGANOV_S_TRAPEZOID_INTEGRATION_TRAPEZOID_INTEGRATION_H_
#define MODULES_TASK_1_BOGANOV_S_TRAPEZOID_INTEGRATION_TRAPEZOID_INTEGRATION_H_

double getSequentialTrapezoidIntegral(double(*f)(double), double a, double b, int N);
double getParallelTrapezoidIntegral(double(*f)(double), double a, double b, int N);

#endif  // MODULES_TASK_1_BOGANOV_S_TRAPEZOID_INTEGRATION_TRAPEZOID_INTEGRATION_H_
