#ifndef CEARTHLAYERMANAGER_H
#define CEARTHLAYERMANAGER_H

#include <QObject>
#include <QColor>

class CLayerConfig
{
public:
	// 场景类型
	enum SceneType
	{
		MapType,			// 二维
		EarthType			// 三维
	};

	// 树节点类型
	enum TreeItemType
	{
		TopCategory,		// 一级分类
		SecondCategory,		// 二级分类
		LayerItem,			// 图层项

		SeaLayerCategory // 海图add by yq00012 
	};

	// 图层类型
	enum LayerType
	{
		UndefinedLayer,		// 未定义
		ImageLayer,			// 影像图层
		ElevationLayer,		// 高程图层
		VectorLayer,		// 矢量图层
		EffectLayer,		// 效果图层
		PlottingLayer,		// 标绘图层
		LiveLayer,			//实景图层
		SeaLayer           // 海图图层
	};

	// 矢量图层类型
	enum VectorLayerType
	{
		DotType,	
		LineType, 
		FacetType
	};

	// 可见范围
	struct StVisibleRange
	{
		double minVal;	// 最小值
		double maxVal;	// 最大值
	};

	// 矢量点图层信息
	struct StDotLayerInfo 
	{
		QString layerName;	// 名称
		QString dataPath;	// 路径
		StVisibleRange visibleRange;	// 可见范围
		bool isallVisible;                   // 是否全可见
		QString fontName;		// 字体类型
		float fontSize;			// 字体大小
		QColor fontColor;		// 字体颜色
		QString iconPath;		// 图标路径
		double iconSize;			// 图标大小
	};

	// 矢量线图层信息
	struct StLineLayerInfo 
	{
		QString layerName;	// 名称
		QString dataPath;	// 路径
		StVisibleRange visibleRange;	// 可见范围
		bool isallVisible;                   // 是否全可见
		QColor lineColor;	// 线色
		float lineWidth;	// 线宽
		unsigned int lineType;	// 线型
		float opacity;		// 不透明度
	};

	// 矢量面图层信息
	struct StFacetLayerInfo 
	{
		QString layerName;	// 名称
		QString dataPath;	// 路径
		StVisibleRange visibleRange;	// 可见范围
		bool isallVisible;                   // 是否全可见

		QColor lineColor;	// 线色
		float lineTransparency;		// 线不透明度
		float lineWidth;	// 线宽
		unsigned short lineType;	// 线型
		QColor fillColor;	// 填充色 
		float fillTransparency;		//填充色不透明度
	};

	// 影像图层信息
	struct StImageLayerInfo
	{
		QString layerName;	// 名称
		QString dataPath;	// 路径
		float opacity;		// 不透明度
		StVisibleRange visibleRange;	// 可见范围
		bool isallVisible;                   // 是否全可见
		bool cacheEnable;              // 缓存开关
	};

	// 高程图层信息
	struct StElevationLayerInfo
	{
		QString layerName;	// 名称
		QString dataPath;	// 路径
		bool cacheEnable;              // 缓存开关
	};

	// 实景图层信息
	struct StLiveLayerInfo
	{
		QString strName;		// 名称
		QString strDescription;	// 描述
		double dLon;			// 经度
		double dLat;			// 纬度
		int nHeight;			// 高度
		int nType;				// 路径类型
		QString strPath;		// 路径
		int nScale;				// 缩放系数
		double dRotate;			// 旋转角度
	};
};

#endif // CEARTHLAYERMANAGER_H
