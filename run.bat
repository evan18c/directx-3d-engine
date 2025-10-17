@echo off
g++ source\*.cpp source\Engine\Core\*.cpp source\Engine\Graphics\*.cpp source\Engine\Utils\*.cpp source\Game\*.cpp -Iinclude -ld3d11 -ld3dcompiler -ldxguid -ldxgi -luser32 -lgdi32 -o bin\main.exe -w
bin\main.exe