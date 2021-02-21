#pragma once

#include <string>
#include <esp_http_server.h>

#include "request.h"

class WebDavRequestEspIdf : public WebDavRequest {
public:
        WebDavRequestEspIdf(httpd_req_t *req, std::string path) :
                WebDavRequest(path), req(req) {}

        std::string getHeader(std::string name) override {
                size_t len = httpd_req_get_hdr_value_len(req, name.c_str());
                if (len <= 0)
                        return "";

                std::string s;
                s.resize(len);
                httpd_req_get_hdr_value_str(req, name.c_str(), &s[0], len+1);

                return s;
        }

        size_t getContentLength() override {
                if (!req)
                        return 0;

                return req->content_len;
        }

        int readBody(char *buf, int len) override {
                int ret = httpd_req_recv(req, buf, len);
                if (ret == HTTPD_SOCK_ERR_TIMEOUT)
                        /* Retry receiving if timeout occurred */
                        return 0;

                return ret;
        }

private:
        httpd_req_t *req;
};
