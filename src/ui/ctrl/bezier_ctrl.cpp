#include <string.h>
#include <stdio.h>
#include "ui/ctrl/bezier_ctrl.h"

namespace agg
{

    //------------------------------------------------------------------------
    BezierCtrlBase::BezierCtrlBase() :
        View(0,0,1,1,false),
        m_stroke(m_curve),
        m_poly(4, 5.0),
        m_idx(0)
    {
        m_poly.in_polygon_check(false);
        m_poly.xn(0) = 100.0;
        m_poly.yn(0) =   0.0;
        m_poly.xn(1) = 100.0;
        m_poly.yn(1) =  50.0;
        m_poly.xn(2) =  50.0;
        m_poly.yn(2) = 100.0;
        m_poly.xn(3) =   0.0;
        m_poly.yn(3) = 100.0;
    }


    //------------------------------------------------------------------------
    void BezierCtrlBase::curve(double x1, double y1, 
                                 double x2, double y2, 
                                 double x3, double y3,
                                 double x4, double y4)
    {
        m_poly.xn(0) = x1;
        m_poly.yn(0) = y1;
        m_poly.xn(1) = x2;
        m_poly.yn(1) = y2;
        m_poly.xn(2) = x3;
        m_poly.yn(2) = y3;
        m_poly.xn(3) = x4;
        m_poly.yn(3) = y4;
        curve();
    }

    //------------------------------------------------------------------------
    curve4& BezierCtrlBase::curve()
    {
        m_curve.init(m_poly.xn(0), m_poly.yn(0),
                     m_poly.xn(1), m_poly.yn(1),
                     m_poly.xn(2), m_poly.yn(2),
                     m_poly.xn(3), m_poly.yn(3));
        return m_curve;
    }

    //------------------------------------------------------------------------
    void BezierCtrlBase::rewind(unsigned idx)
    {
        m_idx = idx;

        m_curve.approximation_scale(scale());
        switch(idx)
        {
        default:
        case 0:                 // Control line 1
            m_curve.init(m_poly.xn(0),  m_poly.yn(0), 
                        (m_poly.xn(0) + m_poly.xn(1)) * 0.5,
                        (m_poly.yn(0) + m_poly.yn(1)) * 0.5,
                        (m_poly.xn(0) + m_poly.xn(1)) * 0.5,
                        (m_poly.yn(0) + m_poly.yn(1)) * 0.5,
                         m_poly.xn(1),  m_poly.yn(1));
            m_stroke.rewind(0);
            break;

        case 1:                 // Control line 2
            m_curve.init(m_poly.xn(2),  m_poly.yn(2), 
                        (m_poly.xn(2) + m_poly.xn(3)) * 0.5,
                        (m_poly.yn(2) + m_poly.yn(3)) * 0.5,
                        (m_poly.xn(2) + m_poly.xn(3)) * 0.5,
                        (m_poly.yn(2) + m_poly.yn(3)) * 0.5,
                         m_poly.xn(3),  m_poly.yn(3));
            m_stroke.rewind(0);
            break;

        case 2:                 // Curve itself
            m_curve.init(m_poly.xn(0), m_poly.yn(0), 
                         m_poly.xn(1), m_poly.yn(1),
                         m_poly.xn(2), m_poly.yn(2),
                         m_poly.xn(3), m_poly.yn(3));
            m_stroke.rewind(0);
            break;

        case 3:                 // Point 1
            m_ellipse.init(m_poly.xn(0), m_poly.yn(0), point_radius(), point_radius(), 20);
            m_ellipse.rewind(0);
            break;

        case 4:                 // Point 2
            m_ellipse.init(m_poly.xn(1), m_poly.yn(1), point_radius(), point_radius(), 20);
            m_ellipse.rewind(0);
            break;

        case 5:                 // Point 3
            m_ellipse.init(m_poly.xn(2), m_poly.yn(2), point_radius(), point_radius(), 20);
            m_ellipse.rewind(0);
            break;

        case 6:                 // Point 4
            m_ellipse.init(m_poly.xn(3), m_poly.yn(3), point_radius(), point_radius(), 20);
            m_ellipse.rewind(0);
            break;
        }
    }


    //------------------------------------------------------------------------
    unsigned BezierCtrlBase::vertex(double* x, double* y)
    {
        unsigned cmd = path_cmd_stop;
        switch(m_idx)
        {
        case 0:
        case 1:
        case 2:
            cmd = m_stroke.vertex(x, y);
            break;

        case 3:
        case 4:
        case 5:
        case 6:
        case 7:
            cmd = m_ellipse.vertex(x, y);
            break;
        }

        if(!is_stop(cmd))
        {
            transform_xy(x, y);
        }
        return cmd;
    }



    //------------------------------------------------------------------------
    bool BezierCtrlBase::in_rect(double x, double y) const
    {
        return false;
    }


    //------------------------------------------------------------------------
    bool BezierCtrlBase::OnMouseButtonDown(double x, double y)
    {
        inverse_transform_xy(&x, &y);
        return m_poly.OnMouseButtonDown(x, y);
    }


