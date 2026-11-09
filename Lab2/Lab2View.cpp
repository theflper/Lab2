
// Lab2View.cpp : implementation of the CLab2View class
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "Lab2.h"
#endif

#include "Lab2Doc.h"
#include "Lab2View.h"

#include <cmath>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CLab2View

IMPLEMENT_DYNCREATE(CLab2View, CView)

BEGIN_MESSAGE_MAP(CLab2View, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_KEYDOWN()
END_MESSAGE_MAP()

// CLab2View construction/destruction

CLab2View::CLab2View() noexcept
{
	// TODO: add construction code here

}

CLab2View::~CLab2View()
{
}

BOOL CLab2View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CLab2View drawing

namespace
{
	DWORD transformMode(bool right)
	{
		if (right)
			return MWT_RIGHTMULTIPLY;

		return MWT_LEFTMULTIPLY;
	}
	CString putanjaDoFajla(LPCTSTR imeFajla)
	{
		TCHAR exePutanja[MAX_PATH] = {};
		DWORD duzina = GetModuleFileName(nullptr, exePutanja, MAX_PATH);
		if (duzina == 0)
			return CString();

		CString putanja(exePutanja, static_cast<int>(duzina));
		int poslednjaKosaCrta = putanja.ReverseFind(_T('\\'));
		if (poslednjaKosaCrta >= 0)
			putanja = putanja.Left(poslednjaKosaCrta + 1);

		return putanja + imeFajla;
	}

	void crtaj_kaktus(CDC* pDC, LPCTSTR imeFajla)
	{
		CString putanja = putanjaDoFajla(imeFajla);
		HENHMETAFILE hEmf = GetEnhMetaFile(putanja);
		if (hEmf == NULL)
			return;

		ENHMETAHEADER header = {};
		GetEnhMetaFileHeader(hEmf, sizeof(header), &header);
		CRect sourceRect(header.rclBounds.left, header.rclBounds.top,
			header.rclBounds.right, header.rclBounds.bottom);
		PlayEnhMetaFile(pDC->m_hDC, hEmf, &sourceRect);
		DeleteEnhMetaFile(hEmf);
	}

	BOOL primeniTransformaciju(CDC* pDC, const XFORM& transform, DWORD mode)
	{
		XFORM oldTransform;
		if (!pDC->GetWorldTransform(&oldTransform))
			return FALSE;

		XFORM result;
		if (mode == MWT_LEFTMULTIPLY)
			CombineTransform(&result, &transform, &oldTransform);
		else
			CombineTransform(&result, &oldTransform, &transform);

		return pDC->SetWorldTransform(&result);
	}
}
void CLab2View::crtaj_krug(CDC* pDC, int x, int y, int r)
{
	CPen olovka(PS_SOLID, 1, RGB(0, 0, 0));
	CBrush cetka(RGB(0, 204, 0));
	CPen* staraOlovka = pDC->SelectObject(&olovka);
	CBrush* staraCetka = pDC->SelectObject(&cetka);
	pDC->Ellipse(x - r, y - r, x + r, y + r);
	pDC->SelectObject(staraOlovka);
	pDC->SelectObject(staraCetka);
	olovka.DeleteObject();
	cetka.DeleteObject();
}
void CLab2View::crtaj_svetli_kaktus(CDC* pDC)
{
	crtaj_kaktus(pDC, _T("cactus_part_light.emf"));
}

void CLab2View::crtaj_tamni_kaktus(CDC* pDC)
{
	crtaj_kaktus(pDC, _T("cactus_part.emf"));
}
void CLab2View::crtaj_rotiran_tamni_kaktus(CDC* pDC,double angle) {
	PushTransformacija(pDC);
	Rotate(pDC, angle, false);
	Translate(pDC, -10.0f, -74.0f, false);
	Scale(pDC, 0.08f, 0.33f, false);
	crtaj_tamni_kaktus(pDC);
	PopTransformacija(pDC);
}
void CLab2View::crtaj_rotiran_deblji_tamni_kaktus(CDC* pDC, double angle) {
	PushTransformacija(pDC);
	Rotate(pDC, angle, false);
	Translate(pDC, -17.0f, -75.0f, false);
	Scale(pDC, 0.15f, 0.33f, false);
	crtaj_tamni_kaktus(pDC);
	PopTransformacija(pDC);
}
void CLab2View::crtaj_saksiju(CDC* pDC)
{
	CPen olovka(PS_SOLID, 1, RGB(0, 0, 0));
	CBrush cetka(RGB(222, 148, 0));
	CPen* staraOlovka = pDC->SelectObject(&olovka);
	CBrush* staraCetka = pDC->SelectObject(&cetka);
	pDC->Rectangle(0, 0, 120, 20);
	CPoint tacke[4];
	tacke[0] = CPoint(10, 20);
	tacke[1] = CPoint(20, 70);
	tacke[2] = CPoint(100, 70);
	tacke[3] = CPoint(110, 20);
	pDC->Polygon(tacke, 4);
	pDC->SelectObject(staraOlovka);
	pDC->SelectObject(staraCetka);
	olovka.DeleteObject();
	cetka.DeleteObject();
}
BOOL CLab2View::Scale(CDC* pDC, float scaleX, float scaleY, bool right)
{
	if (pDC == nullptr)
		return FALSE;
	DWORD mode = transformMode(right);

	XFORM transform = { scaleX, 0.0f, 0.0f, scaleY, 0.0f, 0.0f };
	pDC->SetGraphicsMode(GM_ADVANCED);
	return primeniTransformaciju(pDC, transform, mode);
}

BOOL CLab2View::Rotate(CDC* pDC, float angle, bool right)
{
	if (pDC == nullptr)
		return FALSE;
	DWORD mode = transformMode(right);

	const float radians = angle * 3.14159265358979323846f / 180.0f;
	const float cosine = std::cos(radians);
	const float sine = std::sin(radians);
	XFORM transform = { cosine, sine, -sine, cosine, 0.0f, 0.0f };
	pDC->SetGraphicsMode(GM_ADVANCED);
	return primeniTransformaciju(pDC, transform, mode);
}

BOOL CLab2View::Translate(CDC* pDC, float offsetX, float offsetY, bool right)
{
	if (pDC == nullptr)
		return FALSE;
	DWORD mode = transformMode(right);

	XFORM transform = { 1.0f, 0.0f, 0.0f, 1.0f, offsetX, offsetY };
	pDC->SetGraphicsMode(GM_ADVANCED);
	return primeniTransformaciju(pDC, transform, mode);
}

void CLab2View::PushTransformacija(CDC* pDC)
{
	XFORM currentTransform;
	if (pDC->GetWorldTransform(&currentTransform))
		transformStack.push_back(currentTransform);
}

void CLab2View::PopTransformacija(CDC* pDC)
{
	if (transformStack.empty())
		return;
	//uzmi sa vrha steka kao prethodno stanje transformacija
	pDC->SetWorldTransform(&transformStack.back());
	//skini element sa vrha steka
	transformStack.pop_back();
}

void CLab2View::DrawFigure(CDC* pDC)
{
	transformStack.clear();
	int previousMode = pDC->SetGraphicsMode(GM_ADVANCED);
	//pocetak kaktusa
	PushTransformacija(pDC);
	Translate(pDC, 250.0f, 425.0f, false);

	//centar rotacije1
	PushTransformacija(pDC);
	Rotate(pDC, (float)ugao2, false);
	//crtanje prvog dela kaktusa
	PushTransformacija(pDC);
	PushTransformacija(pDC);
	Scale(pDC, 0.25f, 0.31f, false);
	Translate(pDC, -107.0f, -227.0f, false);
	crtaj_svetli_kaktus(pDC);
	//Vrati se na pocetno stanje kaktusa
	PopTransformacija(pDC);
	crtaj_krug(pDC, 0, 0, 10);
	//sledeci krug centar
	Translate(pDC, 0.0f, -75.0f, false);
	//sledeci deo
	crtaj_rotiran_tamni_kaktus(pDC, 0.0f);
	crtaj_rotiran_tamni_kaktus(pDC,45.0f);
	crtaj_rotiran_tamni_kaktus(pDC,-45.0f);
	crtaj_krug(pDC, 0, 0, 10);
	//dalja translacija na levo i gore
	Translate(pDC, -55.0f, -55.0f, false);
	crtaj_rotiran_deblji_tamni_kaktus(pDC, 0.0f);
	crtaj_rotiran_deblji_tamni_kaktus(pDC, -90.0f);
	crtaj_krug(pDC, 0, 0, 10);
	//gornji kaktus
	Translate(pDC, 0.0f, -75.0f, false);
	//crtanje najdebljeg kaktusa
	PushTransformacija(pDC);
	Translate(pDC, -24.0f, -66.0f, false);
	Scale(pDC, 0.23f, 0.28f, false);
	crtaj_tamni_kaktus(pDC);
	PopTransformacija(pDC);
	crtaj_krug(pDC, 0, 0, 10);
	//druga strana
	Translate(pDC, 110.0f, 75.0f, false);//100 je izmedju ali je jedan 5 levo drugi 5 desno to je jos 10
	crtaj_rotiran_deblji_tamni_kaktus(pDC, 90.0);
	//zadnje grane i krug
	Translate(pDC, 75.0f, 0.0f, false);
	crtaj_rotiran_deblji_tamni_kaktus(pDC, 45.0f);
	crtaj_rotiran_deblji_tamni_kaktus(pDC, 135.0f);
	crtaj_krug(pDC, 0, 0, 10);
	PopTransformacija(pDC);//vracanje na pocetak
	crtaj_krug(pDC, 0, 0, 10);
	//nova translacija
	Translate(pDC, 55.0f, -130.0f, false);
	// Rotacija cele grupe oko tacke (305, 295)
	Rotate(pDC, (float)ugao1, false);
	// Konkretna grana
	PushTransformacija(pDC);
	Translate(pDC, 15.0f, -5.0f, false);
	Scale(pDC, 0.15f, 0.3f, false);
	Rotate(pDC, -180.0f, false);
	crtaj_svetli_kaktus(pDC);
	// Povratak sa transformacije centra
	PopTransformacija(pDC);
	crtaj_krug(pDC, 0, 0, 10);
	// Povratak na pocetno stanje kaktusa
	PopTransformacija(pDC);
	//povratak na koordinatni pocetak
	PopTransformacija(pDC);


	PushTransformacija(pDC);
	Translate(pDC, 190.0f, 430.0f, false);
	crtaj_saksiju(pDC);
	PopTransformacija(pDC);

	pDC->SetGraphicsMode(previousMode);
}

void CLab2View::DrawGrid(CDC* pDC)
{
	if (grid)
	{
		CPen gridPen(PS_SOLID, 1, RGB(255, 255, 255));
		CPen* pOldPen = pDC->SelectObject(&gridPen);
		// Draw grid lines
		for (int i = 0; i <= 500; i += 25)
		{
			pDC->MoveTo(i, 0);
			pDC->LineTo(i, 500);
			pDC->MoveTo(0, i);
			pDC->LineTo(500, i);
		}
		pDC->SelectObject(pOldPen);
		gridPen.DeleteObject();
	}
}

void CLab2View::OnDraw(CDC* pDC)
{
	CLab2Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	pDC->FillSolidRect(CRect(0, 0, 500, 500), RGB(135, 206, 235));
	DrawFigure(pDC);
	DrawGrid(pDC);
}


// CLab2View printing

BOOL CLab2View::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CLab2View::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CLab2View::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}


// CLab2View diagnostics

#ifdef _DEBUG
void CLab2View::AssertValid() const
{
	CView::AssertValid();
}

void CLab2View::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CLab2Doc* CLab2View::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CLab2Doc)));
	return (CLab2Doc*)m_pDocument;
}
#endif //_DEBUG


// CLab2View message handlers

void CLab2View::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: Add your message handler code here and/or call default
	if(nChar=='g' || nChar=='G')
	{
		grid = !grid;
		Invalidate();
	}
	if (nChar == 'a' || nChar == 'A')
	{
		ugao1 -= 10.00f;
		Invalidate();
	}
	if (nChar == 'd' || nChar == 'D')
	{
		ugao1 += 10.00f;
		Invalidate();
	}
	if (nChar == 's' || nChar == 'S')
	{
		ugao2 += 10.00f;
		Invalidate();
	}
	if (nChar == 'w' || nChar == 'W')
	{
		ugao2 -= 10.00f;
		Invalidate();
	}
	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}
