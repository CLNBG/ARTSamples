========================================================================
			工程功能说明
========================================================================
1、本卡为同步采样，最大采样频率为 800K
2、每个通道可选择不同的模拟输入量程；
4、分非空方式、半满方式和DMA两种方式来读取AD数据，DMA方式效率更高。
========================================================================
			工程文件说明
========================================================================
PCI9757.h 驱动接口头文件（用户开发自已的应用程序时，将此文件复制到您的工程中，且此文件
          不要改动, 在使用前，请在您的源程序中加入语句#include "PCI9757.h"即可）
PCI9757.lib 驱动接口导入库文件（用户开发自已的应用程序时，将此文件复制到您的工程中)

ADThread.cpp 使用子线程进行AD采样的源代码演示
HistDataDoc.cpp HistDataFrm.cpp HistDataView.cpp AD历史数据回放的源代码演示
DAView.cpp 演示了如何操作DA输入输出过程
Globalvar.h 包含了用户自定义的各种全局变量
UserDef.h 包含了自定义变量类型等信息
========================================================================
			工程完善记录
========================================================================
2013-05-02 V6.00.00 新增此演示工程 ZHM	
2014-09-24 V6.00.01 添加获取权限 LYB

========================================================================
			工程中主要类关系
========================================================================
CWinApp
	CSysApp

CMDIFrameWnd
	CMDITabFrameWnd
		CMainFrame

CMDIChildWnd
	CADFrm
	CDIOFrm
	CPLSFrm

CDocument
	CADDoc
	CDIODoc
	CPLSDoc

CScrollView
	CADDigitView
	CADWaveView

CFormView
	CADParaCfgView
	CADStatusView
	CDIOView
	CPLSView

CWnd
	COScopeCtrl