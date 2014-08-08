#ifndef _CITERTOOLS_THREAD_POOL_H_
#define _CITERTOOLS_THREAD_POOL_H_

#include <cit/definition.h>

#ifndef _CITERTOOLS_CPP11_
#error "Nothing in <cit/thread/pool.h> can be used when not using C++11!"
#endif /* N _CITERTOOLS_CPP11_ */

#include <atomic>
#include <condition_variable>
#include <future>
#include <mutex>
#include <queue>
#include <thread>
#include <vector>

namespace cit {
    namespace thread {
        namespace _pool {
            template<size_t N>
            struct unpack_tuple_functor {
	            template<typename F, typename... TupleArgs, typename... Args>
	            inline static auto call(F && functor, std::tuple<TupleArgs...> && tuple, Args &&... args)->decltype(unpack_tuple_functor<N - 1>::call(std::forward<F>(functor), std::forward<std::tuple<TupleArgs...>>(tuple), std::get<N - 1>(tuple), args...)) {
		            return unpack_tuple_functor<N - 1>::call(std::forward<F>(functor), std::forward<std::tuple<TupleArgs...>>(tuple), std::get<N - 1>(tuple), std::forward<Args>(args)...);
	            }
            };

            template<>
            struct unpack_tuple_functor<0> {
	            template<typename F, typename... TupleArgs, typename... Args>
	            inline static auto call(F && functor, std::tuple<TupleArgs...> && tuple, Args &&... args)->decltype(functor(args...)) {
		            return functor(std::forward<Args>(args)...);
	            }
            };

            template<typename F, typename... TupleArgs>
            inline auto call_unpack_tuple(F && functor, std::tuple<TupleArgs...> && tuple)->decltype(
                    unpack_tuple_functor<sizeof... (TupleArgs)>::call(std::forward<F>(functor), std::forward<std::tuple<TupleArgs...>>(tuple))) {
	            return unpack_tuple_functor<sizeof... (TupleArgs)>::call(std::forward<F>(functor), std::forward<std::tuple<TupleArgs...>>(tuple));
            }
        }

        class pool_t {
	        friend class std::thread;
        public:

	        explicit inline pool_t(size_t num_threads = std::thread::hardware_concurrency()) : threads(num_threads), threads_running(true), idle_thread_count(0) {
		        for (size_t i = 0; i < num_threads; ++i)
		            threads[i] = new std::thread(&pool_t::entry_point, this, i);
	        }

	        pool_t(const pool_t & copy) = delete;
	        ~pool_t() {
	            threads_running = false;
	            enqueue_condition.notify_all();

	            for (std::thread *thread : threads) {
		            try {
			            thread->join();
		            }
		            catch (const std::system_error &) {}

		            delete thread;
	            }

	            while (!queue.empty()) {
		            delete queue.front();
		            queue.pop();
	            }
	        }

	        template<typename Function, typename... Args>
	        auto push(Function && f, Args &&... args)->std::future<decltype(f(args...))> {
		        typedef decltype(f(args...)) R;
		        task_t<Function, decltype(f(args...)), Args...> *task(new task_t<Function, decltype(f(args...)), Args...>(std::forward<Function>(f), std::forward<Args>(args)...));

		        {
			        std::lock_guard<std::mutex> lock(queue_mutex);
			        queue.push(task);
		        }

		        enqueue_condition.notify_one();
		        return task->get();
	        }

	        /*
	         * Removes any pending queued tasks. Any futures will return the exception std::runtime_error.
	         */
	        void clear() {
		        std::lock_guard<std::mutex> lock(queue_mutex);
		        while (!queue.empty()) {
			        queue.front()->fail(std::runtime_error("thread_pool::clear"));
			        queue.pop();
		        }
	        }

	        /*
	         * Waits until all threads are idle.
	         */
	        void wait_idle() {
		        //std::unique_lock<std::mutex> lock(idle_thread_count_mutex);
		        std::unique_lock<std::mutex> lock(queue_mutex);
		        wait_idle_condition.wait(lock, [this] { return idle_thread_count == threads.size() && queue.empty(); });
	        }

        private:

	        struct queued_task_t {
		        virtual void process() = 0;
		        virtual void fail(std::exception && e) = 0;
		        inline virtual ~queued_task_t() {}
	        };

	        template<typename Function, typename R, typename... Args>
	        class task_t : public queued_task_t {
	        public:
		        inline task_t(Function && f, Args &&... args) : f(std::forward<Function>(f)), args(std::forward<Args>(args)...) {}

		        inline std::future<R> get() {
			        return promise.get_future();
		        }

		        void process() {
			        try {
				        promise.set_value(_pool::call_unpack_tuple(std::move(f), std::move(args)));
			        }
			        catch (...) {
				        promise.set_exception(std::current_exception());
			        }
		        }

		        void fail(std::exception && e) {
			        // Ugly, but seems to be the way to convert a std::exception to a std::exception_ptr.
			        try {
				        throw e;
			        } catch (...) {
				        promise.set_exception(std::current_exception());
			        }
		        }

	        private:
		        Function f;
		        std::promise<R> promise;
		        std::tuple<Args...> args;
	        };

	        typedef std::vector<std::thread *> thread_container_t;
	        typedef std::queue<queued_task_t *> task_container_t;

	        void entry_point(size_t id) {
	            while (threads_running) {
		            queued_task_t *task = nullptr;

		            {
			            std::unique_lock<std::mutex> lock(queue_mutex);
			            if (queue.empty()) {

                            // TODO: Really atomic? We have the lock anyway?
				            if (++idle_thread_count == threads.size())
					            wait_idle_condition.notify_all();

				            enqueue_condition.wait(lock, [this] { return !queue.empty() || !threads_running; });

				            --idle_thread_count;
			            }

			            if (!queue.empty()) {
				            task = queue.front();
				            queue.pop();
			            }
		            }

		            if (task) {
			            task->process();
			            delete task;
			            fprintf(stderr, "Thread %lu finished task.\n", id);
		            }
	            }
	        }

	        thread_container_t threads;
	        volatile bool threads_running;
	        task_container_t queue;
	        std::mutex queue_mutex;
	        std::condition_variable enqueue_condition;

	        std::atomic_size_t idle_thread_count;
	        std::condition_variable wait_idle_condition;
        };
    }
}

#endif /* N _THREAD_POOL_H_ */
