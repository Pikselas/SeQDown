#pragma once
#include<Windows.h>
#include<functional>
#include<span>
#include <stdexcept>
class WebRequest
{
private:
    class Initializer
    {
    private:
        HMODULE hdl;
    public:
        std::function<void(const char*, void*, void(void*, void*))> OnRequestPath;
        std::function<int(void*, char*, unsigned int)> ReadRequestBody;
        std::function<void(void*)> CloseRequest;
        std::function<void()> StartServer;
    public:
        Initializer();
        ~Initializer();
    };
private:
    static Initializer init_obj;
public:
    using closer_type = std::function<void()>;
    using reader_type = std::function<int(std::span<char>)>;
private:
    using RequestUserType = std::function<void(reader_type, closer_type)>;
    RequestUserType OnRequest;
private:
    static void ReadCall(void* self, void* request);
public:
    static void Start();
public:
    WebRequest(const std::string& path , RequestUserType request_user);
};
