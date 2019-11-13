// Copyright 2019 Konnov Sergey

#ifndef MODULES_TASK_1_KONNOV_S_TRAPEZ_INTEGR_TRAPEZ_INTEGR_H_
#define MODULES_TASK_1_KONNOV_S_TRAPEZ_INTEGR_TRAPEZ_INTEGR_H_
#include <functional>

double GetTrapezIntegrParallel(double l, double r, int n, const std::function<double(double)>& f);
double GetTrapezIntegrSequential(double l, double r, int n, const std::function<double(double)>& f);

#endif  // MODULES_TASK_1_KONNOV_S_TRAPEZ_INTEGR_TRAPEZ_INTEGR_H_
