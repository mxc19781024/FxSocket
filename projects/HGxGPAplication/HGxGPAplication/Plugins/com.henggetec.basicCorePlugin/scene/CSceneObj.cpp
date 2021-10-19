#include <QLayout>
#include <QTimer>
#include <QEvent>
#include <QDragEnterEvent>

#include "scene_ui/ISceneUiService.h"

#include "CSceneObj.h"
#include "public/CSceneDndManager.h"
#include "CSceneDndManager_p.h"
#include "public/ISceneDropInterface.h"

#include <gdpScene/CommonGeo.h>
#include <bdpScene/scene/GxSceneBase.h>
#include <bdpScene/scene/GxControlManager.h>

CSceneObj::CSceneObj(HGxScene::CGxSceneBase* pScene, QWidget *pDockWgt, QObject *parent)
	: ISceneInterface(parent)
	, m_pFxScene(pScene)
	, m_pDockWgt(pDockWgt)
{
// 	QWidget *pSceneWgt = sceneWidget();
// 	pSceneWgt->setMinimumSize(200, 200);
// 	
// 	// 接受drop
// 	pSceneWgt->setAcceptDrops(true);
// 
// 	pSceneWgt->installEventFilter(this);

	pDockWgt->setMinimumSize(200, 200);

	// 接受drop
	pDockWgt->setAcceptDrops(true);

	pDockWgt->installEventFilter(this);
}

CSceneObj::~CSceneObj()
{
	if (m_pDockWgt != nullptr)
	{
		delete m_pDockWgt;
		m_pDockWgt = nullptr;
	}
}

std::string CSceneObj::sceneId() const 
{
	return m_pFxScene->getID();

	return 0;
}

QString CSceneObj::sceneName() const 
{
	return QString::fromUtf8(m_pFxScene->getObjectName());
}

QString CSceneObj::sceneType() const 
{
	return QString::fromUtf8(m_pFxScene->getObjectType());
}

QWidget *CSceneObj::sceneWidget() const 
{
	//return m_pFxScene->getWidget();
	return NULL;
}

QWidget *CSceneObj::dockWidget() const 
{
	return m_pDockWgt;
}

QWidget *CSceneObj::getSceneRenderWgt()
{
// 	QLayout *pLayout = m_pFxScene->getWidget()->layout();
// 	if (pLayout)
// 	{
// 		QLayoutItem *pItem = pLayout->itemAt(0);
// 		if (pItem)
// 		{
// 			return pItem->widget();
// 		}
// 	}
	return nullptr;
}


HGxScene::CGxSceneBase* CSceneObj::fxScene() const
 {
 	return m_pFxScene;
 }

bool CSceneObj::eventFilter(QObject *obj, QEvent *ev)
{
	switch (ev->type())
	{
	case QEvent::DragEnter:
		{
 			CSceneDndManager *pDndMgr = CSceneDndManagerPrivate::instance();
   			QDragEnterEvent *event = static_cast<QDragEnterEvent *>(ev);
   			if (pDndMgr->isDroppable(event))
   			{
   				event->acceptProposedAction();
   			}
		}
		break;
	case QEvent::Drop:
	{
		QDropEvent *event = static_cast<QDropEvent *>(ev);
		event->acceptProposedAction();

		QPoint screenPos = event->pos();
		CSceneDndManager *pDndMgr = CSceneDndManagerPrivate::instance();
		ISceneDropInterface *pDropHandler = pDndMgr->dropHandler(event);
		if (pDropHandler != nullptr)
		{
			if (sceneType() == GX_EARTH_SCENE_TYPE)
			{
				pDropHandler->handleDrop(sceneId(), screenPos);
			}
			else if (sceneType() == GX_MAP_SCENE_TYPE)
			{
				pDropHandler->handleDrop(sceneId(), screenPos);
			}
		}
	}
		break;
	default:
		break;
	}
	return QObject::eventFilter(obj, ev);
}
