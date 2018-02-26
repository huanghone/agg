#pragma once

#include "agg_pixfmt_rgb.h"
#include "agg_rendering_buffer.h"
#include "agg_renderer_scanline.h"
#include "agg_rasterizer_scanline_aa.h"
#include "agg_scanline_u.h"

#include "agg_ellipse.h"

class Canvas {
public:
	typedef agg::pixfmt_bgr24 pixfmt;
	typedef agg::renderer_base<pixfmt> renderer_base_type;
	typedef agg::renderer_scanline_aa_solid<renderer_base_type> renderer_scanline;
	typedef agg::rasterizer_scanline_aa<> rasterizer_scanline;
	typedef agg::scanline_u8 scanline;

	template<class VertexSource>
	void DrawPath(VertexSource& vs, agg::rgba8 color, unsigned path_id = 0) {
		ras.add_path(vs, path_id);
		ren.color(color);
		agg::render_scanlines(ras, sl, ren);
	}

	void DrawEllipse(double x, double y, double rx, double ry, agg::rgba8 color) {
		agg::ellipse ell(x, y, 1.5, 1.5, 8);
		DrawPath(ell, agg::rgba(0, 0, 0, 0.5));
	}

////////////////////////////
// RAS
	void reset() {
		ras.reset();
	}

//////////////////////////
// REN
	void attach(renderer_base_type& ren_base) {
		ren.attach(ren_base);
	}

	void color(const agg::rgba8& c) { ren.color(c); }
	const agg::rgba8& color() const { return ren.color(); }

	rasterizer_scanline ras;
	scanline sl;
	renderer_scanline ren;
};