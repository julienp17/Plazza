/*
** EPITECH PROJECT, 2021
** plazza [WSL: Ubuntu-20.04]
** File description:
** Logger
*/


#ifndef INC_LOGGER_HPP_
#define INC_LOGGER_HPP_

#define FILELOG_MAX_LEVEL ldebug

#include <sstream>
#include <string>
#include <stdio.h>
#include <sys/time.h>

static inline std::string getTime() {
    char buffer[11] = "";
    char result[100] = "";
    time_t t;
    tm r;
    struct timeval tv;

    time(&t);
    strftime(buffer, sizeof(buffer), "%X", localtime_r(&t, &r));
    gettimeofday(&tv, 0);
    std::sprintf(result, "%s.%03ld", buffer, (long)tv.tv_usec / 1000);
    return result;
}


enum TLogLevel {
    lerror,
    lwarning,
    linfo,
    ldebug
};

template <typename T>
class Log {
 public:
    Log(void) {}
    virtual ~Log();
    std::ostringstream& Get(TLogLevel level = linfo);

    static TLogLevel& ReportingLevel();
    static std::string ToString(TLogLevel level);
    static TLogLevel FromString(const std::string& level);

 private:
    std::ostringstream os;
    Log(const Log&);
    Log &operator=(const Log&);
};

template <typename T>
std::ostringstream &Log<T>::Get(TLogLevel level) {
    os << getTime();
    os << " " << ToString(level) << ": ";
    os << std::string(level > ldebug ? level - ldebug : 0, '\t');
    return os;
}

template <typename T>
Log<T>::~Log() {
    os << std::endl;
    T::Output(os.str());
}

template <typename T>
TLogLevel &Log<T>::ReportingLevel() {
    static TLogLevel reportingLevel = ldebug;

    return reportingLevel;
}

template <typename T>
std::string Log<T>::ToString(TLogLevel level) {
    static const char* const buffer[] = {"ERROR", "WARNING", "INFO", "DEBUG"};

    return buffer[level];
}

template <typename T>
TLogLevel Log<T>::FromString(const std::string& level) {
    if (level == "DEBUG")
        return ldebug;
    if (level == "INFO")
        return linfo;
    if (level == "WARNING")
        return lwarning;
    if (level == "ERROR")
        return lerror;
    Log<T>().Get(lwarning) << "Unknown logging level '" << level
        << "'. Using INFO level as default.";
    return linfo;
}

class Output2FILE {
 public:
    static FILE* &Stream();
    static void Output(const std::string& msg);
};

inline FILE*& Output2FILE::Stream() {
    static FILE* pStream = stderr;
    return pStream;
}

inline void Output2FILE::Output(const std::string& msg) {
    FILE* pStream = Stream();

    if (!pStream)
        return;
    fprintf(pStream, "%s", msg.c_str());
    fflush(pStream);
}

class FILELog : public Log<Output2FILE> {};

#define FILE_LOG(level) \
    if (level > FILELOG_MAX_LEVEL) ;\
    else if (level > FILELog::ReportingLevel() || !Output2FILE::Stream()) ; \
    else FILELog().Get(level)

inline void initLogger(const char * file, TLogLevel level) {
    FILELog::ReportingLevel() = level;
    FILE* log_fd = fopen(file, "w");
    Output2FILE::Stream() = log_fd;
}

inline void endLogger() {
    if (Output2FILE::Stream())
        fclose(Output2FILE::Stream());
}

#endif  // INC_LOGGER_HPP_
