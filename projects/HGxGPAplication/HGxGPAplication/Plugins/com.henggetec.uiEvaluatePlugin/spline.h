/**
* @brief  计算绘制贝塞尔曲线需要的第一个控制点
* @param  points [in] 序列映射坐标系上的绝对坐标点
* @return QVector<QPointF> [out] 控制点
* @note
*/
QVector<qreal> firstControlPoints(const QVector<qreal>& vector)
{
	QVector<qreal> result;

	int count = vector.count();
	result.resize(count);
	result[0] = vector[0] / 2.0;

	QVector<qreal> temp;
	temp.resize(count);
	temp[0] = 0;

	qreal b = 2.0;

	for (int i = 1; i < count; i++)
	{
		temp[i] = 1 / b;
		b = (i < count - 1 ? 4.0 : 3.5) - temp[i];
		result[i] = (vector[i] - result[i - 1]) / b;
	}

	for (int i = 1; i < count; i++)
	{
		result[count - i - 1] -= temp[count - i] * result[count - i];
	}

	return result;
}

/**
	 * @brief  计算绘制贝塞尔曲线需要的控制点
	 * @param  points [in] 序列映射坐标系上的绝对坐标点
	 * @return QVector<QPointF> [out] 控制点
	 * @note
	 */
	QVector<QPointF> calculateControlPoints(const QVector<QPointF> &points)
	{
		QVector<QPointF> controlPoints;
		controlPoints.resize(points.count() * 2 - 2);

		int n = points.count() - 1;

		if (n == 1)
		{
			controlPoints[0].setX((2 * points[0].x() + points[1].x()) / 3);
			controlPoints[0].setY((2 * points[0].y() + points[1].y()) / 3);
			controlPoints[1].setX(2 * controlPoints[0].x() - points[0].x());
			controlPoints[1].setY(2 * controlPoints[0].y() - points[0].y());
			return controlPoints;
		}

		QVector<qreal> vector;
		vector.resize(n);

		vector[0] = points[0].x() + 2 * points[1].x();

		for (int i = 1; i < n - 1; i++)
		{
			vector[i] = 4 * points[i].x() + 2 * points[i + 1].x();
		}

		vector[n - 1] = (8 * points[n - 1].x() + points[n].x()) / 2.0;

		QVector<qreal> xControl = firstControlPoints(vector);

		vector[0] = points[0].y() + 2 * points[1].y();

		for (int i = 1; i < n - 1; ++i)
		{
			vector[i] = 4 * points[i].y() + 2 * points[i + 1].y();
		}

		vector[n - 1] = (8 * points[n - 1].y() + points[n].y()) / 2.0;

		QVector<qreal> yControl = firstControlPoints(vector);

		for (int i = 0, j = 0; i < n; ++i, ++j)
		{
			controlPoints[j].setX(xControl[i]);
			controlPoints[j].setY(yControl[i]);

			j++;

			if (i < n - 1)
			{
				controlPoints[j].setX(2 * points[i + 1].x() - xControl[i + 1]);
				controlPoints[j].setY(2 * points[i + 1].y() - yControl[i + 1]);
			}
			else
			{
				controlPoints[j].setX((points[n].x() + xControl[n - 1]) / 2);
				controlPoints[j].setY((points[n].y() + yControl[n - 1]) / 2);
			}
		}

		return controlPoints;
	}


	
	
	/// 具体使用，计算曲线路径
// 		QPainterPath painterPath;
// 		painterPath.moveTo(m_points.at(0));
// 		if (m_points.size() >= 2)
// 		{
// 			m_controlPoints = calculateControlPoints(m_points);
// 		}
// 
// 		for (int i = 0; i < m_points.size() - 1; i++)
// 		{
// 			const QPointF &point = m_points.at(i + 1);
// 			painterPath.cubicTo(m_controlPoints[2 * i], m_controlPoints[2 * i + 1], point);
// 		}
	
	
	