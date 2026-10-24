# FAILED

## 基本信息
不详（整体很快）/ 直接务实 / 平稳

## 面试流程
自我介绍 → 项目深挖（veRL/AReaL 异步 post-training） → CUDA 软硬件基础 → 反问

## 问题清单
| # | 类型 | 问题 | 我的回答（≤15字） | 正确性 |
|---|---|---|---|---|
| 1 | 项目 | 异步 post-training 为什么能加速？ | 生成训练流水重叠，换吞吐 | ✅ |
| 2 | 项目 | 同步与异步的区别？ | 严格交替 vs 流水并行 | ✅ |
| 3 | 项目 | GRPO 怎么算 advantage？ | 组内相对奖励，减均值除标准差 | ✅ |
| 4 | 项目 | 2/6→4/4 的依据？ | rollout 是瓶颈，补推理卡 | ✅ |
| 5 | 项目 | 推测解码怎么用于 rollout？ | draft 猜、target 并行验证 | ⚠️ |
| 6 | 项目 | 昇腾与 GPU 的差异？ | CANN/HCCL 生态与算子差异 | ✅ |
| 7 | CUDA | SM/warp/thread 关系？ | grid>block>warp>thread | ✅ |
| 8 | CUDA | occupancy 受什么限制？ | 寄存器、共享内存、block 大小 | ✅ |
| 9 | CUDA | 合并访存与 bank conflict？ | 连续地址；避免同 bank | ✅ |
| 10 | CUDA | 如何判断带宽/算力瓶颈？ | 用 roofline 看 compute/memory | ✅ |
| 11 | CUDA | stream 与 CUDA graph 作用？ | 异步 overlap；降 launch 开销 | ✅ |

## 逐题复盘

### Q5 推测解码怎么用于 rollout？
- 问题：把 decode 笼统归为带宽 bound，未说明 spec decoding 的收益前提。
- 正确答案：
  1. 小 batch/带宽 bound 收益大；大 batch 转 compute bound，收益下降。
  2. 加速比取决于接受率与 draft 开销，需满足收益条件。
  3. draft 实时更新要权衡更新成本与分布漂移。

## 手撕代码
无

## 反问
问团队业务：10B 具身/自驾模型训练，并向集成厂商交付算力卡。

## 薄弱知识点
推测解码收益边界，异步 RL staleness 控制，大 batch 性能建模

## 行动项
- [ ] 用接受率/加速比公式讲清推测解码何时划算。
- [ ] 补异步 RL 的 staleness 控制与重要性采样。
- [ ] 准备 10B 模型具身/自驾场景的训练推理瓶颈案例。
