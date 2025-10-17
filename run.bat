@echo off
g++ source\*.cpp source\Engine\Core\*.cpp source\Engine\Graphics\*.cpp source\Engine\Utils\*.cpp source\Game\*.cpp source\Engine\External\imgui\*.cpp -Iinclude -Iinclude/Engine/External/imgui -Iinclude/Engine/External/imgui/backends -ld3d11 -ld3dcompiler -ldxguid -ldxgi -luser32 -lgdi32 -ldwmapi -o bin\main.exe -w
bin\main.exe