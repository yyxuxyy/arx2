#include "StdAfx.h"
#include "math.h"

#include "acedCmdNF.h"


class CEntCal
{
public:

	static double PI()
	{
		return 4 * atan(1.0);
	}
	static double Max(double a, double b)
	{
		return (a > b) ? a : b;
	}
	static double Min(double a, double b)
	{
		return (a < b) ? a : b;
	}
	static double DToR(double angle)
	{
		return angle * PI() / 180;
	}
	static double RToD(double angle)
	{
		return angle * 180 / PI();
	}
	static AcGePoint3d Pt2dTo3d(AcGePoint2d pt)
	{
		AcGePoint3d ptTemp(pt.x, pt.y, 0);
		return ptTemp;
	}
	static AcGePoint2d Pt3dTo2d(AcGePoint3d pt)
	{
		AcGePoint2d ptTemp(pt.x, pt.y);
		return ptTemp;
	}

	static AcGePoint3d MiddlePoint(AcGePoint3d pt1, AcGePoint3d pt2)
	{
		AcGePoint3d pt;
		pt[X] = (pt1[X] + pt1[X]) / 2;
		pt[Y] = (pt1[Y] + pt1[Y]) / 2;
		pt[Z] = (pt1[Z] + pt1[Z]) / 2;
		return pt;
	}
	static AcGePoint2d MiddlePoint(AcGePoint2d pt1, AcGePoint2d pt2)
	{
		AcGePoint2d pt;
		pt[X] = (pt1[X] + pt1[X]) / 2;
		pt[Y] = (pt1[Y] + pt1[Y]) / 2;

		return pt;
	}
	static AcGePoint3d PolarPoint(const AcGePoint3d &basePoint, double angle, double distance)
	{
		ads_point ptFrom, ptTo;
		ptFrom[X] = basePoint.x;
		ptFrom[Y] = basePoint.y;
		ptFrom[Z] = basePoint.z;
		acutPolar(ptFrom, angle, distance, ptTo);
		return asPnt3d(ptTo);
	}
	static AcGePoint3d PXYPoint(const AcGePoint3d &basePoint, double xx, double yy, double zz)
	{
		ads_point ptTo;
		ptTo[X] = basePoint.x + xx;
		ptTo[Y] = basePoint.y + yy;
		ptTo[Z] = basePoint.z + zz;
		return asPnt3d(ptTo);
	}
	static AcGePoint2d PXYPoint(const AcGePoint2d &basePoint, double xx, double yy)
	{
		ads_point pt;
		pt[X] = basePoint.x + xx;
		pt[Y] = basePoint.y + yy;
		return asPnt2d(pt);
	}
	static AcGePoint3d RelativePoint(const AcGePoint3d &basePoint, double x, double y)
	{
		AcGePoint3d pt(basePoint.x + x, basePoint.y + y, basePoint.z);

		return pt;
	}
	static void AcGePoint3dToadsPoint(AcGePoint3d pt1, ads_point &pt2)
	{
		pt2[X] = pt1.x;
		pt2[Y] = pt1.y;
		pt2[Z] = pt1.z;
	}
	static void AcGePoint2dToadsPoint(AcGePoint2d pt1, ads_point &pt2)
	{
		pt2[X] = pt1.x;
		pt2[Y] = pt1.y;
	}

	static Acad::ErrorStatus getPointAtDistInGeCurve(const AcGeCurve3d *pGeCurve, double dist, AcGePoint3d &pt)
	{
		Acad::ErrorStatus es = Acad::eOk;

		if (pGeCurve != NULL)
		{
			AcGePoint3d spt;
			double pa = 0., dp = 0.;
			Adesk::Boolean posParamDir = Adesk::kTrue;

			pGeCurve->hasStartPoint(spt);
			dp = pGeCurve->paramOf(spt);
			pa = pGeCurve->paramAtLength(dp, dist, posParamDir);
			pt = pGeCurve->evalPoint(pa);
		}
		else
			es = Acad::eInvalidInput;

		return es;
	}
	static Acad::ErrorStatus getPointAtDistInGeCurve(const AcGeCurve2d *pGeCurve, double dist, AcGePoint2d &pt)
	{

		Acad::ErrorStatus es;

		if (pGeCurve != NULL)
		{
			AcGePoint2d spt;
			double pa = 0., dp = 0.;
			Adesk::Boolean posParamDir = Adesk::kTrue;
			pGeCurve->hasStartPoint(spt);
			dp = pGeCurve->paramOf(spt);
			pa = pGeCurve->paramAtLength(dp, dist, posParamDir);
			pt = pGeCurve->evalPoint(pa);
		}
		else
			es = Acad::eInvalidInput;

		return es;
	}

