/// @file
/// @brief Файл с реализацией интерфейса исполнителя блока команд

#include "async.h"
#include "cp_queue.h"
#include "executor.h"
#include "logger.h"
#include "thread.h"
#include <algorithm>
#include <map>
#include <memory>
#include <shared_mutex>
#include <sstream>

namespace async {

namespace
{

struct Context
{
    Context(std::size_t id) :
        id_(id)
    {
    }

    std::size_t id_ = 0;
    std::atomic_bool isDisconnected_{false};
};

class AsyncThread : public Thread
{
public:
    AsyncThread() : Thread("asyncThread") { }

    ~AsyncThread()
    {
        Thread::stop([]{}, true); // Ожиданиие завершения потока, чтобы сохранить жизнь разделяемых объектов AsyncThread
    }

    void asyncLoop(std::size_t n)
    {
        logging::Logger logger;
        logger.addSink(std::make_unique<logging::CoutSink>());
        logger.addSink(std::make_unique<logging::FileSink>());
        Executor executor(logger);

        Bulk bulk;

        std::tuple<std::shared_ptr<Context>, std::string, bool> item;
        while (queue_.waitPop(item))
        {
            auto ctx = std::get<0>(item);
            auto& data = std::get<1>(item);
            auto isLastData = std::get<2>(item);

            std::size_t id = ctx->id_;

            // Если строка с разделителями, значит пришел блок с динамическим размером
            if (std::count(data.begin(), data.end(), '\n'))
            {
                std::stringstream is;
                is << data;

                std::string cmd;
                while (std::getline(is, cmd))
                {
                    bulk.push_back(cmd);
                }

                executor.exec(bulk);
                bulk.clear();
            }
            else
            {
                // пустая команда используется для исполнения накопившихся команд независимо от размера блока
                if (data.empty())
                {
                    executor.exec(bulk);
                    bulk.clear();
                }
                else
                {
                    bulk.push_back(data);
                    if (bulk.size() == n)
                    {
                        executor.exec(bulk);
                        bulk.clear();
                    }
                }
            }
            if (isLastData)
            {
                std::unique_lock<std::shared_timed_mutex> lock(ctxMutex_);
                ctxMap_.erase(id);
            }
        }
    }

    // std::map для сортивоки ключей, чтобы легко получить максимальное значение ключей
    std::map<std::size_t, std::shared_ptr<Context>> ctxMap_;
    std::shared_timed_mutex ctxMutex_;
    ConsumerProducerQueue<std::tuple<std::shared_ptr<Context>, std::string, bool>> queue_;
};

AsyncThread asyncThread;


} //namespace

handle_t connect(std::size_t n)
{
    if (!asyncThread.isStarted())
    {
        static std::once_flag flag;
        std::call_once(flag, [n]{
                auto f = [n]{ asyncThread.asyncLoop(n); };
                asyncThread.start(f);
            });
    }

    std::size_t id = 0;
    std::unique_lock<std::shared_timed_mutex> lock(asyncThread.ctxMutex_);
    if (!asyncThread.ctxMap_.empty())
    {
        id = asyncThread.ctxMap_.rbegin()->first;
    }
    id++;
    asyncThread.ctxMap_.insert(std::make_pair(id, std::make_shared<Context>(id)));

    return id;
}

void receive(handle_t handle, const char *data, std::size_t size)
{
    std::size_t id = static_cast<size_t>(handle);

    std::shared_lock<std::shared_timed_mutex> lock(asyncThread.ctxMutex_);
    auto it = asyncThread.ctxMap_.find(id);
    if (it == asyncThread.ctxMap_.end())
    {
        return;
    }
    auto ctx = it->second;
    lock.unlock();

    if (!ctx->isDisconnected_)
    {
        asyncThread.queue_.waitPush(std::make_tuple(ctx, std::string(data, size), false));
    }
}

void disconnect(handle_t handle)
{
    std::size_t id = static_cast<size_t>(handle);

    std::shared_lock<std::shared_timed_mutex> lock(asyncThread.ctxMutex_);
    auto it = asyncThread.ctxMap_.find(id);
    if (it == asyncThread.ctxMap_.end())
    {
        return;
    }
    auto ctx = it->second;
    lock.unlock();

    ctx->isDisconnected_.store(true);
    asyncThread.queue_.waitPush(std::make_tuple(ctx, std::string(), true));
}

} //namespace async
