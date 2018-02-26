//----------------------------------------------------------------------------
// Anti-Grain Geometry (AGG) - Version 2.5
// A high quality rendering engine for C++
// Copyright (C) 2002-2006 Maxim Shemanarev
// Contact: mcseem@antigrain.com
//          mcseemagg@yahoo.com
//          http://antigrain.com
// 
// AGG is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
// 
// AGG is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
// 
// You should have received a copy of the GNU General Public License
// along with AGG; if not, write to the Free Software
// Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, 
// MA 02110-1301, USA.
//----------------------------------------------------------------------------

#ifndef AGG_CTRL_INCLUDED
#define AGG_CTRL_INCLUDED

#include <vector>

#include "agg_trans_affine.h"
#include "agg_renderer_scanline.h"

namespace agg {

class View {
public:
    //--------------------------------------------------------------------
    virtual ~View() {}
    View(double x1, double y1, double x2, double y2, bool flip_y) :
        m_x1(x1), m_y1(y1), m_x2(x2), m_y2(y2), 
        m_flip_y(flip_y),
        m_mtx(0)
    {
    }

	void add(View& c) {
		child_views_.push_back(&c);
	}

	virtual bool in_rect(double x, double y) const { return true; }
	virtual bool on_mouse_button_down(double x, double y) { return true; }
	virtual bool on_mouse_button_up(double x, double y) { return true; }
	virtual bool on_mouse_move(double x, double y, bool button_flag) { return true; }
	virtual bool on_arrow_keys(bool left, bool right, bool down, bool up) { return true; }

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
};

template<class Rasterizer, class Scanline, class Renderer, class Ctrl> 
void render_ctrl(Rasterizer& ras, Scanline& sl, Renderer& r, Ctrl& c) {
	unsigned i;
	for(i = 0; i < c.num_paths(); i++) {
		ras.reset();
		ras.add_path(c, i);
		render_scanlines_aa_solid(ras, sl, r, c.color(i));
	}
}

template<class Rasterizer, class Scanline, class Renderer, class Ctrl> 
void render_ctrl_rs(Rasterizer& ras, Scanline& sl, Renderer& r, Ctrl& c) {
	unsigned i;
	for(i = 0; i < c.num_paths(); i++) {
		ras.reset();
		ras.add_path(c, i);
		r.color(c.color(i));
		render_scanlines(ras, sl, r);
	}
}

class RootView : public View {
public:
	RootView() : View(0, 0, 0, 0, true), m_cur_ctrl(-1) {}

	virtual bool in_rect(double x, double y) const override {
		for (auto view : child_views_) {
			if (view->in_rect(x, y)) return true;
		}
		return false;
	}

	virtual bool on_mouse_button_down(double x, double y) override {
		for (auto view : child_views_) {
			if (view->on_mouse_button_down(x, y)) return true;
		}
		return false;
	}

	virtual bool on_mouse_button_up(double x, double y) override {
		bool flag = false;
		for (auto view: child_views_) {
			if (view->on_mouse_button_up(x, y)) flag = true;
		}
		return flag;
	}

	virtual bool on_mouse_move(double x, double y, bool button_flag) override {
		for (auto view : child_views_) {
			if (view->on_mouse_move(x, y, button_flag)) return true;
		}
		return false;
	}

	virtual bool on_arrow_keys(bool left, bool right, bool down, bool up) override {
		if (m_cur_ctrl >= 0) {
			return child_views_[m_cur_ctrl]->on_arrow_keys(left, right, down, up);
		}
		return false;
	}

	View* GetViewForPoint(double x, double y) {
		return NULL;
	}

	bool set_cur(double x, double y) {
		unsigned i;
		for (i = 0; i < child_views_.size(); i++) {
			if (child_views_[i]->in_rect(x, y)) {
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
	int           m_cur_ctrl;
};

}


#endif
