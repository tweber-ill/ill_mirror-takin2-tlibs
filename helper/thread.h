/**
 * Thread helpers
 * @author Tobias Weber <tobias.weber@tum.de>
 * @date aug-2015
 * @license GPLv2 or GPLv3
 */

#ifndef __TLIBS_THREAD_H__
#define __TLIBS_THREAD_H__

#include <future>
#include <thread>
#include <mutex>
#include <list>
#include <functional>
#include <algorithm>
#include <type_traits>
#include <memory>


namespace tl {


/**
 * minimalistic thread pool
 */
template<class t_func> class ThreadPool
{
	public:
		using t_ret = typename std::result_of<t_func&()>::type;
		using t_fut = std::list<std::future<t_ret>>;
		using t_task = std::list<std::packaged_task<t_ret()>>;


	protected:
		std::list<std::unique_ptr<std::thread>> m_lstThreads;
		mutable std::mutex m_mtx;


		// list of wrapped function to be executed
		t_task m_lstTasks;
		// futures with function return values
		t_fut m_lstFutures;


		// signal to start jobs
		std::promise<void> m_signalStartIn;
		std::future<void> m_signalStartOut = std::move(m_signalStartIn.get_future());


	public:
		ThreadPool(unsigned int iNumThreads = std::thread::hardware_concurrency(),
			void (*pThStartFunc)() = nullptr)
		{
			// start 'iNumThreads' threads
			for(unsigned int iThread=0; iThread<iNumThreads; ++iThread)
			{
				m_lstThreads.emplace_back(
				std::unique_ptr<std::thread>(new std::thread([this, pThStartFunc, iThread]()
				{
					// callback to invoke before starting job thread
					if(pThStartFunc) (*pThStartFunc)();
					m_signalStartOut.wait();

					while(1)
					{
						std::unique_lock<std::mutex> lock0(m_mtx);

						// is a task available
						if(m_lstTasks.size() > 0)
						{
							// pop task from list
							std::packaged_task<t_ret()> task =
								std::move(m_lstTasks.front());
							m_lstTasks.pop_front();

							lock0.unlock();

							// run task
							//tl::log_debug("Thread ", iThread, ": running task.");
							task();
						}
						else
							break;
					}
				})));
			}
		}


		virtual ~ThreadPool()
		{
			JoinAll();
			m_lstThreads.clear();
		}


		/**
		 * add a function to be executed, giving a packaged task and a future.
		 */
		void AddTask(const std::function<t_ret()>& fkt)
		{
			std::packaged_task<t_ret()> task(fkt);
			std::future<t_ret> fut = task.get_future();

			std::lock_guard<std::mutex> lock(m_mtx);
			m_lstTasks.emplace_back(std::move(task));
			m_lstFutures.emplace_back(std::move(fut));
		}


		/**
		 * issue start signal
		 */
		void StartTasks()
		{
			m_signalStartIn.set_value();
		}


		t_fut& GetFutures() { return m_lstFutures; }
		t_task& GetTasks() { return m_lstTasks; }


		/**
		 * wait for all tasks to be finished
		 */
		void JoinAll()
		{
			std::for_each(m_lstThreads.begin(), m_lstThreads.end(),
				[](std::unique_ptr<std::thread>& pThread)
				{
					if(pThread)
						pThread->join();
				});
		}
};

}
#endif
