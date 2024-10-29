# GAMES101

## SETUP

1. Confirm `GenProj4VS2022.bat`

2. Run `vendor/premake/premake5.exe` to generate the solution file for visual studio 2022.

---

## LABS

[All labs](https://games-cn.org/forums/topic/allhw/)

---

### lab1 

Ignored.

---

### lab2

|||
| :---: | :---: |
| Naive | ![Naive](/images/lab2_Naive.png) |
| MSAA | ![MSAA](/images/lab2_4xMSAA.png) |
| SSAA | ![SSAA](/images/lab2_4xSSAA.png) |

---

### lab3

| | |
| :---: | :---: |
| normal | ![normal](/images/lab3_normal.png) |
| phong | ![phong](/images/lab3_phong.png) |
| texture | ![texture](/images/lab3_texture.png) |
| bump | ![bump](/images/lab3_bump.png) |
| displacement | ![displacement](/images/lab3_displacement.png) |



Bilinear interpolation implemented:

| | naive | bilinear |
| :---: | :---: | :---: |
| 1024x1024 | ![naive](/images/lab3_texture.png) | ![bilinear](/images/lab3_texture_bilinear.png) |
| 512x512 | ![naive](/images/lab3_texture_512x512.png) | ![bilinear](/images/lab3_texture_512x512_bilinear.png) |

---

### lab4

Ignored.

---

### lab5

Whitted-Style Ray Tracing:

![whitted](/images/lab5_whitted.png)

---

### lab6

Whitted-Style Ray Tracing with BVH acceleration: 

| | Middle Split | SAH |
| :---: | :---: | :---: |
| BVH Generation time(secs)| 0 | 13 |
| Rendering time(secs) | 4 | 4 |

![BVH](/images/lab6_BVH.png)

### lab7

Path Tracing:

| spp | |
| :---: | :---: |
| 1 | ![spp1](/images/lab7_spp1.png) |
| 2 | ![spp4](/images/lab7_spp2.png) |
| 4 | ![spp4](/images/lab7_spp4.png) |
| 16 | ![spp16](/images/lab7_spp16.png) |
