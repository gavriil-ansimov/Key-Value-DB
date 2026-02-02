#pragma once

#include<boost/asio.hpp>

#include<thread>
#include<vector>
#include<atomic>
#include<memory>

class WorkerPool{
public:
    explicit WorkerPool(std::size_t size);

    boost::asio::io_context& next_io();

    ~WorkerPool();

private:
    struct Worker{
        boost::asio::io_context io;
        boost::asio::executor_work_guard<
        boost::asio::io_context::executor_type> work_guard;
        std::thread thread;
        Worker();
    };
    std::vector<std::unique_ptr<Worker>> workers_;
    std::atomic<std::size_t> next_{0};
};