	static void searchBlockByName(const ACHAR name, ads_name &ss)
	{
		struct resbuf *rb;
		rb = acutBuildList(-4, _T("<AND"), RTDXF0, _T("INSERT"), 2, name, -4, _T("AND>"), 0);

		acedSSGet(_T("X"), NULL, NULL, rb, ss);
	}
};
class CEntModify
{

public:

	static Acad::ErrorStatus ChangeLayer(AcDbObjectId id, AcString strLayer)
	{
		AcDbEntity *pEnt;

		acdbOpenObject(pEnt, id, AcDb::kForWrite);
		pEnt->setLayer(strLayer);
		pEnt->close();
		return Acad::eOk;
	}

	static Acad::ErrorStatus ChangeLinetype(AcDbObjectId id, AcString strLinetype)
	{
		AcDbEntity *pEnt;

		acdbOpenObject(pEnt, id, AcDb::kForWrite);
		pEnt->setLinetype(strLinetype);
		pEnt->close();
		return Acad::eOk;
	}
	static Acad::ErrorStatus ChangeColor(AcDbObjectId id, Adesk::UInt16 colorIndex)
	{
		AcDbEntity *pEnt;

		acdbOpenObject(pEnt, id, AcDb::kForWrite);
		pEnt->setColorIndex(colorIndex);
		pEnt->close();
		return Acad::eOk;
	}
	static Acad::ErrorStatus Rotate(AcDbObjectId entId, AcGePoint2d ptBase, double rotation)
	{
		AcGeMatrix3d xform;
		AcGeVector3d vec(0, 0, 1);
		xform.setToRotation(rotation, vec, CEntCal::Pt2dTo3d(ptBase));

		AcDbEntity *pEnt;
		Acad::ErrorStatus es = acdbOpenObject(pEnt, entId, AcDb::kForWrite);
		pEnt->transformBy(xform);
		pEnt->close();

		return es;
	}

	static Acad::ErrorStatus Move(AcDbObjectId entId, AcGePoint3d ptBase, AcGePoint3d ptDest)
	{
		AcGeMatrix3d xform;
		AcGeVector3d vec(ptDest.x - ptBase.x, ptDest.y - ptBase.y, ptDest.z - ptBase.z);
		xform.setToTranslation(vec);

		AcDbEntity *pEnt;
		Acad::ErrorStatus es = acdbOpenObject(pEnt, entId, AcDb::kForWrite);
		pEnt->transformBy(xform);
		pEnt->close();
		return es;
	}
	static Acad::ErrorStatus Scale(AcDbObjectId entId, AcGePoint3d ptBase, double scaleFactor)
	{
		AcGeMatrix3d xform;
		xform.setToScaling(scaleFactor, ptBase);

		AcDbEntity *pEnt;
		Acad::ErrorStatus es = acdbOpenObject(pEnt, entId, AcDb::kForWrite);
		pEnt->transformBy(xform);
		pEnt->close();

		return es;
	}
};
class CEntMake
{
public:
	static AcDbObjectId AddLine(AcGePoint3d ptStart, AcGePoint3d ptEnd)
	{
		AcDbLine* pLine = new AcDbLine(ptStart, ptEnd);

		AcDbObjectId lineId;
		return AddToModelSpace(pLine);

	}
	static AcDbObjectId AddCircle(AcGePoint3d pt, AcGeVector3d vec, double radius)
	{
		AcDbCircle* pCircle = new AcDbCircle(pt, vec, radius);
		AcDbObjectId circleId;
		return AddToModelSpace(pCircle);
	}
	static AcDbObjectId AddCircle(AcGePoint3d pt, double radius)
	{
		AcDbObjectId circleId;
		AcGeVector3d vec(0, 0, 1);
		return AddCircle(pt, vec, radius);

	}
	static AcDbObjectId AddCircle(AcGePoint3d pt1, AcGePoint3d pt2)
	{
		AcGePoint3d pt;
		pt = CEntCal::MiddlePoint(pt1, pt2);
		double radius = pt1.distanceTo(pt2) / 2;
		return AddCircle(pt, radius);
	}
	static AcDbObjectId AddCircle(AcGePoint2d pt1, AcGePoint2d pt2, AcGePoint2d pt3)
	{
		AcGePoint2d ptCenter;
		double radius;
		AcGeCircArc2d geArc(pt1, pt2, pt3);
		ptCenter = geArc.center();
		radius = ptCenter.distanceTo(pt1);
		return AddCircle(CEntCal::Pt2dTo3d(ptCenter), radius);
	}

