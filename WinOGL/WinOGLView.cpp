
// WinOGLView.cpp : CWinOGLView クラスの実装
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS は、プレビュー、縮小版、および検索フィルター ハンドラーを実装している ATL プロジェクトで定義でき、
// そのプロジェクトとのドキュメント コードの共有を可能にします。
#ifndef SHARED_HANDLERS
#include "WinOGL.h"
#endif

#include "WinOGLDoc.h"
#include "WinOGLView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CWinOGLView

IMPLEMENT_DYNCREATE(CWinOGLView, CView)

BEGIN_MESSAGE_MAP(CWinOGLView, CView)
//	ON_WM_LBUTTONDBLCLK()
ON_WM_LBUTTONDOWN()
ON_WM_CREATE()
ON_WM_DESTROY()
ON_WM_ERASEBKGND()
ON_WM_SIZE()
END_MESSAGE_MAP()

// CWinOGLView コンストラクション/デストラクション

CWinOGLView::CWinOGLView() noexcept
{
	// TODO: 構築コードをここに追加します。

}

CWinOGLView::~CWinOGLView()
{
}

BOOL CWinOGLView::PreCreateWindow(CREATESTRUCT& cs)
{


	return CView::PreCreateWindow(cs);
}

// CWinOGLView 描画

void CWinOGLView::OnDraw(CDC* pDC) {
	CWinOGLDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	wglMakeCurrent(pDC->m_hDC, m_hRC);
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT /* | GL_DEPTH_BUFFER_BIT*/);

	AC.Draw();

	glFlush();
	SwapBuffers(pDC->m_hDC);
	wglMakeCurrent(pDC->m_hDC, NULL);
}


// CWinOGLView の診断

#ifdef _DEBUG
void CWinOGLView::AssertValid() const
{
	CView::AssertValid();
}

void CWinOGLView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CWinOGLDoc* CWinOGLView::GetDocument() const // デバッグ以外のバージョンはインラインです。
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CWinOGLDoc)));
	return (CWinOGLDoc*)m_pDocument;
}
#endif //_DEBUG


// CWinOGLView メッセージ ハンドラー

//void CWinOGLView::OnLButtonDblClk(UINT nFlags, CPoint point) {
	// TODO: ここにメッセージ ハンドラー コードを追加するか、既定の処理を呼び出します。

//	CView::OnLButtonDblClk(nFlags, point);
//}

void CWinOGLView::OnLButtonDown(UINT nFlags, CPoint point) {
	// 描画領域の大きさを取得
	CRect rect;
	GetClientRect(rect);
	
	viewingTransformation(point.x, point.y, rect);

	AC.AddVertex(x_Ldown, y_Ldown);

	RedrawWindow();
	CView::OnLButtonDown(nFlags, point);
}

int CWinOGLView::OnCreate(LPCREATESTRUCT lpCreateStruct) {
	if(CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	PIXELFORMATDESCRIPTOR pfd =
	{
	sizeof(PIXELFORMATDESCRIPTOR),
	1,
	PFD_DRAW_TO_WINDOW |
	PFD_SUPPORT_OPENGL |
	PFD_DOUBLEBUFFER,
	PFD_TYPE_RGBA,
	32,
	0,0,0,0,0,0,
	0,0,0,0,0,0,0,
	24,
	0,0,
	PFD_MAIN_PLANE,
	0,
	0,0,0
	};
	CClientDC clientDC(this);
	int pixelFormat = ChoosePixelFormat(clientDC.m_hDC,
										&pfd);
	SetPixelFormat(clientDC.m_hDC, pixelFormat, &pfd);
	m_hRC = wglCreateContext(clientDC.m_hDC);

	return 0;
}

void CWinOGLView::OnDestroy() {
	CView::OnDestroy();
	wglDeleteContext(m_hRC);
}

BOOL CWinOGLView::OnEraseBkgnd(CDC* pDC) {
	return true;
}
void CWinOGLView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);

	CClientDC clientDC(this);
	wglMakeCurrent(clientDC.m_hDC, m_hRC);
	glViewport(0, 0, cx, cy);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	float aspect;
	if (cx > cy) {
		aspect = static_cast<double>(cx) / static_cast<double>(cy);
		glOrtho(-aspect, aspect, -1.0, 1.0, -100.0, 100.0);
	}
	else {
		aspect = static_cast<double>(cy) / static_cast<double>(cx);
		glOrtho(-1.0, 1.0, -aspect, aspect, -100.0, 100.0);
	}


	glMatrixMode(GL_MODELVIEW);
	RedrawWindow();
	wglMakeCurrent(clientDC.m_hDC, NULL);
}

void CWinOGLView::viewingTransformation(float x, float y, CRect rect) {
	// デバイス座標系->正規化座標系
	x = x / rect.Width();
	y = y / rect.Height();

	// 正規化座標系->ワールド座標系
	x *= 2;
	y *= 2;
	x -= 1.0;
	y -= 1.0;
	y *= -1.0;

	x_Ldown = x;
	y_Ldown = y;

	if (rect.Width() > rect.Height()) {
		x_Ldown *= static_cast<float>(rect.Width()) / static_cast<float>(rect.Height());
	}
	else {
		y_Ldown *= static_cast<float>(rect.Height()) / static_cast<float>(rect.Width());
	}
}

