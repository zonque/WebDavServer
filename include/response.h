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

        // Functions that depend on the underlying web server implementation
        virtual void setStatus(int code, std::string message) = 0;
        virtual void setHeader(std::string header, std::string value) = 0;
        virtual void setHeader(std::string header, size_t value) = 0;
        virtual bool setContent(const char *buf, size_t len) = 0;
        virtual bool sendChunk(const char *buf, size_t len) = 0;
        virtual void closeChunk() = 0;

        std::vector<WebDavMultiStatusResponse> responses;

protected:
        std::string contentType;
};