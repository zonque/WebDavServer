#pragma once

#include <string>
#include <vector>
#include <map>

#include <tinyxml2.h>

using namespace tinyxml2;

class WebDavMultiStatusResponse {
public:
        std::string href;
        std::string status;
        std::map<std::string, std::string> props;
        bool isCollection;
};

class WebDavResponse {
public:
        WebDavResponse() : contentType("text/plain") {}

        void setDavHeaders();
        bool flush();
        void setHeader(std::string header, std::string value);
        void setHeader(std::string header, size_t value);

        // Functions that depend on the underlying web server implementation
        virtual void setStatus(int code, std::string message) = 0;
        virtual bool setContent(const char *buf, size_t len) = 0;
        virtual bool sendChunk(const char *buf, size_t len) = 0;
        virtual void closeChunk() = 0;

        std::vector<WebDavMultiStatusResponse> responses;

protected:
        virtual void writeHeader(const char *header, const char *value) = 0;

        std::string contentType;
        std::map<std::string, std::string> headers;
};