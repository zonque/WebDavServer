#pragma once

#include <string>
#include <libsoup/soup.h>

#include "response.h"

class WebDavResponseSoup : public WebDavResponse {
public:
        WebDavResponseSoup(SoupMessage *msg) : msg(msg) {
                setDavHeaders();
        }
        ~WebDavResponseSoup() {}

        void setStatus(int code, std::string message) override {
                soup_message_set_status_full(msg, code, message.c_str());
        }

        void setHeader(std::string header, std::string value) override {
                soup_message_headers_append(msg->response_headers, header.c_str(), value.c_str());
        }

        void setHeader(std::string header, size_t value) override {
                char tmp[16];
                snprintf(tmp, sizeof(tmp), "%lu", value);
                soup_message_headers_append(msg->response_headers, header.c_str(), tmp);
        }

        bool setContent(const char *buf, size_t len) override {
                soup_message_set_response(msg, contentType.c_str(), SOUP_MEMORY_COPY, buf, len);
                return true;
        }

        bool sendChunk(const char *buf, size_t len) override {
                soup_message_body_append(msg->response_body, SOUP_MEMORY_COPY, buf, len);
                return true;
        }

        void closeChunk() override {}

private:
        SoupMessage *msg;
};