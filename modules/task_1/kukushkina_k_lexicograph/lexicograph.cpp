// Copyright 2019 Kukushkina Ksenia

#include <mpi.h>

#include <string>
#include <algorithm>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <iostream>

#include "lexicograph.h"


std::string generateString(const int length) {
  
  if (length <= 0)
    throw "Wrong size";
  int n;
  std::string s;
  for (int i = 0; i < length; i++) {
    n = std::rand() % 3;
    switch (n) {
    case 0:
      s += char(std::rand() % 10 + 48); // 0 - 9
      break;
    case 1:
      s += char(std::rand() % 26 + 65); // A - Z
      break; 
    case 2:
      s += char(std::rand() % 26 + 97); // a - z
      break;
    }   
  }
  return s;
}

bool isFragmentOrdered(const std::string source) {
  if (source.empty())
    return true;
  int size = source.size();
  for (int i = 1; i < size; i++)
    if (source[i] < source[i - 1])
      if ((source[i - 1] != ' ') && (source[i] != ' ')) {
        return false;
      }
  return true;
}

bool isStringOrdered(const std::string source1, const std::string source2) {
  if (source1.empty() || source2.empty()) {
    throw "Empty string";
  }

  std::string source(source1);
  source += ' ' + source2 + ' ';
  int size, rank;
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  int len = source.size() / size;
  int rem = source.size() % size;
  bool lres, res;
  if (rank == 0) {
    for (int process = 1; process < size; process++) {
      MPI_Send(&source[0] + process * len + rem, len + 1,
        MPI_CHAR, process, 0, MPI_COMM_WORLD);
    }
  }

  std::string str;
  if (rank == 0) {
    str = source.substr(0, len + rem + 1);
  }
  else {
    MPI_Status status;
    MPI_Recv(&str[0], len + 1, MPI_CHAR, 0, 0, MPI_COMM_WORLD, &status);
  }

  lres = isFragmentOrdered(str);
  MPI_Reduce(&lres, &res, 1, MPI_C_BOOL, MPI_LAND, 0, MPI_COMM_WORLD);
  return res;
}