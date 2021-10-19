#ifndef GXSERVICEQUERIER_H
#define GXSERVICEQUERIER_H

#include <ctkPluginContext.h>

#include <QPointer>

#include "GxPluginFrameworkCore.h"

/**
* @class	: CGxServiceQuerier
* @brief	: 获取服务的代理类
* @note
* @author	: w00025
*/

namespace HGxPFWCore
{
    template<typename TServiceType>
    class CGxServiceQuerier
    {
    public:
        CGxServiceQuerier(/*const */ctkPluginContext *pContext)
        {
            m_servicePtr = CGxPluginFrameworkCore::instance()->getService<TServiceType>(pContext);
        }

        /**
          * @brief 判断观察的对象是否有效
        */
        bool valid() const
        {
            if (!m_servicePtr)
            {
                return false;
            }
            return true;
        }
        operator bool() const { return valid(); }

        /// 重载运算符，方便使用
        TServiceType* operator->() const { return m_servicePtr/*.data()*/; }

        /**
          * @brief 获取服务对象指针
        */
        TServiceType* get() const { return m_servicePtr/*.data()*/; }

        /**
          * @brief 隐式转换
        */
        operator TServiceType*() { return this->get(); }


    protected:
        // 具体的服务对象指针类
        TServiceType	*m_servicePtr;
        //QScopedPointer<TServiceType> m_servicePtr;
    };
}

#endif // !GXSERVICEQUERIER_H
