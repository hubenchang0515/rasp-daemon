#include "Hello.h"

Hello::Hello()
{
    m_interface.exportMethod(RASP_WARP_NAME(hello));
    m_object.exportInterface(m_interface);
    m_service.exportObject(m_object);

    Rasp::DBus::Service::registerService(&m_service);
}

Hello::~Hello()
{
    Rasp::DBus::Service::unregisterService(&m_service);
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
void Hello::hello(const Glib::RefPtr<Gio::DBus::Connection>& connection,
                            const Glib::ustring& sender,
                            const Glib::ustring& objectPath,
                            const Glib::ustring& interfaceName,
                            const Glib::ustring& methodName,
                            const Glib::VariantContainerBase& args,
                            const Glib::RefPtr<Gio::DBus::MethodInvocation>& invocation)
{
    const auto var = Glib::Variant<Glib::ustring>::create(Glib::ustring::compose("hello world %1 times", ++n));
    Glib::VariantContainerBase response = Glib::VariantContainerBase::create_tuple(var);
    invocation->return_value(response);
}
