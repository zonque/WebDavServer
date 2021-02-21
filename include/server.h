#pragma once

#include "request.h"
#include "response.h"

class WebDavServer {
public:
        WebDavServer(std::string rootPath, std::string rootURI);
        ~WebDavServer() {};

        std::string pathToURI(std::string path);
        std::string uriToPath(std::string uri);

        int doCopy(WebDavRequest &req, WebDavResponse &resp);
        int doDelete(WebDavRequest &req, WebDavResponse &resp);
        int doGet(WebDavRequest &req, WebDavResponse &resp);
        int doHead(WebDavRequest &req, WebDavResponse &resp);
        int doLock(WebDavRequest &req, WebDavResponse &resp);
        int doMkcol(WebDavRequest &req, WebDavResponse &resp);
        int doMove(WebDavRequest &req, WebDavResponse &resp);
        int doOptions(WebDavRequest &req, WebDavResponse &resp);
        int doPropfind(WebDavRequest &req, WebDavResponse &resp);
        int doProppatch(WebDavRequest &req, WebDavResponse &resp);
        int doPut(WebDavRequest &req, WebDavResponse &resp);
        int doUnlock(WebDavRequest &req, WebDavResponse &resp);

private:
        std::string rootPath, rootURI;

        std::string formatTime(time_t t);
        int sendPropResponse(WebDavResponse &resp, std::string path, int recurse);
        void sendMultiStatusResponse(WebDavResponse &resp, WebDavMultiStatusResponse &msr);
};