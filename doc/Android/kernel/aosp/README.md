* intro


AOSP 通用内核是长期支持 (LTS) 内核的下游，包含与 Android 社区相关但尚未合并到 LTS 的补丁程序。这些补丁程序可能包括：

1. 针对 Android 需求定制的功能（例如交互式 cpufreq 调节器）。

1. 由于实现方面的问题而被上游拒绝的功能（例如 MTP/PTP、Paranoid Networking）。

1. 可供 Android 设备使用但仍处于开发上游阶段的功能（例如 Energy Aware Scheduling/EAS）。

1. 对其他方有用的供应商/原始设备制造商 (OEM) 功能（例如 sdcardfs）。

https://android.googlesource.com/kernel/common/


最大的特性包括：

1. 19.8% Energy Aware Scheduling (kernel/sched)

1. 13.8% 网络 (net/netfilter)

1. 13.5% Sdcardfs (fs/sdcardfs)

1. 9.4% USB (drivers/usb)

1. 7.2% SoC (arch/arm64, arch/x86)

1. 6.2% f2fs（fs/f2fs - 从上游向后移植）

1. 6.1% 输入 (drivers/input/misc)

1. 5.4% FIQ 调试程序 (drivers/staging/android/fiq_debugger)

1. 3.6% Goldfish 模拟器 (drivers/platform/goldfish)

1. 3.4% Verity (drivers/md)

1. 11.6% 其他
