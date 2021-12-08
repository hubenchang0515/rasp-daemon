#include "Object.h"

namespace Rasp
{

namespace DBus
{


Object::Object(const Glib::ustring& path) noexcept:
    m_path(path)
{

}

/*****************************************************************************
 * @brief 返回对象路径
 * @return 对象路径
 * ***************************************************************************/
Glib::ustring Object::path() const noexcept
{
    return m_path;
}

/*****************************************************************************
 * @brief 返回 XML 字符串
 * @return xml
 * ***************************************************************************/
Glib::ustring Object::XML() const noexcept
{
    Glib::ustring xml = "<node>\n";
    for (auto& i : m_interfaces)
    {
        xml += i.second.XML();
    }
    xml += "</node>";
    return xml;
}

/*****************************************************************************
 * @brief 导出接口
 * @param[in] service 服务
 * @return id
 * ***************************************************************************/
bool Object::exportInterface(const Interface& interface) noexcept
{
    auto iter = m_interfaces.find(interface.name());
    if (iter == m_interfaces.end())
    {
        m_interfaces.emplace(interface.name(), interface);
        return true;
    }

    return false;
}

/*****************************************************************************
 * @brief 删除接口
 * @param[in] service 服务
 * @return id
 * ***************************************************************************/
bool Object::unexportInterface(const Glib::ustring& name) noexcept
{
    auto iter = m_interfaces.find(name);
    if (iter != m_interfaces.end())
    {
        m_interfaces.erase(iter);
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
void Object::onMethodCall(const Glib::RefPtr<Gio::DBus::Connection>& connection,
                                const Glib::ustring& sender,
                                const Glib::ustring& objectPath,
                                const Glib::ustring& interfaceName,
                                const Glib::ustring& methodName,
                                const Glib::VariantContainerBase& args,
                                const Glib::RefPtr<Gio::DBus::MethodInvocation>& invocation)  const noexcept
{
    auto iter = m_interfaces.find(interfaceName);
    if (iter == m_interfaces.end())
    {
        Gio::DBus::Error err{Gio::DBus::Error::UNKNOWN_INTERFACE, interfaceName};
        invocation->return_error(err);
        return;
    }

    iter->second.onMethodCall(connection, sender, objectPath, interfaceName, methodName, args, invocation);
}

}; // namespace DBus

}; // namespace Rasp