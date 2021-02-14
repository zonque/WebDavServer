#include <string>
#include <tinyxml2.h>

#include "request.h"

using namespace tinyxml2;

static const char *xmlElementName(const XMLElement *e, const char **ns) {
        const char *colon = strchr(e->Name(), ':');
        const char *name = e->Name();
        if (colon)
                name = colon+1;

        for (const XMLAttribute *a = e->FirstAttribute(); a; a = a->Next()) {
                const char *aname = a->Name();
                if (strstr(aname, "xmlns") == aname)
                        *ns = a->Value();
        }

        return name;
}

bool WebDavRequest::parseRequest() {
        std::string s;

        s = getHeader("Overwrite");
        if (!s.empty()) {
                if (s == "F")
                        overwrite = false;
                else if (s != "T")
                        return false;
        }

        s = getHeader("Depth");
        if (!s.empty()) {
                if (s == "0")
                        depth = DEPTH_0;
                else if (s == "1")
                        depth = DEPTH_1;
                else if (s != "infinity")
                        return false;
        }

        return true;
}

std::string WebDavRequest::getDestination() {
        std::string destination = getHeader("Destination");
        std::string host = getHeader("Host");

        if (destination.empty() || host.empty())
                return "";

        size_t pos = destination.find(host);
        if (pos == std::string::npos)
                return "";

        return destination.substr(pos + host.length());
}

bool WebDavRequest::parseBodyXML() {
        size_t size = getContentLength();
        char *buf = (char *) malloc(size);
        if (!buf)
                return false;

        bool ret = false;

        if (readBody(buf, size) == (int) size)
                ret = (xml.Parse(buf, size) == XML_SUCCESS);

        free(buf);

        if (!ret)
                return false;

        XMLElement *root = xml.FirstChildElement();
        if (!root)
                return false;

        const char *ns = NULL;
        const char *name = xmlElementName(root, &ns);

        if (ns && strlen(ns) == 0)
                return false;

        if (strcmp(name, "propfind") == 0) {
                XMLElement *prop = root->FirstChildElement();
                if (!prop)
                        return true;

                name = xmlElementName(prop, &ns);
                if (strcmp(name, "prop") == 0) {
                        for (XMLElement *p = prop->FirstChildElement(); p; p = p->NextSiblingElement()) {
                                name = xmlElementName(p, &ns);
                                if (ns && strlen(ns) == 0)
                                        return false;
                        }
                }
        }

        return ret;
}
