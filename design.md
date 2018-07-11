# Deepin Graphics Driver Manager
驱动管理器的主要工作是分析设备的显卡类型（或多显卡组合类型），根据不同的类型提供不同驱动安装的方案。

## 架构
设备的分析结果由GraphicsDeviceInfo类提供，各种不同的显卡类型提供有哪些方案由`resources/config/resolutions/resolutions.json`文件决定。GraphicsDeviceInfo类会在初始化过程中自行分析显卡类型，只需要将初始化后的GraphicsDeviceInfo对象交给ResolutionsBuilder类即可，ResolutionsBuilder类会根据GraphicsDeviceInfo提供的显卡类型，在resolutions.json文件中找到对应类型的方案，构建出一个方案列表Resolutions，列表中的每一个方案都是一个抽象出来的Resolution。UI方面使用Resolution作为数据model将方案展示出来。

## overlayfs
overlyfs允许一个通常为读写的目录树被覆盖到另一个只读目录树上，默认所有修改都指向上层可写层。这样就可以保证在不真正改变系统的情况下，测试驱动的安装和使用效果，在驱动运行正常后询问用户是否真正执行安装操作。

安装操作是在重启后通过启动`driver-installer.service`服务开始执行的（`resources/service/driver-installer.service`），每个方案提供的install脚本都接收一个参数，如果参数**不为"post"**，则会在overlayfs上进行安装操作（一般会传入"test"参数），否则安装脚本会使用`overlayroot-chroot`命令将安装操作执行在用户的真实系统上。

## resolutions.json文件
resolutions.json文件是程序数据的来源，可通过编辑此文件增加删除方案。

resolutions.json提供了一个由各种显卡类型组成的json列表，每一个显卡类型是一个json对象，**显卡类型又被抽象为Resolutions类**，其可用的属性如下表所示：

|属性名             |解释|
|----               |----|
|type               |显卡类型id，对应GraphicsDeviceInfo类的检测结果|
|name               |名称|
|icon_name          |图标文件名称|
|status             |shell脚本，用于检测当前显卡类型下正在使用哪一个方案|
|resolutions        |提供了当前显卡类型下有哪些可用的方案|

上表中显卡类型的属性resolutions的值是一个由各个方案组成json列表，每一个方案是一个json对象，**方案又被抽象为Resolution类**，其可用的属性有下表所示：

|属性名             |解释|
|----               |----|
|id                 |方案的id|
|keep_gltest        |在overlay阶段安装驱动成功后是否进行测试|
|title              |标题|
|name               |名称|
|description        |介绍|
|condition          |shell脚本，用于检测当前方案是否可行，ResolutionWidget根据此结果决定自己是否要显示|
|prepare            |shell脚本，下载所需要的驱动及其依赖到本地，避免overlay阶段没有网络，或其他安装前的准备工作|
|install            |shell脚本，一般要根据是否传入"post"参数决定安装操作是真正执行还是在overlay上执行|
|remove             |shell脚本，也接收"post"参数决定卸载操作是否真正执行|
|version            |shell脚本，获取驱动包版本|

## 其他脚本
除了上述不同方案需要的准备、安装、卸载的脚本，在scripts的根目录下还有几个同样重要的脚本，它们控制的是整个驱动卸载、安装、测试的流程：

|文件名                       |作用|
|----                         |----|
|`dgradvrmgr-prepare.sh`      |在重启前执行，做overlay阶段需要的准备工作|
|`dgradvrmgr.sh`              |在overlay上卸载旧驱动安装新驱动并测试|
|`dgradvrmgr-post.sh`         |如果测试通过则用来将数据同步到用户硬盘上|


## 具体流程
程序启动时读取`resolutions.json`，并使用widget展示数据，用户点击`切换`按钮后调用`dgradvrmgr-prepare.sh`脚本做准备工作，具体如下：

- 执行具体方案提供的prepare脚本，一般用来下载安装包
- 复制具体方案的安装卸载脚本到`/usr/bin`目录下，以供重启时使用
- 在`/usr/lib/deepin-graphics-driver-manager`目录生成config.conf，存储新旧方案名，驱动安装是否成功等数据

如果`dgradvrmgr-prepare.sh`脚本顺利执行完毕，就会提醒用户重启。在重启时启动lightdm之前会启动`driver-installer.service`服务，这个服务做一些额外的准备工作后就会将重启前复制到`/usr/bin`目录下的脚本**移动**到`/tmp`目录下，然后运行`dgradvrmgr.sh`脚本，开始在overlay上卸载旧驱动，安装新驱动，如果一切顺利会再启动`gltest`小茶壶测试程序，如果用户认为可以通过测试，那么就会去执行`dgradvrmgr-post.sh`脚本同步数据到硬盘，最后将成功的flag输入到上面提到的配置文件中并重启。

重启后会启动`$HOME/.config/autostart/deepin-gradvrmgr-notify.desktop`的一次性通知(执行完毕后会删除)，这个程序检测上面提到的配置文件，读取新旧方案名称，检测是否有已成功的字段，然后进行相应的提示。

