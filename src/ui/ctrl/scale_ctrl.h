#pragma once

#include "gfx/agg_basics.h"
#include "gfx/agg_math.h"
#include "gfx/agg_ellipse.h"
#include "gfx/agg_trans_affine.h"
#include "gfx/agg_color_rgba.h"
#include "ui/ctrl/view.h"


namespace agg {

class ScaleCtrlBase : public View {
  enum move_e {
    move_nothing,
    move_value1,
    move_value2,
    move_slider
  };

public:
  ScaleCtrlBase(double x1, double y1, double x2, double y2, bool flip_y=false);

  void border_thickness(double t, double extra=0.0);
  void resize(double x1, double y1, double x2, double y2);
        
  double min_delta() const { return m_min_d; }
  void min_delta(double d) { m_min_d = d; }
        
  double value1() const { return m_value1; }
  void value1(double value);

  double value2() const { return m_value2; }
  void value2(double value);

  void move(double d);

  virtual bool InRect(double x, double y) const;
  virtual bool OnMouseButtonDown(double x, double y);
  virtual bool OnMouseButtonUp(double x, double y);
  virtual bool OnMouseMove(double x, double y, bool button_flag);
  virtual bool OnArrowKeys(bool left, bool right, bool down, bool up);

  // Vertex soutce interface
  unsigned num_paths() { return 5; };
  void     rewind(unsigned path_id);
  unsigned vertex(double* x, double* y);

private:
  void calc_box();

  double   m_border_thickness;
  double   m_border_extra;
  double   m_value1;
  double   m_value2;
  double   m_min_d;
  double   m_xs1;
  double   m_ys1;
  double   m_xs2;
  double   m_ys2;
  double   m_pdx;
  double   m_pdy;
  move_e   m_move_what;
  double   m_vx[32];
  double   m_vy[32];

  ellipse  m_ellipse;

  unsigned m_idx;
  unsigned m_vertex;

};

template<class ColorT> 
class ScaleCtrl : public ScaleCtrlBase {
public:
  ScaleCtrl(double x1, double y1, double x2, double y2, bool flip_y=false) :
    ScaleCtrlBase(x1, y1, x2, y2, flip_y),
    m_background_color(rgba(1.0, 0.9, 0.8)),
    m_border_color(rgba(0.0, 0.0, 0.0)),
    m_pointers_color(rgba(0.8, 0.0, 0.0, 0.8)),
    m_slider_color(rgba(0.2, 0.1, 0.0, 0.6))
{
    m_colors[0] = &m_background_color;
    m_colors[1] = &m_border_color;
    m_colors[2] = &m_pointers_color;
    m_colors[3] = &m_pointers_color;
    m_colors[4] = &m_slider_color;
  }
          

  void background_color(const ColorT& c) { m_background_color = c; }
  void border_color(const ColorT& c)     { m_border_color = c; }
  void pointers_color(const ColorT& c)   { m_pointers_color = c; }
  void slider_color(const ColorT& c)     { m_slider_color = c; }

  const ColorT& color(unsigned i) const { return *m_colors[i]; } 

private:
  ScaleCtrl(const ScaleCtrl<ColorT>&);
  const ScaleCtrl<ColorT>& operator = (const ScaleCtrl<ColorT>&);

  ColorT m_background_color;
  ColorT m_border_color;
  ColorT m_pointers_color;
  ColorT m_slider_color;
  ColorT* m_colors[5];
};

}

