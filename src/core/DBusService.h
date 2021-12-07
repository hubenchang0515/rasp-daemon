#ifndef DBUS_SERVICE_H
#define DBUS_SERVICE_H

#include <map>

#include <giomm.h>
#include <glibmm.h>

class DBusService
{
public:
    using Method = void (DBusService::*)(const Glib::RefPtr<Gio::DBus::Connection>& connection,
                                const Glib::ustring& sender,
                                const Glib::ustring& objectPath,
                                const Glib::ustring& interfaceName,
                                const Glib::ustring& methodName,
                                const Glib::VariantContainerBase& args,
                                const Glib::RefPtr<Gio::DBus::MethodInvocation>& invocation);

    DBusService();
    virtual ~DBusService() = default;

    virtual Glib::ustring XML() = 0;
    virtual Glib::ustring path() = 0;
    virtual Glib::ustring name() = 0;

    /*****************************************************************************
     * @brief 注册对象
     * @param[in] obj 对象
     * @param[in] type 总线类型
     * @return id
     * ***************************************************************************/
    static guint reg(DBusService* obj, Gio::DBus::BusType type=Gio::DBus::BusType::BUS_TYPE_SESSION);

    /*****************************************************************************
     * @brief 反注册对象
     * @param[in] name 名字
     * @param[in] del 是否同时删除对象
     * @return 是否成功 
     * ***************************************************************************/
    static bool unreg(const Glib::ustring& name, bool del=false);

    /*****************************************************************************
     * @brief 反注册对象
     * @param[in] obj 对象
     * @param[in] del 是否同时删除对象
     * @return 是否成功 
     * ***************************************************************************/
    static bool unreg(DBusService* obj, bool del=false);

    /*****************************************************************************
     * @brief 注册方法
     * @param[in] name 方法名
     * @param[in] del 是否同时删除对象
     * @return 是否成功 
     * ***************************************************************************/
    bool addMethod(Glib::ustring name, DBusService::Method fn);

    template<typename T>
    bool addMethod(Glib::ustring name, T fn)
    {
        auto m = static_cast<Method>(fn);
        return addMethod(name, m);
    }


private:
    guint m_dbusNameId;
    guint m_dbusObjectId;
    Gio::DBus::InterfaceVTable m_vtable; // DBus 虚表

    std::map<Glib::ustring, DBusService::Method> m_methods; // 方法函数表

    /* 对象表 */
    static std::map<Glib::ustring, DBusService*> objs;

    /*****************************************************************************
     * @brief 回调函数，DBus 获得总线
     * @param[in] connection DBus连接
     * @param[in] name 名字
     * ***************************************************************************/
    static void onBusAcquired(const Glib::RefPtr<Gio::DBus::Connection>& connection,
                                const Glib::ustring& name);

    /*****************************************************************************
     * @brief 回调函数，DBus 获得连接名
     * @param[in] connection DBus连接
     * @param[in] name 名字
     * ***************************************************************************/
    static void onNameAcquired(const Glib::RefPtr<Gio::DBus::Connection>& connection, 
                                const Glib::ustring& name);

    /*****************************************************************************
     * @brief 回调函数，DBus 失去连接名
     * @param[in] connection DBus连接
     * @param[in] name 名字
     * ***************************************************************************/
    static void onNameLost(const Glib::RefPtr<Gio::DBus::Connection>& connection, 
                                const Glib::ustring& name);

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
    static void onMethodCall(const Glib::RefPtr<Gio::DBus::Connection>& connection,
                                const Glib::ustring& sender,
                                const Glib::ustring& objectPath,
                                const Glib::ustring& interfaceName,
                                const Glib::ustring& methodName,
                                const Glib::VariantContainerBase& args,
                                const Glib::RefPtr<Gio::DBus::MethodInvocation>& invocation);
};

#endif // DBUS_SERVICE_H