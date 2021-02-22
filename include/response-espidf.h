#pragma once

#include <string>
#include <esp_http_server.h>

#include "response.h"

class WebDavResponseEspIdf : public WebDavResponse {
public:
        WebDavResponseEspIdf(httpd_req_t *req) : req(req), status(NULL), chunked(false) {
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

        void setContentType(const char *ct) override {
                httpd_resp_set_type(req, ct);
        }

        bool sendChunk(const char *buf, ssize_t len = -1) override {
                chunked = true;

                if (len == -1)
                        len = strlen(buf);

                return httpd_resp_send_chunk(req, buf, len) == ESP_OK;
        }

        void closeChunk() override {
                httpd_resp_send_chunk(req, NULL, 0);
        }

        void closeBody() override {
                if (!chunked)
                        httpd_resp_send(req, "", 0);
        }

private:
        httpd_req_t *req;
        char *status;
        bool chunked;
};
