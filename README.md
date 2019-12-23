# RayTracing Project

整个project的基本框架参考
[!][https://github.com/RayTracing/raytracing.github.io/tree/master/src/InOneWeekend]

里的 main.cc

做出来的效果也是若干反射率折射率不一的物体在背景光照射下组成的图像。
可能会增加由键盘控制的视角移动观察的功能。


#场景构建
大概需要实现的是：
- 向场景过中的若干物体(比如sphere, box,其他obj)
- 设置背景光（平行光或点光源）
- 和 通过AbstractItem进行交互,再讨论
  
#碰撞检测
- 实现更加详细的Ray
- 对每个探测光线，进行

#视角移动
- 和Camera通过getPixel进行交互
- 键盘事件的相应，视角的变化。