    //------------------------------------------------------------------------
    bool BezierCtrlBase::OnMouseMove(double x, double y, bool button_flag)
    {
        inverse_transform_xy(&x, &y);
        return m_poly.OnMouseMove(x, y, button_flag);
    }


    //------------------------------------------------------------------------
    bool BezierCtrlBase::OnMouseButtonUp(double x, double y)
    {
        return m_poly.OnMouseButtonUp(x, y);
    }


    //------------------------------------------------------------------------
    bool BezierCtrlBase::on_arrow_keys(bool left, bool right, bool down, bool up)
    {
        return m_poly.on_arrow_keys(left, right, down, up);
    }






    //------------------------------------------------------------------------
    Curve3CtrlBase::Curve3CtrlBase() :
        View(0,0,1,1,false),
        m_stroke(m_curve),
        m_poly(3, 5.0),
        m_idx(0)
    {
        m_poly.in_polygon_check(false);
        m_poly.xn(0) = 100.0;
        m_poly.yn(0) =   0.0;
        m_poly.xn(1) = 100.0;
        m_poly.yn(1) =  50.0;
        m_poly.xn(2) =  50.0;
        m_poly.yn(2) = 100.0;
    }


    //------------------------------------------------------------------------
    void Curve3CtrlBase::curve(double x1, double y1, 
                                 double x2, double y2, 
                                 double x3, double y3)
    {
        m_poly.xn(0) = x1;
        m_poly.yn(0) = y1;
        m_poly.xn(1) = x2;
        m_poly.yn(1) = y2;
        m_poly.xn(2) = x3;
        m_poly.yn(2) = y3;
        curve();
    }

    //------------------------------------------------------------------------
    curve3& Curve3CtrlBase::curve()
    {
        m_curve.init(m_poly.xn(0), m_poly.yn(0),
                     m_poly.xn(1), m_poly.yn(1),
                     m_poly.xn(2), m_poly.yn(2));
        return m_curve;
    }

    //------------------------------------------------------------------------
    void Curve3CtrlBase::rewind(unsigned idx)
    {
        m_idx = idx;

        switch(idx)
        {
        default:
        case 0:                 // Control line
            m_curve.init(m_poly.xn(0),  m_poly.yn(0), 
                        (m_poly.xn(0) + m_poly.xn(1)) * 0.5,
                        (m_poly.yn(0) + m_poly.yn(1)) * 0.5,
                         m_poly.xn(1),  m_poly.yn(1));
            m_stroke.rewind(0);
            break;

        case 1:                 // Control line 2
            m_curve.init(m_poly.xn(1),  m_poly.yn(1), 
                        (m_poly.xn(1) + m_poly.xn(2)) * 0.5,
                        (m_poly.yn(1) + m_poly.yn(2)) * 0.5,
                         m_poly.xn(2),  m_poly.yn(2));
            m_stroke.rewind(0);
            break;

        case 2:                 // Curve itself
            m_curve.init(m_poly.xn(0), m_poly.yn(0), 
                         m_poly.xn(1), m_poly.yn(1),
                         m_poly.xn(2), m_poly.yn(2));
            m_stroke.rewind(0);
            break;

        case 3:                 // Point 1
            m_ellipse.init(m_poly.xn(0), m_poly.yn(0), point_radius(), point_radius(), 20);
            m_ellipse.rewind(0);
            break;

        case 4:                 // Point 2
            m_ellipse.init(m_poly.xn(1), m_poly.yn(1), point_radius(), point_radius(), 20);
            m_ellipse.rewind(0);
            break;

        case 5:                 // Point 3
            m_ellipse.init(m_poly.xn(2), m_poly.yn(2), point_radius(), point_radius(), 20);
            m_ellipse.rewind(0);
            break;
        }
    }


    //------------------------------------------------------------------------
    unsigned Curve3CtrlBase::vertex(double* x, double* y)
    {
        unsigned cmd = path_cmd_stop;
        switch(m_idx)
        {
        case 0:
        case 1:
        case 2:
            cmd = m_stroke.vertex(x, y);
            break;

        case 3:
        case 4:
        case 5:
        case 6:
            cmd = m_ellipse.vertex(x, y);
            break;
        }

        if(!is_stop(cmd))
        {
            transform_xy(x, y);
        }
        return cmd;
    }



    //------------------------------------------------------------------------
    bool Curve3CtrlBase::in_rect(double x, double y) const
    {
        return false;
    }


    //------------------------------------------------------------------------
    bool Curve3CtrlBase::OnMouseButtonDown(double x, double y)
    {
        inverse_transform_xy(&x, &y);
        return m_poly.OnMouseButtonDown(x, y);
    }


    //------------------------------------------------------------------------
    bool Curve3CtrlBase::OnMouseMove(double x, double y, bool button_flag)
    {
        inverse_transform_xy(&x, &y);
        return m_poly.OnMouseMove(x, y, button_flag);
    }


    //------------------------------------------------------------------------
    bool Curve3CtrlBase::OnMouseButtonUp(double x, double y)
    {
        return m_poly.OnMouseButtonUp(x, y);
    }


    //------------------------------------------------------------------------
    bool Curve3CtrlBase::on_arrow_keys(bool left, bool right, bool down, bool up)
    {
        return m_poly.on_arrow_keys(left, right, down, up);
    }












}

