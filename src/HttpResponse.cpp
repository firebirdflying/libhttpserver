#include <cstdio>
#include "HttpResponse.hpp"
#include "Webserver.hpp"

using namespace std;

namespace httpserver
{
//RESPONSE
HttpResponse::HttpResponse():
	content("{}"),
	responseCode(200),
	fp(-1)
{
	this->setHeader(HttpUtils::http_header_content_type, "application/json");
}

HttpResponse::HttpResponse
(
	const string& content, 
	int responseCode,
	const std::string& contentType,
	const HttpResponse::ResponseType_T& responseType
):
	responseType(responseType)
{
	HttpResponseInit(content, responseCode, contentType, responseType);
}

void HttpResponse::HttpResponseInit
(
	const string& content,
	int responseCode,
	const std::string& contentType,
	const HttpResponse::ResponseType_T& responseType
)
{
	if(responseType == HttpResponse::FILE_CONTENT)
	{
		FILE* f;
		if(!(f = fopen(content.c_str(), "r")))
		{
			this->content = NOT_FOUND_ERROR;
			this->responseCode = 404;
			this->setHeader(HttpUtils::http_header_content_type, contentType);
			this->fp = -1;
		}
		else
		{
			this->responseCode = responseCode;
			this->filename = content;
			this->fp = fileno(f);
		}
	}
	else
	{
		this->content = content;
		this->responseCode = responseCode;
		this->setHeader(HttpUtils::http_header_content_type, contentType);
		this->fp = -1;
	}
}

HttpResponse::HttpResponse(const HttpResponse& o)
{
	this->content = o.content;
	this->responseCode = o.responseCode;
	this->headers = o.headers;
	this->footers = o.footers;
}

void HttpResponse::setContentType(const string& value)
{
	this->headers[HttpUtils::http_header_content_type] = value;
}

void HttpResponse::setHeader(const string& key, const string& value)
{
	this->headers[key] = value;
}

void HttpResponse::setFooter(const string& key, const string& value)
{
	this->footers[key] = value;
}

const string HttpResponse::getHeader(const string& key) 
{
	return this->headers[key];
}

const string HttpResponse::getFooter(const string& key) 
{
	return this->footers[key];
}

void HttpResponse::setContent(const string& content) 
{
	this->content = content;
}

const string HttpResponse::getContent() 
{
	return this->content;
}

void HttpResponse::removeHeader(const string& key) 
{
	this->headers.erase(key);
}

const std::vector<std::pair<std::string, std::string> > HttpResponse::getHeaders() 
{
	std::vector<std::pair<std::string, std::string> > toRet;
	map<string, string, HeaderComparator>::const_iterator it;
	for(it = headers.begin(); it != headers.end(); it++)
		toRet.push_back(make_pair<string, string>((*it).first,(*it).second));
	return toRet;
}

const std::vector<std::pair<std::string, std::string> > HttpResponse::getFooters() 
{
	std::vector<std::pair<std::string, std::string> > toRet;
	map<string, string, ArgComparator>::const_iterator it;
	for(it = footers.begin(); it != footers.end(); it++)
		toRet.push_back(make_pair<string, string>((*it).first,(*it).second));
	return toRet;
}

void HttpResponse::setHeaders(const map<string, string>& headers) 
{
	map<string, string>::const_iterator it;
	for(it = headers.begin(); it != headers.end(); it ++)
		this->headers[it->first] = it->second;
}

void HttpResponse::setFooters(const map<string, string>& footers) 
{
	map<string, string>::const_iterator it;
	for(it = footers.begin(); it != footers.end(); it ++)
		this->footers[it->first] = it->second;
}

int HttpResponse::getResponseCode() 
{
	return this->responseCode;
}

void HttpResponse::setResponseCode(int responseCode) 
{
	this->responseCode = responseCode;
}

};