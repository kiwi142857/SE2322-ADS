## 项目结构

``` c
├── 522031910299+陈启炜+Lab2.md  // 实验报告
├── data  // 测试数据
│   └── siftsmall
│       ├── base.bvecs
│       ├── gnd.ivecs
│       └── query.bvecs
├── hnsw.cpp  // 测试代码
├── Lab2_HNSW.pdf  // 项目任务要求
├── log.txt  // 输出结果文件
├── Makefile // Makefile
├── README.md // 编译方式说明
├── src  // 实现代码
│   ├── base.hpp
│   └── hnsw.hpp
├── test.cpp  // 性能测试文件
└── util  // 辅助函数
    ├── ground_truth.hpp
    ├── parameter.hpp
    ├── util.hpp
    └── vecs_io.hpp
```

## 编译、运行测试

1. 正确性测试


``` c
修改./util/parameter.hpp参数
// 编译链接正确性测试⽂件
make all
// 运⾏正确性测试
make grade
// 清理编译结果
make clean
```

输出结果显示在终端上
eg.
``` c
load ground truth
load query
load base
querying
M: 20, M_max: 30, ef_construction: 100, ef_search: 100
average recall: 0.999, single query time 0.3 ms
```

2. 性能测试
``` c
修改./util/parameter.hpp参数,设置是否多线程
// 编译链接正确性测试⽂件
make all
// 运⾏正确性测试
make grade
// 清理编译结果
make clean
```

输出结果显示在终端上
eg.
``` c
M: 20, M_max: 30, ef_construction: 100, ef_search: 100
load ground truth
load query
load base
average insert time 60289.7 ns
querying
average recall: 0.999, single query time 507890.0 ns
```

