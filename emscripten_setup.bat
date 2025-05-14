git clone https://github.com/emscripten-core/emsdk.git
cd emsdk
git pull
call emsdk.bat install 4.0.6
call emsdk.bat activate 4.0.6
call emsdk_env.bat
cd ..
