#include "Interface.h"
#include "Object.h"

namespace Rasp
{

namespace DBus
{

Interface::Interface(const Glib::ustring& name) noexcept :
    m_name(name),
    m_parent(nullptr)
{

}

/*****************************************************************************
 * @brief 返回接口名
 * @return 接口名
 * ***************************************************************************/
Glib::ustring Interface::name() const noexcept
{
    return m_name;
}

/*****************************************************************************
 * @brief 返回 XML 字符串
 * @return xml
 * ***************************************************************************/
Glib::ustring Interface::XML() const noexcept
{
    Glib::ustring xml = Glib::ustring::compose("<node>\n  <interface name='%1'>\n", m_name);
    for (auto& m : m_methods)
    {
        xml += m.second->XML();
    }
    xml += "  </interface>\n</node>\n";

    return xml;
}

/*****************************************************************************
 * @brief 刷新所属服务
 * @return id
 * ***************************************************************************/
guint Interface::update()
{
    if (m_parent != nullptr)
        return m_parent->update();

    return 0;
}

/*****************************************************************************
 * @brief 导出方法
 * @param[in] method 方法
 * @return 是否成功 
 * ***************************************************************************/
bool Interface::exportMethod(const Glib::RefPtr<Method>& method) noexcept
{
    auto iter = m_methods.find(method->name());
    if (iter == m_methods.end())
    {
        m_methods[method->name()] = method;
        update();
        return true;
    }

    return false;
}

/*****************************************************************************
 * @brief 删除方法
 * @param[in] name 方法名
 * @return 是否成功 
 * ***************************************************************************/
bool Interface::unexportMethod(const Glib::ustring& name) noexcept
{
    auto iter = m_methods.find(name);
    if (iter != m_methods.end())
    {
        m_methods.erase(iter);
        update();
        return true;
    }

    return false;
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
void Interface::onMethodCall(const Glib::RefPtr<Gio::DBus::Connection>& connection,
                                const Glib::ustring& sender,
                                const Glib::ustring& objectPath,
                                const Glib::ustring& interfaceName,
                                const Glib::ustring& methodName,
                                const Glib::VariantContainerBase& args,
                                const Glib::RefPtr<Gio::DBus::MethodInvocation>& invocation) const noexcept
{
    auto iter = m_methods.find(methodName);
    if (iter == m_methods.end())
    {
        Gio::DBus::Error err{Gio::DBus::Error::UNKNOWN_METHOD, methodName};
        invocation->return_error(err);
        return;
    }

    iter->second->onMethodCall(connection, sender, objectPath, interfaceName, methodName, args, invocation);
}

}; // namespace DBus

}; // namespace Rasp