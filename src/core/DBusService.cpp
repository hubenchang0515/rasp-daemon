#include "DBusService.h"

DBusService::DBusService():
    m_dbusNameId(0),
    m_dbusObjectId(0),
    m_vtable{sigc::ptr_fun(&onMethodCall)}
{
    
}

/* 对象表 */
std::map<Glib::ustring, DBusService*> DBusService::objs;

/*****************************************************************************
 * @brief 注册对象
 * @param[in] name 名字
 * @param[in] obj 对象
 * @return 是否成功 
 * ***************************************************************************/
guint DBusService::reg(DBusService* obj, Gio::DBus::BusType type)
{
    auto name = obj->name();
    if (objs.find(name) != objs.end())
    {
        fprintf(stderr, "conflicting service name '%s'\n", name.c_str());
        return 0;
    }

    objs[name] = obj;
    obj->m_dbusNameId = Gio::DBus::own_name(type, 
                                    name,
                                    sigc::ptr_fun(&onBusAcquired), 
                                    sigc::ptr_fun(&onNameAcquired),
                                    sigc::ptr_fun(&onNameLost));

    return obj->m_dbusNameId;
}

/*****************************************************************************
 * @brief 反注册对象
 * @param[in] name 名字
 * @param[in] del 是否同时删除对象
 * @return 是否成功 
 * ***************************************************************************/
bool DBusService::unreg(const Glib::ustring& name, bool del)
{
    auto iter = objs.find(name);
    if (iter == objs.end())
    {
        fprintf(stderr, "unknown service name '%s'\n", name.c_str());
        return false;
    }

    auto obj = iter->second;
    Gio::DBus::unown_name(obj->m_dbusNameId);
    objs.erase(name);
    if (del)
        delete obj;
    return true;
}

/*****************************************************************************
 * @brief 反注册对象
 * @param[in] obj 对象
 * @param[in] del 是否同时删除对象
 * @return 是否成功 
 * ***************************************************************************/
bool DBusService::unreg(DBusService* obj, bool del)
{
    auto name = obj->name(); 
    auto iter = objs.find(name);
    if (iter == objs.end())
    {
        fprintf(stderr, "unknown service name '%s'\n", name.c_str());
        return false;
    }

    Gio::DBus::unown_name(obj->m_dbusNameId);
    objs.erase(name);
    if (del)
        delete obj;
    return true;
}

/*****************************************************************************
 * @brief 注册方法
 * @param[in] name 方法名
 * @param[in] del 是否同时删除对象
 * @return 是否成功 
 * ***************************************************************************/
bool DBusService::addMethod(Glib::ustring name, DBusService::Method fn)
{
    auto iter = m_methods.find(name);
    if (iter != m_methods.end())
    {
        fprintf(stderr, "conflicting method name '%s'\n", name.c_str());
        return false;
    }

    m_methods[name] = fn;
    return true;
}

/*****************************************************************************
 * @brief 回调函数，DBus 获得总线
 * @param[in] connection DBus连接
 * @param[in] name 名字
 * ***************************************************************************/
void DBusService::onBusAcquired(const Glib::RefPtr<Gio::DBus::Connection>& connection,
                            const Glib::ustring& name)
{
    try
    {
        auto obj = DBusService::objs.at(name);
        auto introspectionData = Gio::DBus::NodeInfo::create_for_xml(obj->XML());
        obj->m_dbusObjectId = connection->register_object(obj->path(), 
                                                        introspectionData->lookup_interface(), 
                                                        obj->m_vtable);
    }
    catch (const Glib::Error& err)
    {
        fprintf(stderr, "Registration of object failed.\n");
    }
}

/*****************************************************************************
 * @brief 回调函数，DBus 获得连接名
 * @param[in] connection DBus连接
 * @param[in] name 名字
 * ***************************************************************************/
void DBusService::onNameAcquired(const Glib::RefPtr<Gio::DBus::Connection>& connection, 
                            const Glib::ustring& name)
{
    // TODO: See https://bugzilla.gnome.org/show_bug.cgi?id=646427
}

/*****************************************************************************
 * @brief 回调函数，DBus 失去连接名
 * @param[in] connection DBus连接
 * @param[in] name 名字
 * ***************************************************************************/
void DBusService::onNameLost(const Glib::RefPtr<Gio::DBus::Connection>& connection, 
                            const Glib::ustring& name)
{
    auto obj = DBusService::objs.at(name);
    connection->unregister_object(obj->m_dbusObjectId);
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
void DBusService::onMethodCall(const Glib::RefPtr<Gio::DBus::Connection>& connection,
                            const Glib::ustring& sender,
                            const Glib::ustring& objectPath,
                            const Glib::ustring& interfaceName,
                            const Glib::ustring& methodName,
                            const Glib::VariantContainerBase& args,
                            const Glib::RefPtr<Gio::DBus::MethodInvocation>& invocation)
{
    
    auto iter = DBusService::objs.find(interfaceName);
    if (iter == DBusService::objs.end())
    {
        Gio::DBus::Error e{Gio::DBus::Error::UNKNOWN_INTERFACE, interfaceName};
        invocation->return_error(e);
        return;
    }

    try
    {
        auto obj = DBusService::objs.at(interfaceName);
        auto fn = obj->m_methods.at(methodName);
        (obj->*fn)(connection, sender, objectPath, interfaceName, methodName, args, invocation);
    }
    catch (const std::exception& err)
    {
        Gio::DBus::Error e{Gio::DBus::Error::FAILED, "Unknown Error"};
        invocation->return_error(e);
    }
}