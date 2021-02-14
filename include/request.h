#pragma once

#include <string>
#include <tinyxml2.h>

using namespace tinyxml2;

class WebDavRequest {
public:
        enum Depth {
                DEPTH_0 = 0,
                DEPTH_1 = 1,
                DEPTH_INFINITY = 2,
        };

        WebDavRequest(std::string path) : path(path), depth(DEPTH_INFINITY), overwrite(true) {}

        bool parseRequest();
        std::string getDestination();
        bool parseBodyXML();

        std::string getPath() { return path; }
        enum Depth getDepth() { return depth; }
        bool getOverwrite() { return overwrite; }

        // Functions that depend on the underlying web server implementation
        virtual std::string getHeader(std::string name) = 0;
        virtual size_t getContentLength() = 0;
        virtual int readBody(char *buf, int len) = 0;

protected:
        std::string path;
        enum Depth depth;
        bool overwrite;

        XMLDocument xml;
};