	static AcDbObjectId AddArc(AcGePoint3d pt, AcGeVector3d vec, double radius, double startAng, double endAng)
	{
		AcDbArc* pArc = new AcDbArc(pt, vec, radius, startAng, endAng);
		AcDbObjectId arcId;
		return AddToModelSpace(pArc);

	}
	static AcDbObjectId AddArc(AcGePoint2d pt, double radius, double startAng, double endAng)
	{
		AcGePoint3d ptCenter = CEntCal::Pt2dTo3d(pt);
		AcGeVector3d vec(0, 0, 1);
		return AddArc(ptCenter, vec, radius, startAng, endAng);
	}
	static AcDbObjectId AddArc(AcGePoint2d pt1, AcGePoint2d pt2, AcGePoint2d pt3)
	{
		AcGeCircArc2d  geArc(pt1, pt2, pt3);
		AcGePoint2d ptCenter = geArc.center();
		double radius = geArc.radius();
		AcGeVector2d startVec = AcGeVector2d(pt1.x - ptCenter.x, pt1.y - ptCenter.y);
		AcGeVector2d endVec = AcGeVector2d(pt2.x - ptCenter.x, pt2.y - ptCenter.y);
		double startAng = startVec.angle();
		double endAng = endVec.angle();
		return AddArc(ptCenter, radius, startAng, endAng);

	}

	static AcDbObjectId AddPolyline(AcGePoint2dArray points, double width = 0)
	{
		int numVertices = points.length();
		AcDbPolyline* pPolyline = new AcDbPolyline(numVertices);

		for (int i = 0; i < numVertices; i++)
		{
			pPolyline->addVertexAt(i, points.at(i), 0, width, width);
		}
		return AddToModelSpace(pPolyline);
	}

	static AcDbObjectId AddPolyline(AcGePoint2d ptStart, AcGePoint2d ptEnd, double width)
	{
		AcGePoint2dArray points;
		points.append(ptStart);
		points.append(ptEnd);
		return AddPolyline(points, width);
	}

	static AcDbObjectId Add3dPolyline(AcGePoint3dArray points)
	{
		AcDb3dPolyline* pPoly3d = new AcDb3dPolyline(AcDb::k3dSimplePoly, points);
		return AddToModelSpace(pPoly3d);
	}

	static AcDbObjectId AddPolygon(AcGePoint2d ptCenter, int number, double radius, double rotation, double width)
	{
		AcGePoint2dArray points;
		double angle = 2 * CEntCal::PI() / (double)number;
		for (int i = 0; i < number; i++)
		{
			AcGePoint2d pt;
			pt.x = ptCenter.x + radius * cos(i*angle);
			pt.y = ptCenter.y + radius * sin(i*angle);

			points.append(pt);
		}

		AcDbObjectId polyId = AddPolyline(points, width);

		AcDbEntity* pEnt;
		acdbOpenObject(pEnt, polyId, AcDb::kForWrite);
		AcDbPolyline* pPoly = AcDbPolyline::cast(pEnt);
		if (pPoly != NULL)
		{
			pPoly->setClosed(Adesk::kTrue);
		}
		pEnt->close();

		CEntModify::Rotate(polyId, ptCenter, rotation);
		return polyId;


	}

