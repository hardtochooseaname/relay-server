### V1是纯套接字编程的echo服务器  

### v2在此基础上封装了套接字编程底层的函数  
新增类：  
- Socket：套接字相关函数的封装  
- InetAddr：地址结构的封装  
- Buffer：对套接字收发数据所用缓冲区的封装  

### V3是epoll实现的echo服务器端程序  
没有新增类，仅在V2基础上通过epoll实现了并发  

### V4对epoll进行封装并实现基础的reactor模型  
新增类：    
- Channel：与一个连接相关的所有东西  
- Epoller：对epoll的封装  
- Eventloop：对轮询处理就绪事件的while循环封装  

### V5在V4基础上进一步理清楚类之间的关系，让封装更优雅  
三个类的关系：  
1. Epoller不依赖于其他任何类，构造函数里面就是创建一个空的epoll实例，同时提供操纵epoll的接口函数  
2. Channel持有Epoller的指针，需要使用其接口update事件  
3. Eventloop持有Epoller的指针，需要在循环中调用Epollerwait；也持有就绪channel的指针列表，for中调用eventsHandler 
 
修改类：  
- 更改三者关系为：Channel -> Socket -> Buffer：只用创建一次channel->channel创建socket->socket创建buffer   

### V6把中继服务端的业务逻辑抽象成一个Server类  
新增类：  
- Server：整个中继服务器业务的抽象，包含了acceptor的业务逻辑，中继的逻辑单独拿出来抽象成了Proxy类   
- Proxy：中继/代理服务的抽象    
- Task：服务器业务逻辑的进一步抽象，使得在Server中直接调用Task接口，而Task封装了实际的业务逻辑（中继、回射等）   
这样当服务器想要实现不同的服务时，就只用：   
1. 新增对应的业务逻辑（类似于Proxy类）  
2. 把业务封装到新的Task类中 --> "****Task.cpp"，同时在Task.h中新增必要的修改  
3. 修改CMakeLists.txt，把之前服务的"****Task.cpp"改成最新的，添加新的可执行性文件  

### V7在V6基础上进一步完善
整理目录结构：两个main重命名并放在根目录（客户端、服务端）  
补充服务端的echo业务（TaskEcho和Echo类）  
修改CMakeLists.txt，生成三个可执行文件  

完善各个情况下发送到cout的提示信息


### V8利用继承和虚函数的多态特性，重构Task类
修改：  
1. 设置Task为抽象基类，ProxyTask和EchoTask为派生类  
2. Server类、Task类的解耦，两者持有Eventloop  
3. 修改Socket和Buffer类，Socket继承自Buffer，使得两者只向上暴露一个，上层通过Socket进行读写    


### V9把用户端改成多线程 + 改进客户端  
用户端：直接用POSIX实现最基本的多线程   
客户端：  
- 封装log类，实现日志记录  
1. 一个log基类，传入文件路径，初始化打开文件，两个主要接口（append日志记录，查阅日志记录）  
2. 一个proxylog派生类，
3. 每条记录格式：timestamp,  



### V10的修改方向
客户端修改：
- 客户端从流中读取需要发送的数据，使得可以把流定向到标准输入或者文件或者string流  
- 客户端启动时从命令行同时获取代理服务器和目标服务器的信息，然后创建新的客户端连接时，建立到代理的连接后，自动向代理发送目标服务器的信息，让其建立连接  
- 需要增加状态机处理：
  - 正在建立到代理服务器连接  
  - 正在建立到目标服务器的中继连接  
  - 已建立到目标服务器的中继连接   
  - 断开连接  


代理服务器修改：  
- 状态机：  
  - 等待解析目标服务器地址（如果地址出错，向client反馈，并断开到客户端连接，进入到断开连接状态；如果地址正确，进入到连接目标服务器状态） 
  - 正在连接到目标服务器（成功连接或者无法连接）  
  - 无法连接到目标服务器（向客户端反馈，同时断开与客户端连接，进入到断开连接状态）  
  - 成功连接到目标服务器（向客户端反馈，进入到以建立中继连接状态）  
  - 已建立中继连接（这是正常工作状态 - 如果客户端发送EOF或者某处连接出错，则都进入断开连接状态）  
  - 正在断开连接（判断只用断开一端还是两端连接，进入到已断开连接状态）  
  - 已断开连接状态（task对象已经被加入到已结束任务队列中等待被回收了）  