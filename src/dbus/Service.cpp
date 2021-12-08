#include "Service.h"

namespace Rasp
{

namespace DBus
{

/* 服务名 => 服务 */
std::map<Glib::ustring, Service*> Service::services;

/* 对象路径 => 所属服务 */
std::map<Glib::ustring, Service*> Service::objServices;

Service::Service(const Glib::ustring& name):
    m_name(name),
    m_vtable{sigc::ptr_fun(&onMethodCall)},
    m_ownerId(0)
{

}

/*****************************************************************************
 * @brief 返回服务名
 * @return 服务名
 * ***************************************************************************/
Glib::ustring Service::name() const noexcept
{
    return m_name;
}

/*****************************************************************************
 * @brief 导出对象
 * @param[in] obj 对象
 * @return 是否成功
 * ***************************************************************************/
bool Service::exportObject(const Object& obj) noexcept
{
    auto iter = m_objects.find(obj.path());
    if (iter == m_objects.end())
    {
        m_objects.emplace(obj.path(), obj);
        return true;
    }

    return false;
}

/*****************************************************************************
 * @brief 删除对象
 * @param[in] path 对象路径
 * @return 是否成功
 * ***************************************************************************/
bool Service::unexportObject(const Glib::ustring& path) noexcept
{
    auto iter = m_objects.find(path);
    if (iter != m_objects.end())
    {
        m_objects.erase(iter);
        return true;
    }

    return false;
}


/*****************************************************************************
 * @brief 注册服务
 * @param[in] service 服务对象
 * @param[in] type 总线类型
 * @return 注册id
 * ***************************************************************************/
guint Service::registerService(Service* service, Gio::DBus::BusType type)
{
    for (auto& obj : service->m_objects)
    {
        auto path = obj.first;
        if (Service::objServices.find(path) != Service::objServices.end())
        {
            fprintf(stderr, "conflicting object path '%s'\n", path.c_str());
            return 0;
        }
    }
    
    
    for (auto& obj : service->m_objects)
    {
        Service::objServices.emplace(obj.first, service);
    }
    Service::services.emplace(service->m_name, service);

    service->m_ownerId = Gio::DBus::own_name(type, 
                                    service->m_name,
                                    sigc::ptr_fun(&onBusAcquired), 
                                    sigc::ptr_fun(&onNameAcquired),
                                    sigc::ptr_fun(&onNameLost));
    

    return service->m_ownerId;
}

/*****************************************************************************
 * @brief 注销服务
 * @param[in] name 名字
 * @param[in] del 是否同时删除对象
 * @return 是否成功 
 * ***************************************************************************/
bool Service::unregisterService(const Glib::ustring& name, bool del)
{
    // TODO: 删除服务
    return true;
}

/*****************************************************************************
 * @brief 注销服务
 * @param[in] service 服务对象
 * @param[in] del 是否同时删除对象
 * @return 是否成功 
 * ***************************************************************************/
bool Service::unregisterService(Service* service, bool del)
{
    auto name = service->name(); 
    auto iter = services.find(name);
    if (iter == services.end())
    {
        fprintf(stderr, "unknown service name '%s'\n", name.c_str());
        return false;
    }

    Gio::DBus::unown_name(service->m_ownerId);
    services.erase(name);
    if (del)
        delete service;
    return true;
}

/*****************************************************************************
 * @brief 回调函数，DBus 获得总线
 * @param[in] connection DBus连接
 * @param[in] name 名字
 * ***************************************************************************/
void Service::onBusAcquired(const Glib::RefPtr<Gio::DBus::Connection>& connection,
                            const Glib::ustring& name)
{
    try
    {
        auto service = Service::services.at(name);

        for (auto iter : service->m_objects)
        {
            auto name = iter.first;
            auto obj = iter.second;
            auto introspectionData = Gio::DBus::NodeInfo::create_for_xml(obj.XML());
            service->m_objIds[name] = connection->register_object(obj.path(), 
                                        introspectionData->lookup_interface(), 
                                        service->m_vtable);
        }
    }
    catch (const std::exception& err)
    {
        fprintf(stderr, "onBusAcquired: %s\n", err.what());
    }
}

/*****************************************************************************
 * @brief 回调函数，DBus 获得连接名
 * @param[in] connection DBus连接
 * @param[in] name 名字
 * ***************************************************************************/
void Service::onNameAcquired(const Glib::RefPtr<Gio::DBus::Connection>& connection, 
                            const Glib::ustring& name)
{
    // TODO: See https://bugzilla.gnome.org/show_bug.cgi?id=646427
}

/*****************************************************************************
 * @brief 回调函数，DBus 失去连接名
 * @param[in] connection DBus连接
 * @param[in] name 名字
 * ***************************************************************************/
void Service::onNameLost(const Glib::RefPtr<Gio::DBus::Connection>& connection, 
                            const Glib::ustring& name)
{
    try
    {
        auto service = Service::services.at(name);
        for (auto iter : service->m_objIds)
        {
            connection->unregister_object(iter.second);
        }
    }
    catch (const std::exception& err)
    {
        fprintf(stderr, "onNameLost: %s\n", err.what());
    }
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
void Service::onMethodCall(const Glib::RefPtr<Gio::DBus::Connection>& connection,
                            const Glib::ustring& sender,
                            const Glib::ustring& objectPath,
                            const Glib::ustring& interfaceName,
                            const Glib::ustring& methodName,
                            const Glib::VariantContainerBase& args,
                            const Glib::RefPtr<Gio::DBus::MethodInvocation>& invocation)
{
    auto iter = Service::objServices.find(objectPath);
    if (iter == Service::objServices.end())
    {
        Gio::DBus::Error e{Gio::DBus::Error::UNKNOWN_OBJECT, objectPath};
        invocation->return_error(e);
        return;
    }

    auto service = iter->second;
    auto obj = service->m_objects.at(objectPath);
    obj.onMethodCall(connection, sender, objectPath, interfaceName, methodName, args, invocation);
}

}; // namespace DBus

}; // namespace Rasp