	static AcDbObjectId AddRectangle(AcGePoint2d pt1, AcGePoint2d pt2, double width)
	{
		double x1 = pt1.x, x2 = pt2.x;
		double y1 = pt1.y, y2 = pt2.y;

		AcGePoint2d ptLeftBottom(CEntCal::Min(x1, x2), CEntCal::Min(y1, y2));
		AcGePoint2d ptRightBottom(CEntCal::Max(x1, x2), CEntCal::Min(y1, y2));
		AcGePoint2d ptRightTop(CEntCal::Max(x1, x2), CEntCal::Max(y1, y2));
		AcGePoint2d ptLeftTop(CEntCal::Min(x1, x2), CEntCal::Max(y1, y2));

		/*AcGePoint2dArray points=NULL;
		points.append(ptLeftBottom);
		points.append(ptRightBottom);
		points.append(ptRightTop);
		points.append(ptLeftTop);

		AcDbObjectId rectId = CEntMake::AddPolyline(
			points, width);
		AcDbEntity* pEnt;
		acdbOpenObject(pEnt, rectId, AcDb::kForWrite);

		if (pEnt->isKindOf(AcDbPolyline::desc())==Adesk::kTrue)
		{
			AcDbPolyline *pPoly = AcDbPolyline::cast(pEnt);
			if (pPoly!=NULL)
			{
				pPoly->setClosed(Adesk::kTrue);
			}
		}
		pEnt->close();
		return rectId;*/


		AcDbPolyline* pPoly = new AcDbPolyline(4);
		pPoly->addVertexAt(0, ptLeftBottom, 0, width, width);
		pPoly->addVertexAt(1, ptRightBottom, 0, width, width);
		pPoly->addVertexAt(2, ptRightTop, 0, width, width);
		pPoly->addVertexAt(3, ptLeftTop, 0, width, width);
		pPoly->setClosed(Adesk::kTrue);

		return AddToModelSpace(pPoly);

	}

	static AcDbObjectId AddPolyCircle(AcGePoint2d ptCenter, double radius, double width)
	{
		AcGePoint2d pt1, pt2, pt3;
		pt1.x = ptCenter.x + radius;
		pt1.y = ptCenter.y;
		pt2.x = ptCenter.x - radius;
		pt2.y = ptCenter.y;
		pt3.x = ptCenter.x = radius;
		pt3.y = ptCenter.y;

		AcDbPolyline* pPoly = new AcDbPolyline(3);
		pPoly->addVertexAt(0, pt1, 1, width, width);
		pPoly->addVertexAt(1, pt2, 1, width, width);
		pPoly->addVertexAt(2, pt3, 1, width, width);
		pPoly->setClosed(Adesk::kTrue);

		return AddToModelSpace(pPoly);
	}

	static AcDbObjectId AddPolyArc(AcGePoint2d ptCenter, double radius, double startAng, double endAng, double width)
	{
		AcGePoint2d pt1, pt2;
		pt1.x = ptCenter.x + radius * cos(startAng);
		pt1.y = ptCenter.y + radius * sin(startAng);
		pt2.x = ptCenter.x + radius * cos(endAng);
		pt2.y = ptCenter.y + radius * sin(endAng);

		AcDbPolyline* pPoly = new AcDbPolyline(2);
		pPoly->addVertexAt(0, pt1, tan((endAng - startAng) / 4), width, width);
		pPoly->addVertexAt(1, pt2, 0, width, width);

		return AddToModelSpace(pPoly);
	}

	static AcDbObjectId AddEllipse(AcGePoint3d ptCenter, AcGeVector3d vecNormal, AcGeVector3d majorAxis, double ratio)
	{
		AcDbEllipse* pEllipse = new AcDbEllipse(ptCenter, vecNormal, majorAxis, ratio);
		return AddToModelSpace(pEllipse);
	}
	static AcDbObjectId AddEllipse(AcGePoint2d pt1, AcGePoint2d pt2)
	{
		AcGePoint3d ptCenter;
		ptCenter = CEntCal::MiddlePoint(CEntCal::Pt2dTo3d(pt1),
			CEntCal::Pt2dTo3d(pt2));
		AcGeVector3d vec(0, 0, 1);
		AcGeVector3d majorAxis(fabs(pt1.x - pt2.x) / 2, 0, 0);
		double ratio = fabs((pt1.y - pt2.y) / (pt1.x - pt2.x));


		return AddEllipse(ptCenter, vec, majorAxis, ratio);

	}

	static AcDbObjectId AddSpline(AcGePoint3dArray& points, int order = 4, double fitTolerance = 0.0)
	{
		assert(order >= 2 && order <= 26);

		AcDbSpline* pSpline = new AcDbSpline(points, order, fitTolerance);

		return AddToModelSpace(pSpline);
	}

