#include <string.h>
#include "ui/ctrl/cbox_ctrl.h"

namespace agg
{

    //------------------------------------------------------------------------
    CBoxCtrlBase::CBoxCtrlBase(double x, double y, 
                                   const char* l, 
                                   bool flip_y) :
        View(x, y, x + 9.0 * 1.5, y + 9.0 * 1.5, flip_y),
        m_text_thickness(1.5),
        m_text_height(9.0),
        m_text_width(0.0),
        m_status(false),
        m_text_poly(m_text)
    {
        label(l);
    }


    //------------------------------------------------------------------------
    void CBoxCtrlBase::text_size(double h, double w)
    {
        m_text_width = w; 
        m_text_height = h; 
    }

    //------------------------------------------------------------------------
    void CBoxCtrlBase::label(const char* l)
    {
        unsigned len = strlen(l);
        if(len > 127) len = 127;
        memcpy(m_label, l, len);
        m_label[len] = 0;
    }


    //------------------------------------------------------------------------
    bool CBoxCtrlBase::OnMouseButtonDown(double x, double y)
    {
        inverse_transform_xy(&x, &y);
        if(x >= m_x1 && y >= m_y1 && x <= m_x2 && y <= m_y2)
        {
            m_status = !m_status;
            return true;
        }
        return false;
    }


    //------------------------------------------------------------------------
    bool CBoxCtrlBase::OnMouseMove(double, double, bool)
    {
        return false;
    }

    //------------------------------------------------------------------------
    bool CBoxCtrlBase::InRect(double x, double y) const
    {
        inverse_transform_xy(&x, &y);
        return x >= m_x1 && y >= m_y1 && x <= m_x2 && y <= m_y2;
    }

    //------------------------------------------------------------------------
    bool CBoxCtrlBase::OnMouseButtonUp(double, double)
    {
        return false;
    }

    //------------------------------------------------------------------------
    bool CBoxCtrlBase::OnArrowKeys(bool, bool, bool, bool)
    {
        return false;
    }


    //------------------------------------------------------------------------
    void CBoxCtrlBase::rewind(unsigned idx)
    {
        m_idx = idx;

        double d2;
        double t;

        switch(idx)
        {
        default:
        case 0:                 // Border
            m_vertex = 0;
            m_vx[0] = m_x1; 
            m_vy[0] = m_y1;
            m_vx[1] = m_x2;
            m_vy[1] = m_y1;
            m_vx[2] = m_x2;
            m_vy[2] = m_y2;
            m_vx[3] = m_x1; 
            m_vy[3] = m_y2;
            m_vx[4] = m_x1 + m_text_thickness; 
            m_vy[4] = m_y1 + m_text_thickness; 
            m_vx[5] = m_x1 + m_text_thickness; 
            m_vy[5] = m_y2 - m_text_thickness;
            m_vx[6] = m_x2 - m_text_thickness;
            m_vy[6] = m_y2 - m_text_thickness;
            m_vx[7] = m_x2 - m_text_thickness;
            m_vy[7] = m_y1 + m_text_thickness; 
            break;

        case 1:                 // Text
            m_text.text(m_label);
            m_text.start_point(m_x1 + m_text_height * 2.0, m_y1 + m_text_height / 5.0);
            m_text.size(m_text_height, m_text_width);
            m_text_poly.width(m_text_thickness);
            m_text_poly.line_join(round_join);
            m_text_poly.line_cap(round_cap);
            m_text_poly.rewind(0);
            break;

        case 2:                 // Active item
            m_vertex = 0;
            d2 = (m_y2 - m_y1) / 2.0;
            t = m_text_thickness * 1.5;
            m_vx[0] = m_x1 + m_text_thickness;
            m_vy[0] = m_y1 + m_text_thickness;
            m_vx[1] = m_x1 + d2;
            m_vy[1] = m_y1 + d2 - t;
            m_vx[2] = m_x2 - m_text_thickness;
            m_vy[2] = m_y1 + m_text_thickness;
            m_vx[3] = m_x1 + d2 + t;
            m_vy[3] = m_y1 + d2;
            m_vx[4] = m_x2 - m_text_thickness;
            m_vy[4] = m_y2 - m_text_thickness;
            m_vx[5] = m_x1 + d2;
            m_vy[5] = m_y1 + d2 + t;
            m_vx[6] = m_x1 + m_text_thickness;
            m_vy[6] = m_y2 - m_text_thickness;
            m_vx[7] = m_x1 + d2 - t;
            m_vy[7] = m_y1 + d2;
            break;

        }
    }




    //------------------------------------------------------------------------
    unsigned CBoxCtrlBase::vertex(double* x, double* y)
    {
        unsigned cmd = path_cmd_line_to;
        switch(m_idx)
        {
        case 0:
            if(m_vertex == 0 || m_vertex == 4) cmd = path_cmd_move_to;
            if(m_vertex >= 8) cmd = path_cmd_stop;
            *x = m_vx[m_vertex];
            *y = m_vy[m_vertex];
            m_vertex++;
            break;

        case 1:
            cmd = m_text_poly.vertex(x, y);
            break;

        case 2:
            if(m_status)
            {
                if(m_vertex == 0) cmd = path_cmd_move_to;
                if(m_vertex >= 8) cmd = path_cmd_stop;
                *x = m_vx[m_vertex];
                *y = m_vy[m_vertex];
                m_vertex++;
            }
            else
            {
                cmd = path_cmd_stop;
            }
            break;

        default:
            cmd = path_cmd_stop;
            break;
        }

        if(!is_stop(cmd))
        {
            transform_xy(x, y);
        }
        return cmd;
    }
}



