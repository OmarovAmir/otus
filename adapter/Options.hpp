#pragma once

#include <Logger.hpp>
#include <boost/bimap.hpp>
#include <boost/program_options.hpp>
#include <fmt/ranges.h>
#include <thread>

namespace po = boost::program_options;

template <typename T> auto make_notifier(const std::string& param_name)
{
    return [param_name](T param) { fmt::println("{}: {}", param_name, param); };
}

template <typename T> auto make_min_max_notifier(const std::string& param_name, T min, T max)
{
    return [param_name, min, max](T param)
    {
        if (param < min || param > max)
        {
            throw po::validation_error(po::validation_error::invalid_option_value, param_name, std::to_string(param));
        }
        fmt::println("{}: {}", param_name, param);
    };
}

template <typename L, typename R>
boost::bimap<boost::bimaps::set_of<L>, boost::bimaps::set_of<R>>
makeBimap(const std::initializer_list<typename boost::bimap<L, R>::value_type>& list)
{
    return boost::bimap<boost::bimaps::set_of<L>, boost::bimaps::set_of<R>>(list.begin(), list.end());
}

const boost::bimap<boost::bimaps::set_of<Logger::Severity>, boost::bimaps::set_of<std::string>>
    loggerSeverityStringBimap =
        makeBimap<Logger::Severity, std::string>({{Logger::Severity::None, std::string("none")},
                                                  {Logger::Severity::Info, std::string("info")},
                                                  {Logger::Severity::Warning, std::string("warning")},
                                                  {Logger::Severity::Error, std::string("error")},
                                                  {Logger::Severity::Debug, std::string("debug")}});

const std::string& typeToString(const Logger::Severity type)
{
    auto iter = loggerSeverityStringBimap.left.find(type);
    if (iter == loggerSeverityStringBimap.left.end())
        throw std::logic_error("No such type!");
    return iter->second;
}

Logger::Severity typeFromString(const std::string& type)
{
    auto iter = loggerSeverityStringBimap.right.find(type);
    if (iter == loggerSeverityStringBimap.right.end())
        throw std::logic_error("No such type!");
    return iter->second;
}

auto format_as(const Logger::Severity& loggerSeverity) { return typeToString(loggerSeverity); }

std::istream& operator>>(std::istream& in, Logger::Severity& loggerSeverity)
{
    try
    {
        std::string loggerSeverityString;
        in >> loggerSeverityString;
        loggerSeverity = typeFromString(loggerSeverityString);
    }
    catch (std::exception& e)
    {
        in.setstate(std::ios_base::failbit);
    }
    return in;
}

std::ostream& operator<<(std::ostream& out, const Logger::Severity& loggerSeverity)
{
    try
    {
        out << typeToString(loggerSeverity);
    }
    catch (std::exception& e)
    {
        out.setstate(std::ios_base::failbit);
    }
    return out;
}

po::options_description opts_init()
{
    po::options_description opts{"Options"};
    auto optsInit = opts.add_options();
    // Help
    optsInit("help,h", "this screen");
    // Ports
    optsInit("ports,p",
             po::value<std::vector<std::size_t>>()
                 ->multitoken()
                 ->composing()
                 ->notifier(make_notifier<std::vector<std::size_t>>("Ports"))
                 ->required(),
             "ports");
    // Context number
    optsInit(
        "context-number,c",
        po::value<std::size_t>()
            ->default_value(4)
            ->notifier(make_min_max_notifier<std::size_t>("Context number", 1, 8)),
        "context number possible values [1;8]");
    // Logger severity
    optsInit("logger-severity,s",
             po::value<Logger::Severity>()
                 ->default_value(Logger::Severity::None)
                 ->notifier(make_notifier<Logger::Severity>("Logger severity")),
             "logger severity {none, info, warning, error, debug}");
    optsInit("verbose,v",
             po::value<bool>()->default_value(false)->implicit_value(true)->notifier(make_notifier<bool>("Verbose")),
             "verbose");
    return opts;
}

po::variables_map parse_options(int argc, char** argv, const po::options_description& opts)
{
    po::variables_map vm;
    po::store(po::parse_command_line(argc, argv, opts), vm);
    if (vm["verbose"].as<bool>())
    {
        fmt::println("Options:");
        po::notify(vm);
        fmt::println("");
    }
    return vm;
}
