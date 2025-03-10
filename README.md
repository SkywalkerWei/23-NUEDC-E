# 23-NUEDC_E (2024夏季学期)

是对全过程使用到的文件的完整存档。

## 简要解释

- 未``All-in-One``前按题目分解存档，All后按日期存档。
- 按照日期分类目录存档，同一天的文件夹/压缩包下可能存在多个``main.py``文件，这些文件之间不具备严格迭代优化关系。
- 个人来说只是初学者，项目可能有一些不足，仅供参考、学习、交流使用。

## 接口说明

0. 上电直接开始运行，不区分题目
1. 循环找矩形和红点
2. 返回值：
   - 2个值组成，红点的x、y坐标
   - 8个值组成，矩形顶点坐标，依次是左下、右下、右上、左上
3. 多次识别取平均值返回

## Notice

- 在光泽度高的、光滑的、反光明显的表面会出现极大的干扰。
- 红点同理，需要在较粗糙表面。
- 注意到红色激光和绿光在摄像头画面中的中心部分显著发白，实际依靠外圈颜色识别位置，可能需要调整曝光度，或者做滤波。

# 以下内容archived

## 要求

1. 复位：红色光斑能从屏幕任意位置回到原点;
2. 固定绕圈：红色光斑能在 30 秒内沿屏幕四周边线顺时针移动一周（或许只需要找到一个顶点即可）；
3. 用约``1.8cm``宽的黑色电工胶带沿``A4``纸四边贴一个长方形构成靶纸。将此靶纸贴在屏幕自定的位置。启动运动目标控制系统，红色光斑能在``30``秒内沿胶带顺时针移动一周（找四个顶点）;
4. 将3的靶随便转（还是找顶点）
5. 运动目标位置复位，一键启动自动追踪系统，控制绿色光斑能在 2 秒内追踪红色光斑，追踪成功发出连续声光提示。此时两个光斑中心距离应≤3cm。（Unknown）
6. 运动目标重复基本要求（3）~（4）的动作。绿色激光笔发射端可以放置在其放置线段的任意位置，同时启动运动目标及自动追踪系统，绿色光斑能自动追踪红色光斑。启动系统 2 秒后，应追踪成功，发出连续声光提示。此后，追踪过程中两个光斑中心距离大于 3cm 时，定义为追踪失败，一次扣 2 分。连续追踪失败 3 秒以上记为 0 分。运动目标控制系统和自动追踪系统均需设置暂停键。同时按下暂停键，红色和绿色光斑应立即制动，以便测量两个光斑中心距离。（相当于红光按照34动+暂停，绿光5+暂停）

## 题目拆解

1. 距离``1m``的白色屏幕；有效面积``60cm*60cm``；不超过``1mm``宽度的铅笔痕迹；不能存在其他标识；铅笔矩形的位置是固定的，猜测意思可能并不打算让这个矩形能被识别到。
2. 屏幕与支架可以自带到比赛评审现场，有不少于``10``分钟时间恢复与调试时间。
3. 红色激光系统中，激光位置固定，摄像头位置不限制。
4. 绿色激光系统中，激光与摄像头要放在一个东西上面，能被移动（与屏幕平行，位于红色激光笔两侧，距红色激光笔距离大于``0.4m``、小于``1m``），不能与红色激光系统存在通讯，但不强制要求摄像头也在云台上面。
5. 硬件需求：
   1. 红色 / 绿色激光；
   2. 二维云台，自转轴+俯仰轴；
   3. 白色屏幕与支架；
   4. 主控，但是不需要动；
   5. 摄像头。
6. 移动困难：两个激光都是存在角度的。当角度偏差``1°``时，激光射到距离``1m``远的屏幕，偏移量是随角度变化的。解三角形能得出一度约偏移``1.7cm``，距离基础要求的``2cm``、胶带宽度``1.8 cm``已经非常接近了。稍微动一点就会差很多。30秒的时间限制进一步要求速度。使用步进电机+高减速比齿轮？
7. 识别困难：使用QVGA一米外拍``1.8cm``胶带大概``10~20``个像素点的宽度，光斑直径小于``1cm``，更小。（调曝光度？）摄像头的广角畸变在1米外会很明显。以及，摄像头和激光不在同一直线。
8. 映射：摄像头图像映射到屏幕坐标；返回值坐标映射到云台移动。

## 猜想

1. 按下按键，由串口将题目序号发送至OpenMV运行相应题目的程序
2. 红色激光绕黑色矩形框一周：此系统的OpenMV将识别到的胶带的四个顶点以及实时的激光点的位置发送至主控
3. 自动追踪系统要求绿色激光跟踪红色激光点：此系统的OpenMV先回传胶带框顶点，再实时识别红色以及绿色激光点坐标发送至此系统主控，由主控进行云台的控制
4. 黑色矩形框识别：灰度找框，rgb找点
5. 红绿激光点的识别：ROI+阈值
