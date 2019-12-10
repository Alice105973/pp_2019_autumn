// Copyright 2019 Kukushkina Ksenia
#ifndef MODULES_TASK_3_KUKUSHKINA_K_QUICK_BATCHER_QUICK_BATCHER_H_
#define MODULES_TASK_3_KUKUSHKINA_K_QUICK_BATCHER_QUICK_BATCHER_H_

#include <mpi.h>
#include <iostream>
#include <random>
#include <vector>

void generateRand(std::vector<int>& vec, int n);
bool isSorted(std::vector<int>& vec, int n);
void quickSort(std::vector<int>& vec, int low, int high);
void quickBatcherMPI(std::vector<int>& vec, int low, int high);
void S(const std::vector<int>& procs_up, const std::vector<int>& procs_down);
void B(const std::vector<int>& procs);

#endif  // MODULES_TASK_3_KUKUSHKINA_K_QUICK_BATCHER_QUICK_BATCHER_H_
