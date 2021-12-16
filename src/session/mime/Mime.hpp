#ifndef RASPD_SESSION_MIME_H
#define RASPD_SESSION_MIME_H

#include "raspd-dbus.h"

class Mime
{
public:
    Mime():
        m_service{new Rasp::DBus::Service{"org.planc.raspd.Mime"}},
        m_object{new Rasp::DBus::Object{"/org/planc/raspd/Mime"}},
        m_interface{new Rasp::DBus::Interface{"org.planc.raspd.Mime"}},
        m_methodGetDefault{new Rasp::DBus::Method{"GetDefault", Rasp::DBus::Method::warp(this, &Mime::getDefault), {{"mimeType", "s"}}, {{"defaultApp", "s"}}}},
        m_methodSetDefault{new Rasp::DBus::Method{"SetDefault", Rasp::DBus::Method::warp(this, &Mime::setDefault), {{"mimeType", "s"}, {"appId", "s"}}, {}}}
    {
        Rasp::DBus::Service::registerService(m_service);
        m_interface->exportMethod(m_methodGetDefault);
        m_interface->exportMethod(m_methodSetDefault);
        m_object->exportInterface(m_interface);
        m_service->exportObject(m_object);
    }

    ~Mime()
    {
        Rasp::DBus::Service::unregisterService(m_service->name());
    }

protected:
    /*****************************************************************************
     * @brief 回调函数，DBus 方法调用
     * @param[in] mimeType mime 类型
     * @param[out] defaultApp 默认应用
     * ***************************************************************************/
    void getDefault(const Glib::VariantContainerBase& args,
                    const Glib::RefPtr<Gio::DBus::MethodInvocation>& invocation)
    {
        Glib::Variant<Glib::ustring> mimeType;
        args.get_child(mimeType, 0);
        auto appInfo = Gio::AppInfo::get_default_for_type(mimeType.get());

        if (appInfo.get() == nullptr)
        {
            Gio::DBus::Error e{Gio::DBus::Error::FAILED, Glib::ustring::compose("no default app for mime type '%1'", mimeType)};
            invocation->return_error(e);
            return;
        }

        const auto appName = Glib::Variant<Glib::ustring>::create(appInfo->get_id());
        Glib::VariantContainerBase response = Glib::VariantContainerBase::create_tuple(appName);
        invocation->return_value(response);
    }

    /*****************************************************************************
     * @brief 回调函数，DBus 方法调用
     * @param[in] mimeType mime 类型
     * @param[in] defaultApp 默认应用
     * ***************************************************************************/
    void setDefault(const Glib::VariantContainerBase& args,
                    const Glib::RefPtr<Gio::DBus::MethodInvocation>& invocation)
    {
        Glib::Variant<Glib::ustring> mimeType;
        Glib::Variant<Glib::ustring> appId;
        args.get_child(mimeType, 0);
        args.get_child(appId, 1);

        auto appInfo = Gio::DesktopAppInfo::create(appId.get());
        if (appInfo.get() == nullptr)
        {
            Gio::DBus::Error e{Gio::DBus::Error::FAILED, Glib::ustring::compose("no app info of '%1'", appId)};
            invocation->return_error(e);
            return;
        }

        appInfo->set_as_default_for_type(mimeType.get());
        invocation->return_value(Glib::VariantContainerBase{});
    }

private:
    Glib::RefPtr<Rasp::DBus::Service>     m_service;
    Glib::RefPtr<Rasp::DBus::Object>      m_object;
    Glib::RefPtr<Rasp::DBus::Interface>   m_interface;

    Glib::RefPtr<Rasp::DBus::Method> m_methodGetDefault;
    Glib::RefPtr<Rasp::DBus::Method> m_methodSetDefault;
};

#endif // RASPD_SESSION_MIME_H