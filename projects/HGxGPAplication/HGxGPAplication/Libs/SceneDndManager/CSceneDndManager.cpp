#include <QDragEnterEvent>
#include <QDropEvent>

#include "public/ISceneDropInterface.h"

#include "CSceneDndManager_p.h"
#include "qevent.h"
#include <QMimeData>


CSceneDndManagerPrivate *CSceneDndManagerPrivate::s_pInstance = nullptr;

CSceneDndManagerPrivate::CSceneDndManagerPrivate(QObject *object)
	: CSceneDndManager(object)
{
	s_pInstance = this;
}

CSceneDndManagerPrivate::~CSceneDndManagerPrivate()
{

}

CSceneDndManagerPrivate *CSceneDndManagerPrivate::instance()
{
	if (NULL == s_pInstance)
	{
		new CSceneDndManagerPrivate;
	}

	return s_pInstance;
}

bool CSceneDndManagerPrivate::registerHandler(ISceneDropInterface *pDropHandler)
{
	if (m_setDropHandlerObj.find(pDropHandler) == m_setDropHandlerObj.end())
	{
		m_setDropHandlerObj.insert(pDropHandler);
		return true;
	}
	return false;
}

bool CSceneDndManagerPrivate::unRegisterHandler(ISceneDropInterface *pDropHandler)
{
	if (m_setDropHandlerObj.find(pDropHandler) != m_setDropHandlerObj.end())
	{
		m_setDropHandlerObj.remove(pDropHandler);
		return true;
	}
	return false;
}

bool CSceneDndManagerPrivate::isDroppable(QDragEnterEvent *event)
{
	foreach (auto pDropHandler, m_setDropHandlerObj)
	{
		if (event->mimeData()->hasFormat(pDropHandler->mimeType()))
		{
			return true;
		}
	}
	return false;
}

ISceneDropInterface *CSceneDndManagerPrivate::dropHandler(QDropEvent *event)
{
	foreach (auto pDropHandler, m_setDropHandlerObj)
	{
		if (event->mimeData()->hasFormat(pDropHandler->mimeType()))
		{
			return pDropHandler;
		}
	}
	return nullptr;
}

void CSceneDndManagerPrivate::dndObjDestroyedSlot(QObject *)
{

}
