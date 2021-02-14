#pragma once

#include <string>
#include <esp_http_server.h>

#include "response.h"

class WebDavResponseEspIdf : public WebDavResponse {
public:
        WebDavResponseEspIdf(httpd_req_t *req) : req(req) {
                setDavHeaders();
        }

        void setStatus(int code, std::string message) override {
                char *s;
                asprintf(&s, "%d %s", code, message.c_str());
                httpd_resp_set_status(req, s);
                free(s);
        }

        void setHeader(std::string header, std::string value) override {
                httpd_resp_set_hdr(req, header.c_str(), value.c_str());
        }

        void setHeader(std::string header, size_t value) override {
                char tmp[32];
                snprintf(tmp, sizeof(tmp), "%u", value);
                httpd_resp_set_hdr(req, header.c_str(), tmp);
        }

        bool setContent(const char *buf, size_t len) override {
                // httpd_resp_set_type(req, HTTPD_TYPE_);
                return httpd_resp_send(req, buf, len) == ESP_OK;
        }

        bool sendChunk(const char *buf, size_t len) override {
                return httpd_resp_send_chunk(req, buf, len) == ESP_OK;
        }

        void closeChunk() override {
                httpd_resp_send_chunk(req, NULL, 0);
        }

private:
        httpd_req_t *req;
};
