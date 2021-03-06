#pragma once

#include "gfx/agg_basics.h"
#include "gfx/agg_conv_stroke.h"
#include "gfx/agg_gsv_text.h"
#include "gfx/agg_trans_affine.h"
#include "gfx/agg_color_rgba.h"
#include "ui/ctrl/view.h"

namespace agg {

class CBoxCtrlBase: public View {
public:
	CBoxCtrlBase(double x, double y, const char* label, bool flip_y=false);

	void text_thickness(double t)  { m_text_thickness = t; }
	void text_size(double h, double w=0.0);

	const char* label() { return m_label; }
	void label(const char* l);

	bool status() const { return m_status; }
	void status(bool st) { m_status = st; }

	virtual bool InRect(double x, double y) const;
	virtual bool OnMouseButtonDown(double x, double y);
	virtual bool OnMouseButtonUp(double x, double y);
	virtual bool OnMouseMove(double x, double y, bool button_flag);
	virtual bool OnArrowKeys(bool left, bool right, bool down, bool up);

	// Vertex soutce interface
	unsigned num_paths() { return 3; };
	void     rewind(unsigned path_id);
	unsigned vertex(double* x, double* y);

private:
	double m_text_thickness;
	double m_text_height;
	double m_text_width;
	char m_label[128];
	bool m_status;
	double m_vx[32];
	double m_vy[32];

	gsv_text m_text;
	conv_stroke<gsv_text> m_text_poly;

	unsigned m_idx;
	unsigned m_vertex;
};

class CBoxCtrl : public CBoxCtrlBase {
public:
	CBoxCtrl(double x, double y, const char* label, bool flip_y=false):
		CBoxCtrlBase(x, y, label, flip_y),
		m_text_color(rgba(0.0, 0.0, 0.0)),
		m_inactive_color(rgba(0.0, 0.0, 0.0)),
		m_active_color(rgba(0.4, 0.0, 0.0))
	{
		m_colors[0] = &m_inactive_color;
		m_colors[1] = &m_text_color;
		m_colors[2] = &m_active_color;
	}

	virtual void Paint(Canvas& canvas) override {
		unsigned i;
		for (i = 0; i < num_paths(); i++) {
			canvas.reset();
			canvas.DrawPath(*this, color(i), i);
		}
	}
          
	void text_color(const rgba8& c) { m_text_color = c; }
	void inactive_color(const rgba8& c) { m_inactive_color = c; }
	void active_color(const rgba8& c) { m_active_color = c; }

	const rgba8& color(unsigned i) const { return *m_colors[i]; }

private:
	CBoxCtrl(const CBoxCtrl&);
	const CBoxCtrl& operator = (const CBoxCtrl&);

	rgba8 m_text_color;
	rgba8 m_inactive_color;
	rgba8 m_active_color;
	rgba8* m_colors[3];
};


}
