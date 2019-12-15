#pragma once

#include <string>
#include <algorithm>
#include <cctype>
#include <functional>

struct Url {
    Url(const std::string& url_s) {
        Parse(url_s);
    }
private:
    void Parse(const std::string& url_s) {
        using namespace std;
        const string prot_end("://");
        string::const_iterator prot_i = search(url_s.begin(), url_s.end(),
                                            prot_end.begin(), prot_end.end());
        protocol_.reserve(distance(url_s.begin(), prot_i));
        transform(url_s.begin(), prot_i,
                back_inserter(protocol_),
                ::tolower); // protocol is icase
        if( prot_i == url_s.end() )
            return;
        advance(prot_i, prot_end.length());
        string::const_iterator path_i = find(prot_i, url_s.end(), '/');
        host_.reserve(distance(prot_i, path_i));
        transform(prot_i, path_i,
                back_inserter(host_),
                ::tolower); // host is icase
        string::const_iterator query_i = find(path_i, url_s.end(), '?');
        path_.assign(path_i, query_i);
        if( query_i != url_s.end() )
            ++query_i;
        query_.assign(query_i, url_s.end());
    }

public:
    std::string Protocol() { return protocol_; }
    std::string Host() { return host_; }
    std::string Path() { return path_; }
    std::string Query() { return query_; }
private:
    std::string protocol_, host_, path_, query_;
};