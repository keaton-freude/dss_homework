#include "Poco/StreamCopier.h"
#include "Poco/URI.h"
#include "Poco/Exception.h"
#include "Poco/SharedPtr.h"
#include "Poco/Net/SSLManager.h"
#include "Poco/Net/KeyConsoleHandler.h"
#include "Poco/Net/ConsoleCertificateHandler.h"
#include "Poco/Net/HTTPSClientSession.h"
#include "Poco/Net/HTTPRequest.h"
#include "Poco/Net/HTTPResponse.h"

#include <sstream>
#include <utility>
#include <iostream>

#include "HTTPSClient.h"


using namespace dss;
using namespace Poco;
using namespace Poco::Net;


HTTPSClient::HTTPSClient(std::optional<Proxy> proxy)
    : _proxy(proxy)
{
    Poco::Net::initializeSSL();

    SharedPtr<InvalidCertificateHandler> ptrCert = new ConsoleCertificateHandler(false);
    Context::Ptr ptrContext = new Context(Context::CLIENT_USE, "", "", "cacert.pem", Context::VERIFY_STRICT);
    SSLManager::instance().initializeClient(0, ptrCert, ptrContext);
}

HTTPSClient::~HTTPSClient() {
    Poco::Net::uninitializeSSL();
}

std::string HTTPSClient::Get(const std::string &uriString, std::vector<std::pair<std::string, std::string>> queryParams) {
    URI uri(uriString);
    for(const auto &queryParam : queryParams) {
        uri.addQueryParameter(queryParam.first, queryParam.second);
    }

    HTTPSClientSession s(uri.getHost(), uri.getPort());

    if (_proxy.has_value()) {
        std::cout << "Applying proxy settings to HTTPSClient" << std::endl;
        s.setProxy(_proxy->proxyHost, _proxy->proxyPort);
    }

    HTTPRequest request(HTTPRequest::HTTP_GET, uri.getPathAndQuery());
    s.sendRequest(request);
    HTTPResponse response;
    // Only return a body on a 200 response
    if (response.getStatus() == HTTPResponse::HTTPStatus::HTTP_OK) {
        std::istream& rs = s.receiveResponse(response);
        std::string responseStr;
        StreamCopier::copyToString(rs, responseStr);

        return responseStr;
    }

    // Otherwise, throw an exception with the response code and reason
    std::stringstream ss;
    ss << "HTTP GET Failed. StatusCode: " << response.getStatus() << " Reason: " << response.getReason() << "\n";
    throw std::runtime_error(ss.str().c_str());
}

// Just defer to the queryParam version, with no queryParams
std::string HTTPSClient::Get(const std::string &uriString) {
    return Get(uriString, {});
}