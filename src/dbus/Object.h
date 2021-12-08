#ifndef RASP_DBUS_OBJECT_H
#define RASP_DBUS_OBJECT_H

#include <map>

#include <giomm.h>
#include <glibmm.h>

#include "Interface.h"

namespace Rasp
{

namespace DBus
{

class Service;

class Object
{
    friend Service;
public:
    Object(const Glib::ustring& path) noexcept;

    /*****************************************************************************
     * @brief 返回对象路径
     * @return 对象路径
     * ***************************************************************************/
    Glib::ustring path() const noexcept;

    /*****************************************************************************
     * @brief 返回 XML 字符串
     * @return xml
     * ***************************************************************************/
    Glib::ustring XML() const noexcept;

    /*****************************************************************************
     * @brief 导出接口
     * @param[in] service 服务
     * @return id
     * ***************************************************************************/
    bool exportInterface(const Interface& interface) noexcept;

    /*****************************************************************************
     * @brief 删除接口
     * @param[in] service 服务
     * @return id
     * ***************************************************************************/
    bool unexportInterface(const Glib::ustring& name) noexcept;

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
    void onMethodCall(const Glib::RefPtr<Gio::DBus::Connection>& connection,
                        const Glib::ustring& sender,
                        const Glib::ustring& objectPath,
                        const Glib::ustring& interfaceName,
                        const Glib::ustring& methodName,
                        const Glib::VariantContainerBase& args,
                        const Glib::RefPtr<Gio::DBus::MethodInvocation>& invocation) const noexcept;

private:
    Glib::ustring m_path;
    std::map<Glib::ustring, Interface> m_interfaces;
}; // Object

}; // namespace DBus

}; // namespace Rasp

#endif // RASP_DBUS_OBJECT_H