	static AcDbObjectId AddSpline(const AcGePoint3dArray& points, const AcGeVector3d& startTangent, const AcGeVector3d& endTangent, int order, double fitTolerance)
	{
		assert(order >= 2 && order <= 26);
		AcDbSpline* pSpline = new AcDbSpline(points, startTangent, endTangent, order, fitTolerance);
		return AddToModelSpace(pSpline);
	}

	static AcDbObjectIdArray AddRegion(const AcDbObjectIdArray& curveIds)
	{
		AcDbObjectIdArray regionIds;
		AcDbVoidPtrArray curves;

		AcDbVoidPtrArray regions;
		AcDbEntity* pEnt;
		AcDbRegion* pRegion;

		for (int i = 0; i < curveIds.length(); i++)
		{
			acdbOpenAcDbEntity(pEnt, curveIds.at(i), AcDb::kForRead);
			if (pEnt->isKindOf(AcDbCurve::desc()))
			{
				curves.append(static_cast<void*>(pEnt));
			}
		}

		Acad::ErrorStatus es = AcDbRegion::createFromCurves(curves, regions);

		if (es == Acad::eOk)
		{
			for (int i = 0; i < regions.length(); i++)
			{
				pRegion = static_cast<AcDbRegion*>(regions[i]);
				pRegion->setDatabaseDefaults();
				AcDbObjectId regionId;
				regionId = CEntMake::AddToModelSpace(pRegion);
				pRegion->close();
				regionIds.append(regionId);
			}
		}
		else
		{
			for (int i = 0; i < regions.length(); i++)
			{
				delete (AcRxObject*)regions[i];
			}
		}
		for (int i = 0; i < curves.length(); i++)
		{
			pEnt = static_cast<AcDbEntity*>(curves[i]);
			pEnt->close();
		}

		return regionIds;

	}

	static AcDbObjectId AddText(const AcGePoint3d& ptInsert, const ACHAR* text, AcDbObjectId style = AcDbObjectId::kNull, double height = 2.5, double rotation = 0)
	{
		AcDbText* pText = new AcDbText(ptInsert, text, style, height, rotation);
		return AddToModelSpace(pText);
	}

	static AcDbObjectId AddMText(const AcGePoint3d& ptInsert, const ACHAR* text, AcDbObjectId style = AcDbObjectId::kNull, double height = 2.5, double width = 10)
	{
		AcDbMText* pMText = new AcDbMText();

		pMText->setTextStyle(style);
		pMText->setContents(text);
		pMText->setLocation(ptInsert);
		pMText->setTextHeight(height);
		pMText->setWidth(width);
		pMText->setAttachment(AcDbMText::kBottomLeft);

		return AddToModelSpace(pMText);
	}

	static AcDbObjectId AddHatch(AcDbObjectIdArray objIds, const ACHAR* patName, bool bAssociative)
	{
		Acad::ErrorStatus es;
		AcDbHatch* pHatch = new AcDbHatch;
		AcGeVector3d vec(0, 0, 1);
		pHatch->setNormal(vec);
		pHatch->setElevation(0);
		pHatch->setAssociative(bAssociative);
		pHatch->setPattern(AcDbHatch::kPreDefined, patName);
		es = pHatch->appendLoop(AcDbHatch::kExternal, objIds);
		es = pHatch->evaluateHatch();

		AcDbObjectId hatchId;
		hatchId = CEntMake::AddToModelSpace(pHatch);

		if (bAssociative)
		{
			AcDbEntity* pEnt;
			for (int i = 0; i < objIds.length(); i++)
			{
				es = acdbOpenObject(pEnt, objIds[i], AcDb::kForWrite);
				if (es == Acad::eOk)
				{
					pEnt->addPersistentReactor(hatchId);
					pEnt->close();
				}
			}
		}
		pHatch->close();
		return hatchId;
	}

