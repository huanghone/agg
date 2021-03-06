#include <math.h>
#include <stdio.h>
#include <time.h>
#include "gfx/agg_rendering_buffer.h"
#include "gfx/agg_conv_transform.h"
#include "gfx/agg_conv_stroke.h"
#include "gfx/agg_conv_dash.h"
#include "gfx/agg_scanline_u.h"
#include "gfx/agg_renderer_scanline.h"
#include "gfx/agg_rasterizer_outline_aa.h"
#include "gfx/agg_rasterizer_scanline_aa.h"
#include "gfx/agg_pattern_filters_rgba.h"
#include "gfx/agg_renderer_outline_aa.h"
#include "gfx/agg_renderer_outline_image.h"
#include "gfx/agg_arc.h"
#include "gfx/agg_bezier_arc.h"
#include "gfx/agg_pixfmt_rgb.h"
#include "gfx/canvas.hh"
#include "ui/ctrl/slider_ctrl.h"
#include "ui/ctrl/bezier_ctrl.h"
#include "ui/ctrl/rbox_ctrl.h"
#include "ui/ctrl/cbox_ctrl.h"
#include "ui/ctrl/view.h"
#include "ui/widget.h"

enum flip_y_e { flip_y = false };

typedef agg::pixfmt_bgr24 pixfmt;

void bezier4_point(
	double x1, double y1, double x2, double y2,
	double x3, double y3, double x4, double y4,
	double mu,
	double* x, double* y)
{
   double mum1, mum13, mu3;

   mum1 = 1 - mu;
   mum13 = mum1 * mum1 * mum1;
   mu3 = mu * mu * mu;

   *x = mum13*x1 + 3*mu*mum1*mum1*x2 + 3*mu*mu*mum1*x3 + mu3*x4;
   *y = mum13*y1 + 3*mu*mum1*mum1*y2 + 3*mu*mu*mum1*y3 + mu3*y4;
}

class ContentView: public agg::View {
public:
	ContentView(bool flip_y) :
		View(0, 0, 800, 800, flip_y),
		m_ctrl_color(agg::rgba(0, 0.3, 0.5, 0.8)),
		m_angle_tolerance(5.0, 5.0, 240.0, 12.0, !flip_y),
		m_approximation_scale(5.0, 17 + 5.0, 240.0, 17 + 12.0, !flip_y),
		m_cusp_limit(5.0, 17 + 17 + 5.0, 240.0, 17 + 17 + 12.0, !flip_y),
		m_width(245.0, 5.0, 495.0, 12.0, !flip_y),
		m_show_points(250.0, 15 + 5, "Show Points", !flip_y),
		m_show_outline(250.0, 30 + 5, "Show Stroke Outline", !flip_y),
		m_curve_type(535.0, 5.0, 535.0 + 115.0, 55.0, !flip_y),
		m_case_type(535.0, 60.0, 535.0 + 115.0, 195.0, !flip_y),
		m_inner_join(535.0, 200.0, 535.0 + 115.0, 290.0, !flip_y),
		m_line_join(535.0, 295.0, 535.0 + 115.0, 385.0, !flip_y),
		m_line_cap(535.0, 395.0, 535.0 + 115.0, 455.0, !flip_y),
		m_cur_case_type(-1) {

		m_curve1.line_color(m_ctrl_color);
		m_curve1.curve(170, 424, 13, 87, 488, 423, 26, 333);
		AddChild(&m_curve1);

		m_angle_tolerance.label("Angle Tolerance=%.0f deg");
		m_angle_tolerance.range(0, 90);
		m_angle_tolerance.value(15);
		AddChild(&m_angle_tolerance);

		m_approximation_scale.label("Approximation Scale=%.3f");
		m_approximation_scale.range(0.1, 5);
		m_approximation_scale.value(1.0);
		AddChild(&m_approximation_scale);

		m_cusp_limit.label("Cusp Limit=%.0f deg");
		m_cusp_limit.range(0, 90);
		m_cusp_limit.value(0);
		AddChild(&m_cusp_limit);

		m_width.label("Width=%.2f");
		m_width.range(-50, 100);
		m_width.value(50.0);
		AddChild(&m_width);

		m_show_points.status(true);
		AddChild(&m_show_points);
		
		m_show_outline.no_transform();
		m_show_outline.status(true);
		AddChild(&m_show_outline);

		m_curve_type.add_item("Incremental");
		m_curve_type.add_item("Subdiv");
		m_curve_type.cur_item(1);
		AddChild(&m_curve_type);

		m_case_type.text_size(7);
		m_case_type.text_thickness(1.0);
		m_case_type.add_item("Random");
		m_case_type.add_item("13---24");
		m_case_type.add_item("Smooth Cusp 1");
		m_case_type.add_item("Smooth Cusp 2");
		m_case_type.add_item("Real Cusp 1");
		m_case_type.add_item("Real Cusp 2");
		m_case_type.add_item("Fancy Stroke");
		m_case_type.add_item("Jaw");
		m_case_type.add_item("Ugly Jaw");
		AddChild(&m_case_type);

		m_inner_join.text_size(8);
		m_inner_join.add_item("Inner Bevel");
		m_inner_join.add_item("Inner Miter");
		m_inner_join.add_item("Inner Jag");
		m_inner_join.add_item("Inner Round");
		m_inner_join.cur_item(3);
		AddChild(&m_inner_join);

		m_line_join.text_size(8);
		m_line_join.add_item("Miter Join");
		m_line_join.add_item("Miter Revert");
		m_line_join.add_item("Round Join");
		m_line_join.add_item("Bevel Join");
		m_line_join.add_item("Miter Round");

		m_line_join.cur_item(1);
		AddChild(&m_line_join);

		m_line_cap.text_size(8);
		m_line_cap.add_item("Butt Cap");
		m_line_cap.add_item("Square Cap");
		m_line_cap.add_item("Round Cap");
		m_line_cap.cur_item(0);
		AddChild(&m_line_cap);
	}
	
