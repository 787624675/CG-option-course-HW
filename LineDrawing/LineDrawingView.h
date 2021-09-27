
// LineDrawingView.h: CLineDrawingView 类的接口
//

#pragma once


class CLineDrawingView : public CView
{

private:
	// 捕捉鼠标所用的变量
	BOOL m_startRect = FALSE;
	CPoint m_startPoint = 0;
	CPoint m_OldPoint = 0;
protected: // 仅从序列化创建
	CLineDrawingView() noexcept;
	DECLARE_DYNCREATE(CLineDrawingView)

// 特性
public:
	CLineDrawingDoc* GetDocument() const;

// 操作
public:

// 重写
public:
	virtual void OnDraw(CDC* pDC);  // 重写以绘制该视图
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnLButtonDown(UINT nFlags, CPoint point);
	virtual void OnMouseMove(UINT nFlags, CPoint point);
	virtual void OnLButtonUp(UINT nFlags, CPoint point);

// 实现
public:
	virtual ~CLineDrawingView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成的消息映射函数
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // LineDrawingView.cpp 中的调试版本
inline CLineDrawingDoc* CLineDrawingView::GetDocument() const
   { return reinterpret_cast<CLineDrawingDoc*>(m_pDocument); }
#endif

