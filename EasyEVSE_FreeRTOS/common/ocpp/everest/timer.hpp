// SPDX-License-Identifier: Apache-2.0
// Copyright 2020 - 2021 Pionix GmbH and Contributors to EVerest
#ifndef EVEREST_TIMER_HPP
#define EVEREST_TIMER_HPP

#if 0
#include <boost/asio.hpp>
#endif
#include <chrono>
#include <condition_variable>
#include <time.h>
#if 0
#include <date/date.h>
#include <date/tz.h>
#endif
#include <functional>
#include <mutex>
#include <thread>

#include "OcppAsioContext.hpp"

namespace Everest {
// template <typename TimerClock = date::steady_clock> class Timer {
//template <typename TimerClock = date::utc_clock> class Timer {
template <typename TimerClock> class Timer {
private:
#if 0
    boost::asio::basic_waitable_timer<TimerClock>* timer = nullptr;
#endif
    std::function<void()> callback;
    std::chrono::nanoseconds interval_nanoseconds;
#if 0
    std::condition_variable cv;
    std::mutex wait_mutex;
#endif
    bool running = false;
    OcppAsioContext io_context;
#if 0
    boost::asio::executor_work_guard<boost::asio::io_context::executor_type> work;
    std::thread* timer_thread = nullptr;
#endif

public:
    /// This timer will initialize a boost::asio::io_context
    //explicit Timer() : work(boost::asio::make_work_guard(this->io_context)) {
    explicit Timer() {
#if 0
        this->timer = new boost::asio::basic_waitable_timer<TimerClock>(this->io_context);
        this->timer_thread = new std::thread([this]() { this->io_context.run(); });
#endif
    }

   // explicit Timer(const std::function<void()>& callback) : work(boost::asio::make_work_guard(this->io_context)) {
    explicit Timer(const std::function<void()>& callback) {
#if 0
        this->timer = new boost::asio::basic_waitable_timer<TimerClock>(this->io_context);
        this->timer_thread = new std::thread([this]() { this->io_context.run(); });
#endif
        this->callback = callback;
    }

    //explicit Timer(boost::asio::io_context* io_context) : work(boost::asio::make_work_guard(*io_context))
    explicit Timer(OcppAsioContext* io_context){
#if 0
        this->timer = new boost::asio::basic_waitable_timer<TimerClock>(*io_context);
#endif
    }

//    explicit Timer(boost::asio::io_context* io_context, const std::function<void()>& callback) :
//        work(boost::asio::make_work_guard(*io_context)) {
    explicit Timer(OcppAsioContext* io_context, const std::function<void()>& callback) {
#if 0
        this->timer = new boost::asio::basic_waitable_timer<TimerClock>(*io_context);
#endif
        this->callback = callback;
    }

    ~Timer() {
#if 0
        if (this->timer != nullptr) {
            // stop asio timer
            this->timer->cancel();
#if 0
            if (this->timer_thread != nullptr) {
                this->io_context.stop();
                this->timer_thread->join();
            }
#endif

            delete this->timer;
#if 0
            delete this->timer_thread;
#endif
        }
#endif
    }

    /// Executes the given callback at the given timepoint
    template <class Clock, class Duration = typename Clock::duration>
    void at(const std::function<void()>& callback, const std::chrono::time_point<Clock, Duration>& time_point) {
        this->stop();

        this->callback = callback;

        this->at(time_point);
    }

    /// Executes the at the given timepoint
    template <class Clock, class Duration = typename Clock::duration>
    void at(const std::chrono::time_point<Clock, Duration>& time_point) {
        this->stop();

        if (this->callback == nullptr) {
            return;
        }
#if 0
        if (this->timer != nullptr) {
            // use asio timer
            this->timer->expires_at(time_point);
            this->timer->async_wait([this](const boost::system::error_code& e) {
                if (e) {
                    return;
                }

                this->callback();
            });
        }
#endif
    }

    /// Execute the given callback peridically from now in the given interval
    template <class Rep, class Period>
    void interval(const std::function<void()>& callback, const std::chrono::duration<Rep, Period>& interval) {
        this->stop();

        this->callback = callback;

        this->interval(interval);
    }

    /// Execute peridically from now in the given interval
    template <class Rep, class Period> void interval(const std::chrono::duration<Rep, Period>& interval) {
        this->stop();
        this->interval_nanoseconds = interval;
        if (interval_nanoseconds == std::chrono::nanoseconds(0)) {
            return;
        }

        if (this->callback == nullptr) {
            return;
        }

        if (this->timer != nullptr) {
            // use asio timer
#if 0
            this->callback_wrapper = [this](const boost::system::error_code& e) {
                if (e) {
                    return;
                }

                this->timer->expires_after(this->interval_nanoseconds);
                this->timer->async_wait(this->callback_wrapper);

                this->callback();
            };
#endif

            this->timer->expires_after(this->interval_nanoseconds);
            this->timer->async_wait(this->callback_wrapper);
        }
    }

    // Execute the given callback once after the given interval
    template <class Rep, class Period>
    void timeout(const std::function<void()>& callback, const std::chrono::duration<Rep, Period>& interval) {
        this->stop();

        this->callback = callback;

        this->timeout(interval);
    }

    // Execute the given callback once after the given interval
    template <class Rep, class Period> void timeout(const std::chrono::duration<Rep, Period>& interval) {
        this->stop();

        if (this->callback == nullptr) {
            return;
        }

        if (this->timer != nullptr) {
            // use asio timer
            this->timer->expires_after(interval);
#if 0
            this->timer->async_wait([this](const boost::system::error_code& e) {
                if (e) {
                    return;
                }

                this->callback();
            });
#endif
        }
    }

    /// Stop timer from excuting its callback
    void stop() {
#if 0
        if (this->timer != nullptr) {
            // asio based timer
            this->timer->cancel();
        }
#endif
    }
};

//using SteadyTimer = Timer<date::utc_clock>;
//using SystemTimer = Timer<date::utc_clock>;
} // namespace Everest

#endif // EVEREST_TIMER_HPP
