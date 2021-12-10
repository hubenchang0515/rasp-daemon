#ifndef RASPD_SESSION_HELLO_H
#define RASPD_SESSION_HELLO_H

#include "Service.h"

class Hello
{
public:
    Hello():
        m_service(new Rasp::DBus::Service{"org.planc.raspd.Hello"}),
        m_object(new Rasp::DBus::Object("/org/planc/raspd/Hello")),
        m_interface1(new Rasp::DBus::Interface("org.planc.raspd.Hello1")),
        m_interface2(new Rasp::DBus::Interface("org.planc.raspd.Hello2")),
        m_methodHello(new Rasp::DBus::Method("SayHello", Rasp::DBus::Method::warp(this, &Hello::hello), {{"name", "s"}}, {{"ret", "s"}})),
        m_propertyName(new Rasp::DBus::Property("name", "s", RASP_WARP_GET(getName), RASP_WARP_SET(setName)))
    {
        Rasp::DBus::Service::registerService(m_service);
        m_interface1->exportMethod(m_methodHello);
        m_interface2->exportMethod(m_methodHello);
        m_interface1->exportProperty(m_propertyName);
        m_interface2->exportProperty(m_propertyName);
        m_object->exportInterface(m_interface1);
        m_object->exportInterface(m_interface2);
        m_service->exportObject(m_object);

        printf("%s\n", m_object->XML().c_str());
    }

    ~Hello()
    {
        Rasp::DBus::Service::unregisterService(m_service->name());
    }

protected:
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
    void hello(const Glib::VariantContainerBase& args,
                const Glib::RefPtr<Gio::DBus::MethodInvocation>& invocation)
    {
        Glib::Variant<Glib::ustring> name;
        args.get_child(name, 0);
        m_name = name.get();
        const auto var = Glib::Variant<Glib::ustring>::create(Glib::ustring::compose("hello %1", name.get()));
        Glib::VariantContainerBase response = Glib::VariantContainerBase::create_tuple(var);
        invocation->return_value(response);
    }

    /*****************************************************************************
     * @brief 回调函数，DBus 属性读取
     * @param[in] property 属性
     * @param[in] connection 连接
     * @param[in] sender 发送方
     * @param[in] objectPath 对象路径
     * @param[in] interfaceName 接口名
     * @param[in] propertyName 属性名
     * ***************************************************************************/
    void getName(Glib::VariantBase& property,
                        const Glib::RefPtr<Gio::DBus::Connection>& connection,
                        const Glib::ustring& sender,
                        const Glib::ustring& objectPath,
                        const Glib::ustring& interfaceName,
                        const Glib::ustring& propertyName)
    {
        auto name = Glib::Variant<Glib::ustring>::create(m_name);
        property = name;
    }

    /*****************************************************************************
     * @brief 回调函数，DBus 属性读取
     * @param[in] connection 连接
     * @param[in] sender 发送方
     * @param[in] objectPath 对象路径
     * @param[in] interfaceName 接口名
     * @param[in] propertyName 属性名
     * @param[in] value 属性值
     * ***************************************************************************/
    bool setName(const Glib::RefPtr<Gio::DBus::Connection>& connection,
                    const Glib::ustring& sender,
                    const Glib::ustring& objectPath,
                    const Glib::ustring& interfaceName,
                    const Glib::ustring& propertyName,
                    const Glib::VariantBase& value)
    {
        auto name = Glib::VariantBase::cast_dynamic<Glib::Variant<Glib::ustring>>(value);
        m_name = name.get();
        return true;
    }

private:
    Glib::RefPtr<Rasp::DBus::Service>     m_service;
    Glib::RefPtr<Rasp::DBus::Object>      m_object;
    Glib::RefPtr<Rasp::DBus::Interface>   m_interface1;
    Glib::RefPtr<Rasp::DBus::Interface>   m_interface2;

    // 方法名为hello, 调用this->hello函数, 参数为(String name), 返回值为 (String ret)
    Glib::RefPtr<Rasp::DBus::Method> m_methodHello;

    Glib::RefPtr<Rasp::DBus::Property> m_propertyName;

    Glib::ustring m_name;
};

#endif // RASP_SESSION_HELLO_H