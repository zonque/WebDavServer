#pragma once

#include <string>
#include <esp_http_server.h>

#include "response.h"

class WebDavResponseEspIdf : public WebDavResponse {
public:
        WebDavResponseEspIdf(httpd_req_t *req) : req(req), status(NULL) {
                setDavHeaders();
        }

        ~WebDavResponseEspIdf() {
                free(status);
        }

        void setStatus(int code, std::string message) override {
                free(status);
                status = NULL;

                asprintf(&status, "%d %s", code, message.c_str());
                httpd_resp_set_status(req, status);
        }

        void writeHeader(const char *header, const char *value) override {
                httpd_resp_set_hdr(req, header, value);
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
        char *status;
};
