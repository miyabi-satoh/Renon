@SET INCLUDE=-I"..\3rdparty\boost_1_56_0" -I"..\3rdparty\opencv-2.4.9\build\include"
@SET DEBUG_OPTIONS=-std=gnu++0x -DUNICODE -D_UNICODE -D_DEBUG -O0 -g3 -Wall -c -fmessage-length=0
@SET RELEASE_OPTIONS=-std=gnu++0x -DUNICODE -D_UNICODE -O3 -Wall -c -fmessage-length=0

@pushd %0\..
g++ %DEBUG_OPTIONS% %INCLUDE% "./include/stdafx.h" -o "./Debug/src/stdafx.gch"
windres "./Renon.rc" "./Debug/src/Renon.rc.o"
