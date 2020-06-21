#include "cstdlib"
#include "cstdio"
#include "cstring"
#include "HttpRequest.h"
#include "HttpResponse.h"
#include "ls_error.h"
#include "Session.h"
#include "sys/socket.h"

namespace ls
{
	Session::Session(int buffersize = 1 << 20) : sendBuffer(buffersize), recvBuffer(buffersize)
	{
	
	}
	HttpRequest& Session::Request()
	{
		return request;
	}
	HttpResponse &Session::Response()
	{
		return response;
	}
	Buffer &Session::RecvBuffer()
	{
		return recvBuffer;
	}
	Buffer &Session::SendBuffer()
	{
		return sendBuffer;
	}
	int Session::PushHeader()
	{
		FUNCTION_TRACE();
		int ret;
		err_throw(ret = recvBuffer.Find("\r\n\r\n", 4));
		Text header(ret, '\0');
		recvBuffer.Pop(header);
		recvBuffer.MoveOffset(4);
		ls_log_tag(SESSION_LOG, "%s", header.c_str());
		return request.ParseFrom(header);
	}

	int Session::PushBody()
	{
		FUNCTION_TRACE();
		if(request.Method() == "GET" || request.Method() == "DELETE")
		{
			request.Body() = "";
			return LS_OK;
		}
		Text contentLength = request.GetAttribute("Content-Length");
		err_with(contentLength.compare(""), == 0, "no content length", return LS_LENGTH_REQUIRED);
		int len = atoi(contentLength.c_str());
		err_with(recvBuffer.Size(), < len, "", return LS_INCOMPLETE);
		request.Body().resize(len);
		recvBuffer.Pop(request.Body());
		return LS_OK;
	}
	int Session::Send()
	{
		int n = 0;
		while((n = send(response.Fd(), sendBuffer.Begin(), sendBuffer.Size(), 0)) > 0)
			sendBuffer.MoveOffset(n);
		return sendBuffer.Size() == 0 ? LS_OK : LS_FAILED;
	}
};

