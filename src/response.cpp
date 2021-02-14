#include <map>
#include <string>
#include <tinyxml2.h>
#include <vector>

#include "response.h"

using namespace tinyxml2;

void WebDavResponse::setDavHeaders() {
        setHeader("DAV", "1");
        setHeader("Allow", "PROPFIND,OPTIONS,DELETE,COPY,MOVE,HEAD,POST,PUT,GET");
}

bool WebDavResponse::flush() {
        if (!responses.empty()) {
                setStatus(207, "Multi-Status");
                contentType = "text/xml; charset=\"utf-8\"";
                XMLDocument doc;

                XMLElement *xmlRoot = doc.NewElement("multistatus");
                xmlRoot->SetAttribute("xmlns", "DAV:");

                for (const auto &r: responses) {
                        XMLElement *xmlResponse = doc.NewElement("response");
                        xmlRoot->InsertEndChild(xmlResponse);

                        XMLElement *e = doc.NewElement("href");
                        e->SetText(r.href.c_str());
                        xmlResponse->InsertEndChild(e);

                        XMLElement *xmlPropstat = doc.NewElement("propstat");
                        xmlResponse->InsertEndChild(xmlPropstat);

                        XMLElement *xmlProp = doc.NewElement("prop");
                        xmlPropstat->InsertEndChild(xmlProp);

                        for (const auto &p: r.props) {
                                XMLElement *e = doc.NewElement(p.first.c_str());
                                e->SetText(p.second.c_str());
                                xmlProp->InsertEndChild(e);
                        }

                        e = doc.NewElement("resourcetype");
                        xmlProp->InsertEndChild(e);

                        if (r.isCollection) {
                                XMLElement *c = doc.NewElement("collection");
                                e->InsertEndChild(c);
                        }

                        e = doc.NewElement("status");
                        e->SetText(r.status.c_str());
                        xmlPropstat->InsertEndChild(e);
                }

                doc.InsertEndChild(xmlRoot);

                XMLPrinter printer;
                printer.PushHeader(false, true);
                doc.Print(&printer);

                return setContent(printer.CStr(), printer.CStrSize()-1);
        }

        return false;
}
