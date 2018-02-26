#pragma once

#include <vector>
#include "gfx/agg_trans_affine.h"
#include "gfx/agg_renderer_scanline.h"
#include "gfx/canvas.hh"

namespace agg {

class View {
public:
  virtual ~View() {}
  View(double x1, double y1, double x2, double y2, bool flip_y) :
    m_x1(x1), m_y1(y1), m_x2(x2), m_y2(y2), 
    m_flip_y(flip_y),
    m_mtx(0),
		m_cur_ctrl(-1)
  {
  }

	void AddChild(View* c) {
		child_views_.push_back(c);
	}

	virtual void Paint(Canvas& canvas) {
		for (auto view : child_views_) {
			view->Paint(canvas);
		}
	}

	virtual bool InRect(double x, double y) const {
		for (auto view : child_views_) {
			if (view->InRect(x, y)) return true;
		}
		return false;
	}

	virtual bool OnMouseButtonDown(double x, double y) {
		for (auto view : child_views_) {
			if (view->OnMouseButtonDown(x, y)) return true;
		}
		return false;
	}

	virtual bool OnMouseButtonUp(double x, double y) {
		bool flag = false;
		for (auto view : child_views_) {
			if (view->OnMouseButtonUp(x, y)) flag = true;
		}
		return flag;
	}

	virtual bool OnMouseMove(double x, double y, bool button_flag) {
		for (auto view : child_views_) {
			if (view->OnMouseMove(x, y, button_flag)) return true;
		}
		return false;
	}

	virtual bool OnArrowKeys(bool left, bool right, bool down, bool up) {
		if (m_cur_ctrl >= 0) {
			return child_views_[m_cur_ctrl]->OnArrowKeys(left, right, down, up);
		}
		return false;
	}

	void transform(const trans_affine& mtx) { m_mtx = &mtx; }
	void no_transform() { m_mtx = 0; }

	void transform_xy(double* x, double* y) const {
		if(m_flip_y) *y = m_y1 + m_y2 - *y;
		if(m_mtx) m_mtx->transform(x, y);
	}

	void inverse_transform_xy(double* x, double* y) const {
		if(m_mtx) m_mtx->inverse_transform(x, y);
		if(m_flip_y) *y = m_y1 + m_y2 - *y;
	}

	double scale() const { return m_mtx ? m_mtx->scale() : 1.0; }

	bool set_cur(double x, double y) {
		unsigned i;
		for (i = 0; i < child_views_.size(); i++) {
			if (child_views_[i]->InRect(x, y)) {
				if (m_cur_ctrl != int(i)) {
					m_cur_ctrl = i;
					return true;
				}
				return false;
			}
		}
		if (m_cur_ctrl != -1) {
			m_cur_ctrl = -1;
			return true;
		}
		return false;
	}

private:
	View(const View&);
	const View& operator = (const View&);

protected:
	double m_x1;
	double m_y1;
	double m_x2;
	double m_y2;

protected:
	std::vector<View*> child_views_;
	bool m_flip_y;
	const trans_affine* m_mtx;
	int m_cur_ctrl;
};

class RootView : public View {
public:
	RootView() : View(0, 0, 0, 0, true) {}
};

}
