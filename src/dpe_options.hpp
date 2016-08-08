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

#ifndef CLARA_DPE_OPTIONS_HPP
#define CLARA_DPE_OPTIONS_HPP

#include "third_party/optionparser.h"

#include <xmsg/address.h>

#include <iostream>
#include <vector>

namespace clara {
namespace option {

using namespace ::option;

struct Arg: public ::option::Arg
{
    static void print_error(const char* msg1, const Option& opt, const char* msg2)
    {
        fprintf(stderr, "%s", msg1);
        fwrite(opt.name, opt.namelen, 1, stderr);
        fprintf(stderr, "%s", msg2);
    }

    static option::ArgStatus Unknown(const Option& option, bool msg)
    {
        if (msg) print_error("Unknown option ", option, "\n");
        return option::ARG_ILLEGAL;
    }

    static option::ArgStatus Required(const Option& option, bool msg)
    {
        if (option.arg != 0) return option::ARG_OK;
        if (msg) print_error("Option ", option, " requires an argument\n");
        return option::ARG_ILLEGAL;
    }
};

enum  optionIndex { UNKNOWN, HOST, PORT, FE_HOST, FE_PORT, HELP };

constexpr Descriptor usage[] =
{
    {UNKNOWN, 0, "", "", Arg::Unknown, "usage: c_dpe [options]\n\n"
                                       "  options:" },
    {HOST, 0,"", "host", Arg::Required, "  --host <hostname>  \tuse given host for this DPE" },
    {PORT, 0,"", "port", Arg::Required, "  --port <port>      \tuse given port for this DPE" },
    {FE_HOST, 0,"", "fe-host", Arg::Required, "  --fe-host <hostname>  \tthe host used by the remote front-end" },
    {FE_PORT, 0,"", "fe-port", Arg::Required, "  --fe-port <port>      \tthe port used by the remote front-end" },
    {HELP, 0,"", "help", Arg::None, "  --help  \tprint usage and exit" },
    {0, 0, 0, 0, 0, 0}
};


class DpeOptionsParser final {
public:
    bool parse(int argc, char* argv[])
    {
        argc -= (argc > 0); argv += (argc > 0);
        Stats stats(usage, argc, argv);

        options.resize(stats.options_max);
        buffer.resize(stats.buffer_max);

        parser = Parser(usage, argc, argv, &options[0], &buffer[0]);

        if (parser.nonOptionsCount() > 0) {
            std::cerr << "Invalid command line arguments." << std::endl;
            return false;
        }

        if (parser.error()) {
            return false;
        }

        if (!options[FE_HOST]) {
            if (options[FE_PORT]) {
                std::cerr << "The remote front-end host is required." << std::endl;
            } else {
                std::cerr << "Missing front-end host argument." << std::endl;
            }
            return false;
        }

        parse_options();
        return true;
    }

    bool has_help()
    {
        return options[HELP];
    }

    void print_help()
    {
        option::printUsage(std::cout, usage);
    }
private:
    void parse_options()
    {
        const auto default_host = std::string{"localhost"};
        const auto default_port = std::string{"7771"};

        // Act as front-end by default but if feHost or fePort are passed
        // act as a worker DPE with remote front-end
        bool fe = !options[FE_HOST] && !options[FE_PORT];

        // Get local DPE address
        auto local_host = value_of(HOST, default_host);
        auto local_port = std::stoi(value_of(PORT, default_port));
        local_addr_ = xmsg::ProxyAddress{local_host, local_port};

        if (fe) {
            // Get local FE address (use same local DPE address)
            fe_addr_ = local_addr_;
        } else {
            // Get remote FE address
            auto local_host = value_of(FE_HOST, default_host);
            auto local_port = std::stoi(value_of(FE_PORT, default_port));
            fe_addr_ = xmsg::ProxyAddress{local_host, local_port};
        }
    }

    std::string value_of(int opt, const std::string& val)
    {
        auto arg = options[opt].arg;
        if (arg) {
            return std::string{arg};
        } else {
            return val;
        }
    }

public:
    xmsg::ProxyAddress local_address() const
    {
        return local_addr_;
    }

    xmsg::ProxyAddress frontend_address() const
    {
        return fe_addr_;
    }

    std::string description()
    {
        return desc_;
    }

private:
    std::vector<Option> options;
    std::vector<Option> buffer;

    Parser parser;

    xmsg::ProxyAddress local_addr_;
    xmsg::ProxyAddress fe_addr_;

    std::string desc_;
};

} // end namespace option
} // end namespace clara

#endif // end of include guard: CLARA_DPE_OPTIONS_HPP