	template<class Curve> double measure_time(Curve& curve) {
		return 10.0f;
	}

	template<class Path>
	bool find_point(const Path& path, double dist, unsigned* i, unsigned* j) {
		int k;
		*j = path.size() - 1;

		for (*i = 0; (*j - *i) > 1; ) {
			if (dist < path[k = (*i + *j) >> 1].dist) *j = k;
			else *i = k;
		}
		return true;
	}

	struct curve_point {
		curve_point() {}
		curve_point(double x1, double y1, double mu1) : x(x1), y(y1), mu(mu1) {}
		double x, y, dist, mu;
	};

	template<class Curve> double calc_max_error(Curve& curve, double scale, double* max_angle_error) {
		curve.approximation_scale(m_approximation_scale.value() * scale);
		curve.init(
			m_curve1.x1(), m_curve1.y1(),
			m_curve1.x2(), m_curve1.y2(),
			m_curve1.x3(), m_curve1.y3(),
			m_curve1.x4(), m_curve1.y4());

		agg::pod_bvector<agg::vertex_dist, 8> curve_points;
		unsigned cmd;
		double x, y;
		curve.rewind(0);
		while (!agg::is_stop(cmd = curve.vertex(&x, &y))) {
			if (agg::is_vertex(cmd)) {
				curve_points.add(agg::vertex_dist(x, y));
			}
		}
		unsigned i;
		double curve_dist = 0;
		for (i = 1; i < curve_points.size(); i++) {
			curve_points[i - 1].dist = curve_dist;
			curve_dist += agg::calc_distance(
				curve_points[i - 1].x, curve_points[i - 1].y,
				curve_points[i].x, curve_points[i].y);
		}
		curve_points[curve_points.size() - 1].dist = curve_dist;

		agg::pod_bvector<curve_point, 8> reference_points;
		for (i = 0; i < 4096; i++) {
			double mu = i / 4095.0;
			bezier4_point(
				m_curve1.x1(), m_curve1.y1(),
				m_curve1.x2(), m_curve1.y2(),
				m_curve1.x3(), m_curve1.y3(),
				m_curve1.x4(), m_curve1.y4(),
				mu, &x, &y);
			reference_points.add(curve_point(x, y, mu));
		}

		double reference_dist = 0;
		for (i = 1; i < reference_points.size(); i++) {
			reference_points[i - 1].dist = reference_dist;
			reference_dist += agg::calc_distance(
				reference_points[i - 1].x, reference_points[i - 1].y,
				reference_points[i].x, reference_points[i].y);
		}
		reference_points[reference_points.size() - 1].dist = reference_dist;


		unsigned idx1 = 0;
		unsigned idx2 = 1;
		double max_error = 0;
		for (i = 0; i < reference_points.size(); i++) {
			if (find_point(curve_points, reference_points[i].dist, &idx1, &idx2)) {
				double err = fabs(agg::calc_line_point_distance(
					curve_points[idx1].x, curve_points[idx1].y,
					curve_points[idx2].x, curve_points[idx2].y,
					reference_points[i].x, reference_points[i].y));
				if (err > max_error) max_error = err;
			}
		}

		double aerr = 0;
		for (i = 2; i < curve_points.size(); i++) {
			double a1 =
				atan2(
					curve_points[i - 1].y - curve_points[i - 2].y,
					curve_points[i - 1].x - curve_points[i - 2].x);

			double a2 =
				atan2(
					curve_points[i].y - curve_points[i - 1].y,
					curve_points[i].x - curve_points[i - 1].x);

			double da = fabs(a1 - a2);
			if (da >= agg::pi) da = 2 * agg::pi - da;
			if (da > aerr) aerr = da;
		}


		*max_angle_error = aerr * 180.0 / agg::pi;
		return max_error * scale;
	}


