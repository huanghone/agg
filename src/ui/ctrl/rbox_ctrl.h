#pragma once

#include "gfx/agg_array.h"
#include "gfx/agg_ellipse.h"
#include "gfx/agg_conv_stroke.h"
#include "gfx/agg_gsv_text.h"
#include "gfx/agg_trans_affine.h"
#include "gfx/agg_color_rgba.h"
#include "ui/ctrl/view.h"

namespace agg {

class RBoxCtrlBase: public View {
public:
	RBoxCtrlBase(double x1, double y1, double x2, double y2, bool flip_y=false);

	void border_width(double t, double extra=0.0);
	void text_thickness(double t)  { m_text_thickness = t; }
	void text_size(double h, double w=0.0);

	void add_item(const char* text);
	int cur_item() const { return m_cur_item; }
	void cur_item(int i) { m_cur_item = i; }

	virtual bool in_rect(double x, double y) const;
	virtual bool OnMouseButtonDown(double x, double y);
	virtual bool OnMouseButtonUp(double x, double y);
	virtual bool OnMouseMove(double x, double y, bool button_flag);
	virtual bool on_arrow_keys(bool left, bool right, bool down, bool up);

	// Vertex soutce interface
	unsigned num_paths() { return 5; };
	void rewind(unsigned path_id);
	unsigned vertex(double* x, double* y);

private:
	void calc_rbox();

	double m_border_width;
	double m_border_extra;
	double m_text_thickness;
	double m_text_height;
	double m_text_width;
	pod_array<char> m_items[32];
	unsigned m_num_items;
	int m_cur_item;

	double m_xs1;
	double m_ys1;
	double m_xs2;
	double m_ys2;

	double m_vx[32];
	double m_vy[32];
	unsigned m_draw_item;
	double m_dy;

	ellipse m_ellipse;
	conv_stroke<ellipse> m_ellipse_poly;
	gsv_text m_text;
	conv_stroke<gsv_text> m_text_poly;

	unsigned m_idx;
	unsigned m_vertex;
};

class RBoxCtrl: public RBoxCtrlBase {
public:
  RBoxCtrl(double x1, double y1, double x2, double y2, bool flip_y=false) :
		RBoxCtrlBase(x1, y1, x2, y2, flip_y),
		m_background_color(rgba(1.0, 1.0, 0.9)),
		m_border_color(rgba(0.0, 0.0, 0.0)),
		m_text_color(rgba(0.0, 0.0, 0.0)),
		m_inactive_color(rgba(0.0, 0.0, 0.0)),
		m_active_color(rgba(0.4, 0.0, 0.0))
  {
		m_colors[0] = &m_background_color;
		m_colors[1] = &m_border_color;
		m_colors[2] = &m_text_color;
		m_colors[3] = &m_inactive_color;
		m_colors[4] = &m_active_color;
  }

	virtual void Paint(Canvas& canvas) override {
		unsigned i;
		for (i = 0; i < num_paths(); i++) {
			canvas.reset();
			canvas.DrawPath(*this, color(i), i);
		}
	}

  void background_color(const rgba8& c) { m_background_color = c; }
  void border_color(const rgba8& c) { m_border_color = c; }
  void text_color(const rgba8& c) { m_text_color = c; }
  void inactive_color(const rgba8& c) { m_inactive_color = c; }
  void active_color(const rgba8& c) { m_active_color = c; }

  const rgba8& color(unsigned i) const { return *m_colors[i]; } 

private:
  RBoxCtrl(const RBoxCtrl&);
  const RBoxCtrl& operator = (const RBoxCtrl&);
       
	rgba8 m_background_color;
	rgba8 m_border_color;
	rgba8 m_text_color;
	rgba8 m_inactive_color;
	rgba8 m_active_color;
	rgba8* m_colors[5];
};

}