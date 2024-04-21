@echo off
set Engine_PATH=I:\Epic\UE_5.1\Engine
set PLUGIN_PATH=J:\Inoland\Perforce\UnrealEngineProjects\InoNation\Plugins\InoPlugins\inoessenials\InoEssentials.uplugin
set OUTPUT_DIR=J:\Inoland\Perforce\UnrealEngineProjects\InoNation\Plugins\InoPlugins\inoessenials\Packages
set TargetPlatforms=Linux
start /B /wait %Engine_PATH%\Build\BatchFiles\RunUAT.bat BuildPlugin -Plugin=%PLUGIN_PATH% -Package=%OUTPUT_DIR% -TargetPlatforms=%TargetPlatforms%
pause