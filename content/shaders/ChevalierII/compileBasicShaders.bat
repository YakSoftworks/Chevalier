C:\VulkanSDK\1.4.309.0\Bin\glslc.exe basicVertShader.vert -o basicVert.spv
C:\VulkanSDK\1.4.309.0\Bin\glslc.exe basicFragShader.frag -o basicFrag.spv

echo Compile Lights

C:\VulkanSDK\1.4.309.0\Bin\glslc.exe ../Lights/Light.vert -o LightVert.spv
C:\VulkanSDK\1.4.309.0\Bin\glslc.exe ../Lights/Light.frag -o LightFrag.spv

echo Validate Shaders



pause