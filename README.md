# HuiRuWorkStation

 ## 项目描述
  基于Qt5/C++11/OpenCV3/MySQL5的工业视觉与运动控制集成数据平台。

 ## 功能概述
​	 使用OpenCV采集图像数据
​	 通过共享内存高效传输图像
​	 核心模块异步进行目标识别与坐标计算
​	 将结果写入MySQL数据库
​	 运动模块从数据库读取指令执行动作

 ## 技术栈
​	 C++/Qt(GUI&核心逻辑)
​	 OpenCV(图像处理)
​	 MySQL(数据持久化)
​	 SharedMemory(高性能通信) 
