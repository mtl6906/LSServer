```
0. 简介

	LSServer是一个用C++实现的HTTP中间件, 用于接收来自客户端的HTTP请求, 并根据用户部署的servlet来对请求进行处理, 再将处理的结果返回给客户端.

1. 架构

	1.1 模块

		LSServer将对HTTP报文的处理拆分成了5个模块, 详见上述架构图.

		1.1.1 HttpCore

		主要用来管理Session的分配与回收以及epoll的事件管理.

		Session的分配与回收, 见上述流程图.

		1.1.2 Recver
	
			监听来自Client的连接请求, 监听到新的连接时, core为其分配一个Session并在EventManager中注册对应的epoll事件.

		收到已有连接的请求时, 将对应的请求转化成HttpRequest对象, 存储到Session中, 并放入输出队列.

		1.1.3 Dispatcher

			对输入队列中的每个消息, 根据其uri判断是匹配上了某个servlet, 将该Session指针与HttpServlet指针, 

		封装到Conetxt中, 放入输出队列.

		1.1.4 Worker

			有两个带锁的LockedCache对象.

			ILC, 负责接收来自Dispatcher的Context.

			OLC, 负责存放处理完的Session, 由Sender取走.
	
			对ILC中的每个消息, 用Servlet处理对应的Session, 并将结果存储到Session中的HttpResponse对象中,
	
			再将Session放入OLC中.

		1.1.5 Sender

			将LockedCache中处理完毕的Session导出到输入队列.

			对输入队列中的每个消息, 将HttpRespone转换程对应的响应字符串, 回传给Client, 注销对应的事件, 并回收Session.

	1.2 组件

		1.2.1 HttpRequest

			是对HTTP请求报文的封装

		1.2.2 HttpResponse

			是对HTTP响应报文的封装

		1.2.3 Session

			是对HttpRequest和HttpResponse对象的封装
		
		1.2.4 HttpServlet
		
			一个Servlet可以对应一组服务, 负责解析Request中的参数, 调用URI对应的函数对其进行

			处理, 再将结果写到Response中.

			通常一个Servlet对应一个pojo.

			也可被用来扩展系统功能, 比如ErrorServlet, 专门用来处理错误的请求, FileServlet, 用来传输静态页面.


		1.2.5 Context

			是对Session和HttpServlet对象的封装

1.2 特点

	1. 参数可配置, 配置文件在config目录下

	2. 支持RESTful的四个HTTP方法: GET, PUT, DELETE, POST

	3. 支持log开关(在run.sh中设置)
	
1.3 编译

	cd LSServer

	sh build.sh

1.4 使用

	cp build/main .

	sh run.sh

1.5 例子

	project目录下是一个标准的项目结构, classes用于存放web项目中的servlet, www用于存放静态页面

	需要注意的是, 除了servlet依赖于LSServer之外, controller, service, dao, pojo, 是完全独立于

	LLServer的(pojo中使用了LSJson, 但LSJson实际也是与LSServer无关的一个库, 你完全可以使用另外

	一个Json库来替代它).

	servlet层用于处理函数的注册, 参数的解析, 返回值的映射.

	函数的注册: 指将一个controller对应的成员函数绑定到一个对应的uri上

	参数的解析: 注册的函数是被封装在一个lambda表达式中的, 其中进行了请求数据到参数的转换, 例子

		    中是将Json格式的字符串转换为了Json对象, 并传递给对应的constroller成员函数

	返回值的映射: 将成员函数的返回值映射到LSServer中对应的HTTP返回值, 这将决定响应的状态码

	controller, service, dao层的作用, 这里就不多介绍了, 是否采用此模型完全取决于你.

	换句话说, servlet层之下就完全由开发人员进行设计了, 只要将对应的函数注册到servlet中就好.

	自定义的目录也要添加到CMakeLists.txt中, 如需引用外部的动态库也由开发人员在CMakeLists.txt

	中自行修改.

1.6 性能测试

	//	端口在config中进行配置

	使用wrk -c 10000 -t1 -T10 http://127.0.0.1:8080/1.html进行测试

	要在root模式下(sudo su), 使用ulimit -n 65535将fd的大小调整, 否则最大只能测试出1000qps

	(wrk和./lsserver所运行的终端都要修改这个值)

	本机配置是intel i5 8400, 内存8G, 使用上述命令进行测试得到的qps大约是3w左右

1.7 问题和建议

	见issue.

```