	static AcDbObjectId AddDimAligned(const AcGePoint3d& pt1, const AcGePoint3d& pt2, const AcGePoint3d& ptLine, const ACHAR* dimText, AcDbObjectId dimStyle)
	{
		AcDbAlignedDimension* pAlignedDim = new AcDbAlignedDimension(pt1,
			pt2, ptLine, dimText, dimStyle);
		return AddToModelSpace(pAlignedDim);
	}
	static AcDbObjectId AddDimAligned(const AcGePoint3d& pt1, const AcGePoint3d& pt2, const AcGePoint3d& ptLine, const ACHAR* dimText, const AcGeVector3d& vecOffset /* = AcGeVector3d::kIdentity */)
	{
		AcDbAlignedDimension* pDim = new AcDbAlignedDimension(pt1,
			pt2, ptLine, dimText, AcDbObjectId::kNull);
		AcDbObjectId dimId;
		dimId = CEntMake::AddToModelSpace(pDim);

		AcDbEntity* pEnt;
		Acad::ErrorStatus es;
		es = acdbOpenObject(pEnt, dimId, AcDb::kForWrite);
		AcDbAlignedDimension* pDim2 = AcDbAlignedDimension::cast(pEnt);
		if (pDim2 != NULL)
		{
			pDim2->setDimtmove(1);
			AcGePoint3d ptText = pDim2->textPosition();
			ptText = ptText + vecOffset;
			pDim2->setTextPosition(ptText);
		}
		pEnt->close();
		return dimId;

	}

	static AcDbObjectId AddDimRotated(const AcGePoint3d& pt1, const AcGePoint3d& pt2, const AcGePoint3d& ptLine, double rotation, const ACHAR* dimText, AcDbObjectId dimStyle)
	{
		AcDbRotatedDimension* pDim = new AcDbRotatedDimension(rotation,
			pt1, pt2, ptLine, dimText, dimStyle);
		return AddToModelSpace(pDim);
	}

	static AcDbObjectId AddDimRadial(const AcGePoint3d& ptCenter, const AcGePoint3d& ptChord, double leaderLength, const ACHAR* dimText = NULL, AcDbObjectId dimStyle = AcDbObjectId::kNull)
	{
		AcDbRadialDimension* pDim = new AcDbRadialDimension(ptCenter,
			ptChord, leaderLength, dimText, dimStyle);
		return AddToModelSpace(pDim);
	}

	static AcDbObjectId AddDimRadial(const AcGePoint3d& ptCenter, double radius, double angle, double leaderLength)
	{
		AcGePoint3d ptchord = CEntCal::PolarPoint(ptCenter, angle, radius);

		return AddDimRadial(ptCenter, ptchord, leaderLength);
	}

	static AcDbObjectId AddDimDiametric(const AcGePoint3d& ptChord1, const AcGePoint3d& ptChord2, double leaderLength, const ACHAR* dimText = NULL, AcDbObjectId dimStyle = AcDbObjectId::kNull)
	{
		AcDbDiametricDimension* pDim = new AcDbDiametricDimension(ptChord1,
			ptChord2, leaderLength, dimText, dimStyle);
		return AddToModelSpace(pDim);
	}

	static AcDbObjectId AddDimDiametric(const AcGePoint3d& ptCenter, double radius, double angle, double leaderLength/* =5 */)
	{
		AcGePoint3d ptChord1, ptChord2;
		ptChord1 = CEntCal::PolarPoint(ptCenter, angle, radius);
		ptChord2 = CEntCal::PolarPoint(ptCenter,
			angle + CEntCal::PI(), radius);

		return AddDimDiametric(ptChord1, ptChord2, leaderLength);
	}

	static AcDbObjectId AddDim2LineAngular(const AcGePoint3d& ptStart1, const AcGePoint3d& ptEnd1, const AcGePoint3d& ptStart2, const AcGePoint3d& ptEnd2, const AcGePoint3d& ptArc, const ACHAR* dimText /* = NULL */, AcDbObjectId dimStyle /* = AcDbObjectId::kNull */)
	{
		AcDb2LineAngularDimension* pDim = new AcDb2LineAngularDimension(ptStart1,
			ptEnd1, ptStart2, ptEnd2, ptArc, dimText, dimStyle);
		return AddToModelSpace(pDim);
	}

	static AcDbObjectId AddDim3PtAngular(const AcGePoint3d ptCenter, const AcGePoint3d& ptEnd1, const AcGePoint3d ptEnd2, const AcGePoint3d& ptArc, const ACHAR* dimText /* = NULL */, AcDbObjectId dimStyle /* = AcDbObjectId::kNull */)
	{
		AcDb3PointAngularDimension* pDim = new AcDb3PointAngularDimension(ptCenter,
			ptEnd1, ptEnd2, ptArc, dimText, dimStyle);
		return AddToModelSpace(pDim);
	}

