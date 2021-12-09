#ifndef RASP_DBUS_INTERFACE_H
#define RASP_DBUS_INTERFACE_H

#include <map>

#include <giomm.h>
#include <glibmm.h>

#include "Method.h"

namespace Rasp
{

namespace DBus
{

class Object;

class Interface : public Glib::Object
{
    friend Rasp::DBus::Object;
public:
    explicit Interface(const Glib::ustring& name) noexcept;

    /*****************************************************************************
     * @brief 返回接口名
     * @return 接口名
     * ***************************************************************************/
    Glib::ustring name() const noexcept;

    /*****************************************************************************
     * @brief 返回 XML 字符串
     * @return xml
     * ***************************************************************************/
    Glib::ustring XML() const noexcept;

    /*****************************************************************************
     * @brief 刷新所属服务
     * @return id
     * ***************************************************************************/
    guint update();

    /*****************************************************************************
     * @brief 导出方法
     * @param[in] method 方法
     * @return 是否成功 
     * ***************************************************************************/
    bool exportMethod(const Glib::RefPtr<Method>& method) noexcept;

    /*****************************************************************************
     * @brief 删除方法
     * @param[in] name 方法名
     * @return 是否成功 
     * ***************************************************************************/
    bool unexportMethod(const Glib::ustring& name) noexcept;

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
    Glib::ustring m_name;
    std::map<Glib::ustring, Glib::RefPtr<Method>> m_methods; // 方法函数表

protected:
    Rasp::DBus::Object* m_parent;

}; // class Interface

}; // namespace DBus

}; // namespace Rasp

#endif // RASP_DBUS_Interface_H