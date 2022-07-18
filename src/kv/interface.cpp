#include "include/interface.h"
#include <algorithm>
#include <cstring>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

namespace newsql_kv {

#define DEBUG

#define MAX_LEN 100

class User {
 public:
  int64_t id;
  char user_id[128];
  char name[128];
  int64_t salary;
};

struct SalaryIndex {
  int64_t salary;
  int64_t index;
};

enum Column { Id = 0, Userid, Name, Salary };

struct WriteBuffer {
  WriteBuffer() { users_.resize(MAX_LEN); }

  User &current() { return users_[i++]; }

  int i = 0;
  std::vector<User> users_;

  ~WriteBuffer() {
    // lock
    // copy users to global user
  }
};

thread_local WriteBuffer write_buffer;

std::vector<User> users;  // sort by name after write

std::unordered_map<std::string, int> uid_index;  // after sort, build uid_index // user_id char[128]

std::unordered_map<uint64_t, int> id_index;  // after sort, build id_index
std::vector<SalaryIndex> salary_index;       // after sort, build salary_index
bool is_build_index = false;

void BuildIndex() {
  is_build_index = true;
  auto cmp = [&](const User &u1, const User &u2) {
    int result = memcmp(u1.name, u2.name, 128);
    return result <= 0;
  };
  users.insert(users.end(), write_buffer.users_.begin(), write_buffer.users_.begin() + write_buffer.i);

  sort(users.begin(), users.end(), cmp);
  salary_index.resize(users.size());
  id_index.reserve(users.size());
  uid_index.reserve(users.size());

  int size = users.size();

  for (int i = 0; i < size; i++) {
    id_index[users[i].id] = i;
    uid_index[std::string(users[i].user_id, 128)] = i;
    salary_index[i].salary = i;
  }
  auto sa_cmp = [](SalaryIndex &s1, SalaryIndex &s2) {
    // TODO(wangyi):
    return true;
  };
  sort(salary_index.begin(), salary_index.end(), sa_cmp);

#ifdef DEBUG
  for (auto &u : users) {
    std::cout << u.id << std::endl;
  }
#endif
}

void engine_write(void *ctx, const void *data, size_t len) {
  User &user = write_buffer.current();
  memcpy(&user, data, len);
}

int solveId(const void *column_key, size_t column_key_len, void *res) {
  int64_t id = (int64_t)column_key;
  int index = id_index[id];
  // TODO(wangyi): may be not found ?
  return index;
}

int SolveUserId(const void *column_key, size_t column_key_len, void *res) {
  char uid[128];
  memcpy(uid, column_key, column_key_len);
  auto find = uid_index.find(std::string(uid, 128));
  // TODO(wangyi) maybe not found
  return find->second;
}

int SolveName(const void *column_key, size_t column_key_len, void *res) {
  // lower_bound in users
  int l = 0, r = users.size();
  while (l < r) {
    int mid = (l + r) / 2;
    int cmp = memcmp(users[mid].name, column_key, column_key_len);
    if (cmp <= 0) {
      l = mid;
    } else {
      r = mid - 1;
    }
  }
  return l;
}

int SolveSalary(const void *column_key, size_t column_key_len, void *res) {
  int64_t salary = (int64_t)column_key;
  int l = 0, r = users.size();
  while (l < r) {
    int mid = (l + r) / 2;
    if (users[mid].salary <= salary) {
      l = mid;
    } else {
      r = mid - 1;
    }
  }
  return l;
}

size_t engine_read(void *ctx, int32_t select_column, int32_t where_column, const void *column_key,
                   size_t column_key_len, void *res) {
  if (!is_build_index) BuildIndex();
  int users_size = users.size();
  bool b = true;
  size_t res_num = 0;
  //    switch (where_column) {
  //        case Id:
  //            int id = solveId(column_key, column_key_len, res);
  //            break;
  //        case Userid:
  //
  //
  //    }
  for (int i = 0; i < users_size; ++i) {
    switch (where_column) {
      case Id:
        b = memcmp(column_key, &users[i].id, column_key_len) == 0;
        break;
      case Userid:
        b = memcmp(column_key, users[i].user_id, column_key_len) == 0;
        break;
      case Name:
        b = memcmp(column_key, users[i].name, column_key_len) == 0;
        break;
      case Salary:
        b = memcmp(column_key, &users[i].salary, column_key_len) == 0;
        break;
      default:
        b = false;
        break;  // wrong
    }
    if (b) {
      ++res_num;
      switch (select_column) {
        case Id:
          memcpy(res, &users[i].id, 8);
          res = reinterpret_cast<char *>(res) + 8;
          break;
        case Userid:
          memcpy(res, users[i].user_id, 128);
          res = reinterpret_cast<char *>(res) + 128;
          break;
        case Name:
          memcpy(res, users[i].name, 128);
          res = reinterpret_cast<char *>(res) + 128;
          break;
        case Salary:
          memcpy(res, &users[i].salary, 8);
          res = reinterpret_cast<char *>(res) + 8;
          break;
        default:
          break;  // wrong
      }
    }
  }
  return res_num;
}

void *engine_init(const char *host_info, const char *const *peer_host_info, size_t peer_host_info_num,
                  const char *aep_dir, const char *disk_dir) {
  return nullptr;
}

void engine_deinit(void *ctx) {}

}  // namespace newsql_kv
