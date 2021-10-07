# LearnOpenGL
OpenGL学习记录 , 使用了GLFW/GLAD/GLM/stb_image.h等第三方库.

# Demo
所有的Demo都是以 **test_** 开头的 **.cpp** 文件 , 在 **./OpenGL** 中.  
模型、贴图等资源在 **./OpenGL/Resources** 下,可以选择性下载.

* test.cpp  
  光照、阴影、顺序无关的混合(OIT)、天空盒(采用立方体贴图).  
    
  <image src="https://user-images.githubusercontent.com/57032017/135709547-9e4b50d0-1eb1-44e8-9b2f-e4aedb8f9549.png" width="60%">

* test_externalModel.cpp  
  外部模型导入  
    
  <image src="https://user-images.githubusercontent.com/57032017/135709348-5f90cb09-c391-4673-a442-38e0098d30f3.gif" width="60%">

* test_instancing.cpp  
  OpenGL实例化(快速渲染大量世界坐标不同的相同物体)  
  
  <image src="https://user-images.githubusercontent.com/57032017/135709186-81b4bd87-5d1f-437d-a4a7-6659d294fc17.gif" width="60%">
  
* test_test_stencilTesting.cpp  
  模板测试(示例为添加边框. 左为原模式,右为电视模型添加边框)  
    
  <table><tr>
  <td><image src="https://user-images.githubusercontent.com/57032017/135757212-c7f92c2e-b266-4c69-83a2-e3119fa8fde9.png" width=100% height="100%" border=0></td>
  <td><image src="https://user-images.githubusercontent.com/57032017/135757097-a1c34753-2be2-412f-b6e3-5d9f8b230358.png" width=100% height="100%" border=0></td>
  </tr></table>
  
* test_frameBuffers.cpp / test_frameBuffers2.cpp  
  帧缓冲(将一帧的渲染结果存到Texture,再渲染这个Texture,采样时可以做出很多效果. 图示:右上角显示灰度画面)  
    
  <image src="https://user-images.githubusercontent.com/57032017/135990331-3e4478a2-8c59-4e64-88c6-470684313082.gif" width="60%">  
  
* test_cubeMap.cpp  
  立方体贴图、环境反射、环境折射  
      
  <table><tr>
  <td><image src="https://user-images.githubusercontent.com/57032017/136307394-bc60ed3c-ee0a-4902-8c5f-ae0225aaba11.gif" width=100% height="100%" border=0><p>反射</p></td>
  <td><image src="https://user-images.githubusercontent.com/57032017/136307433-e56a8ba0-a70b-4bf7-a187-634b3e7e95ae.gif" width=100% height="100%" border=0><p>折射(玻璃电视)</p></td>
  </tr></table>
