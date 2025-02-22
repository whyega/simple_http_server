#pragma once

#include <condition_variable>
#include <functional>
#include <future>
#include <mutex>
#include <queue>
#include <thread>
#include <vector>

namespace util {
/**
 * @class ThreadPool
 * @brief Класс для работы с пулом потоков.
 *
 * Пул потоков позволяет выполнять задачи асинхронно и быстро. Он работает с
 * одним или несколькими потоками, которые выполняют задачи последовательно. Пул
 * потоков использует синхронизацию для управления задачами, что обеспечивает
 * быстродействие и удобство использования.
 */
class ThreadPool {
 private:
  std::vector<std::thread> workers;
  std::queue<std::function<void()>> tasks;

  std::mutex queueMutex;
  std::condition_variable condition;
  bool stop = false;

 public:
  /**
   * @brief Конструктор класса ThreadPool.
   *
   * @param numThreads Количество потоков в пуле.
   */
  ThreadPool(std::size_t numThreads);

  /**
   * @brief Деструктор класса ThreadPool.
   *
   * Останавливает все потоки и ожидает, пока они завершат работу.
   */
  ~ThreadPool();

  /**
  * @brief Выполнение задачи в пуле.
  *
  * @param f Функция, которую нужно выполнить.
  * @param args Аргументы функции.
  * @return Объект Future, который может быть использован для ожидания
  завершения
  * задачи.
  */
  template <class F, class... Args>
  auto enqueue(F&& f, Args&&... args)
      -> std::future<std::invoke_result_t<F, Args...>> {
    using return_type = std::invoke_result_t<F, Args...>;

    auto task = std::make_shared<std::packaged_task<return_type()>>(
        [func = std::forward<F>(f),
         args = std::make_tuple(std::forward<Args>(args)...)]() mutable {
          return std::apply(func, std::move(args));
        });

    std::future<return_type> res = task->get_future();
    {
      std::unique_lock<std::mutex> lock(queueMutex);

      if (stop) {
        return std::future<return_type>{};
      }

      tasks.emplace([task]() { (*task)(); });
    }
    condition.notify_one();
    return res;
  }
};
}  // namespace util