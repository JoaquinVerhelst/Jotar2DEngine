#pragma once
#include <coroutine>
#include <chrono>
#include <thread>
#include <future>
#include <functional>
#include <iostream>

namespace JV
{
    class AsyncWait {
    public:
        struct promise_type;
        using handle_type = std::coroutine_handle<promise_type>;

        AsyncWait(handle_type handle) : handle_(handle) {}
        ~AsyncWait() {
            if (handle_) handle_.destroy();
        }

        struct promise_type {
            std::promise<void> promise_;

            auto get_return_object() {
                return AsyncWait(handle_type::from_promise(*this));
            }

            auto initial_suspend() {
                return std::suspend_always{};
            }

            auto final_suspend() noexcept {
                return std::suspend_always{};
            }

            void return_void() {}

            void unhandled_exception() {
                promise_.set_exception(std::current_exception());
            }

            auto yield_value(std::chrono::seconds duration) {
                std::async([duration, this]() {
                    std::this_thread::sleep_for(duration);
                    promise_.set_value();
                    });
                return std::suspend_always{};
            }
        };

        bool await_ready() {
            return false;
        }

        void await_suspend(std::coroutine_handle<> handle) {
            std::thread([handle]() {
                handle();
                }).detach();
        }

        void await_resume() {}

        bool resume() {
            if (!handle_.done()) {
                handle_.resume();
                return !handle_.done();
            }
            return false;
        }

    private:
        handle_type handle_;
    };

}