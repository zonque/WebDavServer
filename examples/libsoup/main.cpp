#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#include <libsoup/soup.h>
#include <glib/gstdio.h>

#include <server.h>

#include "request-soup.h"
#include "response-soup.h"

static void
server_callback(SoupServer        *server,
                SoupMessage       *msg, 
                const char        *path,
                GHashTable        *query,
                SoupClientContext *client,
                gpointer           user_data)
{
  WebDav::Server *ws = (WebDav::Server *) user_data;
  WebDav::ResponseSoup resp(msg);

  soup_message_headers_append(msg->response_headers, "Access-Control-Allow-Origin", "*");
  soup_message_headers_append(msg->response_headers, "Access-Control-Allow-Headers", "*");
  soup_message_headers_append(msg->response_headers, "Access-Control-Allow-Methods", "*");

  printf(">>> %s %s\n", msg->method, path);

  std::string p = ws->uriToPath(path);
  if (p.empty()) {
          resp.setStatus(400, "Invalid path");
          return;
  }

  WebDav::RequestSoup req(msg, p);
  if (!req.parseRequest()) {
          resp.setStatus(400, "Invalid headers");
          return;
  }

  int ret = 0;

  if (msg->method == SOUP_METHOD_COPY)
    ret = ws->doCopy(req, resp);
  else if (msg->method == SOUP_METHOD_DELETE)
    ret = ws->doDelete(req, resp);
  else if (msg->method == SOUP_METHOD_GET)
    ret = ws->doGet(req, resp);
  else if (msg->method == SOUP_METHOD_HEAD)
    ret = ws->doHead(req, resp);
  else if (msg->method == SOUP_METHOD_LOCK)
    ret = ws->doLock(req, resp);
  else if (msg->method == SOUP_METHOD_MKCOL)
    ret = ws->doMkcol(req, resp);
  else if (msg->method == SOUP_METHOD_MOVE)
    ret = ws->doMove(req, resp);
  else if (msg->method == SOUP_METHOD_OPTIONS)
    ret = ws->doOptions(req, resp);
  else if (msg->method == SOUP_METHOD_PROPFIND)
    ret = ws->doPropfind(req, resp);
  else if (msg->method == SOUP_METHOD_PROPPATCH)
    ret = ws->doProppatch(req, resp);
  else if (msg->method == SOUP_METHOD_PUT)
    ret = ws->doPut(req, resp);
  else if (msg->method == SOUP_METHOD_UNLOCK)
    ret = ws->doUnlock(req, resp);
  else
    soup_message_set_status(msg, SOUP_STATUS_NOT_IMPLEMENTED);

  soup_message_set_status(msg, ret);

  printf("<<< %s %s  (ret %d)\n", msg->method, path, ret);
  resp.flushHeaders();
  resp.closeBody();
}

int main (int argc, char **argv)
{
  GMainLoop *loop;
  SoupServer *server;
  WebDav::Server ws("dav-root", "/dav");

  server = soup_server_new(SOUP_SERVER_SERVER_HEADER, "webdav-emulator", NULL);
  if (!server) {
    g_print("Error creating server\n");
    return 1;
  }

  soup_server_add_handler(server, NULL, server_callback, &ws, NULL);
  soup_server_listen_local(server, 9999, (SoupServerListenOptions) 0, NULL);
  g_print("Waiting for requests...\n");

  loop = g_main_loop_new(NULL, TRUE);
  g_main_loop_run(loop);

  return 0;
}
