#include "WebRequest.h"

WebRequest::Initializer::Initializer()
{
    hdl = LoadLibrary("D:/CODINGS/with VS/SeqDownWebHost/x64/Release/SeqDownWebHost.dll");
    if (hdl == nullptr)
    {
        throw std::runtime_error("Failed to load dll");
    }
    OnRequestPath = (void(*)(const char*, void*, void (*callback)(void*, void*)))GetProcAddress(hdl, "OnRequestPath");
    ReadRequestBody = (int(*)(void* , char* , unsigned int ))GetProcAddress(hdl, "ReadRequestBody");
    CloseRequest = (void(*)(void*))GetProcAddress(hdl, "CloseRequest");
    StartServer = (void(*)())GetProcAddress(hdl, "StartServer");
}

WebRequest::Initializer::~Initializer()
{
    FreeLibrary(hdl);
}

void WebRequest::ReadCall(void* self, void* request)
{
    reinterpret_cast<WebRequest*>(self)->OnRequest
    (
        [request](std::span<char> buffer)
        {
            return init_obj.ReadRequestBody(request, buffer.data(), buffer.size());
        }, 
        [request]()
        {
            init_obj.CloseRequest(request);
        }
     );
}

void WebRequest::Start()
{
    init_obj.StartServer();
}

WebRequest::WebRequest(const std::string& path , RequestUserType request_user)
{
    init_obj.OnRequestPath(path.c_str(), this, ReadCall);
    OnRequest = request_user;
}