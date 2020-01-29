# 火车订票系统 #

# 开发背景：
	解决线下排队购票难的问题;
	实现火车车次信息的查询、显示功能，可以帮助用户方便、快捷地预定火车票，还可以对用户订票信息进行保存。

# 需求分析：
	本系统以用户预定火车票的一系列流程为主线，将火车车次详细信息进行显示、保存;
	同时提供火车的剩余票数，以供用户查询，决定是否预定;
	当预定成功后，提供保存用户的订票信息的功能。

# 系统设计：
1、系统目标：
	显示火车车次信息及可供订票数;
	对输入车次或要到达的城市提供查询;
	输入要到达城市的车次信息，确定是否定票;
	可对输入的火车车次信息进行修改;
	显示火车车票信息;
	对火车车票信息及订票人的信息进行保存;

2、系统功能结构：
	1）输入火车票信息
	2）查询火车票信息
	3）订票
	4）修改火车票信息
	5）显示火车车票信息
	6）保存订票信息和火车票信息到指定的磁盘文件
	主要由以上6个模块组成。
