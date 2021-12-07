#include "Hello.h"

namespace Raspd
{

Hello::Hello()
{
    reg(this);
    addMethod("Hello", &Hello::methodHello);
}

Hello::~Hello()
{
    unreg(this);
}

Glib::ustring Hello::XML()
{
    return  "<node>"
            "   <interface name='org.planc.raspd.Hello'>"
            "       <method name='Hello'>"
            "           <arg type='s' name='world' direction='out'/>"
            "       </method>"
            "   </interface>"
            "</node>";
}

Glib::ustring Hello::path()
{
    return "/org/planc/raspd/Hello";
}

Glib::ustring Hello::name()
{
    return "org.planc.raspd.Hello";
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
void Hello::methodHello(const Glib::RefPtr<Gio::DBus::Connection>& connection,
                            const Glib::ustring& sender,
                            const Glib::ustring& objectPath,
                            const Glib::ustring& interfaceName,
                            const Glib::ustring& methodName,
                            const Glib::VariantContainerBase& args,
                            const Glib::RefPtr<Gio::DBus::MethodInvocation>& invocation)
{
    const auto var = Glib::Variant<Glib::ustring>::create("hello world");
    Glib::VariantContainerBase response = Glib::VariantContainerBase::create_tuple(var);
    invocation->return_value(response);
}

}; // namespace Raspd
