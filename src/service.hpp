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

#ifndef CLARA_SERVICE_HPP
#define CLARA_SERVICE_HPP

#include <clara/engine.hpp>

#include "base.hpp"
#include "service_engine.hpp"
#include "service_loader.hpp"

#include <mutex>

namespace clara {

struct ServiceParameters
{
    std::string engine_name;
    std::string engine_lib;
    std::string initial_state;
    std::string description;
    int pool_size;
};


class Service : public Base
{
public:
    Service(const Component& self,
            const Component& frontend,
            const ServiceParameters& params);

    Service(const Service&) = delete;
    Service& operator=(const Service&) = delete;

    ~Service();

public:
    void start();

    void stop();

    void setup(xmsg::Message& msg);

    void configure(xmsg::Message& msg);

    void execute(xmsg::Message& msg);

    void callback(xmsg::Message& msg);

private:
    std::mutex mutex_;
    std::mutex cb_mutex_;

    ServiceLoader loader_;
    std::unique_ptr<ServiceEngine> service_;
    std::unique_ptr<xmsg::Subscription> sub_;
};

} // end namespace clara

#endif // end of include guard: CLARA_SERVICE_HPP