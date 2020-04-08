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
#include <boost/asio.hpp>


namespace tl {


/**
 * wrapper for boost thread pool
 */
template<class t_func>
class ThreadPool
{
	public:
		using t_ret = typename std::result_of<t_func&()>::type;
		using t_fut = std::list<std::future<t_ret>>;
		using t_task = std::list<std::packaged_task<t_ret()>>;


	protected:
		boost::asio::thread_pool m_tp;
		std::mutex m_mtx, m_mtxStart;

		// list of wrapped function to be executed
		t_task m_lstTasks;

		// futures with function return values
		t_fut m_lstFutures;

		// function to run before each thread (not task)
		void (*m_pThStartFunc)() = nullptr;


	public:
		ThreadPool(unsigned int iNumThreads = std::thread::hardware_concurrency(),
			void (*pThStartFunc)() = nullptr)
			: m_tp{iNumThreads}, m_pThStartFunc{pThStartFunc}
		{}


		virtual ~ThreadPool()
		{
			Join();
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

			std::packaged_task<t_ret()>* thetask = &m_lstTasks.back();;

			boost::asio::post(m_tp, [this, thetask]() -> void
			{
				{
					// ensure that this is only called per-thread, not per-task
					std::lock_guard<std::mutex> lockStart(m_mtxStart);

					thread_local bool bThreadAlreadySeen{0};
					if(m_pThStartFunc && !bThreadAlreadySeen)
					{
						bThreadAlreadySeen = 1;
						(*m_pThStartFunc)();
					}
				}

				(*thetask)();
			});
		}


		t_fut& GetResults() { return m_lstFutures; }


		t_task& GetTasks() { return m_lstTasks; }


		/**
		 * wait for all tasks to be finished
		 */
		void Join()
		{
			m_tp.join();
		}
};


}
#endif
