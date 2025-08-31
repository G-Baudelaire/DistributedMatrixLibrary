#include <gtest/gtest.h>
#include <mpi.h>

class MpiEnv final : public testing::Environment {
public:
  void SetUp() override {
    int argc = 0;
    char** argv = nullptr;
    int provided = 0;
    MPI_Init_thread(&argc, &argv, MPI_THREAD_MULTIPLE, &provided);
  }
  void TearDown() override {
    int finalized = 0;
    MPI_Finalized(&finalized);
    if (!finalized) MPI_Finalize();
  }
};

int main(int argc, char** argv) {
  testing::InitGoogleTest(&argc, argv);
  testing::AddGlobalTestEnvironment(new MpiEnv());
  return RUN_ALL_TESTS();
}
