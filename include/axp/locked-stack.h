#pragma once

#include <condition_variable>
#include <mutex>
#include <optional>
#include <vector>


template <typename type>
class locked_stack {
public:
    void push(type value) {
        std::lock_guard<std::mutex> lock(mutex_);
        stack_.push_back(std::move(value));

        new_task_.notify_one();
    }

    std::optional<type> wait_task() {
        std::unique_lock<std::mutex> lock(mutex_);
        new_task_.wait(lock, [this] {
            return should_stop_immediately_ || stack_.size() != 0;
        });

        if (stack_.empty())
            return {};

        type value = stack_.back();
        stack_.pop_back();

        // I suppose that however is getting this task can push more:
        ++ units_that_push_;

        return value;
    }

    void sub_potential_pussher() {
        std::lock_guard<std::mutex> lock(mutex_);
        -- units_that_push_;
        emptied_.notify_all();
    }

    void wait_empty() {
        std::unique_lock<std::mutex> lock(mutex_);
        emptied_.wait(lock, [this] {
            return units_that_push_ == 0 && stack_.empty();
        });

        notify_no_more_tasks();
    }

private:
    std::vector<type> stack_;
    std::mutex mutex_;

    std::condition_variable new_task_;
    std::condition_variable emptied_;
    std::size_t units_that_push_ = 0;

    bool should_stop_immediately_ = false;

    void notify_no_more_tasks() {
        should_stop_immediately_ = true;
        new_task_.notify_all();
    }
};
