#ifndef RASPD_SESSION_HELLO_H
#define RASPD_SESSION_HELLO_H

#include "Service.h"

class Hello
{
public:
    Hello():
        m_service(new Rasp::DBus::Service{"org.planc.raspd.Hello"}),
        m_object(new Rasp::DBus::Object("/org/planc/raspd/Hello")),
        m_interface(new Rasp::DBus::Interface("org.planc.raspd.Hello")),
        m_methodHello(new Rasp::DBus::Method("SayHello", RASP_WARP_METHOD(hello), {{"name", "s"}}, {{"ret", "s"}}))
    {
        m_interface->exportMethod(m_methodHello);
        m_object->exportInterface(m_interface);
        m_service->exportObject(m_object);

        Rasp::DBus::Service::registerService(m_service);
    }

    ~Hello()
    {
        Rasp::DBus::Service::unregisterService(m_service->name());
    }

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
                                const Glib::RefPtr<Gio::DBus::MethodInvocation>& invocation)
    {
        Glib::Variant<Glib::ustring> name;
        args.get_child(name, 0);
        const auto var = Glib::Variant<Glib::ustring>::create(Glib::ustring::compose("hello %1", name.get()));
        Glib::VariantContainerBase response = Glib::VariantContainerBase::create_tuple(var);
        invocation->return_value(response);
    }

private:
    Glib::RefPtr<Rasp::DBus::Service>     m_service;
    Glib::RefPtr<Rasp::DBus::Object>      m_object;
    Glib::RefPtr<Rasp::DBus::Interface>   m_interface;

    // 方法名为hello, 调用this->hello函数, 参数为(String name), 返回值为 (String ret)
    Glib::RefPtr<Rasp::DBus::Method> m_methodHello;
};

#endif // RASP_SESSION_HELLO_H