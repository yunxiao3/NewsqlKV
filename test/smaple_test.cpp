#include <cstring>
#include <iostream>

#include "gtest/gtest.h"
#include "include/interface.h"

namespace newsql_kv {

class TestUser {
 public:
  int64_t id;
  char user_id[128];
  char name[128];
  int64_t salary;
};

enum TestColumn { Id = 0, Userid, Name, Salary };

TEST(SampleTest, SampleTest) {
  TestUser user;
  user.id = 0;
  user.salary = 100;
  memcpy(&user.name, "hello", 5);
  void *ctx = engine_init(nullptr, nullptr, 0, "/mnt/aep/", "/mnt/disk/");
  engine_write(ctx, &user, sizeof(user));
  char res[2000 * 128];
  std::cout << engine_read(ctx, Id, Name, &user.name, 8, res) << std::endl;
  std::cout << "res:" << reinterpret_cast<int64_t *>(res);
  engine_deinit(ctx);
}

}  // namespace newsql_kv
