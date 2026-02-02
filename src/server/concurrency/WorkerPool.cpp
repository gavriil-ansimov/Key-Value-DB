#include"WorkerPool.h"

WorkerPool::Worker::Worker()
    : io()
    , work_guard(boost::asio::make_work_guard(io))
    , thread([this]{io.run();}) {}

WorkerPool::WorkerPool(std::size_t size) {
    if (size == 0) 
        throw std::invalid_argument("WorkerPool size must be > 0");
    workers_.reserve(size);
    for (std::size_t i = 0; i != size; ++i)
        workers_.emplace_back(std::make_unique<Worker>());
}

boost::asio::io_context& WorkerPool::next_io() {
    auto idx = next_.fetch_add(1, std::memory_order_relaxed);
    return workers_[idx % workers_.size()]->io;
}

WorkerPool::~WorkerPool() {
    for (auto& w : workers_) {
        w->work_guard.reset();
        w->io.stop();
    }
    for (auto& w : workers_) {
        if (w->thread.joinable())
            w->thread.join();
    }
}