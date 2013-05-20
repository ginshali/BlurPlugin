mkdir Publish
mkdir Publish\x64
mkdir Publish\x64\BlurPlugin
mkdir Publish\x86
mkdir Publish\x86\BlurPlugin

copy Release\x64\BlurPlugin.dll Publish\x64
copy h_blur.hlsl Publish\x64\BlurPlugin
copy v_blur.hlsl Publish\x64\BlurPlugin

copy Release\Win32\BlurPlugin.dll Publish\x86
copy h_blur.hlsl Publish\x86\BlurPlugin
copy v_blur.hlsl Publish\x86\BlurPlugin