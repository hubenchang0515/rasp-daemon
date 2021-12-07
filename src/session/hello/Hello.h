#ifndef RASPD_HELLO_H
#define RASPD_HELLO_H

#include "DBusService.h"

namespace Raspd
{

class Hello : public DBusService
{
public:
    Hello();
    ~Hello();
    virtual Glib::ustring XML() override;
    virtual Glib::ustring path() override;
    virtual Glib::ustring name() override;

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
    void methodHello(const Glib::RefPtr<Gio::DBus::Connection>& connection,
                                const Glib::ustring& sender,
                                const Glib::ustring& objectPath,
                                const Glib::ustring& interfaceName,
                                const Glib::ustring& methodName,
                                const Glib::VariantContainerBase& args,
                                const Glib::RefPtr<Gio::DBus::MethodInvocation>& invocation);
};

}; // namespace Raspd

#endif // RASP_HELLO_H