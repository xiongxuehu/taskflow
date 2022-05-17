// A simple example to capture the following task dependencies.
//
//           +---+
//     +---->| B |-----+
//     |     +---+     |
//   +---+           +-v-+
//   | A |           | D |
//   +---+           +-^-+
//     |     +---+     |
//     +---->| C |-----+
//           +---+
//
#include <chrono>
#include <taskflow/taskflow.hpp>  // the only include you need
#include <thread>

using namespace std::chrono;

int main() {
  // auto future = std::async(std::launch::async, []() {
  //   std::cout << "I'm a thread" << std::endl;
  //   return 100;
  // });

  // future.get();

  tf::Executor executor(1);
  tf::Taskflow taskflow("simple");

  auto [A, B, C, D] = taskflow.emplace(
      []() {
        std::cout << "thread:" << std::this_thread::get_id() << ", TaskA\n";
        std::this_thread::sleep_for(duration<float, std::milli>(3000));
        return "A";
      },
      []() {
        std::cout << "thread:" << std::this_thread::get_id() << ", TaskB\n";
        std::this_thread::sleep_for(duration<float, std::milli>(1000));
        return "B";
      },
      []() {
        std::cout << "thread:" << std::this_thread::get_id() << ", TaskC\n";
        return "C";
      },
      []() {
        std::cout << "thread:" << std::this_thread::get_id() << ", TaskD\n";
        return "D";
      });

  std::cout << "main thread:" << std::this_thread::get_id() << std::endl;
  std::cout << A.has_work() << std::endl;

  A.precede(B, C);  // A runs before B and C
  D.succeed(B);     // D runs after  B and C
  auto E = D;

  executor.run(taskflow).get();

  return 0;
}
