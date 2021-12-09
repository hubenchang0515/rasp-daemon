#ifndef RASP_DBUS_METHOD_H
#define RASP_DBUS_METHOD_H

#include <map>
#include <vector>
#include <functional>

#include <giomm.h>
#include <glibmm.h>

#define RASP_LAMBDA_FUN_ARGS const Glib::RefPtr<Gio::DBus::Connection>& connection,          \
                             const Glib::ustring& sender,                                    \
                             const Glib::ustring& objectPath,                                \
                             const Glib::ustring& interfaceName,                             \
                             const Glib::ustring& methodName,                                \
                             const Glib::VariantContainerBase& args,                         \
                             const Glib::RefPtr<Gio::DBus::MethodInvocation>& invocation     \

#define RASP_LAMBDA_PASS_ARGS connection,         \
                              sender,             \
                              objectPath,         \
                              interfaceName,      \
                              methodName,         \
                              args,               \
                              invocation          \

#define RASP_WARP_METHOD(func) [this](RASP_LAMBDA_FUN_ARGS){func(RASP_LAMBDA_PASS_ARGS);}

namespace Rasp
{

namespace DBus
{

class Interface;

class Method : public Glib::Object
{
    friend Interface;
public:
    // 回调函数的类型
    using Callback = std::function<void(const Glib::RefPtr<Gio::DBus::Connection>& connection,
                                        const Glib::ustring& sender,
                                        const Glib::ustring& objectPath,
                                        const Glib::ustring& interfaceName,
                                        const Glib::ustring& methodName,
                                        const Glib::VariantContainerBase& args,
                                        const Glib::RefPtr<Gio::DBus::MethodInvocation>& invocation)>;

    /*****************************************************************************
     * @brief 构造一个 DBus 方法
     * @param[in] name 方法名
     * @param[in] fn 响应方法调用的回调函数
     * @param [in] inArgs 输入参数列表，是参数名映射到类型字符串的map
     * @param [in] outArgs 返回值列表，是参数名映射到类型字符串的map
     * ***************************************************************************/
    explicit Method(const Glib::ustring& name, const Callback& fn, 
                    const std::map<Glib::ustring, Glib::ustring>& inArgs={}, 
                    const std::map<Glib::ustring, Glib::ustring>& outArgs={}) noexcept;

    /*****************************************************************************
     * @brief 返回方法名
     * @return 方法名
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

protected:
    /*****************************************************************************
     * @brief 触发回调函数，DBus 方法调用
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
    Glib::ustring m_name;   // 方法名
    Callback m_callback;    // 回调函数

    // 参数名 => 类型字符串
    std::map<Glib::ustring, Glib::ustring> m_inArgs;
    std::map<Glib::ustring, Glib::ustring> m_outArgs;

protected:
    Rasp::DBus::Interface* m_parent;
};

}; // RASP_DBUS_METHOD_H

}; // RASP_DBUS_METHOD_H

#endif // 