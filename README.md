- [Ray\_Tracing Series](#ray_tracing-series)
  - [Ray\_Tracing\_In\_One\_Weekend](#ray_tracing_in_one_weekend)
    - [Final Results](#final-results)
    - [基本思路](#基本思路)
    - [如何实现相机？](#如何实现相机)
    - [如何表示物体和相交？](#如何表示物体和相交)
    - [反走样](#反走样)
    - [材质模拟](#材质模拟)
    - [怎么产生遮挡和镜子般的反射](#怎么产生遮挡和镜子般的反射)
  - [Ray\_Tracing\_In\_Next\_Week](#ray_tracing_in_next_week)
    - [Motion Blur](#motion-blur)
    - [BVH](#bvh)
    - [Solid Texture](#solid-texture)
    - [Perlin](#perlin)

# Ray_Tracing Series
关于该项目有一些重要的点需要阐述，即整体项目的框架和实现思路  


## Ray_Tracing_In_One_Weekend

### Final Results

<div align=center><img width="1024" height="576" src="https://github.com/Xiaojie-Huang/Ray_Tracing/blob/master/RT_WEEKEND_RESULT.png"/></div>

### 基本思路
在不借助任何外部库文件的情况下，要得到最直观的渲染结果，所要做的就是直接写图像文件  
选用PPM图像文件直接写入每个像素的rgb值  
想要查看PPM图像文件强烈建议Google直接搜索PPM Viewer，可以直接在线查看图片    
   
### 如何实现相机？
对相机来说最为重要的就是定义其观察空间，本文通过指定原点和对应轴向的方式将观察空间和屏幕空间均予以定义  
当我们需要具体相机发出的射线时，可以通过当前像素点（观察空间）减去相机原点来得到

### 如何表示物体和相交？
主要是通过方程联立，根据根的情况来判断具体是否相交；  
对于物体，则使用类继承体系来综合表示

### 反走样
本篇的反走样实现起来比较简单,均值采样

### 材质模拟
对于物体表面材质的模拟，我们需要模拟光线和物体表面作用的结果
首先需要生成随机光线，随后进行效果模拟    
对于**漫反射材质**，第一种模拟方法是通过在法线方向的单位圆内随机采样，生成光线，并将其结果叠加（算是一种hack）  
第二种模拟方法是目前更为通用的模拟表面点的所有方向来的光线，和表面法线无关  
对于**金属材质**，模拟方式是通过直接计算反射后的光线进而进行交互，注意一下向量反射的实现方法  
对于**绝缘体材质**，需要考虑反射和折射，我们的模拟方法是对于每一束光线，随机选择其行为：反射或折射  
对于折射，根据snell定律
在整体材质系统中，系统需要做的是生成一个外界来的光线，并且计算该光线对最终像素颜色的贡献

### 怎么产生遮挡和镜子般的反射
遮挡是通过在Hit_Record中记录当前最近平面，在其余光线交互时，如果碰撞点求得的根不在合理范围，该次交互着色会被忽略  
着色scatter后会对该束光线进行递归，对于每束光束我们设置了一个递归深度，表示光线的最大交互次数  
当光线最终没有碰撞时，则会逃逸，返回（0，0，0）  
镜子般的反射则是通过材质控制的

## Ray_Tracing_In_Next_Week

### Motion Blur
整体思路是为光线和相机加上时间属性，运动模糊在实际拍摄时就是在快门开启和关闭时间内的所拍相片的均值  
我们要做的就是模拟这个过程，通过增加时间属性，然后在这个时间范围内随机的发射光线，最后将得到的结果均值即可

### BVH
BVH树作为优化性能的方法之一，本质上就是通过将场景细分，从而使得碰撞检测过程得到加速，不用对每个物体进行遍历  
对于生成AABB盒的过程，采用的是slab方法，即检测区域重叠，详情见图片，其描述了在二维情况下如何判断直线是否和aabb盒相交的情况
生成BVH树的过程  
1.随机选择一条轴  
2.将图元排序  
3.二分方式BVH节点中  
不断建树，叶子结点即为真正图元

### Solid Texture
贴图无非就是对于物体表面的处理，一般可以描述其表面材质，和光线如何交互  
对物体表面处理要么用代码形式解决，要么就是对图片进行查询  
贴图就是后者
Solid Texture直接无论uv坐标返回同样颜色值即可

### Perlin
柏林噪声部分个人不是很感兴趣（其实是还不是太了解），所以这里不过多展开