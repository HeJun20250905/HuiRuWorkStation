@echo off
:: -------------------------------------------------------
:: 【核心修改】强制切换到脚本所在目录
cd /d %~dp0
:: -------------------------------------------------------

chcp 65001 >nul
setlocal enabledelayedexpansion

:: 1. 设置颜色
color 0A

:: 2. 获取路径
set "CurrentDir=%~dp0"

:: 3. 清理当前目录
echo -------------------------------------------------------
echo [当前目录] 正在清理: "%CurrentDir%"
echo -------------------------------------------------------

:: 删除当前目录下的 .vs 文件夹
set "VsPath=%CurrentDir%.vs"
if exist "!VsPath!" (
    echo [删除中] .vs 文件夹 ...
    rd /s /q "!VsPath!"
) else (
    echo [跳过] 未找到 .vs 文件夹
)

:: 删除当前目录下的 out\build 文件夹
set "OutBuildPath=%CurrentDir%out\build"
if exist "!OutBuildPath!" (
    echo [删除中] out\build 文件夹 ...
    rd /s /q "!OutBuildPath!"
) else (
    echo [跳过] 未找到 out\build 文件夹
)

echo -------------------------------------------------------
echo 清理完成！
echo -------------------------------------------------------
pause