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

#ifndef CLARA_CONSTANTS_HPP
#define CLARA_CONSTANTS_HPP

#include <string>

namespace clara {

namespace constants {

constexpr int java_port = 7771;
constexpr int cpp_port = 7781;
constexpr int python_port = 7791;
constexpr int reg_port_shift = 4;

const std::string java_lang = "java";
const std::string python_lang = "python";
const std::string cpp_lang = "cpp";

const std::string dpe = "dpe";
const std::string start_dpe = "startDpe";
const std::string stop_dpe = "stopDpe";
const std::string stop_remote_dpe = "stopRemoteDpe";
const std::string dpe_exit = "dpeExit";
const std::string ping_dpe = "pingDpe";
const std::string ping_remote_dpe = "pingRemoteDpe";
const std::string dpe_alive = "dpeAlive";
const std::string dpe_report = "dpeReport";

const std::string container = "container";
const std::string state_container = "getContainerState";
const std::string start_container = "startContainer";
const std::string start_remote_container = "startRemoteContainer";
const std::string stop_container = "stopContainer";
const std::string stop_remote_container = "stopRemoteContainer";
const std::string container_down = "containerIsDown";
const std::string remove_container = "removeContainer";

const std::string state_service = "getServiceState";
const std::string start_service = "startService";
const std::string start_remote_service = "startRemoteService";
const std::string stop_service = "stopService";
const std::string stop_remote_service = "stopRemoteService";
const std::string deploy_service = "deployService";
const std::string remove_service = "removeService";
const std::string service_report_done = "serviceReportDone";
const std::string service_report_data = "serviceReportData";

const std::string set_front_end = "setFrontEnd";
const std::string set_front_end_remote = "setFrontEndRemote";

const std::string shared_memory_key = "clara/shmkey";

const std::string mapkey_sep = "#";
const std::string data_sep = "?";
const std::string lang_sep = "_";
const std::string port_sep = "%";

const std::string info = "INFO";
const std::string warning = "WARNING";
const std::string error = "ERROR";
const std::string done = "done";
const std::string data = "data";

const std::string udf = "undefined";

} // end namespace constants

} // end namespace clara

#endif // end of include guard: CLARA_CONSTANTS_HPP
