#pragma once

#include "gfx/agg_basics.h"
#include "gfx/agg_ellipse.h"
#include "gfx/agg_bspline.h"
#include "gfx/agg_conv_stroke.h"
#include "gfx/agg_path_storage.h"
#include "gfx/agg_trans_affine.h"
#include "gfx/agg_color_rgba.h"
#include "ui/ctrl/view.h"

namespace agg {

class SplineCtrlBase: public View {
public:
  SplineCtrlBase(double x1, double y1, double x2, double y2, unsigned num_pnt, bool flip_y=false);

  // Set other parameters
  void border_width(double t, double extra=0.0);
  void curve_width(double t) { m_curve_width = t; }
  void point_size(double s)  { m_point_size = s; }

  // Event handlers. Just call them if the respective events
  // in your system occure. The functions return true if redrawing
  // is required.
  virtual bool InRect(double x, double y) const;
  virtual bool OnMouseButtonDown(double x, double y);
  virtual bool OnMouseButtonUp(double x, double y);
  virtual bool OnMouseMove(double x, double y, bool button_flag);
  virtual bool OnArrowKeys(bool left, bool right, bool down, bool up);

  void active_point(int i);

  const double* spline()  const { return m_spline_values;  }
  const int8u*  spline8() const { return m_spline_values8; }
  double value(double x) const;
  void value(unsigned idx, double y);
  void point(unsigned idx, double x, double y);
  void x(unsigned idx, double x) { m_xp[idx] = x; }
  void y(unsigned idx, double y) { m_yp[idx] = y; }
  double x(unsigned idx) const { return m_xp[idx]; }
  double y(unsigned idx) const { return m_yp[idx]; }
  void  update_spline();

  // Vertex soutce interface
  unsigned num_paths() { return 5; }
  void rewind(unsigned path_id);
  unsigned vertex(double* x, double* y);

private:
  void calc_spline_box();
  void calc_curve();
  double calc_xp(unsigned idx);
  double calc_yp(unsigned idx);
  void set_xp(unsigned idx, double val);
  void set_yp(unsigned idx, double val);

  unsigned m_num_pnt;
  double   m_xp[32];
  double   m_yp[32];
  bspline  m_spline;
  double   m_spline_values[256];
  int8u    m_spline_values8[256];
  double   m_border_width;
  double   m_border_extra;
  double   m_curve_width;
  double   m_point_size;
  double   m_xs1;
  double   m_ys1;
  double   m_xs2;
  double   m_ys2;
  path_storage              m_curve_pnt;
  conv_stroke<path_storage> m_curve_poly;
  ellipse                   m_ellipse;
  unsigned m_idx;
  unsigned m_vertex;
  double   m_vx[32];
  double   m_vy[32];
  int      m_active_pnt;
  int      m_move_pnt;
  double   m_pdx;
  double   m_pdy;
  const trans_affine* m_mtx;
};

template<class ColorT> 
class SplineCtrl : public SplineCtrlBase {
public:
	SplineCtrl(double x1, double y1, double x2, double y2, unsigned num_pnt, bool flip_y=false) :
		SplineCtrlBase(x1, y1, x2, y2, num_pnt, flip_y),
		m_background_color(rgba(1.0, 1.0, 0.9)),
		m_border_color(rgba(0.0, 0.0, 0.0)),
		m_curve_color(rgba(0.0, 0.0, 0.0)),
		m_inactive_pnt_color(rgba(0.0, 0.0, 0.0)),
		m_active_pnt_color(rgba(1.0, 0.0, 0.0))
	{
		m_colors[0] = &m_background_color;
		m_colors[1] = &m_border_color;
		m_colors[2] = &m_curve_color;
		m_colors[3] = &m_inactive_pnt_color;
		m_colors[4] = &m_active_pnt_color;
	}

  // Set colors
  void background_color(const ColorT& c)   { m_background_color = c; }
  void border_color(const ColorT& c)       { m_border_color = c; }
  void curve_color(const ColorT& c)        { m_curve_color = c; }
  void inactive_pnt_color(const ColorT& c) { m_inactive_pnt_color = c; }
  void active_pnt_color(const ColorT& c)   { m_active_pnt_color = c; }
  const ColorT& color(unsigned i) const { return *m_colors[i]; } 

private:
  SplineCtrl(const SplineCtrl<ColorT>&);
  const SplineCtrl<ColorT>& operator = (const SplineCtrl<ColorT>&);

  ColorT  m_background_color;
  ColorT  m_border_color;
  ColorT  m_curve_color;
  ColorT  m_inactive_pnt_color;
  ColorT  m_active_pnt_color;
  ColorT* m_colors[5];
};

}
