
// LineDrawingView.cpp: CLineDrawingView 类的实现
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "LineDrawing.h"
#endif

#include "LineDrawingDoc.h"
#include "LineDrawingView.h"

#include < vector> 
using namespace std;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CLineDrawingView

IMPLEMENT_DYNCREATE(CLineDrawingView, CView)

BEGIN_MESSAGE_MAP(CLineDrawingView, CView)
	// 标准打印命令
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CLineDrawingView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
END_MESSAGE_MAP()

// CLineDrawingView 构造/析构

CLineDrawingView::CLineDrawingView() noexcept
{
	// TODO: 在此处添加构造代码
	m_startRect = FALSE;
	m_startPoint = 0;
	m_OldPoint = 0;

}

CLineDrawingView::~CLineDrawingView()
{
}

BOOL CLineDrawingView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CView::PreCreateWindow(cs);
}

void CLineDrawingView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	//Cor.left = point.x;
	//Cor.top = point.y;

	m_startRect = TRUE;
	m_startPoint = point;
	m_OldPoint = point;
	CView::OnLButtonDown(nFlags, point);
}

void CLineDrawingView::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	//CClientDC dc(this);
	//dc.SetROP2(R2_NOT);
	//dc.SelectStockObject(NULL_BRUSH);
	if (m_startRect)
	{
		//dc.Rectangle(CRect(m_startPoint, m_OldPoint));
		//dc.Rectangle(CRect(m_startPoint, point));
		
	}
	m_OldPoint = point;
	CView::OnMouseMove(nFlags, point);
}

void CLineDrawingView::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	//Cor.right = point.x;
	//Cor.bottom = point.y;

	m_startRect = FALSE;
	CClientDC dc(this);
	//dc.SetROP2(R2_NOT);
	//dc.SelectStockObject(NULL_BRUSH);
	//dc.Rectangle(CRect(m_startPoint, m_OldPoint));
	CView::OnLButtonUp(nFlags, point);
}


struct point2f {
	float x;
	float y;
};
struct point2i {
	int x;
	int y;
};
void line_DDA(float x1, float y1, float x2, float y2, vector<point2f>& res);
void line_Bressenham(int x1, int y1, int x2, int y2, vector<point2i>& res);
void CLineDrawingView::OnDraw(CDC* /*pDC*/)
{
	CLineDrawingDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 在此处为本机数据添加绘制代码
	

	//填充矩形区域所用的画刷
	CClientDC dc(this); // device context for painting
	HBRUSH hBrush = CreateSolidBrush(RGB(255, 0, 0)); //创建画刷并设置颜色
	int rect_u = 5;
	int rect_v = 5;
	CBrush brush(RGB(255, 0, 0));//选定画刷
	float x1 = 0.0, y1 = 0.0, x2 = 90.0, y2 = 90.0;
	vector<point2f> res_DDA = vector<point2f>();
	vector<point2i> res_B = vector<point2i>();
	// 使用 line_DDA 生成矩形的坐标
	line_DDA(x1, y1, x2, y2,res_DDA);

	for (unsigned int i = 0; i < res_DDA.size(); i++) {

		printf("矩形中心坐标%d:(%f,%f)", i, res_DDA[i].x, res_DDA[i].y);
		//CRect tmpRect = CRect(, ); //矩形区域
		//dc.FillRect(&CRect(int(res_DDA[i].x), int(res_DDA[i].y), int(res_DDA[i].x)+ rect_u, int(res_DDA[i].y)+ rect_v), &brush);//填充矩形
	}
	hBrush = CreateSolidBrush(RGB(0, 0, 255)); //创建画刷并设置颜色
	CBrush brush1(RGB( 0, 0,255));//选定画刷
	line_Bressenham(int(x1), int(y1), int(x2), int(y2), res_B);
	for (unsigned int i = 0; i < res_B.size(); i++) {

		//printf("矩形中心坐标%d:(%f,%f)", i, res_B[i].x, res_B[i].y);
		//CRect tmpRect = CRect(, ); //矩形区域
		dc.FillRect(&CRect(int(res_B[i].x), int(res_B[i].y), int(res_B[i].x)+ rect_u, int(res_B[i].y)+ rect_v), &brush1);//填充矩形
	}
	// draw grid
	int right = 1000;
	int bottom = 1000;

	for (int i = 0; i < 100; i++) {
		dc.MoveTo(0, i*rect_u);
		dc.LineTo(right, i*rect_u);
		dc.MoveTo(i*rect_v, 0);
		dc.LineTo(i*rect_v, bottom);
	}

	
}

void line_DDA(float x1, float y1, float x2, float y2, vector<point2f>& res)
{
	float x, y, dx, dy, xinc, yinc;
	int L;

	x = x1;         // 当前迭代正在计算的变量值
	y = y1;

	dx = x2 - x1;   // 迭代所需基本变量
	dy = y2 - y1;   

	abs(int(dx)) > abs(int(dy)) ? L = abs(int(dx)) : L = abs(int(dy));         // 比较大小决定在哪个坐标作为恒定自增坐标（防止断层）

	xinc = dx / L;             // 计算本次迭代增加的步长
	yinc = dy / L;

	for (int i = 0;i < L;i++)
	{
		x += xinc;             // x,y 增加步长
		y += yinc;
		res.push_back(point2f{ x,y });
	}
}
int sign(int x) {
	return x > 0 ? 1 : (x < 0 ? -1 : 0);
}
void line_Bressenham(int x1, int y1, int x2, int y2, vector<point2i>& res)
{
	int temp, x, y, dx, dy, e, s1, s2, swap = 0;

	x = x1;
	y = y1;

	dx = abs(x2 - x1);
	dy = abs(y2 - y1);

	s1 = sign(x2 - x1);
	s2 = sign(y2 - y1);

	if (dx < dy)
	{
		temp = dx;
		dx = dy;
		dy = temp;
		swap = 1;
	}

	e = 2 * dy - dx;
	for (int i = 0;i < dx;i++)
	{
		res.push_back(point2i{ x,y });
		//image.setPixel(x, y, qRgb(255, 255, 255));
		if (e >= 0)
		{
			x += s1;
			y += s2;
			e = e + 2 * (dy - dx);
		}
		else
		{
			if (swap == 1)
			{
				y += s2;
			}
			else
			{
				x += s1;
			}
			e = e + 2 * dy;
		}
	}
	//ui->label->setPixmap(QPixmap::fromImage(image));

}


// CLineDrawingView 打印


void CLineDrawingView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CLineDrawingView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 默认准备
	return DoPreparePrinting(pInfo);
}

void CLineDrawingView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加额外的打印前进行的初始化过程
}

void CLineDrawingView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加打印后进行的清理过程
}

void CLineDrawingView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CLineDrawingView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CLineDrawingView 诊断

#ifdef _DEBUG
void CLineDrawingView::AssertValid() const
{
	CView::AssertValid();
}

void CLineDrawingView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CLineDrawingDoc* CLineDrawingView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CLineDrawingDoc)));
	return (CLineDrawingDoc*)m_pDocument;
}
#endif //_DEBUG


// CLineDrawingView 消息处理程序
