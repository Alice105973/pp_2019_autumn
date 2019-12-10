// Copyright 2019 Kukushkina Ksenia
#include <../../../modules/task_3/kukushkina_k_quick_batcher/quick_batcher.h>
#include <algorithm>
#include <vector>
#include <iomanip>

static int offset = 0;
std::vector<int> comparators(0);

void generateRand(std::vector<int> &vec, int n) {
  std::mt19937 gen;
  for (int i = 0; i < n; i++) {
    gen.seed(static_cast<int>(time(0)) + offset++);
    vec[i] = gen() % 100;
  }
}

bool isSorted(std::vector<int>& vec, int n) {
  for (int i = 0; i < n - 1; i++)
    if (vec[i] > vec[i + 1])
      return false;
  return true;
}

int partition(std::vector<int>& vec, int low, int high) {
  int pivot = vec[high];
  int tmp;
  int i = (low - 1);
    for (int j = low; j <= high - 1; j++) {
      if (vec[j] < pivot) {
        i++;
        tmp = vec[i];
        vec[i] = vec[j];
        vec[j] = tmp;
      }
    }
  tmp = vec[i + 1];
  vec[i + 1] = vec[high];
  vec[high] = tmp;
  return (i + 1);
}

void quickSort(std::vector<int>& vec, int low, int high) {
  if (low < high)
  {
    int pivot = partition(vec, low, high);
    quickSort(vec, low, pivot - 1);
    quickSort(vec, pivot + 1, high);
  }
}

void S(const std::vector<int>& procs_up, const std::vector<int>& procs_down)
{
  /*std::cout << "up: ";
  for (int i = 0; i < procs_up.size(); i++)
    std::cout << procs_up[i] << " ";
  std::cout << std::endl;
  std::cout << "down: ";
  for (int i = 0; i < procs_down.size(); i++)
    std::cout << procs_down[i] << " ";
  std::cout << std::endl;*/
  int proc_count = procs_up.size() + procs_down.size();
  if (proc_count < 2) {
    return;
  }
  else if (proc_count == 2) {
    //std::cout << procs_up[0] << " " << procs_down[0] << " ";
    comparators.push_back(procs_up[0]);
    comparators.push_back(procs_down[0]);
    return;
  }

  std::vector<int> procs_up_odd;
  std::vector<int> procs_down_odd;
  std::vector<int> procs_up_even;
  std::vector<int> procs_down_even;
  std::vector<int> procs_result(proc_count);

  for (int i = 0; i < procs_up.size(); i++) {
    if (i % 2) {
      procs_up_even.push_back(procs_up[i]);
    } else {
      procs_up_odd.push_back(procs_up[i]);
    }
  }
  for (int i = 0; i < procs_down.size(); i++) {
    if (i % 2) {
      procs_down_even.push_back(procs_down[i]);
    } else {
      procs_down_odd.push_back(procs_down[i]);
    }
  }

  S(procs_up_odd, procs_down_odd);
  S(procs_up_even, procs_down_even);

  for (int i = 0; i < procs_up.size(); i++) {
    procs_result[i] = procs_up[i];
  }
  for (int i = 0; i < procs_down.size(); i++) {
    procs_result[i + procs_up.size()] = procs_down[i];
  }

  std::copy(procs_up.begin(), procs_up.end(), procs_result.begin());
  std::copy(procs_down.begin(), procs_down.end(),
    procs_result.begin() + procs_up.size());

  for (int i = 1; i < procs_result.size() - 1; i += 2) {
    comparators.push_back(procs_result[i]);
    //std::cout << i << ":" << procs_result[i] << " " << procs_result[i + 1] << " ";
    comparators.push_back(procs_result[i + 1]);
  }
}

void B(const std::vector<int>& procs)
{
  if (procs.size() < 2) {
    return;
  }

  std::vector<int> procs_up(procs.size() / 2);
  std::vector<int> procs_down(procs.size() / 2 + procs.size() % 2);

  std::copy(procs.begin(), procs.begin() + procs_up.size(), procs_up.begin());
  std::copy(procs.begin() + procs_up.size(), procs.end(), procs_down.begin());

  B(procs_up);
  B(procs_down);
  S(procs_up, procs_down);
  /*std::cout << "comparators: ";
  for (int i = 0; i < comparators.size(); i++)
    std::cout << comparators[i] << " ";
  std::cout << std::endl;*/
}

void quickBatcherMPI(std::vector<int>& vec, int low, int high) {
  int size, rank, n = high - low + 1;
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Status status;
  while (n % size != 0) {
    vec.push_back(200);
    n++;
  }
  int len = n / size;
  std::vector<int> p(size);
  for (int i = 0; i < size; i++)
    p[i] = i;
  std::vector<int> resvec(len), curvec(len), tempvec(len);
  for (int i = 0; i < len; i++)
    resvec[i] = vec[rank * len + i];
  quickSort(resvec, 0, len - 1);
  comparators.clear();
  B(p);
  for (int i = 0; i < comparators.size(); i += 2) {
    int a = comparators[i], b = comparators[i + 1];
    if (rank == a) {
      MPI_Send(&resvec[0], len, MPI_INT, b, 0, MPI_COMM_WORLD);
      MPI_Recv(&curvec[0], len, MPI_INT, b, 0, MPI_COMM_WORLD, &status);

      for (int resi = 0, curi = 0, tmpi = 0; tmpi < len; tmpi++) {
        int res = resvec[resi], cur = curvec[curi];
        if (res < cur) {
          tempvec[tmpi] = res;
          resi++;
        } else {
          tempvec[tmpi] = cur;
          curi++;
        }
      }
      std::vector<int> t = resvec;
      resvec = tempvec;
      tempvec = t;
    } else if (rank == b) {
      MPI_Recv(&curvec[0], len, MPI_INT, a, 0, MPI_COMM_WORLD, &status);
      MPI_Send(&resvec[0], len, MPI_INT, a, 0, MPI_COMM_WORLD);
      int start = len - 1;
      for (int resi = start, curi = start, tmpi = start; tmpi >= 0; tmpi--) {
        int res = resvec[resi], cur = curvec[curi];
        if (res > cur) {
          tempvec[tmpi] = res;
          resi--;
        } else {
          tempvec[tmpi] = cur;
          curi--;
        }
      }
      std::vector<int> t = resvec;
      resvec = tempvec;
      tempvec = t;
    }
  }
}