	static AcDbObjectId AddDimOrdinate(Adesk::Boolean xAxis, const AcGePoint3d& ptStart, const AcGePoint3d& ptEnd, const ACHAR* dimText = NULL, AcDbObjectId dimStyle = AcDbObjectId::kNull)
	{
		AcDbOrdinateDimension* pDim = new AcDbOrdinateDimension(xAxis,
			ptStart, ptEnd, dimText, dimStyle);
		return AddToModelSpace(pDim);
	}
	static AcDbObjectIdArray AddDimOrdinate(const AcGePoint3d& ptDef, const AcGePoint3d& ptTextX, const AcGePoint3d& ptTextY)
	{
		AcDbObjectId dimId;
		AcDbObjectIdArray dimIds;

		dimId = AddDimOrdinate(Adesk::kTrue,
			ptDef, ptTextX);
		dimIds.append(dimId);
		dimId = AddDimOrdinate(Adesk::kFalse,
			ptDef, ptTextY);
		dimIds.append(dimId);

		return dimIds;
	}
	static AcDbObjectIdArray AddDimOrdinate(const AcGePoint3d& ptDef, const AcGeVector3d& vecOffsetX, const AcGeVector3d& vecOffsetY)
	{
		AcGePoint3d ptTextX = ptDef + vecOffsetX;
		AcGePoint3d ptTextY = ptDef + vecOffsetY;

		return AddDimOrdinate(ptDef, ptTextX, ptTextY);
	}


	static AcDbObjectId AddNuts(AcGePoint2d pt, double rad)
	{
		struct resbuf *rb;
		ads_point ptx;
		CEntCal::AcGePoint2dToadsPoint(pt, ptx);
		rb = acutBuildList(RTSTR, _T("donut"), RTREAL, 0.0, RTREAL, rad, RTPOINT, ptx, RTSTR, _T(""), RTNONE);

		acedCmdS(rb);

		if (rb != NULL)
		{
			acutRelRb(rb);
		}
		ads_name ent;
		acdbEntLast(ent);
		AcDbObjectId entId;
		acdbGetObjectId(entId, ent);
		return entId;

	}
	//²éÑ¯blk_str¿éÊÇ·ñ´æÔÚ
	static AcDbObjectId AddBlockReference(AcGePoint2d pt, const ACHAR* blk_str)
	{
		Acad::ErrorStatus es;
		AcDbDatabase *pNewDb = new AcDbDatabase(Adesk::kFalse);
		acDocManager->lockDocument(acDocManager->curDocument());

		es = pNewDb->readDwgFile(blk_str, AcDbDatabase::OpenMode::kForReadAndWriteNoShare, false);

		AcDbDatabase *pDb = acdbHostApplicationServices()->workingDatabase();
		AcDbBlockTable *pBlockTable;
		pDb->getSymbolTable(pBlockTable, AcDb::kForRead);
		AcDbBlockTableRecord *pBlockTableRecord;
		if (pBlockTable->getAt(blk_str,pBlockTableRecord,AcDb::kForRead)==Acad::eOk)
		{
			acutPrintf(_T("block %s already "), blk_str);
			return;
		}
		pDb->insert(AcGeMatrix3d::kIdentity, pNewDb);
		acDocManager->unlockDocument(acDocManager->curDocument());

		ads_name ent;
		acdbEntLast(ent);
		AcDbObjectId entId;
		acdbGetObjectId(entId, ent);
		return entId;


	}

	//
	static AcDbObjectId AddToModelSpace(AcDbEntity* pEnt)
	{
		AcDbBlockTable* pBlockTable;
		acdbHostApplicationServices()->workingDatabase()
			->getSymbolTable(pBlockTable, AcDb::kForRead);

		AcDbBlockTableRecord* pBlockTableRecord;
		pBlockTable->getAt(ACDB_MODEL_SPACE, pBlockTableRecord, AcDb::kForWrite);
		pBlockTable->close();

		AcDbObjectId id;
		pBlockTableRecord->appendAcDbEntity(id, pEnt);

		pBlockTableRecord->close();
		pEnt->close();

		return id;
	}
};
