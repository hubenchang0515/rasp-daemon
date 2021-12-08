#ifndef RASPD_SESSION_HELLO_H
#define RASPD_SESSION_HELLO_H

#include "Service.h"

class Hello
{
public:
    Hello();
    ~Hello();

    /*****************************************************************************
     * @brief 回调函数，DBus 方法调用
     * @param[in] connection DBus连接
     * @param[in] sender 发送方
     * @param[in] objectPath 对象路径
     * @param[in] interfaceName 接口名
     * @param[in] methodName 方法名
     * @param[in] args 参数
     * @param[in] invocation 
     * ***************************************************************************/
    void hello(const Glib::RefPtr<Gio::DBus::Connection>& connection,
                                const Glib::ustring& sender,
                                const Glib::ustring& objectPath,
                                const Glib::ustring& interfaceName,
                                const Glib::ustring& methodName,
                                const Glib::VariantContainerBase& args,
                                const Glib::RefPtr<Gio::DBus::MethodInvocation>& invocation);

private:
    Rasp::DBus::Service     m_service{"org.planc.raspd.Hello"};
    Rasp::DBus::Object      m_object{"/org/planc/raspd/Hello"};
    Rasp::DBus::Interface   m_interface{"org.planc.raspd.Hello"};

    RASP_WARP_METHOD("hello", hello, {}, {{"ret", "s"}});

    int n = 0;
};

#endif // RASP_SESSION_HELLO_H