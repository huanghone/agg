#pragma once

#include "gfx/agg_math.h"
#include "gfx/agg_ellipse.h"
#include "gfx/agg_trans_affine.h"
#include "gfx/agg_color_rgba.h"
#include "gfx/agg_conv_stroke.h"
#include "gfx/agg_conv_curve.h"
#include "gfx/canvas.hh"
#include "ui/ctrl/polygon_ctrl.h"

namespace agg {

class BezierCtrlBase: public View {
public:
  BezierCtrlBase();

  void curve(
		double x1, double y1,
		double x2, double y2,
		double x3, double y3,
		double x4, double y4);
  curve4& curve();

  double x1() const { return m_poly.xn(0); }
  double y1() const { return m_poly.yn(0); }
  double x2() const { return m_poly.xn(1); }
  double y2() const { return m_poly.yn(1); }
  double x3() const { return m_poly.xn(2); }
  double y3() const { return m_poly.yn(2); }
  double x4() const { return m_poly.xn(3); }
  double y4() const { return m_poly.yn(3); }

  void x1(double x) { m_poly.xn(0) = x; }
  void y1(double y) { m_poly.yn(0) = y; }
  void x2(double x) { m_poly.xn(1) = x; }
  void y2(double y) { m_poly.yn(1) = y; }
  void x3(double x) { m_poly.xn(2) = x; }
  void y3(double y) { m_poly.yn(2) = y; }
  void x4(double x) { m_poly.xn(3) = x; }
  void y4(double y) { m_poly.yn(3) = y; }

  void line_width(double w) { m_stroke.width(w); }
  double line_width() const   { return m_stroke.width(); }

  void point_radius(double r) { m_poly.point_radius(r); }
  double point_radius() const { return m_poly.point_radius(); }

  virtual bool InRect(double x, double y) const;
  virtual bool OnMouseButtonDown(double x, double y);
  virtual bool OnMouseButtonUp(double x, double y);
  virtual bool OnMouseMove(double x, double y, bool button_flag);
  virtual bool OnArrowKeys(bool left, bool right, bool down, bool up);

  // Vertex source interface
  unsigned num_paths() { return 7; };
  void rewind(unsigned path_id);
  unsigned vertex(double* x, double* y);

private:
  curve4 m_curve;
  ellipse m_ellipse;
  conv_stroke<curve4> m_stroke;
  polygon_ctrl_impl m_poly;
  unsigned m_idx;
};

class BezierCtrl: public BezierCtrlBase {
public:
  BezierCtrl():m_color(rgba8(0, 0, 0)) { }

	virtual void Paint(Canvas& canvas) override {
		unsigned i;
		for (i = 0; i < num_paths(); i++) {
			canvas.reset();
			canvas.DrawPath(*this, color(i), i);
		}
	}

  void line_color(const agg::rgba8& c) { m_color = c; }
  const agg::rgba8& color(unsigned i) const { return m_color; }

private:
  BezierCtrl(const BezierCtrl&);
  const BezierCtrl& operator = (const BezierCtrl&);

	agg::rgba8 m_color;
};

class Curve3CtrlBase: public View {
public:
	Curve3CtrlBase();

	void curve(
		double x1, double y1, 
		double x2, double y2, 
		double x3, double y3);
	curve3& curve();

	double x1() const { return m_poly.xn(0); }
	double y1() const { return m_poly.yn(0); }
	double x2() const { return m_poly.xn(1); }
	double y2() const { return m_poly.yn(1); }
	double x3() const { return m_poly.xn(2); }
	double y3() const { return m_poly.yn(2); }

	void x1(double x) { m_poly.xn(0) = x; }
	void y1(double y) { m_poly.yn(0) = y; }
	void x2(double x) { m_poly.xn(1) = x; }
	void y2(double y) { m_poly.yn(1) = y; }
	void x3(double x) { m_poly.xn(2) = x; }
	void y3(double y) { m_poly.yn(2) = y; }

	void line_width(double w) { m_stroke.width(w); }
	double line_width() const   { return m_stroke.width(); }

	void point_radius(double r) { m_poly.point_radius(r); }
	double point_radius() const   { return m_poly.point_radius(); }

	virtual bool InRect(double x, double y) const;
	virtual bool OnMouseButtonDown(double x, double y);
	virtual bool OnMouseButtonUp(double x, double y);
	virtual bool OnMouseMove(double x, double y, bool button_flag);
	virtual bool OnArrowKeys(bool left, bool right, bool down, bool up);

	// Vertex source interface
	unsigned num_paths() { return 6; };
	void rewind(unsigned path_id);
	unsigned vertex(double* x, double* y);

private:
  curve3 m_curve;
  ellipse m_ellipse;
  conv_stroke<curve3> m_stroke;
  polygon_ctrl_impl m_poly;
  unsigned m_idx;
};

class Curve3Ctrl : public Curve3CtrlBase {
public:
	Curve3Ctrl(): m_color(rgba(0.0, 0.0, 0.0)) { }
          
	void line_color(const agg::rgba8& c) { m_color = c; }
	const agg::rgba8& color(unsigned i) const { return m_color; }

private:
	Curve3Ctrl(const Curve3Ctrl&);
	const Curve3Ctrl& operator = (const Curve3Ctrl&);

	agg::rgba8 m_color;
};

}

