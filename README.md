# rasptool-daemon
daemon of rasptool - rasptool的后端守护进程

## Source Directory Description / 源码目录说明

* `src` - root directory of source / 源码根目录
  * `dbus` - DBus service basic warpper / Dbus 服务基础封装
  * `common` - common code like read CPU usage / 公共代码，例如读取 CPU 占用
  * `system` - service daemon on system bus / 系统总线上的服务进程
  * `session` - service daemon on session bus / 会话总线上的服务进程
    * `hello` - service named `org.planc.raspd.Hello`