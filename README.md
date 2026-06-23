# Switch 家长控制图形化管理工具

一个用于 Nintendo Switch (Atmosphère CFW) 的家长控制密码管理图形化工具。

## 功能简介

- 图形化界面管理 Switch 家长控制
- 基于 borealis 框架，原生 Horizon 系统风格
- 支持密码计算、PIN 码管理等功能

## 编译步骤

### 前置要求

1. 安装 [devkitPro](https://devkitpro.org/wiki/Getting_Started)
   - 确保安装了 `switch-dev` 组件
   - 设置环境变量 `DEVKITPRO`

2. 安装依赖库
   ```bash
   pacman -S switch-borealis switch-mbedtls
   ```

### 编译项目

1. 克隆仓库（包含 submodules）
   ```bash
   git clone --recursive https://github.com/yourusername/switch-pctl-gui.git
   cd switch-pctl-gui
   ```

2. 创建构建目录
   ```bash
   cmake -B build -DCMAKE_TOOLCHAIN_FILE=$DEVKITPRO/cmake/DevkitA64Libnx.cmake
   ```

3. 编译
   ```bash
   make -C build
   ```

4. 生成的 NRO 文件位于 `build/switch-pctl-gui.nro`

### 手动安装

将 `switch-pctl-gui.nro` 复制到 Switch 的 SD 卡：
```
/switch/switch-pctl-gui/switch-pctl-gui.nro
```

## 运行要求

- Nintendo Switch 主机
- 已安装 Atmosphère 自制固件
- 已安装 borealis 依赖（通过 hbmenu 运行）

## 项目结构

```
switch-pctl-gui/
├── CMakeLists.txt          # CMake 构建配置
├── README.md               # 项目说明
├── .gitignore              # Git 忽略规则
├── .gitmodules             # Git 子模块配置
├── romfs/                  # ROMFS 资源目录
│   └── fonts/              # 字体文件
├── include/                # 头文件目录
│   └── main.h
├── source/                 # 源文件目录
│   └── main.cpp
└── externals/              # 外部依赖
    └── borealis/           # borealis 框架 (git submodule)
```

## 许可证

本项目基于 GPLv3 许可证开源。

## 致谢

- [borealis](https://github.com/nintendo-switch-custom-firmware/borealis) - Switch 原生风格 GUI 框架
- [PctlManager](https://github.com/tailiang2008/NX-Pctl-Manager) - 参考项目
