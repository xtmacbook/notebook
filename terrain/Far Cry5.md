## Terrain Rendering Far Cry5

### Terrain Quad Tree
> sector: 64mx64m

> world: 160 x 160 sectors;10kmx10km

> Terrain Resolution 0.5m

We subdivide each **2km** tile into a quad tree hierarchy of tiles.We stream tiles according to the LOD and distance from the player.This means that we have tens of thousands of tiles on disk.But we only need to allocate memory for, and load, a maximum of about **500 resident tiles**.
每个quadTree 节点都携带了:HeighMap;world Space normal map;Albedo map  

渲染过程:
1. 首先加载人物周边第一圈精细度最高的LOD ,然后是下一级LOD（距离主人公会更远些的LOD）；
2. 这样会有重复的区域被加载，但是我们需要确定那些是真正需要渲染的部分。我们设定的了规则：从树的顶级节点开始往下，如果满足：
 - All its children are loaded,and
 - We want to increase the level of detail at this node
则我们细划分该节点。
3. 根据viewFrumst 判断要渲染的节点
4. Batch into Shading Groups：不同的距离的节点可能使用不同的Shader。
5. 渲染

不同方案：
  1. 开始1-4 段在CPU中完成，5段的GPU中完成；
  2. 1段在CPU完成，2-5全部在GPU中完成  

２－５在GPU中执行：　
