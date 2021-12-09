#ifndef RASP_DBUS_PROPERTY_H
#define RASP_DBUS_PROPERTY_H

#include <map>

#include <giomm.h>
#include <glibmm.h>

#define RASP_LAMBDA_GET_ARGS1 Glib::VariantBase& property,                              \
                              const Glib::RefPtr<Gio::DBus::Connection>& connection,    \
                              const Glib::ustring& sender,                              \
                              const Glib::ustring& objectPath,                          \
                              const Glib::ustring& interfaceName,                       \
                              const Glib::ustring& propertyName                         \

#define RASP_LAMBDA_GET_ARGS2 property,         \
                              connection,       \
                              sender,           \
                              objectPath,       \
                              interfaceName,    \
                              propertyName      \

#define RASP_LAMBDA_SET_ARGS1 const Glib::RefPtr<Gio::DBus::Connection>& connection,  \
                              const Glib::ustring& sender,                            \
                              const Glib::ustring& objectPath,                        \
                              const Glib::ustring& interfaceName,                     \
                              const Glib::ustring& propertyName,                      \
                              const Glib::VariantBase& property                       \
                              

#define RASP_LAMBDA_SET_ARGS2 connection,       \
                              sender,           \
                              objectPath,       \
                              interfaceName,    \
                              propertyName,     \
                              property          \

#define RASP_WARP_GET(func) [this](RASP_LAMBDA_GET_ARGS1){func(RASP_LAMBDA_GET_ARGS2);}
#define RASP_WARP_SET(func) [this](RASP_LAMBDA_SET_ARGS1){return func(RASP_LAMBDA_SET_ARGS2);}

namespace Rasp
{

namespace DBus
{

class Interface;

class Property : public Glib::Object
{
    friend Interface;
public:
    // 回调函数类型
    using CallBackGet = std::function<void(Glib::VariantBase& property,
                                            const Glib::RefPtr<Gio::DBus::Connection>& connection,
                                            const Glib::ustring& sender,
                                            const Glib::ustring& objectPath,
                                            const Glib::ustring& interfaceName,
                                            const Glib::ustring& propertyName)>;

        // 回调函数类型
    using CallBackSet = std::function<bool(const Glib::RefPtr<Gio::DBus::Connection>& connection,
                                            const Glib::ustring& sender,
                                            const Glib::ustring& objectPath,
                                            const Glib::ustring& interfaceName,
                                            const Glib::ustring& propertyName,
                                            const Glib::VariantBase& value)>;

    /*****************************************************************************
     * @brief 构造一个 DBus 属性
     * @param[in] name 属性名
     * @param[in] type 类型字符串
     * @param[in] getFn get的回调函数
     * @param[in] setFn set的回调函数
     * ***************************************************************************/
    explicit Property(const Glib::ustring& name, const Glib::ustring& type,
                         const CallBackGet& getFn=nullptr, const CallBackSet& setFn=nullptr);

    /*****************************************************************************
     * @brief 返回属性名
     * @return 属性名
     * ***************************************************************************/
    Glib::ustring name() const noexcept;

    /*****************************************************************************
     * @brief 返回属性的类型字符串
     * @return 类型字符串
     * ***************************************************************************/
    Glib::ustring type() const noexcept;

    /*****************************************************************************
     * @brief 返回 XML 字符串
     * @return xml
     * ***************************************************************************/
    Glib::ustring XML() const noexcept;

private:
    Glib::ustring m_name;
    Glib::ustring m_type;
    CallBackGet m_get;
    CallBackSet m_set;

    /*****************************************************************************
     * @brief 回调函数，DBus 属性读取
     * @param[in] property 属性
     * @param[in] connection 连接
     * @param[in] sender 发送方
     * @param[in] objectPath 对象路径
     * @param[in] interfaceName 接口名
     * @param[in] propertyName 属性名
     * ***************************************************************************/
    void onGetProperty(Glib::VariantBase& property,
                        const Glib::RefPtr<Gio::DBus::Connection>& connection,
                        const Glib::ustring& sender,
                        const Glib::ustring& objectPath,
                        const Glib::ustring& interfaceName,
                        const Glib::ustring& propertyName);

    /*****************************************************************************
     * @brief 回调函数，DBus 属性读取
     * @param[in] connection 连接
     * @param[in] sender 发送方
     * @param[in] objectPath 对象路径
     * @param[in] interfaceName 接口名
     * @param[in] propertyName 属性名
     * @param[in] value 属性值
     * ***************************************************************************/
    bool onSetProperty(const Glib::RefPtr<Gio::DBus::Connection>& connection,
                        const Glib::ustring& sender,
                        const Glib::ustring& objectPath,
                        const Glib::ustring& interfaceName,
                        const Glib::ustring& propertyName,
                        const Glib::VariantBase& value);

}; // class Property

}; // RASP_DBUS_METHOD_H

}; // RASP_DBUS_METHOD_H

#endif // 