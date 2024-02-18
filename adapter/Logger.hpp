#pragma once

#include <fmt/format.h>

class Logger
{
  public:
    enum class Severity
    {
        None,
        Info,
        Warning,
        Error,
        Debug
    };

    static Logger& getInstance()
    {
        static Logger instance;
        return instance;
    }

    void setSeverity(const Severity& severity) { m_severity = severity; }

    void debug(const std::string& message = std::string()) { log(Severity::Debug, message); }

    void info(const std::string& message = std::string()) { log(Severity::Info, message); }

    void warning(const std::string& message = std::string()) { log(Severity::Warning, message); }

    void error(const std::string& message = std::string()) { log(Severity::Error, message); }

  private:
    explicit Logger()
        : m_severity{Severity::None}
    {}

    void log(const Severity& severity, const std::string& message)
    {
        if (m_severity < severity)
        {
            return;
        }

        switch (severity)
        {
        case Severity::Debug:
            fmt::println("[Debug]: {}", message);
            break;
        case Severity::Info:
            fmt::println("[Info]: {}", message);
            break;
        case Severity::Warning:
            fmt::println("[Warning]: {}", message);
            break;
        case Severity::Error:
            fmt::println("[Error]: {}", message);
            break;
        default:
            break;
        }
    }

    Severity m_severity;
};