	virtual void Paint(Canvas& canvas) {
		agg::path_storage path;

		double x, y;
		double curve_time = 0;

		path.remove_all();
		agg::curve4 curve;
		curve.approximation_method(agg::curve_approximation_method_e(m_curve_type.cur_item()));
		curve.approximation_scale(m_approximation_scale.value());
		curve.angle_tolerance(agg::deg2rad(m_angle_tolerance.value()));
		curve.cusp_limit(agg::deg2rad(m_cusp_limit.value()));
		curve_time = measure_time(curve);
		double max_angle_error_01 = 0;
		double max_angle_error_1 = 0;
		double max_angle_error1 = 0;
		double max_angle_error_10 = 0;
		double max_angle_error_100 = 0;
		double max_error_01 = 0;
		double max_error_1 = 0;
		double max_error1 = 0;
		double max_error_10 = 0;
		double max_error_100 = 0;

		max_error_01 = calc_max_error(curve, 0.01, &max_angle_error_01);
		max_error_1 = calc_max_error(curve, 0.1, &max_angle_error_1);
		max_error1 = calc_max_error(curve, 1, &max_angle_error1);
		max_error_10 = calc_max_error(curve, 10, &max_angle_error_10);
		max_error_100 = calc_max_error(curve, 100, &max_angle_error_100);

		curve.approximation_scale(m_approximation_scale.value());
		curve.angle_tolerance(agg::deg2rad(m_angle_tolerance.value()));
		curve.cusp_limit(agg::deg2rad(m_cusp_limit.value()));
		curve.init(
			m_curve1.x1(), m_curve1.y1(),
			m_curve1.x2(), m_curve1.y2(),
			m_curve1.x3(), m_curve1.y3(),
			m_curve1.x4(), m_curve1.y4());

		path.concat_path(curve);

		agg::conv_stroke<agg::path_storage> stroke(path);
		stroke.width(m_width.value());
		stroke.line_join(agg::line_join_e(m_line_join.cur_item()));
		stroke.line_cap(agg::line_cap_e(m_line_cap.cur_item()));
		stroke.inner_join(agg::inner_join_e(m_inner_join.cur_item()));
		stroke.inner_miter_limit(1.01);

		canvas.DrawPath(stroke, agg::rgba(0, 0.5, 0, 0.5));

		unsigned cmd;
		unsigned num_points1 = 0;
		path.rewind(0);

		while (!agg::is_stop(cmd = path.vertex(&x, &y))) {
			if (m_show_points.status()) {
				//DrawEllipse()
				agg::ellipse ell(x, y, 1.5, 1.5, 8);
				canvas.DrawPath(ell, agg::rgba(0, 0, 0, 0.5));
			}
			++num_points1;
		}

		if (m_show_outline.status()) {
			agg::conv_stroke<agg::conv_stroke<agg::path_storage> > stroke2(stroke);
			canvas.DrawPath(stroke2, agg::rgba(0, 0, 0, 0.5));
		}

		char buf[512];
		agg::gsv_text t;
		t.size(8.0);

		agg::conv_stroke<agg::gsv_text> pt(t);
		pt.line_cap(agg::round_cap);
		pt.line_join(agg::round_join);
		pt.width(1.5);

		sprintf(
			buf, "Num Points=%d Time=%.2fmks\n\n"
			" Dist Error: x0.01=%.5f x0.1=%.5f x1=%.5f x10=%.5f x100=%.5f\n\n"
			"Angle Error: x0.01=%.1f x0.1=%.1f x1=%.1f x10=%.1f x100=%.1f",
			num_points1, curve_time,
			max_error_01,
			max_error_1,
			max_error1,
			max_error_10,
			max_error_100,
			max_angle_error_01,
			max_angle_error_1,
			max_angle_error1,
			max_angle_error_10,
			max_angle_error_100);

		t.start_point(10.0, 85.0);
		t.text(buf);

		canvas.DrawPath(pt, agg::rgba8(0, 0, 0));

		View::Paint(canvas);
	}

private:
	int m_cur_case_type;
	agg::rgba8 m_ctrl_color;
	agg::BezierCtrl m_curve1;
	agg::SliderCtrl m_angle_tolerance;
	agg::SliderCtrl m_approximation_scale;
	agg::SliderCtrl m_cusp_limit;
	agg::SliderCtrl m_width;
	agg::CBoxCtrl m_show_points;
	agg::CBoxCtrl m_show_outline;
	agg::RBoxCtrl m_curve_type;
	agg::RBoxCtrl m_case_type;
	agg::RBoxCtrl m_inner_join;
	agg::RBoxCtrl m_line_join;
	agg::RBoxCtrl m_line_cap;
};

class ContentDelegate: public agg::Widget::Delegate {
public:
	virtual agg::View* GetContentView() override {
		return new ContentView(flip_y);
	}
};

int agg_main(int argc, char* argv[]) {
	agg::Widget main_wnd(agg::pix_format_bgr24, flip_y, new ContentDelegate);
	main_wnd.caption("AGG Example");
	if(main_wnd.init(655, 520, agg::window_resize)) {
		return main_wnd.run();
	}

	return 1;
}