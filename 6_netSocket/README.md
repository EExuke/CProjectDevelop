# 网络通信系统 #

# 开发背景：
	局域网下基于TCP/IP协议，使用Socket编程实现的网络通信系统

# 需求分析：
	功能完善，能够进行扩展。
	能够进行点对点连接，也可以通过服务器进行消息的中转。
	系统的每个但愿相互独立。
	能够进行多种方式的连接。
	应保证发送消息的实时性和准确性。
	能够保存聊天记录
	系统简单实用，操作简便
	设计周到，增加程序的实用性

# 系统设计：
1、系统目标：
	基于TCP/IP协议，设计具有2种不同网络拓扑的通信系统。
	
2、系统功能结构：
	网络通信系统：
		点对点客户端
		点对点服务端
		服务器中转服务端
		服务器中转客户端

