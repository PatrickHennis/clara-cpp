/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 4 -*- */
/* vim: set ts=8 sts=4 et sw=4 tw=80: */
/*
 * Copyright (c) 2016.  Jefferson Lab (JLab). All rights reserved. Permission
 * to use, copy, modify, and distribute  this software and its documentation for
 * educational, research, and not-for-profit purposes, without fee and without a
 * signed licensing agreement.
 *
 * IN NO EVENT SHALL JLAB BE LIABLE TO ANY PARTY FOR DIRECT, INDIRECT, SPECIAL
 * INCIDENTAL, OR CONSEQUENTIAL DAMAGES, INCLUDING LOST PROFITS, ARISING
 * OUT OF THE USE OF THIS SOFTWARE AND ITS DOCUMENTATION, EVEN IF JLAB HAS
 * BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * JLAB SPECIFICALLY DISCLAIMS ANY WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE. THE CLARA SOFTWARE AND ACCOMPANYING DOCUMENTATION, IF ANY,
 * PROVIDED HEREUNDER IS PROVIDED "AS IS". JLAB HAS NO OBLIGATION TO PROVIDE
 * MAINTENANCE, SUPPORT, UPDATES, ENHANCEMENTS, OR MODIFICATIONS.
 *
 * This software was developed under the United States Government license.
 * For more information contact author at gurjyan@jlab.org
 * Department of Experimental Nuclear Physics, Jefferson Lab.
 */

#ifndef CLARA_CONCURRENT_UTILS_HPP
#define CLARA_CONCURRENT_UTILS_HPP

#include <algorithm>
#include <atomic>
#include <memory>
#include <mutex>
#include <vector>

namespace clara {
namespace util {

template <typename T>
class ConcurrentRange
{
public:
    using const_iterator = typename T::const_iterator;

public:
    ConcurrentRange(std::shared_ptr<T> cont)
      : cont_{std::move(cont)}
    {
        // nop
    }

    const_iterator begin() const { return cont_->begin(); };

    const_iterator end() const { return cont_->end(); };

private:
    std::shared_ptr<T> cont_;
};


template <typename T>
class CopyOnWriteVector
{
public:
    using pointer_type = std::shared_ptr<T>;
    using container_type = std::vector<pointer_type>;
    using range_type = util::ConcurrentRange<container_type>;

    CopyOnWriteVector()
      : cont_{std::make_shared<container_type>()}
    {
        // nop
    }

    void add(const pointer_type& elem)
    {
        std::lock_guard<std::mutex> lock{m_};
        auto copy = std::make_shared<container_type>(*cont_);
        copy->push_back(elem);
#if !defined(__APPLE__) && defined(__GNUC__) && __GNUC__ < 5
        cont_ = copy;
#else
        std::atomic_exchange(&cont_, copy);
#endif
    }

    void remove(const pointer_type& elem)
    {
        std::lock_guard<std::mutex> lock{m_};
        auto copy = std::make_shared<container_type>(*cont_);
        copy->erase(std::remove(copy->begin(), copy->end(), elem),
                    copy->end());
#if !defined(__APPLE__) && defined(__GNUC__) && __GNUC__ < 5
        cont_ = copy;
#else
        std::atomic_exchange(&cont_, copy);
#endif
    }

    range_type view() const
    {
#if !defined(__APPLE__) && defined(__GNUC__) && __GNUC__ < 5
        std::lock_guard<std::mutex> lock{m_};
        return {cont_};
#else
        return {std::atomic_load(&cont_)};
#endif
    }

private:
    mutable std::mutex m_;
    std::shared_ptr<container_type> cont_;
};

} // end namespace util
} // end namespace clara

#endif // end of include guard: CLARA_CONCURRENT_UTILS_HPP
