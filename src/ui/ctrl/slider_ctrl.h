#pragma once

#include "gfx/agg_basics.h"
#include "gfx/agg_math.h"
#include "gfx/agg_ellipse.h"
#include "gfx/agg_trans_affine.h"
#include "gfx/agg_color_rgba.h"
#include "gfx/agg_gsv_text.h"
#include "gfx/agg_conv_stroke.h"
#include "gfx/agg_path_storage.h"
#include "ui/ctrl/view.h"


namespace agg {

class SliderCtrlBase : public View {
public:
	SliderCtrlBase(double x1, double y1, double x2, double y2, bool flip_y=false);

	void border_width(double t, double extra=0.0);

	void range(double min, double max) { m_min = min; m_max = max; }
	void num_steps(unsigned num) { m_num_steps = num; }
	void label(const char* fmt);
	void text_thickness(double t) { m_text_thickness = t; }

	bool descending() const { return m_descending; }
	void descending(bool v) { m_descending = v; }

	double value() const { return m_value * (m_max - m_min) + m_min; }
	void value(double value);

	virtual bool InRect(double x, double y) const;
	virtual bool OnMouseButtonDown(double x, double y);
	virtual bool OnMouseButtonUp(double x, double y);
	virtual bool OnMouseMove(double x, double y, bool button_flag);
	virtual bool OnArrowKeys(bool left, bool right, bool down, bool up);

	// Vertex source interface
	unsigned num_paths() { return 6; };
	void     rewind(unsigned path_id);
	unsigned vertex(double* x, double* y);

private:
	void calc_box();
	bool normalize_value(bool preview_value_flag);

	double   m_border_width;
	double   m_border_extra;
	double   m_text_thickness;
	double   m_value;
	double   m_preview_value;
	double   m_min;
	double   m_max;
	unsigned m_num_steps;
	bool     m_descending;
	char     m_label[64];
	double   m_xs1;
	double   m_ys1;
	double   m_xs2;
	double   m_ys2;
	double   m_pdx;
	bool     m_mouse_move;
	double   m_vx[32];
	double   m_vy[32];

	ellipse  m_ellipse;

	unsigned m_idx;
	unsigned m_vertex;

	gsv_text              m_text;
	conv_stroke<gsv_text> m_text_poly;
	path_storage          m_storage;

};

class SliderCtrl : public SliderCtrlBase {
public:
	SliderCtrl(double x1, double y1, double x2, double y2, bool flip_y=false) :
		SliderCtrlBase(x1, y1, x2, y2, flip_y),
		m_background_color(rgba(1.0, 0.9, 0.8)),
		m_triangle_color(rgba(0.7, 0.6, 0.6)),
		m_text_color(rgba(0.0, 0.0, 0.0)),
		m_pointer_preview_color(rgba(0.6, 0.4, 0.4, 0.4)),
		m_pointer_color(rgba(0.8, 0.0, 0.0, 0.6))
	{
		m_colors[0] = &m_background_color;
		m_colors[1] = &m_triangle_color;
		m_colors[2] = &m_text_color;
		m_colors[3] = &m_pointer_preview_color;
		m_colors[4] = &m_pointer_color;
		m_colors[5] = &m_text_color;
	}

	virtual void Paint(Canvas& canvas) override {
		unsigned i;
		for (i = 0; i < num_paths(); i++) {
			canvas.reset();
			canvas.DrawPath(*this, color(i), i);
		}
	}

	void background_color(const rgba8& c) { m_background_color = c; }
	void pointer_color(const rgba8& c) { m_pointer_color = c; }

	const rgba8& color(unsigned i) const { return *m_colors[i]; } 

private:
	SliderCtrl(const SliderCtrl&);
	const SliderCtrl& operator = (const SliderCtrl&);

	rgba8 m_background_color;
	rgba8 m_triangle_color;
	rgba8 m_text_color;
	rgba8 m_pointer_preview_color;
	rgba8 m_pointer_color;
	rgba8* m_colors[6];
};

}

