#pragma once

#include <condition_variable>
#include <thread>
#include <vector>
#include <optional>
#include <functional>

#include "locked-stack.h"

template <typename type>
class locked_stack_wrapper {
public:
    locked_stack_wrapper(locked_stack<type> &locked_stack):
        locked_stack_(locked_stack) {}

    void push(type value) { locked_stack_.push(std::move(value)); }

private:
    locked_stack<type> &locked_stack_;
};


template <typename task_type, std::regular_invocable<int, task_type, locked_stack_wrapper<task_type>> process_task_lambda>
class thread_pool {
public:
    thread_pool(std::size_t num_threads, process_task_lambda &&process_task):
        num_threads_(num_threads), process_task_(process_task) {}

    void run() {
        std::vector<std::jthread> threads;
        for (std::size_t i = 0; i < num_threads_; ++i)
            threads.emplace_back(std::bind(&thread_pool::run_single_thread, this, i));

        tasks_.wait_empty();
    }

private:
    std::size_t num_threads_;
    locked_stack<task_type> tasks_;

    std::size_t busy_threads_ = 0;
    process_task_lambda process_task_;


    void run_single_thread(int thread_index) {
        while (std::optional<task_type> maybe_task = tasks_.wait_task()) {
            process_task_(thread_index, *maybe_task, locked_stack_wrapper(tasks_));
            tasks_.sub_potential_pussher();
        }
    }
};
