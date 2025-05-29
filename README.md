# 🚀 CCLogger - 一个高性能、可扩展的异步日志框架

**CCLogger** 是一款为高并发环境设计的日志库，采用经典的生产者-消费者模型，支持异步写入，具备可定制化的日志格式化器和多种 IO 方式。

---

## ✨ 特性亮点

✅ **异步高性能**
日志写入操作与文件 IO 分离，支持数十万行日志的极速写入。
写入接口使用无阻塞队列，后台线程专注日志写入，主线程无延迟！

✅ **同步与异步刷新**

* `flush()`：flush支持异步刷新日志到文件中！
* `sync_flush()`：主线程等待日志真正写入完成后再继续，保障数据完整性。

✅ **灵活可扩展**

* 动态替换 `LoggerFormatFactory`，轻松自定义日志格式（如时间戳、线程 ID、源代码位置信息等）。
* 支持自定义 IO 设备（文件、控制台、网络等），通过抽象接口实现。

✅ **安全的并发支持**

* 内部所有队列操作均为原子操作，线程安全无忧。
* 多线程环境下，性能依旧稳定。

---

## 🖋️ 使用示例

当然，你可以直接参考 [主示例代码](main.cpp) 获得更好的代码阅读体验！

```cpp
#include "IO/fileio.h"
#include "format/logger_format.h"
#include "logger/logger.h"
#include <iostream>
#include <thread>
#include <vector>

// Quick Example!
int main() {
	auto io = new FileIO("example_log.txt");
	CCLogger logger(io);

	auto formatter = new DefLoggerFormatFactory();
	formatter->set_enable_time(true);
	formatter->set_enable_threadid(true);
	logger.set_formattor(formatter);

	const int numThreads = 4;
	const int logsPerThread = 1000;

	std::vector<std::thread> threads;
	for (int i = 0; i < numThreads; ++i) {
		threads.emplace_back([i, logsPerThread, &logger]() {
			for (int j = 0; j < logsPerThread; ++j) {
				logger.push_message("Thread " + std::to_string(i) + " - Message " + std::to_string(j));
			}
		});
	}

	for (auto& t : threads)
		t.join();
	/* force all write in a sync way */
	logger.sync_flush();

	std::cout << "Logging complete! Logs written to 'example_log.txt'.\n";
	return 0;
}
```

---

## 🚀 测试

为了让所有的测试具备说服力，请您单独运行[测试](test)文件夹下的文件测试所有的接口是否正常工作，以及评估测试主机的性能测试！

在笔者的测试机器上得到的结果为：

```
==== 接口测试 ====
接口测试：已写入 3 条日志。

3
日志完整性测试：文件中有 3 条，期望 >= 3
==== 边界条件测试 ====
边界测试：写入空日志、超长日志、正常日志。

==== 正确性测试 ====
100
日志完整性测试：文件中有 100 条，期望 >= 100

==== 压力测试（高并发） ====
并发写入 1000000 条日志，耗时 3019 ms
写入速率：331236 条/秒
```



---

## ⚙️ 快速扩展

* **自定义 IO**：继承 `AbstractIO` 实现 `write_logger`、`force_flush`，支持写入网络、数据库等。
* **自定义格式化器**：继承 `LoggerFormatFactory`，您可以自己自由实现自己的 `format`方法

## 注意！

⚠ 目前sync_flush调用后立即读取仍然存在潜在的再读取时，因此当完成极大量文件读写后，请强制延时若干秒后再次检查读取！
