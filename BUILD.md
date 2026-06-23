# Switch 家长控制图形化管理工具 - 构建说明

## 项目概述

这是一个 Nintendo Switch 的 homebrew 应用（NRO 格式），使用 **borealis** 框架实现 Horizon 系统风格的图形界面，用于管理 Switch 的家长控制功能。

## 功能特性

- ✅ 查看家长控制状态（PIN 长度、限制开关、时长限制、剩余时间）
- ✅ 设置每日游玩时长限制（预设值：5/15/20/30/60/90/120 分钟）
- ✅ 启用/禁用家长控制
- ✅ 修改 PIN 码（调用系统 applet）
- ⚠️ 删除所有配置（待实现）
- ⚠️ 解除手机 App 绑定（待实现）

## 构建环境要求

1. **devkitPro** 已安装（包含 devkitA64、libnx、switch-cmake）
2. **环境变量**：`DEVKITPRO` 已设置（默认 `C:/devkitPro`）
3. **git** 已安装（用于拉取 borealis 子模块）

## 构建步骤

### 1. 拉取 borealis 子模块

```bash
cd switch-pctl-gui
git submodule update --init --recursive
```

**如果 `xfangfang/borealis` 拉取失败**，改用官方仓库：

```bash
# 修改 .gitmodules
[submodule "externals/borealis"]
    url = https://github.com/nintendo-switch-custom-firmware/borealis.git
    branch = master

# 然后重新拉取
git submodule sync
git submodule update --init --recursive
```

### 2. 下载中文字体

1. 下载思源黑体（简体中文）：
   - 地址：<ADDRESS_REMOVED>
   - 文件：`SourceHanSansSC-Regular.otf`

2. 放到 `romfs/fonts/` 目录：
   ```bash
   copy SourceHanSansSC-Regular.otf romfs\fonts\SourceHanSansCN-Regular.otf
   ```

### 3. 配置 CMake 并编译

```bash
# 配置
cmake -B build -DCMAKE_TOOLCHAIN_FILE=%DEVKITPRO%/cmake/DevkitA64Libnx.cmake

# 编译
make -C build
```

### 4. 输出文件

编译成功后，在 `build/` 目录生成：
- `switch-pctl-gui.nro` — NRO 应用文件

### 5. 安装到 Switch

将 `switch-pctl-gui.nro` 复制到 Switch SD 卡的 `/switch/` 目录，然后通过 hbmenu 启动。

## 已知问题

### 1. 中文字体加载可能不工作

`source/main.cpp` 中的字体加载代码是实验性的：

```cpp
// 尝试加载中文字体（borealis API 可能不同）
#ifdef __SWITCH__
    FILE* fontFile = fopen("romfs:/fonts/SourceHanSansCN-Regular.otf", "rb");
    if (fontFile) {
        fseek(fontFile, 0, SEEK_END);
        long fontSize = ftell(fontFile);
        fseek(fontFile, 0, SEEK_SET);
        
        unsigned char* fontData = new unsigned char[fontSize];
        fread(fontData, 1, fontSize, fontFile);
        fclose(fontFile);
        
        // 注意：这可能需要根据 borealis 版本调整
        // brls::Application::loadFont(fontData, fontSize);
        printf("Font loaded: %ld bytes\n", fontSize);
        delete[] fontData;
    }
#endif
```

**如果中文显示为方块**，需要：
1. 确认 borealis 的字体加载 API（查看 `borealis/include/borealis/application.hpp`）
2. 修改 `main.cpp` 中的字体加载代码

### 2. 菜单导航可能需要调整

`source/main_frame.cpp` 使用了 `brls::List` 和 `brls::ListItem`，这是 borealis 的原生控件。如果编译失败，可能是 API 版本不同。

**常见 API 差异**：
- 某些版本用 `brls::List`，某些用 `brls::TableView`
- `brls::ListItem` 的构造函数签名可能不同

### 3. pctl API 编码可能需要调整

`source/pctl_manager.cpp` 中的 `getTimeLimit()` 和 `setDailyTimeLimit()` 假设 `raw[2]~raw[8]` 存储的是分钟数。实际编码可能不同，需要在 Switch 上测试并调整。

## 故障排查

### 编译错误：`borealis.hpp: No such file or directory`

→ 没拉取 borealis 子模块，执行 `git submodule update --init --recursive`

### 编译错误：`DevkitA64Libnx.cmake: No such file or directory`

→ devkitPro 没安装或 `DEVKITPRO` 环境变量没设置

### 运行时崩溃：`pctlInitialize() failed`

→ Switch 没有 jailbreak（需要 Atmosphère CFW）
→ 系统版本不支持 pctl 服务

### 中文显示为方块

→ 字体文件没放到 `romfs:/fonts/`
→ 字体加载 API 调用失败（查看 Switch 控制台输出）

## 下一步开发建议

1. **在实际 Switch 上测试**，确认 pctl API 编码格式
2. **修复中文字体加载**（确认 borealis API）
3. **实现删除配置和解绑功能**（需要研究 libnx pctl 文档）
4. **添加自动测试**（单元测试 pctl 封装层）

## 文件清单

```
switch-pctl-gui/
├── CMakeLists.txt              # 构建配置
├── README.md                  # 项目说明
├── .gitignore                # Git 忽略规则
├── .gitmodules               # Git 子模块配置
├── romfs/                    # ROMFS 资源目录
│   └── fonts/                # 字体文件目录
│       └── .gitkeep
├── externals/                # 外部依赖（borealis）
│   └── borealis/             # Git 子模块
├── include/                  # 头文件
│   ├── main.h               # 主程序头文件
│   ├── main_frame.h         # 主界面 Frame
│   ├── status_view.h        # 状态卡片 View
│   ├── time_limit_frame.h   # 时长设置 Frame
│   ├── pin_change_frame.h   # PIN 修改 Frame
│   ├── confirm_dialog.h     # 确认对话框
│   ├── toast.h              # Toast 提示
│   ├── pctl_manager.h      # pctl 封装类
│   ├── utils.h              # 工具函数
│   └── i18n.h              # 国际化字符串
├── source/                  # 源文件
│   ├── main.cpp             # 应用入口
│   ├── main_frame.cpp       # 主界面实现
│   ├── status_view.cpp      # 状态卡片实现
│   ├── time_limit_frame.cpp  # 时长设置实现
│   ├── pin_change_frame.cpp  # PIN 修改实现
│   ├── confirm_dialog.cpp    # 确认对话框实现
│   ├── toast.cpp            # Toast 提示实现
│   ├── pctl_manager.cpp     # pctl 封装实现
│   ├── utils.cpp            # 工具函数实现
│   └── i18n.cpp            # 国际化字符串表
└── docs/                    # 设计文档
    ├── PRD.md               # 产品需求文档
    └── system_design.md     # 系统架构设计
```

## 联系方式

如有问题，请参考：
- [borealis 官方文档](https://github.com/nintendo-switch-custom-firmware/borealis)
- [libnx 文档](https://switchbrew.org/wiki/Homebrew_Application_Development)
- [PctlManager 参考实现](https://github.com/tailiang2008/NX-Pctl-Manager)
